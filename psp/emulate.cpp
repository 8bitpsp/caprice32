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

#include "menu.h"
#include "emulate.h"

#include "cap32.h"
#include "z80.h"

extern t_CPC CPC;
extern t_drive driveA;
extern t_drive driveB;
extern t_VDU VDU;
extern byte *pbSndBuffer;
extern byte bit_values[8];
extern byte keyboard_matrix[16];

int video_init();
int audio_init();
void audio_shutdown();

#define MAX_DISK_FORMAT     2
#define DEF_SPEED_SETTING   4
#define DEFAULT_DISK_FORMAT 0

dword freq_table[] = { 44100, 44100, 44100, 44100, 44100 };

PspImage *Screen = NULL;

static PspFpsCounter FpsCounter;
static int ScreenX, ScreenY, ScreenW, ScreenH;
static int ClearScreen;

static void RenderVideo();
static void AudioCallback(void* buf, unsigned int *length, void *userdata);

FILE*foo; /* TODO */

/* Initialize emulation */
int InitEmulation()
{
  /* TODO: try 256 if this works */
  if (!(Screen = pspImageCreateVram(512, CPC_VISIBLE_SCR_HEIGHT, 
    PSP_IMAGE_16BPP))) 
      return 0;

  Screen->Viewport.Width = CPC_VISIBLE_SCR_WIDTH;
  Screen->Viewport.Height = CPC_VISIBLE_SCR_HEIGHT;

/* TODO */
foo=fopen("log.txt","w");
fclose(foo);

  int i;

  memset(&CPC, 0, sizeof(CPC));

  CPC.model = 2; // CPC 6128
  CPC.jumpers = 0x1e; // OEM is Amstrad, video refresh is 50Hz
  CPC.ram_size = 128 & 0x02c0; // 128KB RAM
  CPC.limit_speed = 1;
  CPC.auto_pause = 1;
  CPC.printer = 0;
  CPC.mf2 = 0;
  CPC.keyboard = 0;
  CPC.joysticks = 0;

  CPC.scr_vsync = 0;
  CPC.scr_led = 1;
  CPC.scr_fps = 0;
  CPC.scr_tube = 0;
  CPC.scr_intensity = 10;
  CPC.scr_remanency = 0;
  CPC.scr_window = 0;

  CPC.snd_enabled = 1;
  CPC.snd_playback_rate = 2;
  CPC.snd_bits = 1;
  CPC.snd_stereo = 1;
  CPC.snd_volume = 80;
  CPC.snd_pp_device = 0;

  CPC.kbd_layout = 0;

  CPC.max_tracksize = 6144-154;
  CPC.snap_zip = 0;
  CPC.drvA_zip = 0;
  CPC.drvA_format = DEFAULT_DISK_FORMAT;
  CPC.drvB_zip = 0;
  CPC.drvB_format = DEFAULT_DISK_FORMAT;

  strcpy(CPC.snap_file, "");
  strcpy(CPC.tape_file, "");

  strcpy(CPC.snap_path, "/snap");
  strcpy(CPC.drvB_path, "/disk");
  strcpy(CPC.tape_path, "/tape");
  strcpy(CPC.drvA_path, "/disk");

  CPC.tape_zip = 0;

  strcpy(CPC.printer_file, "/printer.dat");
  strcpy(CPC.sdump_file, "/screen.png");
  strcpy(CPC.rom_path, ".");
  strcpy(CPC.rom_mf2, "");

  for (i = 0; i < 16; i++) strcpy(CPC.rom_file[i], "");

//  strcpy(CPC.rom_file[0], "sorcery.sna");

  z80_init_tables(); // init Z80 emulation

  if (video_init()) return 0;
  if (audio_init()) return 0;
  if (emulator_init()) return 0;

  memset(&driveA, 0, sizeof(t_drive)); // clear disk drive A data structure
  memset(&driveB, 0, sizeof(t_drive)); // clear disk drive A data structure

  return 1;
}

/* Process input */
int ParseInput()
{
  static SceCtrlData pad;

  if (!pspCtrlPollControls(&pad))
    return 0;

  keyboard_matrix[0x57 >> 4] |= bit_values[0x57 & 7]; // space
  keyboard_matrix[0x02 >> 4] |= bit_values[0x02 & 7]; // down 
  keyboard_matrix[0x10 >> 4] |= bit_values[0x10 & 7]; // left
  keyboard_matrix[0x01 >> 4] |= bit_values[0x01 & 7]; // right
  keyboard_matrix[0x00 >> 4] |= bit_values[0x00 & 7]; // up

  if (pad.Buttons & PSP_CTRL_CIRCLE)
    keyboard_matrix[0x57 >> 4] &= ~bit_values[0x57 & 7];
  if (pad.Buttons & PSP_CTRL_DOWN)
    keyboard_matrix[0x02 >> 4] &= ~bit_values[0x02 & 7];
  if (pad.Buttons & PSP_CTRL_LEFT)
    keyboard_matrix[0x10 >> 4] &= ~bit_values[0x10 & 7];
  if (pad.Buttons & PSP_CTRL_RIGHT)
    keyboard_matrix[0x01 >> 4] &= ~bit_values[0x01 & 7];
  if (pad.Buttons & PSP_CTRL_UP)
    keyboard_matrix[0x00 >> 4] &= ~bit_values[0x00 & 7];

  return 0;
}

/* Run emulation */
void RunEmulation()
{
  float ratio;

  /* Recompute screen size/position */
  switch (DISPLAY_MODE_UNSCALED) //Options.DisplayMode)
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

  ClearScreen = 1;

  /* Init performance counter */
  pspPerfInitFps(&FpsCounter);

  int z80_exit = EC_FRAME_COMPLETE;
  dword dwOffset;

/*TODO */
snapshot_load("sorcery.sna");

  /* Clear keyboard matrix */
  memset(keyboard_matrix, 0xff, sizeof(keyboard_matrix));

  /* Resume sound */
  pspAudioSetChannelCallback(0, AudioCallback, 0);

  while (!ExitPSP)
  {
    /* Process key events */
    if (ParseInput()) break;

    if (CPC.limit_speed)
    {
      if (CPC.snd_enabled)
      {
      }
      else if (z80_exit == EC_CYCLE_COUNT)
      {
      }
    }

    dwOffset = CPC.scr_pos - CPC.scr_base; // offset in current surface row
    if (VDU.scrln > 0)  // determine current position
      CPC.scr_base = (dword *)Screen->Pixels + (VDU.scrln * CPC.scr_line_offs);
    else CPC.scr_base = (dword *)Screen->Pixels; // reset to surface start
    CPC.scr_pos = CPC.scr_base + dwOffset; // update current rendering position

    z80_exit = z80_execute(); // run the emulation until an exit condition is met

    if (z80_exit == EC_FRAME_COMPLETE) RenderVideo();
  }

  /* Pause sound */
  pspAudioSetChannelCallback(0, NULL, 0);
}

/* Release emulation resources */
void TrashEmulation()
{
  if (Screen) pspImageDestroy(Screen);

  printer_stop();
  emulator_shutdown();

  dsk_eject(&driveA);
  dsk_eject(&driveB);
  tape_eject();

  audio_shutdown();
}

void RenderVideo()
{
  /* Update the display */
  pspVideoBegin();

  /* Clear the buffer first, if necessary */
  if (ClearScreen >= 0)
  {
    ClearScreen--;
    pspVideoClearScreen();
  }

/*
  if (Screen->Depth == PSP_IMAGE_INDEXED && bitmap.pal.update)
  {
    unsigned char r, g, b;
    unsigned short c, i;

    for(i = 0; i < PALETTE_SIZE; i++)
    {
      if (bitmap.pal.dirty[i])
      {
        r = bitmap.pal.color[i][0];
        g = bitmap.pal.color[i][1];
        b = bitmap.pal.color[i][2];
        c = MAKE_PIXEL(r,g,b);

        Screen->Palette[i] = c;
        Screen->Palette[i|0x20] = c;
        Screen->Palette[i|0x40] = c;
        Screen->Palette[i|0x60] = c;
      }
    }
  }
*/
  /* Draw the screen */
  pspVideoPutImage(Screen, ScreenX, ScreenY, ScreenW, ScreenH);

  /* Show FPS counter */
  if (1)
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
//  if (Options.VSync) pspVideoWaitVSync();

  /* Swap buffers */
  pspVideoSwapBuffers();
}

void AudioCallback(void* buf, unsigned int *length, void *userdata)
{
  memcpy(buf, pbSndBuffer, CPC.snd_buffersize);
}
