#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pspkernel.h>
#include <zlib.h>

#include "cap32_psp.h"
#include "psp/menu.h"
#include "langres.h"
#include "emulate.h"

#include "unzip.h"

#include "video.h"
#include "image.h"
#include "ui.h"
#include "ctrl.h"
#include "psp.h"
#include "util.h"
#include "init.h"
#include "file.h"

#define TAB_QUICKLOAD 0
#define TAB_STATE     1
#define TAB_CONTROL   2
#define TAB_OPTION    3
#define TAB_SYSTEM    4
#define TAB_ABOUT     5
#define TAB_MAX       TAB_SYSTEM

#define SYSTEM_SCRNSHOT 0x11
#define SYSTEM_RESET    0x12
#define SYSTEM_MODEL    0x13
#define SYSTEM_SCREEN   0x14
#define SYSTEM_RAM      0x15
#define SYSTEM_DRIVEA   0x16
#define SYSTEM_TAPE     0x17

#define OPTION_DISPLAY_MODE 0x21
#define OPTION_FRAMESKIP    0x22
#define OPTION_VSYNC        0x23
#define OPTION_CLOCK_FREQ   0x24
#define OPTION_SHOW_FPS     0x25
#define OPTION_CONTROL_MODE 0x26
#define OPTION_FRAME_LIMIT  0x27
#define OPTION_ANIMATE      0x28
#define OPTION_AUTOLOAD     0x29

#define RESOURCE_DISK 1
#define RESOURCE_TAPE 2

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
  *QuickloadFilter[] = { "DSK", "CDT", "VOC", "ZIP", '\0' },
  *DiskFilter[] = { "DSK", "ZIP", '\0' },
  *TapeFilter[] = { "CDT", "VOC", "ZIP", '\0' },
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
  ModelOptions[] = {
    MENU_OPTION("CPC 464",  0),
    MENU_OPTION("CPC 664",  1),
    MENU_OPTION("CPC 6128", 2),
    MENU_END_OPTIONS
  },
  ScreenOptions[] = {
    MENU_OPTION(RES_S_SCR_MONOCHR, 1),
    MENU_OPTION(RES_S_SCR_COLOUR,  0),
    MENU_END_OPTIONS
  },
  AutoloadOptions[] = {
    MENU_OPTION(RES_S_DISABLED, -1),
    MENU_OPTION("1", 0),
    MENU_OPTION("2", 1),
    MENU_OPTION("3", 2),
    MENU_OPTION("4", 3),
    MENU_OPTION("5", 4),
    MENU_OPTION("6", 5),
    MENU_OPTION("7", 6),
    MENU_OPTION("8", 7),
    MENU_OPTION("9", 8),
    MENU_OPTION("10",9),
    MENU_END_OPTIONS
  },
  RAMOptions[] = {
    MENU_OPTION("64 kB",  64),
    MENU_OPTION("128 kB", 128),
    MENU_OPTION("192 kB", 192),
    MENU_OPTION("512 kB", 512),
    MENU_OPTION("576 kB", 576),
    MENU_END_OPTIONS
  },
  StorageOptions[] = {
    MENU_OPTION("Placeholder",  64),
    MENU_END_OPTIONS
  },
  ButtonMapOptions[] = {
    /* Unmapped */
    MENU_OPTION(RES_S_NONE, 0),
    /* Special */
    MENU_OPTION(RES_S_SPECIAL_OPEN_MENU, SPC|SPC_MENU),
    MENU_OPTION(RES_S_SPECIAL_SHOW_KEYS, SPC|SPC_SHOW_KEYS),
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
    MENU_OPTION("A", KBD|CPC_a), MENU_OPTION("B", KBD|CPC_b), 
    MENU_OPTION("C", KBD|CPC_c), MENU_OPTION("D", KBD|CPC_d), 
    MENU_OPTION("E", KBD|CPC_e), MENU_OPTION("F", KBD|CPC_f), 
    MENU_OPTION("G", KBD|CPC_g), MENU_OPTION("H", KBD|CPC_h), 
    MENU_OPTION("I", KBD|CPC_i), MENU_OPTION("J", KBD|CPC_j), 
    MENU_OPTION("K", KBD|CPC_k), MENU_OPTION("L", KBD|CPC_l), 
    MENU_OPTION("M", KBD|CPC_m), MENU_OPTION("N", KBD|CPC_n), 
    MENU_OPTION("O", KBD|CPC_o), MENU_OPTION("P", KBD|CPC_p), 
    MENU_OPTION("Q", KBD|CPC_q), MENU_OPTION("R", KBD|CPC_r), 
    MENU_OPTION("S", KBD|CPC_s), MENU_OPTION("T", KBD|CPC_t), 
    MENU_OPTION("U", KBD|CPC_u), MENU_OPTION("V", KBD|CPC_v), 
    MENU_OPTION("W", KBD|CPC_w), MENU_OPTION("X", KBD|CPC_x), 
    MENU_OPTION("Y", KBD|CPC_y), MENU_OPTION("Z", KBD|CPC_z), 
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
//    MENU_ITEM(RES_S_FRAME_SKIPPING,OPTION_FRAMESKIP,FrameSkipOptions,-1,RES_S_CHANGE_FRAME_SKIP_PER_UPD),
//    MENU_ITEM(RES_S_VSYNC,OPTION_VSYNC,ToggleOptions,-1,RES_S_ENABLE_TO_REDUCE_TEARING),
    MENU_ITEM(RES_S_PSP_CLOCK_FREQ,OPTION_CLOCK_FREQ,PspClockFreqOptions,-1,RES_S_LARGER_FASTER_DEPL),
    MENU_ITEM(RES_S_SHOW_FPS,OPTION_SHOW_FPS,ToggleOptions,-1,RES_S_SHOW_HIDE_FPS),
    MENU_HEADER(RES_S_ENHANCEMENTS),
    MENU_ITEM(RES_S_AUTOLOAD_SLOT,OPTION_AUTOLOAD,AutoloadOptions,-1,RES_S_AUTOLOAD_HELP),
    MENU_HEADER(RES_S_MENU),
    MENU_ITEM(RES_S_UI_MODE,OPTION_CONTROL_MODE,ControlModeOptions,-1,RES_S_CHANGE_OK_CANCEL),
    MENU_ITEM(RES_S_ANIMATIONS,OPTION_ANIMATE,ToggleOptions,-1,RES_S_ENABLE_DISABLE_ANIM),
    MENU_END_ITEMS
  },
  SystemMenuDef[] = {
    MENU_HEADER(RES_S_VIDEO),
    MENU_ITEM(RES_S_SCREEN, SYSTEM_SCREEN, ScreenOptions, -1, RES_S_SCREEN_HELP),
    MENU_HEADER(RES_S_STORAGE),
    MENU_ITEM(RES_S_DRIVEA, SYSTEM_DRIVEA, StorageOptions, 0, RES_S_STORAGE_VACANT_HELP),
    MENU_ITEM(RES_S_TAPE, SYSTEM_TAPE, StorageOptions, 0, RES_S_STORAGE_VACANT_HELP),
    MENU_HEADER(RES_S_SYSTEM),
    MENU_ITEM(RES_S_MODEL, SYSTEM_MODEL, ModelOptions, -1, RES_S_MODEL_HELP),
    MENU_ITEM(RES_S_RAM, SYSTEM_RAM, RAMOptions, -1, RES_S_RAM_HELP),
    MENU_HEADER(RES_S_OPTIONS),
    MENU_ITEM(RES_S_RESET, SYSTEM_RESET, NULL, -1, RES_S_RESET_HELP),
    MENU_ITEM(RES_S_SAVE_SCR,  SYSTEM_SCRNSHOT, NULL, -1, RES_S_SAVE_SCR_HELP),
    MENU_END_ITEMS
  };

/* Tab labels */
static const char *TabLabel[] = 
{ RES_S_GAME_TAB, RES_S_SAVE_LOAD_TAB, RES_S_CONTROL_TAB, RES_S_OPTION_TAB,
RES_S_SYSTEM_TAB, RES_S_ABOUT_TAB };

static PspImage* LoadStateIcon(const char *path);
static int LoadState(const char *path);
static PspImage* SaveState(const char *path, PspImage *icon);
int LoadResource(const char *path);

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

static void OnSystemRender(const void *uiobject, const void *item_obj);

static int OnQuickloadOk(const void *browser, const void *path);
static int OnFileOk(const void *browser, const void *path);

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

char *LoadedGame;
static char *DiskPath;
static char *TapePath;
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

PspUiMenu SystemUiMenu =
{
  NULL,                  /* PspMenu */
  OnSystemRender,        /* OnRender() */
  OnMenuOk,              /* OnOk() */
  OnGenericCancel,       /* OnCancel() */
  OnMenuButtonPress,     /* OnButtonPress() */
  OnMenuItemChanged,     /* OnItemChanged() */
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

PspUiFileBrowser FileBrowser = 
{
  OnGenericRender,
  OnFileOk,
  NULL,
  NULL,
  NULL,
  NULL
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
  DiskPath = NULL;
  TapePath = NULL;

  CPC.drvA_zip = false;
  CPC.tape_zip = false;
  
  LoadOptions();

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
  UiMetric.MenuOptionBoxBg = COLOR(0, 0, 0, 0xBB);
  UiMetric.MenuDecorColor = PSP_COLOR_YELLOW;
  UiMetric.TitlePadding = 4;
  UiMetric.TitleColor = PSP_COLOR_WHITE;
  UiMetric.MenuFps = 30;
  UiMetric.TabBgColor = COLOR(0xf5,0xf5,0xf5, 0xff);
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

  /* Initialize system menu */
  SystemUiMenu.Menu = pspMenuCreate();
  pspMenuLoad(SystemUiMenu.Menu, SystemMenuDef);

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
      if (item) pspMenuSelectOptionByValue(item, (void*)(int)Options.Frameskip);
      item = pspMenuFindItemById(OptionUiMenu.Menu, OPTION_VSYNC);
      if (item) pspMenuSelectOptionByValue(item, (void*)Options.VSync);
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
      item = pspMenuFindItemById(OptionUiMenu.Menu, OPTION_AUTOLOAD);
      pspMenuSelectOptionByValue(item, (void*)Options.AutoloadSlot);
      pspUiOpenMenu(&OptionUiMenu, NULL);
      break;
    case TAB_CONTROL:
      ShowControlTab();
      break;
    case TAB_STATE:
      ShowStateTab();
      break;
    case TAB_SYSTEM:
      item = pspMenuFindItemById(SystemUiMenu.Menu, SYSTEM_MODEL);
      pspMenuSelectOptionByValue(item, (void*)CPC.model);
      item = pspMenuFindItemById(SystemUiMenu.Menu, SYSTEM_SCREEN);
      pspMenuSelectOptionByValue(item, (void*)CPC.scr_tube);
      item = pspMenuFindItemById(SystemUiMenu.Menu, SYSTEM_RAM);
      pspMenuSelectOptionByValue(item, (void*)CPC.ram_size);
      
      item = pspMenuFindItemById(SystemUiMenu.Menu, SYSTEM_DRIVEA);
      pspMenuModifyOption(item->Options, 
        (DiskPath) ? pspFileGetFilename(DiskPath) : RES_S_VACANT, NULL);
      pspMenuSetHelpText(item, (DiskPath) 
        ? ((CPC.drvA_zip) ? RES_S_STORAGE_RD_HELP : RES_S_STORAGE_RW_HELP) 
        : RES_S_STORAGE_VACANT_HELP);
      
      item = pspMenuFindItemById(SystemUiMenu.Menu, SYSTEM_TAPE);
      pspMenuModifyOption(item->Options, 
        (TapePath) ? pspFileGetFilename(DiskPath) : RES_S_VACANT, NULL);
      pspMenuSetHelpText(item, (TapePath) 
        ? RES_S_STORAGE_RD_HELP : RES_S_STORAGE_VACANT_HELP);
      
      pspUiOpenMenu(&SystemUiMenu, NULL);
      break;
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
    RES_S_APP_NAME,
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

    if (i == TabIndex || !(uiobject == &FileBrowser))
    {
      /* Draw background of active tab */
      if (i == TabIndex)
        pspVideoFillRect(x - 5, 0, x + width + 5, height + 1, UiMetric.TabBgColor);
  
      /* Draw name of tab */
      pspVideoPrint(UiMetric.Font, x, 0, TabLabel[i], PSP_COLOR_WHITE);
    }
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
      pspUiAlert(RES_S_SAVED_SUCC);
    else pspUiAlert(RES_S_ERROR_NOT_SAVED);
    return 0;
  }
  else return 0;

  return 1;
}

int OnFileOk(const void *browser, const void *path)
{
  if (!LoadResource((char*)path))
    return 0;
  
  /* Reset loaded game */
  if (LoadedGame) free(LoadedGame);
  LoadedGame = strdup((char*)path);

  /* Reset current game path */
  free(GamePath);
  GamePath = pspFileGetParentDirectory(LoadedGame);

  /* Load control set */
  LoadGameConfig(LoadedGame, &ActiveGameConfig);

  /* Reset the menu items */
  PspMenuItem *item;
  item = pspMenuFindItemById(SystemUiMenu.Menu, SYSTEM_DRIVEA);
  pspMenuModifyOption(item->Options, 
    (DiskPath) ? pspFileGetFilename(DiskPath) : RES_S_VACANT, NULL);
  pspMenuSetHelpText(item, (DiskPath) 
    ? ((CPC.drvA_zip) ? RES_S_STORAGE_RD_HELP : RES_S_STORAGE_RW_HELP) 
    : RES_S_STORAGE_VACANT_HELP);
  
  item = pspMenuFindItemById(SystemUiMenu.Menu, SYSTEM_TAPE);
  pspMenuModifyOption(item->Options, 
    (TapePath) ? pspFileGetFilename(DiskPath) : RES_S_VACANT, NULL);
  pspMenuSetHelpText(item, (TapePath) 
    ? RES_S_STORAGE_RD_HELP : RES_S_STORAGE_VACANT_HELP);

  return 1;
}

int OnQuickloadOk(const void *browser, const void *path)
{
  switch(LoadResource((char*)path))
  {
  case 0:
    return 0;
  case RESOURCE_DISK:
    tape_eject();
    free(TapePath);
    TapePath = NULL;
    break;
  case RESOURCE_TAPE:
    dsk_eject(&driveA);
    free(DiskPath);
    DiskPath = NULL;
    break;
  }
  
  /* Reset loaded game */
  if (LoadedGame) free(LoadedGame);
  LoadedGame = strdup((char*)path);

  /* Reset current game path */
  free(GamePath);
  GamePath = pspFileGetParentDirectory(LoadedGame);

  /* Load control set */
  LoadGameConfig(LoadedGame, &ActiveGameConfig);

  ResumeEmulation = 1;
  emulator_reset(false);

  /* Autoload saved state */
  if (Options.AutoloadSlot >= 0)
  {
    const char *config_name = (LoadedGame) 
        ? pspFileGetFilename(LoadedGame) : EmptyCartName;
    char *state_file = (char*)malloc(strlen(SaveStatePath) 
        + strlen(config_name) + 8);
    sprintf(state_file, "%s%s.s%02i", SaveStatePath, config_name,
           Options.AutoloadSlot);
    
    /* Attempt loading saved state */
    LoadState(state_file);
    free(state_file);
  }

  return 1;
}

int OnMenuItemChanged(const struct PspUiMenu *uimenu, PspMenuItem* item, 
                      const PspMenuOption* option)
{
  if (uimenu == &ControlUiMenu)
  {
    ActiveGameConfig.ButtonMap[item->ID] = (unsigned int)option->Value;
  }
  else
  {
    switch(item->ID)
    {
    case SYSTEM_MODEL:
      if ((unsigned int)option->Value != CPC.model) 
      {
        if (!pspUiConfirm(RES_S_RESET_WARNING))
          return 0;
        
        CPC.model = (int)option->Value;
        if ((CPC.model == 2) && (CPC.ram_size < 128)) 
        {
          CPC.ram_size = 128;
          PspMenuItem *ram_size = pspMenuFindItemById(SystemUiMenu.Menu, SYSTEM_RAM);
          pspMenuSelectOptionByValue(ram_size, (void*)CPC.ram_size);
          pspUiAlert(RES_S_RAM_RESIZE_WARNING);
          return 0;
        }

        /* Reconfigure emulator */
        emulator_shutdown();
        emulator_init();
      }
      break;
    case SYSTEM_SCREEN:
      if ((unsigned int)option->Value != CPC.scr_tube)
      {
        CPC.scr_tube = (unsigned int)option->Value;
        video_set_palette();
      }
      break;
    case SYSTEM_RAM:
      if ((unsigned int)option->Value != CPC.ram_size) 
      {
        if (!pspUiConfirm(RES_S_RESET_WARNING))
          return 0;
        
        /* Check for size validity */
        CPC.ram_size = (unsigned int)option->Value;
        if ((CPC.model == 2) && (CPC.ram_size < 128)) CPC.ram_size = 128;
        
        if ((unsigned int)option->Value != CPC.ram_size)
        {
          pspMenuSelectOptionByValue(item, (void*)CPC.ram_size);
          pspUiAlert(RES_S_RAM_RESIZE_WARNING);
          return 0;
        }

        /* Reconfigure emulator */
        emulator_shutdown();
        emulator_init();
      }
      break;
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
    case OPTION_AUTOLOAD:
      Options.AutoloadSlot = (int)option->Value;
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
      pspUiAlert(RES_S_SAVED_SUCC);
    else pspUiAlert(RES_S_ERROR_NOT_SAVED);
  }
  else
  {
    PspMenuItem *item = (PspMenuItem*)sel_item;
    switch(item->ID)
    {
    case SYSTEM_RESET:
      if (!pspUiConfirm(RES_S_RESET_SYSTEM))
        return 1;
      emulator_reset(false);
      ResumeEmulation = 1;
      return 1;
    case SYSTEM_SCRNSHOT:
      if (pspUtilSavePngSeq(ScreenshotPath, (LoadedGame) 
          ? pspFileGetFilename(LoadedGame) : EmptyCartName, Screen))
        pspUiAlert(RES_S_SAVED_SUCC);
      else pspUiAlert(RES_S_ERROR_NOT_SAVED);
      break;
    case SYSTEM_DRIVEA:
      FileBrowser.Filter = DiskFilter;
      pspUiOpenBrowser(&FileBrowser, (DiskPath) ? DiskPath : GamePath);
      break;
    case SYSTEM_TAPE:
      FileBrowser.Filter = TapeFilter;
      pspUiOpenBrowser(&FileBrowser, (TapePath) ? TapePath : GamePath);
      break;
    }
  }

  return 0;
}

int OnMenuButtonPress(const struct PspUiMenu *uimenu, PspMenuItem* sel_item, 
                      u32 button_mask)
{
  PspMenuItem *item;
  
  if (uimenu == &ControlUiMenu)
  {
    if (button_mask & PSP_CTRL_SQUARE)
    {
      /* Save to MS as default mapping */
      if (!SaveGameConfig(DefaultConfigFile, &ActiveGameConfig))
        pspUiAlert(RES_S_ERROR_NOT_SAVED);
      else
      {
        /* Modify in-memory defaults */
        memcpy(&DefaultConfig, &ActiveGameConfig, sizeof(GameConfig));
        pspUiAlert(RES_S_SAVED_AS_DEFAULT);
      }

      return 0;
    }
    else if (button_mask & PSP_CTRL_TRIANGLE)
    {
      int i;

      /* Load default mapping */
      InitGameConfig(&ActiveGameConfig);

      /* Modify the menu */
      for (item = ControlUiMenu.Menu->First, i = 0; item; item = item->Next, i++)
        pspMenuSelectOptionByValue(item, (void*)ActiveGameConfig.ButtonMap[i]);

      return 0;
    }
  }
  else
  {
    if (button_mask & PSP_CTRL_SQUARE)
    {
      switch(sel_item->ID)
      {
      case SYSTEM_DRIVEA:
        if (!DiskPath || CPC.drvA_zip || !pspUiConfirm("Save changes?")) break;
        
        pspUiFlashMessage(RES_S_WRITING_CHANGES);
        dsk_save(DiskPath, &driveA, 'A');
        break;
      }
    }
    else if (button_mask & PSP_CTRL_TRIANGLE)
    {
      switch(sel_item->ID)
      {
      case SYSTEM_DRIVEA:
        if (!DiskPath || !pspUiConfirm("Eject disk?")) break;
        
        CPC.drvA_zip = false;
        dsk_eject(&driveA);
        DiskPath = NULL;
        
        pspMenuModifyOption(sel_item->Options, RES_S_VACANT, NULL);
        pspMenuSetHelpText(sel_item, RES_S_STORAGE_VACANT_HELP);
        break;
      case SYSTEM_TAPE:
        if (!TapePath || !pspUiConfirm("Eject tape?")) break;
        
        CPC.tape_zip = false;
        tape_eject();
        TapePath = NULL;
        
        pspMenuModifyOption(sel_item->Options, RES_S_VACANT, NULL);
        pspMenuSetHelpText(sel_item, RES_S_STORAGE_VACANT_HELP);
        break;
      }
      
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

  if (pspFileCheckIfExists(path) && pspUiConfirm(RES_S_LOAD_STATE))
  {
    if (LoadState(path))
    {
      ResumeEmulation = 1;
      pspMenuFindItemById(((const PspUiGallery*)gallery)->Menu,
                            ((const PspMenuItem*)item)->ID);
      free(path);

      return 1;
    }
    pspUiAlert(RES_S_ERROR_LOADING_STATE);
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
    char caption[64];
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
            && !pspUiConfirm(RES_S_OVERWRITE_STATE)) break;

        pspUiFlashMessage(RES_S_SAVING_WAIT);

        PspImage *icon;
        if (!(icon = SaveState(path, Screen)))
        {
          pspUiAlert(RES_S_ERROR_NOT_SAVED);
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
        if (sceIoGetstat(path, &stat) < 0) sprintf(caption, RES_S_ERROR);
        else sprintf(caption, "%02i/%02i/%02i %02i:%02i%s", 
                     stat.st_mtime.month,
                     stat.st_mtime.day,
                     stat.st_mtime.year - (stat.st_mtime.year / 100) * 100,
                     stat.st_mtime.hour,
                     stat.st_mtime.minute,
                     ((int)item->ID == Options.AutoloadSlot) ? "*" : "");

        pspMenuSetCaption(item, caption);
      }
      else if (button_mask & PSP_CTRL_TRIANGLE)
      {
        if (!pspFileCheckIfExists(path) || !pspUiConfirm(RES_S_DELETE_STATE))
          break;

        if (!pspFileDelete(path))
        {
          pspUiAlert(RES_S_ERROR_STATE_NOT_DEL);
          break;
        }

        /* Trash the old icon (if any) */
        if (item->Icon && item->Icon != NoSaveIcon)
          pspImageDestroy((PspImage*)item->Icon);

        /* Update icon, caption */
        item->Icon = NoSaveIcon;
        pspMenuSetHelpText(item, EmptySlotText);
        pspMenuSetCaption(item, ((int)item->ID == Options.AutoloadSlot) 
            ? RES_S_AUTOLOAD : RES_S_EMPTY);
      }
    } while (0);

    free(path);
    return 0;
  }

  return OnGenericButtonPress(NULL, NULL, button_mask);
}

/* Handles any special drawing for the system menu */
void OnSystemRender(const void *uiobject, const void *item_obj)
{
  int w, h, x, y;
  w = Screen->Viewport.Width >> 1;
  h = Screen->Viewport.Height >> 1;
  x = SCR_WIDTH - w - 8;
  y = (SCR_HEIGHT/2) - (h/2);

  /* Draw a small representation of the screen */
  pspVideoShadowRect(x, y, x + w - 1, y + h - 1, PSP_COLOR_BLACK, 3);
  pspVideoPutImage(Screen, x, y, w, h);
  pspVideoDrawRect(x, y, x + w - 1, y + h - 1, PSP_COLOR_GRAY);

  OnGenericRender(uiobject, item_obj);
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
  Options.ClockFreq = pspInitGetInt(init, "Video", "PSP Clock Frequency", 266);
  Options.ShowFps   = pspInitGetInt(init, "Video", "Show FPS", 0);
  Options.ControlMode = pspInitGetInt(init, "Menu", "Control Mode", 0);
  UiMetric.Animate = pspInitGetInt(init, "Menu",  "Animations", 1);
  
  CPC.model = pspInitGetInt(init, "System", "Model", 2);
  CPC.scr_tube = pspInitGetInt(init, "System", "Colour", 1);
  CPC.ram_size = pspInitGetInt(init, "System", "RAM", 128) & 0x02c0;
  
  Options.AutoloadSlot = pspInitGetInt(init, "Enhancements", "Autoload", -1);

  /* Check RAM setting for validity */
  if (CPC.ram_size > 576) CPC.ram_size = 576;
  else if ((CPC.model == 2) && (CPC.ram_size < 128)) CPC.ram_size = 128;
  
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

  pspInitSetInt(init, "System", "Model", CPC.model);
  pspInitSetInt(init, "System", "Colour", CPC.scr_tube);
  pspInitSetInt(init, "System", "RAM", CPC.ram_size);

  pspInitSetInt(init, "Enhancements", "Autoload", Options.AutoloadSlot);
  
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
      else sprintf(caption, "%02i/%02i/%02i %02i:%02i%s",
                   stat.st_mtime.month,
                   stat.st_mtime.day,
                   stat.st_mtime.year - (stat.st_mtime.year / 100) * 100,
                   stat.st_mtime.hour,
                   stat.st_mtime.minute,
                   ((int)item->ID == Options.AutoloadSlot) ? "*" : "");

      pspMenuSetCaption(item, caption);
      item->Icon = LoadStateIcon(path);
      pspMenuSetHelpText(item, PresentSlotText);
    }
    else
    {
      pspMenuSetCaption(item, ((int)item->ID == Options.AutoloadSlot) 
          ? RES_S_AUTOLOAD : RES_S_EMPTY);
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

int LoadResource(const char *path)
{
  char selected_file[256] = "\0";
  const char *filename = path;
  
  if (pspFileEndsWith(path, "ZIP"))
  {
    unzFile zipfile = NULL;
    unz_global_info gi;
    unz_file_info fi;

    /* Open archive for reading */
    if (!(zipfile = unzOpen(filename)))
    {
      pspUiAlert("Error opening compressed file");
      return 0;
    }

    /* Get global ZIP file information */
    if (unzGetGlobalInfo(zipfile, &gi) != UNZ_OK)
    {
      pspUiAlert("Error reading compressed file information");
      unzClose(zipfile);
      return 0;
    }

    int min_dsk = 11, dsk, i;
    char archived_file[256];
    
    for (i = 0; i < (int)gi.number_entry; i++)
    {
      /* Get name of the archived file */
      if (unzGetCurrentFileInfo(zipfile, &fi, archived_file, 
          sizeof(archived_file), NULL, 0, NULL, 0) != UNZ_OK)
      {
        pspUiAlert("Error reading compressed file information");
        unzClose(zipfile);
        return 0;
      }

      if (pspFileEndsWith(archived_file, "CDT")
          || pspFileEndsWith(archived_file, "VOC"))
      {
        strcpy(selected_file, archived_file);
        break;
      }
      else if (pspFileEndsWith(archived_file, "DSK"))
      {
        /* Check the pre-extension char */
        char digit = archived_file[strlen(archived_file) - 5];

        if (digit >= '0' && digit <= '9')
        {
          /* Find the file with the lowest sequential number */
          /* Treat zero as #10 */
          dsk = (digit == 0) ? 10 : (int)digit - '0';

          if (dsk < min_dsk)
          {
            min_dsk = dsk;
            strcpy(selected_file, archived_file);
          }
        }
        else
        {
          /* Non-sequential DSK - load it immediately */
          strcpy(selected_file, archived_file);
          break;
        }
      }

      /* Go to the next file in the archive */
      if (i + 1 < (int)gi.number_entry)
      {
        if (unzGoToNextFile(zipfile) != UNZ_OK)
        {
          pspUiAlert("Error parsing compressed files");
          unzClose(zipfile);
          return 0;
        }
      }
    }
    
    filename = selected_file;
    unzClose(zipfile);
  }
  
  /* Load game */
  if (pspFileEndsWith(filename, "DSK"))
  {
    if (dsk_load((char*)path, *selected_file ? selected_file : NULL, 
        &driveA, 'A'))
    {
      pspUiAlert(RES_S_ERROR_BAD_DISK);
      return 0;
    }
    
    free(DiskPath);
    DiskPath = strdup(path);
    CPC.drvA_zip = (*selected_file);
    
    return RESOURCE_DISK;
  }
  else if (pspFileEndsWith(filename, "CDT"))
  {
    if (tape_insert((char*)path))
    {
      pspUiAlert(RES_S_ERROR_BAD_TAPE);
      return 0;
    }
    
    free(TapePath);
    TapePath = strdup(path);
    CPC.tape_zip = (*selected_file);
    
    return RESOURCE_TAPE;
  }
  else if (pspFileEndsWith(filename, "VOC"))
  {
    if (tape_insert_voc((char*)path))
    {
      pspUiAlert(RES_S_ERROR_BAD_TAPE);
      return 0;
    }
    
    free(TapePath);
    CPC.tape_zip = (*selected_file);
    TapePath = strdup(path);
    
    return RESOURCE_TAPE;
  }

  return 0;
}

void TrashMenu()
{
  /* Save options */
  SaveOptions();

  /* Free emulation-specific resources */
  TrashEmulation();

  /* Free local resources */
  pspImageDestroy(Background);
  pspImageDestroy(NoSaveIcon);
  
  free(LoadedGame);
  free(DiskPath);
  free(TapePath);
  free(GamePath);
  free(SaveStatePath);
  free(ScreenshotPath);
  free(ConfigPath);

  /* Destroy menu objects */
  pspMenuDestroy(SaveStateGallery.Menu);
  pspMenuDestroy(OptionUiMenu.Menu);
  pspMenuDestroy(ControlUiMenu.Menu);
  pspMenuDestroy(SystemUiMenu.Menu);
}
