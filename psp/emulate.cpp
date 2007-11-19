#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pspkernel.h>
#include <psptypes.h>
#include <psprtc.h>

#include "audio.h"
#include "video.h"
#include "psp.h"
#include "ctrl.h"
#include "perf.h"
#include "image.h"
#include "util.h"
#include "file.h"
#include "kybd.h"

#include "menu.h"
#include "emulate.h"
#include "cap32_psp.h"

#define UNSET_KEY(code) keyboard_matrix[(code)>>4]|=bit_values[(code)&7]
#define SET_KEY(code)   keyboard_matrix[(code)>>4]&=~bit_values[(code)&7]

dword freq_table[] = { 44100 };

extern EmulatorOptions Options;
extern char *ScreenshotPath;
extern char *LoadedGame;

PspImage *Screen = NULL;
struct GameConfig ActiveGameConfig;

/* Button masks */
static const u64 ButtonMask[] = 
{
  PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER, 
  PSP_CTRL_START    | PSP_CTRL_SELECT,
  PSP_CTRL_ANALUP,    PSP_CTRL_ANALDOWN,
  PSP_CTRL_ANALLEFT,  PSP_CTRL_ANALRIGHT,
  PSP_CTRL_UP,        PSP_CTRL_DOWN,
  PSP_CTRL_LEFT,      PSP_CTRL_RIGHT,
  PSP_CTRL_SQUARE,    PSP_CTRL_CROSS,
  PSP_CTRL_CIRCLE,    PSP_CTRL_TRIANGLE,
  PSP_CTRL_LTRIGGER,  PSP_CTRL_RTRIGGER,
  PSP_CTRL_SELECT,    PSP_CTRL_START,
  0 /* End */
};

static PspFpsCounter FpsCounter;
static int ScreenX, ScreenY, ScreenW, ScreenH;
static int ClearScreen, ShowKybd;
static u32 TicksPerUpdate, TicksPerSecond;
static u64 LastTick;
static int Frame;

static int  ParseInput();
static void HandleKeyInput(unsigned int code, int on);
static void RenderVideo();
static void AudioCallback(void* buf, unsigned int *length, void *userdata);

static PspKeyboardLayout *Cpc464Kybd;

/* Initialize emulation */
int InitEmulation()
{
  if (!(Screen = pspImageCreateVram(512, CPC_VISIBLE_SCR_HEIGHT, 
    PSP_IMAGE_INDEXED))) 
      return 0;

  Screen->Viewport.Width = CPC_VISIBLE_SCR_WIDTH;
  Screen->Viewport.Height = CPC_VISIBLE_SCR_HEIGHT;

  Cpc464Kybd = pspKybdLoadLayout("cpc464.lyt", NULL, HandleKeyInput);

  int i;

  memset(&CPC, 0, sizeof(CPC));

  CPC.model = 2; // CPC 6128
  CPC.jumpers = 0x1e; // OEM is Amstrad, video refresh is 50Hz
  CPC.ram_size = 128 & 0x02c0; // 128KB RAM
  CPC.speed = DEF_SPEED_SETTING; // original CPC speed
  CPC.limit_speed = 0;
  CPC.printer = 0;
  CPC.mf2 = 0;
  CPC.keyboard = 0;
  CPC.joysticks = 1;

  CPC.scr_vsync = 0;
  CPC.scr_led = 1;
  CPC.scr_fps = 0;
  CPC.scr_tube = 0;
  CPC.scr_intensity = 10;
  CPC.scr_remanency = 0;
  CPC.scr_window = 0;

  CPC.snd_enabled = 1;
  CPC.snd_playback_rate = 0;
  CPC.snd_bits = 1;
  CPC.snd_stereo = 0;
  CPC.snd_volume = 100;
  CPC.snd_pp_device = 0;

  CPC.kbd_layout = 0;

  CPC.max_tracksize = 6144-154;

  CPC.drvA_format = DEFAULT_DISK_FORMAT;
  CPC.drvB_format = DEFAULT_DISK_FORMAT;

  strcpy(CPC.printer_file, "/printer.dat");
  strcpy(CPC.rom_path, ".");

  for (i = 0; i < 16; i++) strcpy(CPC.rom_file[i], "");

  /* insert AMSDOS in slot 7 */
  strcpy(CPC.rom_file[7], "amsdos.rom"); 

  if (CPC.printer && !printer_start())
    CPC.printer = 0;

  z80_init_tables(); // init Z80 emulation
  video_init_tables(); // generate the byte-to-pixels translation tables

  /* Clear keyboard matrix */
  memset(keyboard_matrix, 0xff, sizeof(keyboard_matrix));

  if (video_init()) return 0;
  if (audio_init()) return 0;
  if (emulator_init()) return 0;

  memset(&driveA, 0, sizeof(t_drive)); // clear disk drive A data structure
  memset(&driveB, 0, sizeof(t_drive)); // clear disk drive A data structure

  return 1;
}

/* Process input */
static int ParseInput()
{
  static SceCtrlData pad;

  /* Check the input */
  if (pspCtrlPollControls(&pad))
  {
#ifdef PSP_DEBUG
    if ((pad.Buttons & (PSP_CTRL_SELECT | PSP_CTRL_START))
      == (PSP_CTRL_SELECT | PSP_CTRL_START))
        pspUtilSaveVramSeq(ScreenshotPath, 
                           (LoadedGame) 
                               ? pspFileGetFilename(LoadedGame) : "BASIC");
#endif

    /* Navigate the virtual keyboard, if shown */
    if (ShowKybd) pspKybdNavigate(Cpc464Kybd, &pad);

    /* Parse input */
    int i, on, code;
    for (i = 0; ButtonMapId[i] >= 0; i++)
    {
      code = ActiveGameConfig.ButtonMap[ButtonMapId[i]];
      on = (pad.Buttons & ButtonMask[i]) == ButtonMask[i];

      /* Check to see if a button set is pressed. If so, unset it, so it */
      /* doesn't trigger any other combination presses. */
      if (on) pad.Buttons |= ButtonMask[i];

      if ((code & KBD) && !ShowKybd)
      {
        if (on) SET_KEY(cpc_kbd[CPC.keyboard][CODE_MASK(code)]);
        else  UNSET_KEY(cpc_kbd[CPC.keyboard][CODE_MASK(code)]);
      }
      else if (code & SPC)
      {
        switch (CODE_MASK(code))
        {
        case SPC_MENU:
          if (on) return 1;
          break;
        case SPC_SHOW_KEYS:
          if (ShowKybd != on && Cpc464Kybd)
          {
            if (on) pspKybdReinit(Cpc464Kybd);
            else { pspKybdReleaseAll(Cpc464Kybd); ClearScreen = 1; }
          }
          ShowKybd = on;
          break;
        }
      }
    }
  }

  return 0;
}

static void HandleKeyInput(unsigned int code, int on)
{
  if (on) SET_KEY(code);
  else  UNSET_KEY(code);
}

/* Run emulation */
void RunEmulation()
{
  float ratio;

  /* Recompute screen size/position */
  switch (Options.DisplayMode)
  {
  default:
  case DISPLAY_MODE_UNSCALED:
    ScreenW = Screen->Viewport.Width;
    ScreenH = Screen->Viewport.Height;
    break;
  case DISPLAY_MODE_FIT_HEIGHT:
    ratio = (float)SCR_HEIGHT / (float)Screen->Viewport.Height;
    ScreenW = (int)((float)Screen->Viewport.Width * ratio) - 2;
    ScreenH = SCR_HEIGHT;
    break;
  case DISPLAY_MODE_FILL_SCREEN:
    ScreenW = SCR_WIDTH - 3;
    ScreenH = SCR_HEIGHT;
    break;
  }

  ScreenX = (SCR_WIDTH / 2) - (ScreenW / 2);
  ScreenY = (SCR_HEIGHT / 2) - (ScreenH / 2);

  /* Recompute update frequency */
  TicksPerSecond = sceRtcGetTickResolution();
  if (Options.LimitSpeed)
  {
    TicksPerUpdate = TicksPerSecond
      / (Options.LimitSpeed / (Options.Frameskip + 1));
    sceRtcGetCurrentTick(&LastTick);
  }

  Frame = 0;
  ShowKybd = 0;
  ClearScreen = 1;

  pspImageClear(Screen, 0);

  /* Init performance counter */
  pspPerfInitFps(&FpsCounter);

  /* Clear keyboard matrix */
  memset(keyboard_matrix, 0xff, sizeof(keyboard_matrix));

  /* Resume sound */
  pspAudioSetChannelCallback(0, AudioCallback, 0);

  static int z80_exit = EC_FRAME_COMPLETE;
  u64 current_tick;

  while (!ExitPSP)
  {
    /* Process key events */
    if (ParseInput()) break;

    /* Wait if needed */
    if (z80_exit == EC_CYCLE_COUNT && Options.LimitSpeed)
    {
      do { sceRtcGetCurrentTick(&current_tick); }
      while (current_tick - LastTick < TicksPerUpdate);
      LastTick = current_tick;
    }

    if (z80_exit == EC_FRAME_COMPLETE)
      CPC.scr_base = (dword *)Screen->Pixels;

    z80_exit = z80_execute(); // run the emulation until an exit condition is met

    if (z80_exit == EC_FRAME_COMPLETE) RenderVideo();
  }

  /* Pause sound */
  pspAudioSetChannelCallback(0, NULL, 0);
}

/* Release emulation resources */
void TrashEmulation()
{
  /* Destroy canvas */
  if (Screen) pspImageDestroy(Screen);

  /* Destroy keyboard layout */
  if (Cpc464Kybd) pspKybdDestroyLayout(Cpc464Kybd);

  printer_stop();
  emulator_shutdown();

  dsk_eject(&driveA);
  dsk_eject(&driveB);
  tape_eject();

  audio_shutdown();
}

static void RenderVideo()
{
  /* Frame skipping */
  if (++Frame <= Options.Frameskip) return;
  else Frame = 0;

  /* Update the display */
  pspVideoBegin();

  /* Clear the buffer first, if necessary */
  if (ClearScreen >= 0)
  {
    ClearScreen--;
    pspVideoClearScreen();
  }

  /* Draw the screen */
  pspVideoPutImage(Screen, ScreenX, ScreenY, ScreenW, ScreenH);

  /* Draw keyboard */
  if (ShowKybd) pspKybdRender(Cpc464Kybd);

  /* Show FPS counter */
  if (Options.ShowFps)
  {
    static char fps_display[32];
    sprintf(fps_display, " %3.02f", pspPerfGetFps(&FpsCounter));

    int width = pspFontGetTextWidth(&PspStockFont, fps_display);
    int height = pspFontGetLineHeight(&PspStockFont);

    pspVideoFillRect(SCR_WIDTH - width, 0, SCR_WIDTH, height, PSP_COLOR_BLACK);
    pspVideoPrint(&PspStockFont, SCR_WIDTH - width, 0, fps_display, PSP_COLOR_WHITE);
  }

  pspVideoEnd();

  /* Wait for VSync signal */
  if (Options.VSync) pspVideoWaitVSync();

  /* Swap buffers */
  pspVideoSwapBuffers();
}

static void AudioCallback(void* buf, unsigned int *length, void *userdata)
{
  memcpy(buf, pbSndBuffer, CPC.snd_buffersize);
}
