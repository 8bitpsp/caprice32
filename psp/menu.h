#ifndef _MENU_H
#define _MENU_H

int  InitMenu();
void DisplayMenu();
void TrashMenu();

#define DISPLAY_MODE_UNSCALED    0
#define DISPLAY_MODE_FIT_HEIGHT  1
#define DISPLAY_MODE_FILL_SCREEN 2

#define KBD 0x0100
#define SPC 0x0200

#define CODE_MASK(x) (x & 0xff)

#define SPC_MENU                   1
#define SPC_SHOW_KEYS              2

#define CONTROL_ANALOG_UP          0x00
#define CONTROL_ANALOG_DOWN        0x01
#define CONTROL_ANALOG_LEFT        0x02
#define CONTROL_ANALOG_RIGHT       0x03
#define CONTROL_BUTTON_UP          0x04
#define CONTROL_BUTTON_DOWN        0x05
#define CONTROL_BUTTON_LEFT        0x06
#define CONTROL_BUTTON_RIGHT       0x07
#define CONTROL_BUTTON_SQUARE      0x08
#define CONTROL_BUTTON_CROSS       0x09
#define CONTROL_BUTTON_CIRCLE      0x0A
#define CONTROL_BUTTON_TRIANGLE    0x0B
#define CONTROL_BUTTON_LTRIGGER    0x0C
#define CONTROL_BUTTON_RTRIGGER    0x0D
#define CONTROL_BUTTON_SELECT      0x0E
#define CONTROL_BUTTON_START       0x0F
#define CONTROL_BUTTON_LRTRIGGERS  0x10
#define CONTROL_BUTTON_STARTSELECT 0x11

/* Button map ID's */
const int ButtonMapId[] = 
{
  CONTROL_BUTTON_LRTRIGGERS,
  CONTROL_BUTTON_STARTSELECT,
  CONTROL_ANALOG_UP,       CONTROL_ANALOG_DOWN,
  CONTROL_ANALOG_LEFT,     CONTROL_ANALOG_RIGHT,
  CONTROL_BUTTON_UP,       CONTROL_BUTTON_DOWN,
  CONTROL_BUTTON_LEFT,     CONTROL_BUTTON_RIGHT,
  CONTROL_BUTTON_SQUARE,   CONTROL_BUTTON_CROSS,
  CONTROL_BUTTON_CIRCLE,   CONTROL_BUTTON_TRIANGLE,
  CONTROL_BUTTON_LTRIGGER, CONTROL_BUTTON_RTRIGGER,
  CONTROL_BUTTON_SELECT,   CONTROL_BUTTON_START,
  -1 /* End */
};

typedef struct
{
  int ShowFps;
  int ControlMode;
  int ClockFreq;
  int DisplayMode;
  int VSync;
  int LimitSpeed;
  int Frameskip;
  int AutoloadSlot;
} EmulatorOptions;

#endif // _MENU_H
