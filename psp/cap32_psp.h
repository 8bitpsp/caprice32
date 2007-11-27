#ifndef _CAP32_PSP_H
#define _CAP32_PSP_H

#include "cap32.h"
#include "z80.h"

#define MOD_CPC_SHIFT   (0x01 << 8)
#define MOD_CPC_CTRL    (0x02 << 8)

typedef enum {
  CPC_0,
  CPC_1,
  CPC_2,
  CPC_3,
  CPC_4,
  CPC_5,
  CPC_6,
  CPC_7,
  CPC_8,
  CPC_9,
  CPC_A,
  CPC_B,
  CPC_C,
  CPC_D,
  CPC_E,
  CPC_F,
  CPC_G,
  CPC_H,
  CPC_I,
  CPC_J,
  CPC_K,
  CPC_L,
  CPC_M,
  CPC_N,
  CPC_O,
  CPC_P,
  CPC_Q,
  CPC_R,
  CPC_S,
  CPC_T,
  CPC_U,
  CPC_V,
  CPC_W,
  CPC_X,
  CPC_Y,
  CPC_Z,
  CPC_a,
  CPC_b,
  CPC_c,
  CPC_d,
  CPC_e,
  CPC_f,
  CPC_g,
  CPC_h,
  CPC_i,
  CPC_j,
  CPC_k,
  CPC_l,
  CPC_m,
  CPC_n,
  CPC_o,
  CPC_p,
  CPC_q,
  CPC_r,
  CPC_s,
  CPC_t,
  CPC_u,
  CPC_v,
  CPC_w,
  CPC_x,
  CPC_y,
  CPC_z,
  CPC_AMPERSAND,
  CPC_ASTERISK,
  CPC_AT,
  CPC_BACKQUOTE,
  CPC_BACKSLASH,
  CPC_CAPSLOCK,
  CPC_CLR,
  CPC_COLON,
  CPC_COMMA,
  CPC_CONTROL,
  CPC_COPY,
  CPC_CPY_DOWN,
  CPC_CPY_LEFT,
  CPC_CPY_RIGHT,
  CPC_CPY_UP,
  CPC_CUR_DOWN,
  CPC_CUR_LEFT,
  CPC_CUR_RIGHT,
  CPC_CUR_UP,
  CPC_CUR_ENDBL,
  CPC_CUR_HOMELN,
  CPC_CUR_ENDLN,
  CPC_CUR_HOMEBL,
  CPC_DBLQUOTE,
  CPC_DEL,
  CPC_DOLLAR,
  CPC_ENTER,
  CPC_EQUAL,
  CPC_ESC,
  CPC_EXCLAMATN,
  CPC_F0,
  CPC_F1,
  CPC_F2,
  CPC_F3,
  CPC_F4,
  CPC_F5,
  CPC_F6,
  CPC_F7,
  CPC_F8,
  CPC_F9,
  CPC_FPERIOD,
  CPC_GREATER,
  CPC_HASH,
  CPC_LBRACKET,
  CPC_LCBRACE,
  CPC_LEFTPAREN,
  CPC_LESS,
  CPC_LSHIFT,
  CPC_MINUS,
  CPC_PERCENT,
  CPC_PERIOD,
  CPC_PIPE,
  CPC_PLUS,
  CPC_POUND,
  CPC_POWER,
  CPC_QUESTION,
  CPC_QUOTE,
  CPC_RBRACKET,
  CPC_RCBRACE,
  CPC_RETURN,
  CPC_RIGHTPAREN,
  CPC_RSHIFT,
  CPC_SEMICOLON,
  CPC_SLASH,
  CPC_SPACE,
  CPC_TAB,
  CPC_UNDERSCORE,
  CPC_J0_UP,
  CPC_J0_DOWN,
  CPC_J0_LEFT,
  CPC_J0_RIGHT,
  CPC_J0_FIRE1,
  CPC_J0_FIRE2,
  CPC_J1_UP,
  CPC_J1_DOWN,
  CPC_J1_LEFT,
  CPC_J1_RIGHT,
  CPC_J1_FIRE1,
  CPC_J1_FIRE2,
  CPC_ES_NTILDE,
  CPC_ES_nTILDE,
  CPC_ES_PESETA,
  CPC_FR_eACUTE,
  CPC_FR_eGRAVE,
  CPC_FR_cCEDIL,
  CPC_FR_aGRAVE,
  CPC_FR_uGRAVE
} CPC_KEYS;

extern t_CPC CPC;
extern t_drive driveA;
extern t_drive driveB;
extern t_VDU VDU;
extern byte *pbSndBuffer;
extern byte bit_values[8];
extern byte keyboard_matrix[16];
extern dword cpc_kbd[3][149];

int  video_init();
int  video_set_palette();
void video_init_tables();
int  audio_init();
void audio_shutdown();

#define MAX_DISK_FORMAT     2
#define DEF_SPEED_SETTING   4
#define DEFAULT_DISK_FORMAT 0

int  emulator_init();
void emulator_shutdown(void);

int snapshot_load_open(void *pfileObject);
int snapshot_save_open(void *pfileObject);

int  printer_start(void);
void printer_stop(void);

void dsk_eject (t_drive *drive);
int  dsk_load(char *pchFileName, char *archive_filename, 
             t_drive *drive, char chID);
int  dsk_save (char *pchFileName, t_drive *drive, char chID);

int  tape_insert (char *pchFileName);
int  tape_insert_voc (char *pchFileName);
void tape_eject (void);

#endif // _CAP32_PSP_H
