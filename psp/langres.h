/* Common definitions */

#define RES_S_GAME_TAB      "Game"
#define RES_S_SAVE_LOAD_TAB "Save/Load"
#define RES_S_CONTROL_TAB   "Controls"
#define RES_S_OPTION_TAB    "Options"
#define RES_S_SYSTEM_TAB    "System"
#define RES_S_ABOUT_TAB     "About"

#define RES_S_PRESENT_SLOT_TEXT \
  "\026"PSP_CHAR_SQUARE"\020 Save\t\026\001\020 Load\t\026"PSP_CHAR_TRIANGLE"\020 Delete"
#define RES_S_EMPTY_SLOT_TEXT \
  "\026"PSP_CHAR_SQUARE"\020 Save"
#define RES_S_CONTROL_HELP_TEXT \
 "\026\250\020 Change mapping\t\026\001\020 Save to \271\t\026\244\020 Set as default\t\026\243\020 Load defaults"

#define RES_S_DISABLED "Disabled"
#define RES_S_ENABLED  "Enabled"

#define RES_S_ACTUAL_SIZE "Actual size"
#define RES_S_FIT_HEIGHT  "Fit height"
#define RES_S_FIT_SCREEN  "Fit screen"

#define RES_S_60FPS_NTSC "60 fps (NTSC)"
#define RES_S_50FPS_PAL  "50 fps (PAL)"

#define RES_S_SKIP_FRAMES_0 "No skipping"
#define RES_S_SKIP_FRAMES_1 "Skip 1 frame"
#define RES_S_SKIP_FRAMES_2 "Skip 2 frames"
#define RES_S_SKIP_FRAMES_3 "Skip 3 frames"
#define RES_S_SKIP_FRAMES_4 "Skip 4 frames"
#define RES_S_SKIP_FRAMES_5 "Skip 5 frames"

#define RES_S_222_MHZ "222 MHz"
#define RES_S_266_MHZ "266 MHz"
#define RES_S_300_MHZ "300 MHz"
#define RES_S_333_MHZ "333 MHz"

#define RES_S_UI_US \
  "\026"PSP_CHAR_CIRCLE"\020 cancels, \026"PSP_CHAR_CROSS"\020 confirms (US)"
#define RES_S_UI_JAPAN \
  "\026"PSP_CHAR_CROSS"\020 cancels, \026"PSP_CHAR_CIRCLE"\020 confirms (Japan)"

#define RES_S_AUDIO  "Audio"
#define RES_S_SYSTEM "System"
#define RES_S_VIDEO  "Video"
#define RES_S_PERF   "Performance"
#define RES_S_STORAGE "Storage"
#define RES_S_MENU   "Menu"
#define RES_S_ERROR  "ERROR"
#define RES_S_SOUND  "Sound"

#define RES_S_SCREEN_SIZE    "Screen size"
#define RES_S_FRAME_LIMITER  "Frame limiter"
#define RES_S_FRAME_SKIPPING "Frame skipping"
#define RES_S_VSYNC          "VSync"
#define RES_S_PSP_CLOCK_FREQ "PSP clock frequency"
#define RES_S_SHOW_FPS       "Show FPS counter"
#define RES_S_UI_MODE        "Button mode"
#define RES_S_ANIMATIONS     "Animations"
#define RES_S_SYSTEM         "System"
#define RES_S_OPTIONS        "Options"
#define RES_S_MODEL          "Model"

#define RES_S_CHANGE_SCREEN_SIZE \
  "\026"PSP_CHAR_RIGHT"\020 Change screen size"
#define RES_S_CHANGE_SCR_UPD_FREQ \
  "\026"PSP_CHAR_RIGHT"\020 Change screen update frequency"
#define RES_S_CHANGE_FRAME_SKIP_PER_UPD \
  "\026"PSP_CHAR_RIGHT"\020 Change number of frames skipped per update"
#define RES_S_ENABLE_TO_REDUCE_TEARING \
  "\026"PSP_CHAR_RIGHT"\020 Enable to reduce tearing; disable to increase speed"
#define RES_S_LARGER_FASTER_DEPL \
  "\026"PSP_CHAR_RIGHT"\020 Larger values: faster emulation, faster battery depletion (default: 222MHz)"
#define RES_S_SHOW_HIDE_FPS \
  "\026"PSP_CHAR_RIGHT"\020 Show/hide the frames-per-second counter"
#define RES_S_CHANGE_OK_CANCEL \
  "\026"PSP_CHAR_RIGHT"\020 Change OK and Cancel button mapping"
#define RES_S_ENABLE_DISABLE_ANIM \
  "\026"PSP_CHAR_RIGHT"\020 Enable/disable in-menu animations"

#define RES_S_RESET    "Reset"
#define RES_S_SAVE_SCR "Save screenshot"

#define RES_S_RESET_HELP    "\026\001\020 Reset"
#define RES_S_SAVE_SCR_HELP "\026\001\020 Save screenshot"

#define RES_S_NONE              "None"
#define RES_S_SPECIAL_OPEN_MENU "Special: Open menu"
#define RES_S_SPECIAL_SHOW_KEYS "Special: Virtual keyboard"

#define RES_S_APP_NAME \
  PSP_APP_NAME" version "PSP_APP_VER" ("__DATE__")"

#define RES_S_SAVED_SUCC          "Saved successfully"
#define RES_S_ERROR_NOT_SAVED     "ERROR: Not saved"
#define RES_S_RESET_SYSTEM        "Reset the system?"
#define RES_S_LOAD_STATE          "Load state?"
#define RES_S_ERROR_LOADING_STATE "ERROR: State failed to load"
#define RES_S_OVERWRITE_STATE     "Overwrite existing state?"
#define RES_S_SAVING_WAIT         "Saving, please wait ..."
#define RES_S_DELETE_STATE        "Delete state?"
#define RES_S_ERROR_STATE_NOT_DEL "ERROR: State not deleted"
#define RES_S_EMPTY               "Empty"
#define RES_S_RESET_WARNING       "This will reset the system. Proceed?"

#define RES_S_SAVED_AS_DEFAULT    "Layout set as default"

/* Emulator-specific definitions */

#define RES_S_SCREEN  "Monitor type"
#define RES_S_RAM     "System RAM"

#define RES_S_SCR_COLOUR  "Colour"
#define RES_S_SCR_MONOCHR "Monochrome"

#define RES_S_KBD_UP        "Keyboard Up"
#define RES_S_KBD_DOWN      "Keyboard Down"
#define RES_S_KBD_LEFT      "Keyboard Left"
#define RES_S_KBD_RIGHT     "Keyboard Right"

#define RES_S_JOY1_UP        "Joystick 1 Up"
#define RES_S_JOY1_DOWN      "Joystick 1 Down"
#define RES_S_JOY1_LEFT      "Joystick 1 Left"
#define RES_S_JOY1_RIGHT     "Joystick 1 Right"
#define RES_S_JOY1_FIRE1     "Joystick 1 Fire 1"
#define RES_S_JOY1_FIRE2     "Joystick 1 Fire 2"

#define RES_S_CAPS_LOCK  "CAPS lock"
#define RES_S_CLEAR      "Clear"
#define RES_S_CONTROL    "Control"
#define RES_S_COPY       "Copy"
#define RES_S_DELETE     "Delete"
#define RES_S_ENTER      "Enter"
#define RES_S_ESC        "Escape"
#define RES_S_LSHIFT     "Left shift"
#define RES_S_POWER      "Power"
#define RES_S_RETURN     "Return"
#define RES_S_RSHIFT     "Right shift"
#define RES_S_SPACE      "Space"
#define RES_S_TAB        "Tab"

#define RES_S_MODEL_HELP \
  "\026"PSP_CHAR_RIGHT"\020 Select CPC hardware model"
#define RES_S_SCREEN_HELP \
  "\026"PSP_CHAR_RIGHT"\020 Select type of emulated monitor"
#define RES_S_RAM_HELP \
  "\026"PSP_CHAR_RIGHT"\020 Select amount of system RAM"
#define RES_S_AUTOLOAD_HELP \
  "\026"PSP_CHAR_RIGHT"\020 Select save state to be loaded automatically"

#define RES_S_ERROR_BAD_TAPE     "Error loading tape image"
#define RES_S_ERROR_BAD_DISK     "Error loading disk image"
#define RES_S_RAM_RESIZE_WARNING \
  "Amount of system memory has been readjusted to fit the selected system model"

#define RES_S_AUTOLOAD "Autoload"
#define RES_S_ENHANCEMENTS "Enhancements"
#define RES_S_AUTOLOAD_SLOT "Autoload slot"

#define RES_S_DRIVEA  "Drive A"
#define RES_S_TAPE    "Tape drive"
#define RES_S_VACANT  "Vacant"

#define RES_S_STORAGE_RD_HELP \
  "\026\001\020 Browse\t\026"PSP_CHAR_TRIANGLE"\020 Eject"
#define RES_S_STORAGE_RW_HELP \
  "\026"PSP_CHAR_SQUARE"\020 Save\t\026\001\020 Browse\t\026"PSP_CHAR_TRIANGLE"\020 Eject"
#define RES_S_STORAGE_VACANT_HELP \
  "\026\001\020 Browse"

#define RES_S_WRITING_CHANGES \
  "Please wait, writing changes to disk image...\n"\
  "Do not turn off or suspend the system"
