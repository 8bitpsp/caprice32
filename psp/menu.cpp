#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pspkernel.h>
#include <zlib.h>

#include "cap32_psp.h"
#include "psp/menu.h"
#include "langres.h"
#include "emulate.h"

#include "video.h"
#include "image.h"
#include "ui.h"
#include "ctrl.h"
#include "psp.h"
#include "util.h"
#include "init.h"
#include "file.h"

/*
#define TAB_QUICKLOAD 0
#define TAB_STATE     1
#define TAB_CONTROL   2
#define TAB_OPTION    3
#define TAB_SYSTEM    4
#define TAB_ABOUT     5
#define TAB_MAX       TAB_SYSTEM
*/
#define TAB_QUICKLOAD 0
#define TAB_STATE     1
#define TAB_CONTROL   2
#define TAB_OPTION    3
#define TAB_ABOUT     4
#define TAB_MAX       TAB_OPTION

#define OPTION_DISPLAY_MODE 1
#define OPTION_FRAMESKIP    2
#define OPTION_VSYNC        3
#define OPTION_CLOCK_FREQ   4
#define OPTION_SHOW_FPS     5
#define OPTION_CONTROL_MODE 6
#define OPTION_FRAME_LIMIT  7
#define OPTION_ANIMATE      8

extern struct GameConfig ActiveGameConfig;
extern PspImage *Screen;

/* Default configuration */
struct GameConfig DefaultConfig =
{
  {
    KBD|CPC_J0_UP,    /* Analog Up    */
    KBD|CPC_J0_DOWN,  /* Analog Down  */
    KBD|CPC_J0_LEFT,  /* Analog Left  */
    KBD|CPC_J0_RIGHT, /* Analog Right */
    KBD|CPC_CUR_UP,   /* D-pad Up     */
    KBD|CPC_CUR_DOWN, /* D-pad Down   */
    KBD|CPC_CUR_LEFT, /* D-pad Left   */
    KBD|CPC_CUR_RIGHT,/* D-pad Right  */
    KBD|CPC_J0_FIRE1, /* Square       */
    KBD|CPC_J0_FIRE2, /* Cross        */
    KBD|CPC_SPACE,    /* Circle       */
    0,                /* Triangle     */
    0,                /* L Trigger    */
    SPC|SPC_SHOW_KEYS,/* R Trigger    */
    0,                /* Select       */
    0,                /* Start        */
    SPC|SPC_MENU,     /* L+R Triggers */
    0,                /* Start+Select */
  }
};

EmulatorOptions Options;

static const char 
  *OptionsFile = "options.ini",
  *ScreenshotDir = "screens",
  *EmptyCartName = "BASIC",
  *SaveStateDir = "savedata",
  *ConfigDir = "config",
  *DefaultConfigFile = "default",
  *QuickloadFilter[] = { "DSK", '\0' },
  PresentSlotText[] = RES_S_PRESENT_SLOT_TEXT,
  EmptySlotText[]   = RES_S_EMPTY_SLOT_TEXT,
  ControlHelpText[] = RES_S_CONTROL_HELP_TEXT;

/* Option definitions */
static const PspMenuOptionDef
  ToggleOptions[] = {
    MENU_OPTION(RES_S_DISABLED, 0),
    MENU_OPTION(RES_S_ENABLED,  1),
    MENU_END_OPTIONS
  },
  ScreenSizeOptions[] = {
    MENU_OPTION(RES_S_ACTUAL_SIZE, DISPLAY_MODE_UNSCALED),
    MENU_OPTION(RES_S_FIT_SCREEN,  DISPLAY_MODE_FILL_SCREEN),
    MENU_END_OPTIONS
  },
  FrameSkipOptions[] = {
    MENU_OPTION(RES_S_SKIP_FRAMES_0, 0),
    MENU_OPTION(RES_S_SKIP_FRAMES_1, 1),
    MENU_OPTION(RES_S_SKIP_FRAMES_2, 2),
    MENU_OPTION(RES_S_SKIP_FRAMES_3, 3),
    MENU_OPTION(RES_S_SKIP_FRAMES_4, 4),
    MENU_OPTION(RES_S_SKIP_FRAMES_5, 5),
    MENU_END_OPTIONS
  },
  FrameLimitOptions[] = {
    MENU_OPTION(RES_S_DISABLED,   0),
    MENU_OPTION(RES_S_50FPS_PAL, 50),
    MENU_END_OPTIONS
  },
  PspClockFreqOptions[] = {
    MENU_OPTION(RES_S_222_MHZ, 222),
    MENU_OPTION(RES_S_266_MHZ, 266),
    MENU_OPTION(RES_S_300_MHZ, 300),
    MENU_OPTION(RES_S_333_MHZ, 333),
    MENU_END_OPTIONS
  },
  ControlModeOptions[] = {
    MENU_OPTION(RES_S_UI_US,    0),
    MENU_OPTION(RES_S_UI_JAPAN, 1),
    MENU_END_OPTIONS
  },
  ButtonMapOptions[] = {
    /* Unmapped */
    MENU_OPTION(RES_S_NONE, 0),
    /* Special */
    MENU_OPTION(RES_S_SPECIAL_OPEN_MENU, SPC|SPC_MENU),
    /* Joystick */
    MENU_OPTION(RES_S_JOY1_UP,   KBD|CPC_J0_UP), 
    MENU_OPTION(RES_S_JOY1_DOWN, KBD|CPC_J0_DOWN), 
    MENU_OPTION(RES_S_JOY1_LEFT, KBD|CPC_J0_LEFT), 
    MENU_OPTION(RES_S_JOY1_RIGHT,KBD|CPC_J0_RIGHT), 
    MENU_OPTION(RES_S_JOY1_FIRE1,KBD|CPC_J0_FIRE1), 
    MENU_OPTION(RES_S_JOY1_FIRE2,KBD|CPC_J0_FIRE2), 
    /* Keyboard: Cursor */
    MENU_OPTION(RES_S_KBD_UP,   KBD|CPC_CUR_UP), 
    MENU_OPTION(RES_S_KBD_DOWN, KBD|CPC_CUR_DOWN), 
    MENU_OPTION(RES_S_KBD_LEFT, KBD|CPC_CUR_LEFT), 
    MENU_OPTION(RES_S_KBD_RIGHT,KBD|CPC_CUR_RIGHT), 
    /* Keyboard: Numbers */
    MENU_OPTION("0", KBD|CPC_0), MENU_OPTION("1", KBD|CPC_1), 
    MENU_OPTION("2", KBD|CPC_2), MENU_OPTION("3", KBD|CPC_3), 
    MENU_OPTION("4", KBD|CPC_4), MENU_OPTION("5", KBD|CPC_5), 
    MENU_OPTION("6", KBD|CPC_6), MENU_OPTION("7", KBD|CPC_7), 
    MENU_OPTION("8", KBD|CPC_8), MENU_OPTION("9", KBD|CPC_9), 
    /* Keyboard: Letters */
    MENU_OPTION("a", KBD|CPC_a), MENU_OPTION("b", KBD|CPC_b), 
    MENU_OPTION("c", KBD|CPC_c), MENU_OPTION("d", KBD|CPC_d), 
    MENU_OPTION("e", KBD|CPC_e), MENU_OPTION("f", KBD|CPC_f), 
    MENU_OPTION("g", KBD|CPC_g), MENU_OPTION("h", KBD|CPC_h), 
    MENU_OPTION("i", KBD|CPC_i), MENU_OPTION("j", KBD|CPC_j), 
    MENU_OPTION("k", KBD|CPC_k), MENU_OPTION("l", KBD|CPC_l), 
    MENU_OPTION("m", KBD|CPC_m), MENU_OPTION("n", KBD|CPC_n), 
    MENU_OPTION("o", KBD|CPC_o), MENU_OPTION("p", KBD|CPC_p), 
    MENU_OPTION("q", KBD|CPC_q), MENU_OPTION("r", KBD|CPC_r), 
    MENU_OPTION("s", KBD|CPC_s), MENU_OPTION("t", KBD|CPC_t), 
    MENU_OPTION("u", KBD|CPC_u), MENU_OPTION("v", KBD|CPC_v), 
    MENU_OPTION("w", KBD|CPC_w), MENU_OPTION("x", KBD|CPC_x), 
    MENU_OPTION("y", KBD|CPC_y), MENU_OPTION("z", KBD|CPC_z), 
    /* Keyboard: Symbols */
    MENU_OPTION("@", KBD|CPC_AT), MENU_OPTION("\\",KBD|CPC_BACKSLASH), 
    MENU_OPTION(":", KBD|CPC_COLON), MENU_OPTION(",", KBD|CPC_COMMA), 
    MENU_OPTION(".", KBD|CPC_FPERIOD), MENU_OPTION("[", KBD|CPC_LBRACKET), 
    MENU_OPTION("-", KBD|CPC_MINUS), MENU_OPTION(".", KBD|CPC_PERIOD), 
    MENU_OPTION("]", KBD|CPC_RBRACKET), MENU_OPTION(";", KBD|CPC_SEMICOLON), 
    MENU_OPTION(";", KBD|CPC_SEMICOLON), MENU_OPTION("/", KBD|CPC_SLASH), 
    /* Etc... */
    MENU_OPTION(RES_S_CAPS_LOCK, KBD|CPC_CAPSLOCK), 
    MENU_OPTION(RES_S_CLEAR,     KBD|CPC_CLR),
    MENU_OPTION(RES_S_CONTROL,   KBD|CPC_CONTROL), 
    MENU_OPTION(RES_S_COPY,      KBD|CPC_COPY),
    MENU_OPTION(RES_S_DELETE,    KBD|CPC_DEL), 
    MENU_OPTION(RES_S_ENTER,     KBD|CPC_ENTER),
    MENU_OPTION(RES_S_ESC,       KBD|CPC_ESC), 
    MENU_OPTION(RES_S_LSHIFT,    KBD|CPC_LSHIFT),
    MENU_OPTION(RES_S_POWER,     KBD|CPC_POWER),
    MENU_OPTION(RES_S_RETURN,    KBD|CPC_RETURN),
    MENU_OPTION(RES_S_RSHIFT,    KBD|CPC_RSHIFT),
    MENU_OPTION(RES_S_SPACE,     KBD|CPC_SPACE),
    MENU_OPTION(RES_S_TAB,       KBD|CPC_TAB),
    /* Keyboard: Function keys */
    MENU_OPTION("F1", KBD|CPC_F1), MENU_OPTION("F2", KBD|CPC_F2), 
    MENU_OPTION("F3", KBD|CPC_F3), MENU_OPTION("F4", KBD|CPC_F4), 
    MENU_OPTION("F5", KBD|CPC_F5), MENU_OPTION("F6", KBD|CPC_F6), 
    MENU_OPTION("F7", KBD|CPC_F7), MENU_OPTION("F8", KBD|CPC_F8), 
    MENU_OPTION("F9", KBD|CPC_F9), MENU_OPTION("F0", KBD|CPC_F0), 
    MENU_END_OPTIONS
  };

/* Menu definitions */
static const PspMenuItemDef
  ControlMenuDef[] = {
    MENU_ITEM(PSP_CHAR_ANALUP, CONTROL_ANALOG_UP, ButtonMapOptions, -1, ControlHelpText),
    MENU_ITEM(PSP_CHAR_ANALDOWN, CONTROL_ANALOG_DOWN, ButtonMapOptions, -1, ControlHelpText),
    MENU_ITEM(PSP_CHAR_ANALLEFT, CONTROL_ANALOG_LEFT, ButtonMapOptions, -1, ControlHelpText),
    MENU_ITEM(PSP_CHAR_ANALRIGHT, CONTROL_ANALOG_RIGHT, ButtonMapOptions, -1, ControlHelpText),
    MENU_ITEM(PSP_CHAR_UP, CONTROL_BUTTON_UP, ButtonMapOptions, -1, ControlHelpText),
    MENU_ITEM(PSP_CHAR_DOWN, CONTROL_BUTTON_DOWN, ButtonMapOptions, -1, ControlHelpText),
    MENU_ITEM(PSP_CHAR_LEFT, CONTROL_BUTTON_LEFT, ButtonMapOptions, -1, ControlHelpText),
    MENU_ITEM(PSP_CHAR_RIGHT, CONTROL_BUTTON_RIGHT, ButtonMapOptions, -1, ControlHelpText),
    MENU_ITEM(PSP_CHAR_SQUARE, CONTROL_BUTTON_SQUARE, ButtonMapOptions, -1, ControlHelpText),
    MENU_ITEM(PSP_CHAR_CROSS, CONTROL_BUTTON_CROSS, ButtonMapOptions, -1, ControlHelpText),
    MENU_ITEM(PSP_CHAR_CIRCLE, CONTROL_BUTTON_CIRCLE, ButtonMapOptions, -1, ControlHelpText),
    MENU_ITEM(PSP_CHAR_TRIANGLE, CONTROL_BUTTON_TRIANGLE, ButtonMapOptions, -1, ControlHelpText),
    MENU_ITEM(PSP_CHAR_LTRIGGER, CONTROL_BUTTON_LTRIGGER, ButtonMapOptions, -1, ControlHelpText),
    MENU_ITEM(PSP_CHAR_RTRIGGER, CONTROL_BUTTON_RTRIGGER, ButtonMapOptions, -1, ControlHelpText),
    MENU_ITEM(PSP_CHAR_SELECT, CONTROL_BUTTON_SELECT, ButtonMapOptions, -1, ControlHelpText),
    MENU_ITEM(PSP_CHAR_START, CONTROL_BUTTON_START, ButtonMapOptions, -1, ControlHelpText),
    MENU_ITEM(PSP_CHAR_LTRIGGER"+"PSP_CHAR_RTRIGGER, CONTROL_BUTTON_LRTRIGGERS, ButtonMapOptions, -1, ControlHelpText),
    MENU_ITEM(PSP_CHAR_START"+"PSP_CHAR_SELECT, CONTROL_BUTTON_STARTSELECT, ButtonMapOptions, -1, ControlHelpText),
    MENU_END_ITEMS
  },
  OptionMenuDef[] = {
    MENU_HEADER(RES_S_VIDEO),
    MENU_ITEM(RES_S_SCREEN_SIZE,OPTION_DISPLAY_MODE,ScreenSizeOptions,-1,RES_S_CHANGE_SCREEN_SIZE),
    MENU_HEADER(RES_S_PERF),
    MENU_ITEM(RES_S_FRAME_LIMITER,OPTION_FRAME_LIMIT,FrameLimitOptions,-1,RES_S_CHANGE_SCR_UPD_FREQ),
    MENU_ITEM(RES_S_FRAME_SKIPPING,OPTION_FRAMESKIP,FrameSkipOptions,-1,RES_S_CHANGE_FRAME_SKIP_PER_UPD),
    MENU_ITEM(RES_S_VSYNC,OPTION_VSYNC,ToggleOptions,-1,RES_S_ENABLE_TO_REDUCE_TEARING),
    MENU_ITEM(RES_S_PSP_CLOCK_FREQ,OPTION_CLOCK_FREQ,PspClockFreqOptions,-1,RES_S_LARGER_FASTER_DEPL),
    MENU_ITEM(RES_S_SHOW_FPS,OPTION_SHOW_FPS,ToggleOptions,-1,RES_S_SHOW_HIDE_FPS),
    MENU_HEADER(RES_S_MENU),
    MENU_ITEM(RES_S_UI_MODE,OPTION_CONTROL_MODE,ControlModeOptions,-1,RES_S_CHANGE_OK_CANCEL),
    MENU_ITEM(RES_S_ANIMATIONS,OPTION_ANIMATE,ToggleOptions,-1,RES_S_ENABLE_DISABLE_ANIM),
    MENU_END_ITEMS
  };

/* Tab labels */
/*
static const char *TabLabel[] = 
{ RES_S_GAME_TAB, RES_S_SAVE_LOAD_TAB, RES_S_CONTROL_TAB, RES_S_OPTION_TAB,
RES_S_SYSTEM_TAB, RES_S_ABOUT_TAB };
*/
static const char *TabLabel[] = 
{
  "Game",
  "Save/Load",
/*
  "System",
*/
  "Controls",
  "Options",
  "About"
};

static PspImage* LoadStateIcon(const char *path);
static int LoadState(const char *path);
static PspImage* SaveState(const char *path, PspImage *icon);

static void ShowControlTab();
static void ShowStateTab();

static void LoadOptions();
static int  SaveOptions();

static void InitGameConfig(GameConfig *config);
static int  LoadGameConfig(const char *config_name, GameConfig *config);
static int  SaveGameConfig(const char *config_name, const GameConfig *config);

/* Interface callbacks */
static int  OnGenericCancel(const void *uiobject, const void *param);
static void OnGenericRender(const void *uiobject, const void *item_obj);
static int  OnGenericButtonPress(const PspUiFileBrowser *browser, 
                                 const char *path, u32 button_mask);

static int OnSplashButtonPress(const struct PspUiSplash *splash, 
                               u32 button_mask);
static void OnSplashRender(const void *uiobject, const void *null);

static int OnQuickloadOk(const void *browser, const void *path);

static int OnMenuItemChanged(const struct PspUiMenu *uimenu, PspMenuItem* item, 
                             const PspMenuOption* option);
static int OnMenuButtonPress(const struct PspUiMenu *uimenu, 
                             PspMenuItem* sel_item, u32 button_mask);
static int OnMenuOk(const void *uimenu, const void* sel_item);

static int OnSaveStateOk(const void *gallery, const void *item);
static int OnSaveStateButtonPress(const PspUiGallery *gallery, 
                                  PspMenuItem* item, u32 button_mask);

static int TabIndex;
static int ResumeEmulation;
static PspImage *Background;
static PspImage *NoSaveIcon;

static char *LoadedGame;
static char *GamePath;
static char *SaveStatePath;
static char *ConfigPath;
char *ScreenshotPath;

/* UI object declarations */
PspUiSplash SplashScreen =
{
  OnSplashRender,
  OnGenericCancel,
  OnSplashButtonPress,
  NULL
};

PspUiFileBrowser QuickloadBrowser = 
{
  OnGenericRender,
  OnQuickloadOk,
  OnGenericCancel,
  OnGenericButtonPress,
  QuickloadFilter,
  0
};

PspUiMenu OptionUiMenu =
{
  NULL,                  /* PspMenu */
  OnGenericRender,       /* OnRender() */
  OnMenuOk,              /* OnOk() */
  OnGenericCancel,       /* OnCancel() */
  OnMenuButtonPress,     /* OnButtonPress() */
  OnMenuItemChanged,     /* OnItemChanged() */
};

PspUiMenu ControlUiMenu =
{
  NULL,                  /* PspMenu */
  OnGenericRender,       /* OnRender() */
  OnMenuOk,              /* OnOk() */
  OnGenericCancel,       /* OnCancel() */
  OnMenuButtonPress,     /* OnButtonPress() */
  OnMenuItemChanged,     /* OnItemChanged() */
};

PspUiGallery SaveStateGallery = 
{
  NULL,                        /* PspMenu */
  OnGenericRender,             /* OnRender() */
  OnSaveStateOk,               /* OnOk() */
  OnGenericCancel,             /* OnCancel() */
  OnSaveStateButtonPress,      /* OnButtonPress() */
  NULL                         /* Userdata */
};

int InitMenu()
{
  /* Reset variables */
  TabIndex = TAB_ABOUT;
  Background = NULL;
  NoSaveIcon = NULL;
  LoadedGame = NULL;
  GamePath = NULL;

  if (!InitEmulation())
    return 0;

  /* Initialize paths */
  if ((SaveStatePath = (char*)malloc(sizeof(char) 
    * (strlen(pspGetAppDirectory()) + strlen(SaveStateDir) + 2))))
  { sprintf(SaveStatePath, "%s%s/", pspGetAppDirectory(), SaveStateDir); }
  if ((ScreenshotPath = (char*)malloc(sizeof(char) 
    * (strlen(pspGetAppDirectory()) + strlen(ScreenshotDir) + 2))))
  { sprintf(ScreenshotPath, "%s%s/", pspGetAppDirectory(), ScreenshotDir); }
  if ((ConfigPath = (char*)malloc(sizeof(char) 
    * (strlen(pspGetAppDirectory()) + strlen(ConfigDir) + 2))))
  { sprintf(ConfigPath, "%s%s/", pspGetAppDirectory(), ConfigDir); }

  LoadOptions();

  /* Initialize UI components */
  UiMetric.Font = &PspStockFont;
  UiMetric.Left = 8;
  UiMetric.Top = 24;
  UiMetric.Right = 472;
  UiMetric.Bottom = 250;
  UiMetric.OkButton = (!Options.ControlMode) 
      ? PSP_CTRL_CROSS : PSP_CTRL_CIRCLE;
  UiMetric.CancelButton = (!Options.ControlMode) 
      ? PSP_CTRL_CIRCLE : PSP_CTRL_CROSS;
  UiMetric.ScrollbarColor = PSP_COLOR_GRAY;
  UiMetric.ScrollbarBgColor = 0x44ffffff;
  UiMetric.ScrollbarWidth = 10;
  UiMetric.TextColor = PSP_COLOR_GRAY;
  UiMetric.SelectedColor = PSP_COLOR_YELLOW;
  UiMetric.SelectedBgColor = COLOR(0xff,0xff,0xff,0x88);
  UiMetric.StatusBarColor = PSP_COLOR_WHITE;
  UiMetric.BrowserFileColor = PSP_COLOR_GRAY;
  UiMetric.BrowserDirectoryColor = PSP_COLOR_YELLOW;
  UiMetric.GalleryIconsPerRow = 5;
  UiMetric.GalleryIconMarginWidth = 8;
  UiMetric.MenuItemMargin = 20;
  UiMetric.MenuSelOptionBg = PSP_COLOR_BLACK;
  UiMetric.MenuOptionBoxColor = PSP_COLOR_GRAY;
  UiMetric.MenuOptionBoxBg = COLOR(0, 0, 33, 0xBB);
  UiMetric.MenuDecorColor = PSP_COLOR_YELLOW;
  UiMetric.TitlePadding = 4;
  UiMetric.TitleColor = PSP_COLOR_WHITE;
  UiMetric.MenuFps = 30;
  UiMetric.TabBgColor = COLOR(0xcc,0xdb,0xe3, 0xff);
  UiMetric.Animate = 1;

  /* Load the background image */
  Background = pspImageLoadPng("background.png");
  UiMetric.Background = Background;

  /* Init NoSaveState icon image */
  NoSaveIcon=pspImageCreate(168, 120, PSP_IMAGE_16BPP);
  pspImageClear(NoSaveIcon, RGB(0x29,0x29,0x29));

  /* Initialize control menu */
  ControlUiMenu.Menu = pspMenuCreate();
  pspMenuLoad(ControlUiMenu.Menu, ControlMenuDef);

  /* Initialize options menu */
  OptionUiMenu.Menu = pspMenuCreate();
  pspMenuLoad(OptionUiMenu.Menu, OptionMenuDef);

  /* Initialize state menu */
  SaveStateGallery.Menu = pspMenuCreate();
  int i;
  PspMenuItem *item;
  for (i = 0; i < 10; i++)
  {
    item = pspMenuAppendItem(SaveStateGallery.Menu, NULL, i);
    pspMenuSetHelpText(item, EmptySlotText);
  }

  /* Load default configuration */
  LoadGameConfig(DefaultConfigFile, &DefaultConfig);
  LoadGameConfig(NULL, &ActiveGameConfig);

  return 1;
}

void DisplayMenu()
{
  PspMenuItem *item;

  /* Menu loop */
  do
  {
    ResumeEmulation = 0;

    /* Set normal clock frequency */
    pspSetClockFrequency(222);
    /* Set buttons to autorepeat */
    pspCtrlSetPollingMode(PSP_CTRL_AUTOREPEAT);

    /* Display appropriate tab */
    switch (TabIndex)
    {
    case TAB_QUICKLOAD:
      pspUiOpenBrowser(&QuickloadBrowser, (LoadedGame) ? LoadedGame : GamePath);
      break;
    case TAB_OPTION:
      /* Init menu options */
      item = pspMenuFindItemById(OptionUiMenu.Menu, OPTION_DISPLAY_MODE);
      pspMenuSelectOptionByValue(item, (void*)Options.DisplayMode);
      item = pspMenuFindItemById(OptionUiMenu.Menu, OPTION_FRAMESKIP);
      pspMenuSelectOptionByValue(item, (void*)(int)Options.Frameskip);
      item = pspMenuFindItemById(OptionUiMenu.Menu, OPTION_VSYNC);
      pspMenuSelectOptionByValue(item, (void*)Options.VSync);
      item = pspMenuFindItemById(OptionUiMenu.Menu, OPTION_CLOCK_FREQ);
      pspMenuSelectOptionByValue(item, (void*)Options.ClockFreq);
      item = pspMenuFindItemById(OptionUiMenu.Menu, OPTION_SHOW_FPS);
      pspMenuSelectOptionByValue(item, (void*)Options.ShowFps);
      item = pspMenuFindItemById(OptionUiMenu.Menu, OPTION_CONTROL_MODE);
      pspMenuSelectOptionByValue(item, (void*)Options.ControlMode);
      item = pspMenuFindItemById(OptionUiMenu.Menu, OPTION_ANIMATE);
      pspMenuSelectOptionByValue(item, (void*)UiMetric.Animate);
      item = pspMenuFindItemById(OptionUiMenu.Menu, OPTION_FRAME_LIMIT);
      pspMenuSelectOptionByValue(item, (void*)Options.LimitSpeed);
      pspUiOpenMenu(&OptionUiMenu, NULL);
      break;
    case TAB_CONTROL:
      ShowControlTab();
      break;
    case TAB_STATE:
      ShowStateTab();
      break;
/*
    case TAB_SYSTEM:
      item = pspMenuFindItemById(SystemUiMenu.Menu, SYSTEM_STEREO);
      pspMenuSelectOptionByValue(item, (void*)stereo_enabled);
      item = pspMenuFindItemById(SystemUiMenu.Menu, SYSTEM_DRIVE);
      pspMenuModifyOption(item->Options,
        pspFileIoGetFilename(sio_filename[0]), NULL);
      item = pspMenuFindItemById(SystemUiMenu.Menu, SYSTEM_MACHINE_TYPE);
      pspMenuSelectOptionByValue(item, (void*)(MACHINE_TYPE(machine_type, ram_size)));
      item = pspMenuFindItemById(SystemUiMenu.Menu, SYSTEM_CROP_SCREEN);
      pspMenuSelectOptionByValue(item, (void*)CropScreen);
      item = pspMenuFindItemById(SystemUiMenu.Menu, SYSTEM_TV_MODE);
      pspMenuSelectOptionByValue(item, (void*)tv_mode);
      pspUiOpenMenu(&SystemUiMenu, NULL);
      break;
*/
    case TAB_ABOUT:
      pspUiSplashScreen(&SplashScreen);
      break;
    }

    if (!ExitPSP)
    {
      /* Set clock frequency during emulation */
      pspSetClockFrequency(Options.ClockFreq);
      /* Set buttons to normal mode */
      pspCtrlSetPollingMode(PSP_CTRL_NORMAL);

      /* Resume emulation */
      if (ResumeEmulation)
      {
        if (UiMetric.Animate) pspUiFadeout();
        RunEmulation();
        if (UiMetric.Animate) pspUiFadeout();
      }
    }
  } while (!ExitPSP);
}

void OnSplashRender(const void *splash, const void *null)
{
  int fh, i, x, y, height;
  const char *lines[] = 
  { 
    PSP_APP_NAME" version "PSP_APP_VER" ("__DATE__")",
    "\026http://psp.akop.org/caprice32",
    " ",
    "2007 Akop Karapetyan (port)",
    "1997-2004 Ulrich Doewich (emulation)",
    NULL
  };

  fh = pspFontGetLineHeight(UiMetric.Font);

  for (i = 0; lines[i]; i++);
  height = fh * (i - 1);

  /* Render lines */
  for (i = 0, y = SCR_HEIGHT / 2 - height / 2; lines[i]; i++, y += fh)
  {
    x = SCR_WIDTH / 2 - pspFontGetTextWidth(UiMetric.Font, lines[i]) / 2;
    pspVideoPrint(UiMetric.Font, x, y, lines[i], PSP_COLOR_GRAY);
  }

  /* Render PSP status */
  OnGenericRender(splash, null);
}

int OnSplashButtonPress(const struct PspUiSplash *splash, 
                        u32 button_mask)
{
  return OnGenericButtonPress(NULL, NULL, button_mask);
}

int OnGenericCancel(const void *uiobject, const void* param)
{
  ResumeEmulation = 1;
  return 1;
}

/* Handles drawing of generic items */
void OnGenericRender(const void *uiobject, const void *item_obj)
{
  /* Draw tabs */
  int i, x, width, height = pspFontGetLineHeight(UiMetric.Font);
  for (i = 0, x = 5; i <= TAB_MAX; i++, x += width + 10)
  {
    width = -10;

    /* Determine width of text */
    width = pspFontGetTextWidth(UiMetric.Font, TabLabel[i]);

    /* Draw background of active tab */
    if (i == TabIndex)
      pspVideoFillRect(x - 5, 0, x + width + 5, height + 1, UiMetric.TabBgColor);

    /* Draw name of tab */
    pspVideoPrint(UiMetric.Font, x, 0, TabLabel[i], PSP_COLOR_WHITE);
  }
}

int OnGenericButtonPress(const PspUiFileBrowser *browser, const char *path, 
                         u32 button_mask)
{
  int tab_index;

  /* If L or R are pressed, switch tabs */
  if (button_mask & PSP_CTRL_LTRIGGER)
  {
    TabIndex--;
    do
    {
      tab_index = TabIndex;
      if (TabIndex < 0) TabIndex = TAB_MAX;
    } while (tab_index != TabIndex);
  }
  else if (button_mask & PSP_CTRL_RTRIGGER)
  {
    TabIndex++;
    do
    {
      tab_index = TabIndex;
      if (TabIndex > TAB_MAX) TabIndex = 0;
    } while (tab_index != TabIndex);
  }
  else if ((button_mask & (PSP_CTRL_START | PSP_CTRL_SELECT)) 
            == (PSP_CTRL_START | PSP_CTRL_SELECT))
  {
    if (pspUtilSaveVramSeq(ScreenshotPath, "ui"))
      pspUiAlert("Saved successfully");
    else pspUiAlert("ERROR: Screenshot not saved");
    return 0;
  }
  else return 0;

  return 1;
}

int OnQuickloadOk(const void *browser, const void *path)
{
  /* TODO: Load game */

  /* Reset loaded game */
  if (LoadedGame) free(LoadedGame);
  LoadedGame = strdup((char*)path);

  /* Reset current game path */
  if (GamePath) free(GamePath);
  GamePath = pspFileGetParentDirectory(LoadedGame);

  /* TODO: Load control set */
  // LoadGameConfig(LoadedGame, &ActiveGameConfig);

  ResumeEmulation = 1;
  // TODO: Coldstart();

  return 1;
}

int OnMenuItemChanged(const struct PspUiMenu *uimenu, PspMenuItem* item, 
                      const PspMenuOption* option)
{
  if (uimenu == &ControlUiMenu)
  {
    ActiveGameConfig.ButtonMap[item->ID] = (unsigned int)option->Value;
  }
  else if (uimenu == &OptionUiMenu)
  {
    switch(item->ID)
    {
    case OPTION_DISPLAY_MODE:
      Options.DisplayMode = (int)option->Value;
      break;
    case OPTION_FRAMESKIP:
      Options.Frameskip = (int)option->Value;
      break;
    case OPTION_VSYNC:
      Options.VSync = (int)option->Value;
      break;
    case OPTION_CLOCK_FREQ:
      Options.ClockFreq = (int)option->Value;
      break;
    case OPTION_FRAME_LIMIT:
      Options.LimitSpeed = (int)option->Value;
      break;
    case OPTION_SHOW_FPS:
      Options.ShowFps = (int)option->Value;
      break;
    case OPTION_ANIMATE:
      UiMetric.Animate = (int)option->Value;
      break;
    case OPTION_CONTROL_MODE:
      Options.ControlMode = (int)option->Value;
      UiMetric.OkButton = (Options.ControlMode) 
        ? PSP_CTRL_CIRCLE : PSP_CTRL_CROSS;
      UiMetric.CancelButton = (Options.ControlMode) 
        ? PSP_CTRL_CROSS : PSP_CTRL_CIRCLE;
      break;
    }
  }

  return 1;
}

int OnMenuOk(const void *uimenu, const void* sel_item)
{
  if (uimenu == &ControlUiMenu)
  {
    /* Save to MS */
    const char *config_name = (LoadedGame) 
        ? pspFileGetFilename(LoadedGame) : EmptyCartName;

    if (SaveGameConfig(config_name, &ActiveGameConfig)) 
      pspUiAlert("Layout saved successfully");
    else pspUiAlert("ERROR: Changes not saved");
  }

  return 0;
}

int OnMenuButtonPress(const struct PspUiMenu *uimenu, PspMenuItem* sel_item, 
                      u32 button_mask)
{
  if (uimenu == &ControlUiMenu)
  {
    if (button_mask & PSP_CTRL_SQUARE)
    {
      /* Save to MS as default mapping */
      if (!SaveGameConfig(DefaultConfigFile, &ActiveGameConfig))
        pspUiAlert("ERROR: Changes not saved");
      else
      {
        /* Modify in-memory defaults */
        memcpy(&DefaultConfig, &ActiveGameConfig, sizeof(GameConfig));
        pspUiAlert("Layout saved as default");
      }

      return 0;
    }
    else if (button_mask & PSP_CTRL_TRIANGLE)
    {
      PspMenuItem *item;
      int i;

      /* Load default mapping */
      InitGameConfig(&ActiveGameConfig);

      /* Modify the menu */
      for (item = ControlUiMenu.Menu->First, i = 0; item; item = item->Next, i++)
        pspMenuSelectOptionByValue(item, (void*)ActiveGameConfig.ButtonMap[i]);

      return 0;
    }
  }

  return OnGenericButtonPress(NULL, NULL, button_mask);
}

int OnSaveStateOk(const void *gallery, const void *item)
{
  char *path;
  const char *config_name;
  config_name = (LoadedGame) ? pspFileGetFilename(LoadedGame) : EmptyCartName;
  path = (char*)malloc(strlen(SaveStatePath) + strlen(config_name) + 8);
  sprintf(path, "%s%s.s%02i", SaveStatePath, config_name,
          ((const PspMenuItem*)item)->ID);

  if (pspFileCheckIfExists(path) && pspUiConfirm("Load state?"))
  {
    if (LoadState(path))
    {
      ResumeEmulation = 1;
      pspMenuFindItemById(((const PspUiGallery*)gallery)->Menu,
                            ((const PspMenuItem*)item)->ID);
      free(path);

      return 1;
    }
    pspUiAlert("ERROR: State failed to load");
  }

  free(path);
  return 0;
}

int OnSaveStateButtonPress(const PspUiGallery *gallery, PspMenuItem *sel, 
                           u32 button_mask)
{
  if (button_mask & PSP_CTRL_SQUARE || button_mask & PSP_CTRL_TRIANGLE)
  {
    char *path;
    char caption[32];
    const char *config_name;
    config_name = (LoadedGame) ? pspFileGetFilename(LoadedGame) : EmptyCartName;
    PspMenuItem *item = pspMenuFindItemById(gallery->Menu, sel->ID);

    path = (char*)malloc(strlen(SaveStatePath) + strlen(config_name) + 8);
    sprintf(path, "%s%s.s%02i", SaveStatePath, config_name, item->ID);

    do /* not a real loop; flow control construct */
    {
      if (button_mask & PSP_CTRL_SQUARE)
      {
        if (pspFileCheckIfExists(path) 
            && !pspUiConfirm("Overwrite existing state?")) break;

        pspUiFlashMessage("Saving, please wait ...");

        PspImage *icon;
        if (!(icon = SaveState(path, Screen)))
        {
          pspUiAlert("ERROR: State not saved");
          break;
        }

        SceIoStat stat;

        /* Trash the old icon (if any) */
        if (item->Icon && item->Icon != NoSaveIcon)
          pspImageDestroy((PspImage*)item->Icon);

        /* Update icon, help text */
        item->Icon = icon;
        pspMenuSetHelpText(item, PresentSlotText);

        /* Get file modification time/date */
        if (sceIoGetstat(path, &stat) < 0) sprintf(caption, "ERROR");
        else sprintf(caption, "%02i/%02i/%02i %02i:%02i", 
                     stat.st_mtime.month,
                     stat.st_mtime.day,
                     stat.st_mtime.year - (stat.st_mtime.year / 100) * 100,
                     stat.st_mtime.hour,
                     stat.st_mtime.minute);

        pspMenuSetCaption(item, caption);
      }
      else if (button_mask & PSP_CTRL_TRIANGLE)
      {
        if (!pspFileCheckIfExists(path) || !pspUiConfirm("Delete state?"))
          break;

        if (!pspFileDelete(path))
        {
          pspUiAlert("ERROR: State not deleted");
          break;
        }

        /* Trash the old icon (if any) */
        if (item->Icon && item->Icon != NoSaveIcon)
          pspImageDestroy((PspImage*)item->Icon);

        /* Update icon, caption */
        item->Icon = NoSaveIcon;
        pspMenuSetHelpText(item, EmptySlotText);
        pspMenuSetCaption(item, "Empty");
      }
    } while (0);

    if (path) free(path);
    return 0;
  }

  return OnGenericButtonPress(NULL, NULL, button_mask);
}

/* Load options */
void LoadOptions()
{
  char *path = (char*)malloc(sizeof(char) * (strlen(pspGetAppDirectory()) 
      + strlen(OptionsFile) + 1));
  sprintf(path, "%s%s", pspGetAppDirectory(), OptionsFile);

  /* Initialize INI structure */
  PspInit *init = pspInitCreate();

  /* Read the file */
  pspInitLoad(init, path);

  /* Load values */
  Options.DisplayMode = pspInitGetInt(init, "Video", "Display Mode", 
                                     DISPLAY_MODE_UNSCALED);
  Options.Frameskip = pspInitGetInt(init, "Video", "Frameskip", 0);
  Options.VSync     = pspInitGetInt(init, "Video", "VSync", 0);
  Options.LimitSpeed = pspInitGetInt(init, "Video", "Speed Limiter", 50);
  Options.ClockFreq = pspInitGetInt(init, "Video", "PSP Clock Frequency", 222);
  Options.ShowFps   = pspInitGetInt(init, "Video", "Show FPS", 0);
  Options.ControlMode = pspInitGetInt(init, "Menu", "Control Mode", 0);
  UiMetric.Animate = pspInitGetInt(init, "Menu",  "Animations", 1);

  if (GamePath) free(GamePath);
  GamePath = pspInitGetString(init, "File", "Game Path", NULL);

  /* Clean up */
  free(path);
  pspInitDestroy(init);
}

/* Save options */
int SaveOptions()
{
  char *path = (char*)malloc(sizeof(char) * (strlen(pspGetAppDirectory()) 
      + strlen(OptionsFile) + 1));
  sprintf(path, "%s%s", pspGetAppDirectory(), OptionsFile);

  /* Initialize INI structure */
  PspInit *init = pspInitCreate();

  /* Set values */
  pspInitSetInt(init, "Video", "Display Mode", Options.DisplayMode);
  pspInitSetInt(init, "Video", "Frameskip", Options.Frameskip);
  pspInitSetInt(init, "Video", "VSync", Options.VSync);
  pspInitSetInt(init, "Video", "Speed Limiter", Options.LimitSpeed);
  pspInitSetInt(init, "Video", "PSP Clock Frequency",Options.ClockFreq);
  pspInitSetInt(init, "Video", "Show FPS", Options.ShowFps);
  pspInitSetInt(init, "Menu",  "Control Mode", Options.ControlMode);
  pspInitSetInt(init, "Menu",  "Animations", UiMetric.Animate);

  if (GamePath) pspInitSetString(init, "File", "Game Path", GamePath);

  /* Save INI file */
  int status = pspInitSave(init, path);

  /* Clean up */
  pspInitDestroy(init);
  free(path);

  return status;
}

/* Initialize game configuration */
void InitGameConfig(GameConfig *config)
{
  if (config != &DefaultConfig) 
    memcpy(config, &DefaultConfig, sizeof(GameConfig));
}

/* Load game configuration */
int LoadGameConfig(const char *config_name, GameConfig *config)
{
  char *path;
  config_name = (config_name)?pspFileGetFilename(config_name):EmptyCartName;
  if (!(path = (char*)malloc(sizeof(char) * (strlen(ConfigPath) 
    + strlen(config_name) + 5)))) return 0;
  sprintf(path, "%s%s.cnf", ConfigPath, config_name);

  /* Open file for reading */
  FILE *file = fopen(path, "r");
  free(path);

  /* If no configuration, load defaults */
  if (!file)
  {
    InitGameConfig(config);
    return 1;
  }

  /* Read contents of struct */
  int nread = fread(config, sizeof(GameConfig), 1, file);
  fclose(file);

  if (nread != 1)
  {
    InitGameConfig(config);
    return 0;
  }

  return 1;
}

/* Save game configuration */
int SaveGameConfig(const char *config_name, const GameConfig *config)
{
  char *path;
  config_name = (config_name)?pspFileGetFilename(config_name):EmptyCartName;
  if (!(path = (char*)malloc(sizeof(char) * (strlen(ConfigPath) 
    + strlen(config_name) + 5)))) return 0;
  sprintf(path, "%s%s.cnf", ConfigPath, config_name);

  /* Open file for writing */
  FILE *file = fopen(path, "w");
  free(path);
  if (!file) return 0;

  /* Write contents of struct */
  int nwritten = fwrite(config, sizeof(GameConfig), 1, file);
  fclose(file);

  return (nwritten == 1);
}

void ShowControlTab()
{
  PspMenuItem *item;
  int i;
  const char *config_name = (LoadedGame)
    ? pspFileGetFilename(LoadedGame) : EmptyCartName;
  char *game_name = strdup(config_name);
  char *dot = strrchr(game_name, '.');
  if (dot) *dot='\0';

  /* Initialize mappings */
  for (item = ControlUiMenu.Menu->First, i = 0; item; item = item->Next, i++)
    pspMenuSelectOptionByValue(item, (void*)ActiveGameConfig.ButtonMap[i]);

  /* Display menu */
  pspUiOpenMenu(&ControlUiMenu, game_name);

  /* Free resources */
  free(game_name);
}

void ShowStateTab()
{
  PspMenuItem *item;
  SceIoStat stat;
  char caption[32];

  const char *config_name = (LoadedGame) 
    ? pspFileGetFilename(LoadedGame) : EmptyCartName;
  char *path = (char*)malloc(strlen(SaveStatePath) + strlen(config_name) + 8);
  char *game_name = strdup(config_name);
  char *dot = strrchr(game_name, '.');
  if (dot) *dot='\0';

  /* Initialize icons */
  for (item = SaveStateGallery.Menu->First; item; item = item->Next)
  {
    sprintf(path, "%s%s.s%02i", SaveStatePath, config_name, item->ID);

    if (pspFileCheckIfExists(path))
    {
      if (sceIoGetstat(path, &stat) < 0) sprintf(caption, RES_S_ERROR);
      else sprintf(caption, "%02i/%02i/%02i %02i:%02i",
                   stat.st_mtime.month,
                   stat.st_mtime.day,
                   stat.st_mtime.year - (stat.st_mtime.year / 100) * 100,
                   stat.st_mtime.hour,
                   stat.st_mtime.minute);

      pspMenuSetCaption(item, caption);
      item->Icon = LoadStateIcon(path);
      pspMenuSetHelpText(item, PresentSlotText);
    }
    else
    {
      pspMenuSetCaption(item, RES_S_EMPTY);
      item->Icon = NoSaveIcon;
      pspMenuSetHelpText(item, EmptySlotText);
    }
  }

  free(path);
  pspUiOpenGallery(&SaveStateGallery, game_name);
  free(game_name);

  /* Destroy any icons */
  for (item = SaveStateGallery.Menu->First; item; item = item->Next)
    if (item->Icon != NULL && item->Icon != NoSaveIcon)
      pspImageDestroy((PspImage*)item->Icon);
}

/* Load state icon */
PspImage* LoadStateIcon(const char *path)
{
  /* Open file for reading */
  FILE *f = fopen(path, "r");
  if (!f) return NULL;

  /* Load image */
  PspImage *image = pspImageLoadPngFd(f);
  fclose(f);

  return image;
}

/* Load state */
int LoadState(const char *path)
{
  /* Open file for reading */
  FILE *f = fopen(path, "r");
  if (!f) return 0;

  /* Load image into temporary object */
  PspImage *image = pspImageLoadPngFd(f);
  if (!image) return 0;
  pspImageDestroy(image);

  /* Flush the file stream */
  fflush(f);

  gzFile z = gzdopen(fileno(f), "r");
  int status = snapshot_load_open(z);
  gzclose(z);
  fclose(f);

  return !status;
}

/* Save state */
PspImage* SaveState(const char *path, PspImage *icon)
{
  /* Open file for writing */
  FILE *f;
  if (!(f = fopen(path, "w")))
    return NULL;

  /* Create thumbnail */
  PspImage *thumb = pspImageCreateThumbnail(icon);
  if (!thumb) { fclose(f); return NULL; }

  /* Write the thumbnail */
  if (!pspImageSavePngFd(f, thumb))
  {
    pspImageDestroy(thumb);
    fclose(f);
    return NULL;
  }

  /* Flush the file stream */
  fflush(f);

  /* Save state */
  gzFile z = gzdopen(fileno(f), "w");
  int status;
  if ((status = snapshot_save_open(z)))
  {
    pspImageDestroy(thumb);
    thumb = NULL;
  }

  gzclose(z);
  fclose(f);

  return thumb;
}

void TrashMenu()
{
  /* Save options */
  SaveOptions();

  /* Free emulation-specific resources */
  TrashEmulation();

  /* Free local resources */
  if (Background) pspImageDestroy(Background);
  if (NoSaveIcon) pspImageDestroy(NoSaveIcon);
  if (LoadedGame) free(LoadedGame);
  if (GamePath) free(GamePath);
  if (SaveStatePath) free(SaveStatePath);
  if (ScreenshotPath) free(ScreenshotPath);
  if (ConfigPath) free(ConfigPath);

  /* Destroy menu objects */
  pspMenuDestroy(SaveStateGallery.Menu);
  if (OptionUiMenu.Menu) pspMenuDestroy(OptionUiMenu.Menu);
  if (ControlUiMenu.Menu) pspMenuDestroy(ControlUiMenu.Menu);
}
