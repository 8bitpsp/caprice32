#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#include "cap32.h"
#include "cap32_psp.h"
#include "z80.h"
#include "4.1.0/crtc.h"
#include "tape.h"

#include "video.h"
#include "image.h"

dword cpc_kbd[3][149] = {
  { // original CPC keyboard
    0x40,                   // CPC_0
    0x80,                   // CPC_1
    0x81,                   // CPC_2
    0x71,                   // CPC_3
    0x70,                   // CPC_4
    0x61,                   // CPC_5
    0x60,                   // CPC_6
    0x51,                   // CPC_7
    0x50,                   // CPC_8
    0x41,                   // CPC_9
    0x85 | MOD_CPC_SHIFT,   // CPC_A
    0x66 | MOD_CPC_SHIFT,   // CPC_B
    0x76 | MOD_CPC_SHIFT,   // CPC_C
    0x75 | MOD_CPC_SHIFT,   // CPC_D
    0x72 | MOD_CPC_SHIFT,   // CPC_E
    0x65 | MOD_CPC_SHIFT,   // CPC_F
    0x64 | MOD_CPC_SHIFT,   // CPC_G
    0x54 | MOD_CPC_SHIFT,   // CPC_H
    0x43 | MOD_CPC_SHIFT,   // CPC_I
    0x55 | MOD_CPC_SHIFT,   // CPC_J
    0x45 | MOD_CPC_SHIFT,   // CPC_K
    0x44 | MOD_CPC_SHIFT,   // CPC_L
    0x46 | MOD_CPC_SHIFT,   // CPC_M
    0x56 | MOD_CPC_SHIFT,   // CPC_N
    0x42 | MOD_CPC_SHIFT,   // CPC_O
    0x33 | MOD_CPC_SHIFT,   // CPC_P
    0x83 | MOD_CPC_SHIFT,   // CPC_Q
    0x62 | MOD_CPC_SHIFT,   // CPC_R
    0x74 | MOD_CPC_SHIFT,   // CPC_S
    0x63 | MOD_CPC_SHIFT,   // CPC_T
    0x52 | MOD_CPC_SHIFT,   // CPC_U
    0x67 | MOD_CPC_SHIFT,   // CPC_V
    0x73 | MOD_CPC_SHIFT,   // CPC_W
    0x77 | MOD_CPC_SHIFT,   // CPC_X
    0x53 | MOD_CPC_SHIFT,   // CPC_Y
    0x87 | MOD_CPC_SHIFT,   // CPC_Z
    0x85,                   // CPC_a
    0x66,                   // CPC_b
    0x76,                   // CPC_c
    0x75,                   // CPC_d
    0x72,                   // CPC_e
    0x65,                   // CPC_f
    0x64,                   // CPC_g
    0x54,                   // CPC_h
    0x43,                   // CPC_i
    0x55,                   // CPC_j
    0x45,                   // CPC_k
    0x44,                   // CPC_l
    0x46,                   // CPC_m
    0x56,                   // CPC_n
    0x42,                   // CPC_o
    0x33,                   // CPC_p
    0x83,                   // CPC_q
    0x62,                   // CPC_r
    0x74,                   // CPC_s
    0x63,                   // CPC_t
    0x52,                   // CPC_u
    0x67,                   // CPC_v
    0x73,                   // CPC_w
    0x77,                   // CPC_x
    0x53,                   // CPC_y
    0x87,                   // CPC_z
    0x60 | MOD_CPC_SHIFT,   // CPC_AMPERSAND
    0x35 | MOD_CPC_SHIFT,   // CPC_ASTERISK
    0x32,                   // CPC_AT
    0x26 | MOD_CPC_SHIFT,   // CPC_BACKQUOTE
    0x26,                   // CPC_BACKSLASH
    0x86,                   // CPC_CAPSLOCK
    0x20,                   // CPC_CLR
    0x35,                   // CPC_COLON
    0x47,                   // CPC_COMMA
    0x27,                   // CPC_CONTROL
    0x11,                   // CPC_COPY
    0x02 | MOD_CPC_SHIFT,   // CPC_CPY_DOWN
    0x10 | MOD_CPC_SHIFT,   // CPC_CPY_LEFT
    0x01 | MOD_CPC_SHIFT,   // CPC_CPY_RIGHT
    0x00 | MOD_CPC_SHIFT,   // CPC_CPY_UP
    0x02,                   // CPC_CUR_DOWN
    0x10,                   // CPC_CUR_LEFT
    0x01,                   // CPC_CUR_RIGHT
    0x00,                   // CPC_CUR_UP
    0x02 | MOD_CPC_CTRL,    // CPC_CUR_ENDBL
    0x10 | MOD_CPC_CTRL,    // CPC_CUR_HOMELN
    0x01 | MOD_CPC_CTRL,    // CPC_CUR_ENDLN
    0x00 | MOD_CPC_CTRL,    // CPC_CUR_HOMEBL
    0x81 | MOD_CPC_SHIFT,   // CPC_DBLQUOTE
    0x97,                   // CPC_DEL
    0x70 | MOD_CPC_SHIFT,   // CPC_DOLLAR
    0x06,                   // CPC_ENTER
    0x31 | MOD_CPC_SHIFT,   // CPC_EQUAL
    0x82,                   // CPC_ESC
    0x80 | MOD_CPC_SHIFT,   // CPC_EXCLAMATN
    0x17,                   // CPC_F0
    0x15,                   // CPC_F1
    0x16,                   // CPC_F2
    0x05,                   // CPC_F3
    0x24,                   // CPC_F4
    0x14,                   // CPC_F5
    0x04,                   // CPC_F6
    0x12,                   // CPC_F7
    0x13,                   // CPC_F8
    0x03,                   // CPC_F9
    0x07,                   // CPC_FPERIOD
    0x37 | MOD_CPC_SHIFT,   // CPC_GREATER
    0x71 | MOD_CPC_SHIFT,   // CPC_HASH
    0x21,                   // CPC_LBRACKET
    0x21 | MOD_CPC_SHIFT,   // CPC_LCBRACE
    0x50 | MOD_CPC_SHIFT,   // CPC_LEFTPAREN
    0x47 | MOD_CPC_SHIFT,   // CPC_LESS
    0x25,                   // CPC_LSHIFT
    0x31,                   // CPC_MINUS
    0x61 | MOD_CPC_SHIFT,   // CPC_PERCENT
    0x37,                   // CPC_PERIOD
    0x32 | MOD_CPC_SHIFT,   // CPC_PIPE
    0x34 | MOD_CPC_SHIFT,   // CPC_PLUS
    0x30 | MOD_CPC_SHIFT,   // CPC_POUND
    0x30,                   // CPC_POWER
    0x36 | MOD_CPC_SHIFT,   // CPC_QUESTION
    0x51 | MOD_CPC_SHIFT,   // CPC_QUOTE
    0x23,                   // CPC_RBRACKET
    0x23 | MOD_CPC_SHIFT,   // CPC_RCBRACE
    0x22,                   // CPC_RETURN
    0x41 | MOD_CPC_SHIFT,   // CPC_RIGHTPAREN
    0x25,                   // CPC_RSHIFT
    0x34,                   // CPC_SEMICOLON
    0x36,                   // CPC_SLASH
    0x57,                   // CPC_SPACE
    0x84,                   // CPC_TAB
    0x40 | MOD_CPC_SHIFT,   // CPC_UNDERSCORE
    0x90,                   // CPC_J0_UP
    0x91,                   // CPC_J0_DOWN
    0x92,                   // CPC_J0_LEFT
    0x93,                   // CPC_J0_RIGHT
    0x94,                   // CPC_J0_FIRE1
    0x95,                   // CPC_J0_FIRE2
    0x60,                   // CPC_J1_UP
    0x61,                   // CPC_J1_DOWN
    0x62,                   // CPC_J1_LEFT
    0x63,                   // CPC_J1_RIGHT
    0x64,                   // CPC_J1_FIRE1
    0x65,                   // CPC_J1_FIRE2
    0xff,                   // CPC_ES_NTILDE
    0xff,                   // CPC_ES_nTILDE
    0xff,                   // CPC_ES_PESETA
    0xff,                   // CPC_FR_eACUTE
    0xff,                   // CPC_FR_eGRAVE
    0xff,                   // CPC_FR_cCEDIL
    0xff,                   // CPC_FR_aGRAVE
    0xff,                   // CPC_FR_uGRAVE
  },
  { // French CPC keyboard
    0x40 | MOD_CPC_SHIFT,   // CPC_0
    0x80 | MOD_CPC_SHIFT,   // CPC_1
    0x81 | MOD_CPC_SHIFT,   // CPC_2
    0x71 | MOD_CPC_SHIFT,   // CPC_3
    0x70 | MOD_CPC_SHIFT,   // CPC_4
    0x61 | MOD_CPC_SHIFT,   // CPC_5
    0x60 | MOD_CPC_SHIFT,   // CPC_6
    0x51 | MOD_CPC_SHIFT,   // CPC_7
    0x50 | MOD_CPC_SHIFT,   // CPC_8
    0x41 | MOD_CPC_SHIFT,   // CPC_9
    0x83 | MOD_CPC_SHIFT,   // CPC_A
    0x66 | MOD_CPC_SHIFT,   // CPC_B
    0x76 | MOD_CPC_SHIFT,   // CPC_C
    0x75 | MOD_CPC_SHIFT,   // CPC_D
    0x72 | MOD_CPC_SHIFT,   // CPC_E
    0x65 | MOD_CPC_SHIFT,   // CPC_F
    0x64 | MOD_CPC_SHIFT,   // CPC_G
    0x54 | MOD_CPC_SHIFT,   // CPC_H
    0x43 | MOD_CPC_SHIFT,   // CPC_I
    0x55 | MOD_CPC_SHIFT,   // CPC_J
    0x45 | MOD_CPC_SHIFT,   // CPC_K
    0x44 | MOD_CPC_SHIFT,   // CPC_L
    0x35 | MOD_CPC_SHIFT,   // CPC_M
    0x56 | MOD_CPC_SHIFT,   // CPC_N
    0x42 | MOD_CPC_SHIFT,   // CPC_O
    0x33 | MOD_CPC_SHIFT,   // CPC_P
    0x85 | MOD_CPC_SHIFT,   // CPC_Q
    0x62 | MOD_CPC_SHIFT,   // CPC_R
    0x74 | MOD_CPC_SHIFT,   // CPC_S
    0x63 | MOD_CPC_SHIFT,   // CPC_T
    0x52 | MOD_CPC_SHIFT,   // CPC_U
    0x67 | MOD_CPC_SHIFT,   // CPC_V
    0x87 | MOD_CPC_SHIFT,   // CPC_W
    0x77 | MOD_CPC_SHIFT,   // CPC_X
    0x53 | MOD_CPC_SHIFT,   // CPC_Y
    0x73 | MOD_CPC_SHIFT,   // CPC_Z
    0x83,                   // CPC_a
    0x66,                   // CPC_b
    0x76,                   // CPC_c
    0x75,                   // CPC_d
    0x72,                   // CPC_e
    0x65,                   // CPC_f
    0x64,                   // CPC_g
    0x54,                   // CPC_h
    0x43,                   // CPC_i
    0x55,                   // CPC_j
    0x45,                   // CPC_k
    0x44,                   // CPC_l
    0x35,                   // CPC_m
    0x56,                   // CPC_n
    0x42,                   // CPC_o
    0x33,                   // CPC_p
    0x85,                   // CPC_q
    0x62,                   // CPC_r
    0x74,                   // CPC_s
    0x63,                   // CPC_t
    0x52,                   // CPC_u
    0x67,                   // CPC_v
    0x87,                   // CPC_w
    0x77,                   // CPC_x
    0x53,                   // CPC_y
    0x73,                   // CPC_z
    0x80,                   // CPC_AMPERSAND
    0x21,                   // CPC_ASTERISK
    0x26 | MOD_CPC_SHIFT,   // CPC_AT
    0xff,                   // CPC_BACKQUOTE
    0x26 | MOD_CPC_CTRL,    // CPC_BACKSLASH
    0x86,                   // CPC_CAPSLOCK
    0x20,                   // CPC_CLR
    0x37,                   // CPC_COLON
    0x46,                   // CPC_COMMA
    0x27,                   // CPC_CONTROL
    0x11,                   // CPC_COPY
    0x02 | MOD_CPC_SHIFT,   // CPC_CPY_DOWN
    0x10 | MOD_CPC_SHIFT,   // CPC_CPY_LEFT
    0x01 | MOD_CPC_SHIFT,   // CPC_CPY_RIGHT
    0x00 | MOD_CPC_SHIFT,   // CPC_CPY_UP
    0x02,                   // CPC_CUR_DOWN
    0x10,                   // CPC_CUR_LEFT
    0x01,                   // CPC_CUR_RIGHT
    0x00,                   // CPC_CUR_UP
    0x02 | MOD_CPC_CTRL,    // CPC_CUR_ENDBL
    0x10 | MOD_CPC_CTRL,    // CPC_CUR_HOMELN
    0x01 | MOD_CPC_CTRL,    // CPC_CUR_ENDLN
    0x00 | MOD_CPC_CTRL,    // CPC_CUR_HOMEBL
    0x71,                   // CPC_DBLQUOTE
    0x97,                   // CPC_DEL
    0x26,                   // CPC_DOLLAR
    0x06,                   // CPC_ENTER
    0x36,                   // CPC_EQUAL
    0x82,                   // CPC_ESC
    0x50,                   // CPC_EXCLAMATN
    0x17,                   // CPC_F0
    0x15,                   // CPC_F1
    0x16,                   // CPC_F2
    0x05,                   // CPC_F3
    0x24,                   // CPC_F4
    0x14,                   // CPC_F5
    0x04,                   // CPC_F6
    0x12,                   // CPC_F7
    0x13,                   // CPC_F8
    0x03,                   // CPC_F9
    0x07,                   // CPC_FPERIOD
    0x23 | MOD_CPC_SHIFT,   // CPC_GREATER
    0x23,                   // CPC_HASH
    0x31 | MOD_CPC_SHIFT,   // CPC_LBRACKET
    0xff,                   // CPC_LCBRACE
    0x61,                   // CPC_LEFTPAREN
    0x21 | MOD_CPC_SHIFT,   // CPC_LESS
    0x25,                   // CPC_LSHIFT
    0x30,                   // CPC_MINUS
    0x34 | MOD_CPC_SHIFT,   // CPC_PERCENT
    0x47 | MOD_CPC_SHIFT,   // CPC_PERIOD
    0x32 | MOD_CPC_SHIFT,   // CPC_PIPE
    0x36 | MOD_CPC_SHIFT,   // CPC_PLUS
    0xff,                   // CPC_POUND
    0x32,                   // CPC_POWER
    0x46 | MOD_CPC_SHIFT,   // CPC_QUESTION
    0x70,                   // CPC_QUOTE
    0x60,                   // CPC_RBRACKET
    0xff,                   // CPC_RCBRACE
    0x22,                   // CPC_RETURN
    0x31,                   // CPC_RIGHTPAREN
    0x25,                   // CPC_RSHIFT
    0x47,                   // CPC_SEMICOLON
    0x37 | MOD_CPC_SHIFT,   // CPC_SLASH
    0x57,                   // CPC_SPACE
    0x84,                   // CPC_TAB
    0x30 | MOD_CPC_SHIFT,   // CPC_UNDERSCORE
    0x90,                   // CPC_J0_UP
    0x91,                   // CPC_J0_DOWN
    0x92,                   // CPC_J0_LEFT
    0x93,                   // CPC_J0_RIGHT
    0x94,                   // CPC_J0_FIRE1
    0x95,                   // CPC_J0_FIRE2
    0x60,                   // CPC_J1_UP
    0x61,                   // CPC_J1_DOWN
    0x62,                   // CPC_J1_LEFT
    0x63,                   // CPC_J1_RIGHT
    0x64,                   // CPC_J1_FIRE1
    0x65,                   // CPC_J1_FIRE2
    0xff,                   // CPC_ES_NTILDE
    0xff,                   // CPC_ES_nTILDE
    0xff,                   // CPC_ES_PESETA
    0x81,                   // CPC_FR_eACUTE
    0x51,                   // CPC_FR_eGRAVE
    0x41,                   // CPC_FR_cCEDIL
    0x40,                   // CPC_FR_aGRAVE
    0x34,                   // CPC_FR_uGRAVE
  },
  { // Spanish CPC keyboard
    0x40,                   // CPC_0
    0x80,                   // CPC_1
    0x81,                   // CPC_2
    0x71,                   // CPC_3
    0x70,                   // CPC_4
    0x61,                   // CPC_5
    0x60,                   // CPC_6
    0x51,                   // CPC_7
    0x50,                   // CPC_8
    0x41,                   // CPC_9
    0x85 | MOD_CPC_SHIFT,   // CPC_A
    0x66 | MOD_CPC_SHIFT,   // CPC_B
    0x76 | MOD_CPC_SHIFT,   // CPC_C
    0x75 | MOD_CPC_SHIFT,   // CPC_D
    0x72 | MOD_CPC_SHIFT,   // CPC_E
    0x65 | MOD_CPC_SHIFT,   // CPC_F
    0x64 | MOD_CPC_SHIFT,   // CPC_G
    0x54 | MOD_CPC_SHIFT,   // CPC_H
    0x43 | MOD_CPC_SHIFT,   // CPC_I
    0x55 | MOD_CPC_SHIFT,   // CPC_J
    0x45 | MOD_CPC_SHIFT,   // CPC_K
    0x44 | MOD_CPC_SHIFT,   // CPC_L
    0x46 | MOD_CPC_SHIFT,   // CPC_M
    0x56 | MOD_CPC_SHIFT,   // CPC_N
    0x42 | MOD_CPC_SHIFT,   // CPC_O
    0x33 | MOD_CPC_SHIFT,   // CPC_P
    0x83 | MOD_CPC_SHIFT,   // CPC_Q
    0x62 | MOD_CPC_SHIFT,   // CPC_R
    0x74 | MOD_CPC_SHIFT,   // CPC_S
    0x63 | MOD_CPC_SHIFT,   // CPC_T
    0x52 | MOD_CPC_SHIFT,   // CPC_U
    0x67 | MOD_CPC_SHIFT,   // CPC_V
    0x73 | MOD_CPC_SHIFT,   // CPC_W
    0x77 | MOD_CPC_SHIFT,   // CPC_X
    0x53 | MOD_CPC_SHIFT,   // CPC_Y
    0x87 | MOD_CPC_SHIFT,   // CPC_Z
    0x85,                   // CPC_a
    0x66,                   // CPC_b
    0x76,                   // CPC_c
    0x75,                   // CPC_d
    0x72,                   // CPC_e
    0x65,                   // CPC_f
    0x64,                   // CPC_g
    0x54,                   // CPC_h
    0x43,                   // CPC_i
    0x55,                   // CPC_j
    0x45,                   // CPC_k
    0x44,                   // CPC_l
    0x46,                   // CPC_m
    0x56,                   // CPC_n
    0x42,                   // CPC_o
    0x33,                   // CPC_p
    0x83,                   // CPC_q
    0x62,                   // CPC_r
    0x74,                   // CPC_s
    0x63,                   // CPC_t
    0x52,                   // CPC_u
    0x67,                   // CPC_v
    0x73,                   // CPC_w
    0x77,                   // CPC_x
    0x53,                   // CPC_y
    0x87,                   // CPC_z
    0x60 | MOD_CPC_SHIFT,   // CPC_AMPERSAND
    0x21 | MOD_CPC_SHIFT,   // CPC_ASTERISK
    0x32,                   // CPC_AT
    0x26 | MOD_CPC_SHIFT,   // CPC_BACKQUOTE
    0x26,                   // CPC_BACKSLASH
    0x86,                   // CPC_CAPSLOCK
    0x20,                   // CPC_CLR
    0x34 | MOD_CPC_SHIFT,   // CPC_COLON
    0x47,                   // CPC_COMMA
    0x27,                   // CPC_CONTROL
    0x11,                   // CPC_COPY
    0x02 | MOD_CPC_SHIFT,   // CPC_CPY_DOWN
    0x10 | MOD_CPC_SHIFT,   // CPC_CPY_LEFT
    0x01 | MOD_CPC_SHIFT,   // CPC_CPY_RIGHT
    0x00 | MOD_CPC_SHIFT,   // CPC_CPY_UP
    0x02,                   // CPC_CUR_DOWN
    0x10,                   // CPC_CUR_LEFT
    0x01,                   // CPC_CUR_RIGHT
    0x00,                   // CPC_CUR_UP
    0x02 | MOD_CPC_CTRL,    // CPC_CUR_ENDBL
    0x10 | MOD_CPC_CTRL,    // CPC_CUR_HOMELN
    0x01 | MOD_CPC_CTRL,    // CPC_CUR_ENDLN
    0x00 | MOD_CPC_CTRL,    // CPC_CUR_HOMEBL
    0x81 | MOD_CPC_SHIFT,   // CPC_DBLQUOTE
    0x97,                   // CPC_DEL
    0x70 | MOD_CPC_SHIFT,   // CPC_DOLLAR
    0x06,                   // CPC_ENTER
    0x31 | MOD_CPC_SHIFT,   // CPC_EQUAL
    0x82,                   // CPC_ESC
    0x80 | MOD_CPC_SHIFT,   // CPC_EXCLAMATN
    0x17,                   // CPC_F0
    0x15,                   // CPC_F1
    0x16,                   // CPC_F2
    0x05,                   // CPC_F3
    0x24,                   // CPC_F4
    0x14,                   // CPC_F5
    0x04,                   // CPC_F6
    0x12,                   // CPC_F7
    0x13,                   // CPC_F8
    0x03,                   // CPC_F9
    0x07,                   // CPC_FPERIOD
    0x37 | MOD_CPC_SHIFT,   // CPC_GREATER
    0x71 | MOD_CPC_SHIFT,   // CPC_HASH
    0x21,                   // CPC_LBRACKET
    0xff,                   // CPC_LCBRACE
    0x50 | MOD_CPC_SHIFT,   // CPC_LEFTPAREN
    0x47 | MOD_CPC_SHIFT,   // CPC_LESS
    0x25,                   // CPC_LSHIFT
    0x31,                   // CPC_MINUS
    0x61 | MOD_CPC_SHIFT,   // CPC_PERCENT
    0x37,                   // CPC_PERIOD
    0x32 | MOD_CPC_SHIFT,   // CPC_PIPE
    0x23 | MOD_CPC_SHIFT,   // CPC_PLUS
    0xff,                   // CPC_POUND
    0x30,                   // CPC_POWER
    0x36 | MOD_CPC_SHIFT,   // CPC_QUESTION
    0x51 | MOD_CPC_SHIFT,   // CPC_QUOTE
    0x23,                   // CPC_RBRACKET
    0xff,                   // CPC_RCBRACE
    0x22,                   // CPC_RETURN
    0x41 | MOD_CPC_SHIFT,   // CPC_RIGHTPAREN
    0x25,                   // CPC_RSHIFT
    0x34,                   // CPC_SEMICOLON
    0x36,                   // CPC_SLASH
    0x57,                   // CPC_SPACE
    0x84,                   // CPC_TAB
    0x40 | MOD_CPC_SHIFT,   // CPC_UNDERSCORE
    0x90,                   // CPC_J0_UP
    0x91,                   // CPC_J0_DOWN
    0x92,                   // CPC_J0_LEFT
    0x93,                   // CPC_J0_RIGHT
    0x94,                   // CPC_J0_FIRE1
    0x95,                   // CPC_J0_FIRE2
    0x60,                   // CPC_J1_UP
    0x61,                   // CPC_J1_DOWN
    0x62,                   // CPC_J1_LEFT
    0x63,                   // CPC_J1_RIGHT
    0x64,                   // CPC_J1_FIRE1
    0x65,                   // CPC_J1_FIRE2
    0x35 | MOD_CPC_SHIFT,   // CPC_ES_NTILDE
    0x35,                   // CPC_ES_nTILDE
    0x30 | MOD_CPC_SHIFT,   // CPC_ES_PESETA
    0xff,                   // CPC_FR_eACUTE
    0xff,                   // CPC_FR_eGRAVE
    0xff,                   // CPC_FR_cCEDIL
    0xff,                   // CPC_FR_aGRAVE
    0xff,                   // CPC_FR_uGRAVE
  }
};

#define ERR_OUT_OF_MEMORY        9

#define ERR_INPUT_INIT           1
#define ERR_VIDEO_INIT           2
#define ERR_VIDEO_SET_MODE       3
#define ERR_VIDEO_SURFACE        4
#define ERR_VIDEO_PALETTE        5
#define ERR_VIDEO_COLOUR_DEPTH   6
#define ERR_AUDIO_INIT           7
#define ERR_AUDIO_RATE           8
#define ERR_OUT_OF_MEMORY        9
#define ERR_CPC_ROM_MISSING      10
#define ERR_NOT_A_CPC_ROM        11
#define ERR_ROM_NOT_FOUND        12
#define ERR_FILE_NOT_FOUND       13
#define ERR_FILE_BAD_ZIP         14
#define ERR_FILE_EMPTY_ZIP       15
#define ERR_FILE_UNZIP_FAILED    16
#define ERR_SNA_INVALID          17
#define ERR_SNA_SIZE             18
#define ERR_SNA_CPC_TYPE         19
#define ERR_SNA_WRITE            20
#define ERR_DSK_INVALID          21
#define ERR_DSK_SIDES            22
#define ERR_DSK_SECTORS          23
#define ERR_DSK_WRITE            24
#define MSG_DSK_ALTERED          25
#define ERR_TAP_INVALID          26
#define ERR_TAP_UNSUPPORTED      27
#define ERR_TAP_BAD_VOC          28
#define ERR_PRINTER              29
#define ERR_BAD_MF2_ROM          30
#define ERR_SDUMP                31

char chROMFile[3][14] = {
   "cpc464.rom",
   "cpc664.rom",
   "cpc6128.rom"
};

#define MAX_KBD_LAYOUTS 2
#include "4.1.0/rom_mods.c"

t_disk_format disk_format[MAX_DISK_FORMAT] = {
  { "178K Data Format", 40, 1, 9, 2, 0x52, 0xe5, {{ 0xc1, 0xc6, 0xc2, 0xc7, 0xc3, 0xc8, 0xc4, 0xc9, 0xc5 }} },
  { "169K Vendor Format", 40, 1, 9, 2, 0x52, 0xe5, {{ 0x41, 0x46, 0x42, 0x47, 0x43, 0x48, 0x44, 0x49, 0x45 }} }
};

extern PspImage *Screen;

t_CPC CPC;
t_CRTC CRTC;
t_FDC FDC;
t_GateArray GateArray;
t_PPI PPI;
t_PSG PSG;
t_VDU VDU;
t_drive driveA;
t_drive driveB;

dword dwTicks, dwTicksOffset, dwTicksTarget, dwTicksTargetFPS;
dword dwFPS, dwFrameCount;
dword dwXScale = 1, dwYScale = 1;
dword dwSndBufferCopied;

dword dwBreakPoint, dwTrace, dwMF2ExitAddr;
dword dwMF2Flags = 0;
byte *pbGPBuffer = NULL;
byte *pbSndBuffer = NULL;
byte *pbSndBufferEnd = NULL;
byte *pbSndStream = NULL;
byte *membank_read[4], *membank_write[4], *memmap_ROM[256];
byte *pbRAM = NULL;
byte *pbROMlo = NULL;
byte *pbROMhi = NULL;
byte *pbExpansionROM = NULL;
byte *pbMF2ROMbackup = NULL;
byte *pbMF2ROM = NULL;
byte *pbTapeImage = NULL;
byte *pbTapeImageEnd = NULL;
byte mode0_table[512], mode1_table[1024];
byte keyboard_matrix[16];

static byte *membank_config[8][4];

FILE *pfileObject;
FILE *pfoPrinter;

extern byte bTapeLevel;
extern t_z80regs z80;

extern dword *ScanPos;
extern dword *ScanStart;
extern word MaxVSync;

#define psg_write \
{ \
   byte control = PSG.control & 0xc0; /* isolate PSG control bits */ \
   if (control == 0xc0) { /* latch address? */ \
      PSG.reg_select = psg_data; /* select new PSG register */ \
   } else if (control == 0x80) { /* write? */ \
      if (PSG.reg_select < 16) { /* valid register? */ \
         SetAYRegister(PSG.reg_select, psg_data); \
      } \
   } \
}

typedef void (*MH)(dword);
typedef void (*BH)(void);

MH mode_handler[4] = {
   draw32bpp_mode0_double,
   draw32bpp_mode1_double,
   draw32bpp_mode2_double,
   draw32bpp_mode0_double,
};
BH border_handler = draw32bpp_border_double;

static byte CRTC_values[2][14] = {
   {0x3f, 0x28, 0x2e, 0x8e, 0x1f, 0x06, 0x19, 0x1b, 0x00, 0x07, 0x00, 0x00, 0x30, 0x00},
   {0x3f, 0x28, 0x2e, 0x8e, 0x26, 0x00, 0x19, 0x1e, 0x00, 0x07, 0x00, 0x00, 0x30, 0x00}
};

byte bit_values[8] = {
  0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};

static double colours_rgb[32][3] = {
  { 0.5, 0.5, 0.5 }, { 0.5, 0.5, 0.5 },{ 0.0, 1.0, 0.5 }, { 1.0, 1.0, 0.5 },
  { 0.0, 0.0, 0.5 }, { 1.0, 0.0, 0.5 },{ 0.0, 0.5, 0.5 }, { 1.0, 0.5, 0.5 },
  { 1.0, 0.0, 0.5 }, { 1.0, 1.0, 0.5 },{ 1.0, 1.0, 0.0 }, { 1.0, 1.0, 1.0 },
  { 1.0, 0.0, 0.0 }, { 1.0, 0.0, 1.0 },{ 1.0, 0.5, 0.0 }, { 1.0, 0.5, 1.0 },
  { 0.0, 0.0, 0.5 }, { 0.0, 1.0, 0.5 },{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 1.0 },
  { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 },{ 0.0, 0.5, 0.0 }, { 0.0, 0.5, 1.0 },
  { 0.5, 0.0, 0.5 }, { 0.5, 1.0, 0.5 },{ 0.5, 1.0, 0.0 }, { 0.5, 1.0, 1.0 },
  { 0.5, 0.0, 0.0 }, { 0.5, 0.0, 1.0 },{ 0.5, 0.5, 0.0 }, { 0.5, 0.5, 1.0 }
};

static double colours_green[32] = {
   0.5647, 0.5647, 0.7529, 0.9412,
   0.1882, 0.3765, 0.4706, 0.6588,
   0.3765, 0.9412, 0.9098, 0.9725,
   0.3451, 0.4078, 0.6275, 0.6902,
   0.1882, 0.7529, 0.7216, 0.7843,
   0.1569, 0.2196, 0.4392, 0.5020,
   0.2824, 0.8471, 0.8157, 0.8784,
   0.2510, 0.3137, 0.5333, 0.5961
};

dword colour_table[32];

void ga_init_banking (void)
{
   byte *romb0, *romb1, *romb2, *romb3, *romb4, *romb5, *romb6, *romb7;
   byte *pbRAMbank;

   romb0 = pbRAM;
   romb1 = pbRAM + 1*16384;
   romb2 = pbRAM + 2*16384;
   romb3 = pbRAM + 3*16384;

   pbRAMbank = pbRAM + ((GateArray.RAM_bank + 1) * 65536);
   romb4 = pbRAMbank;
   romb5 = pbRAMbank + 1*16384;
   romb6 = pbRAMbank + 2*16384;
   romb7 = pbRAMbank + 3*16384;

   membank_config[0][0] = romb0;
   membank_config[0][1] = romb1;
   membank_config[0][2] = romb2;
   membank_config[0][3] = romb3;

   membank_config[1][0] = romb0;
   membank_config[1][1] = romb1;
   membank_config[1][2] = romb2;
   membank_config[1][3] = romb7;

   membank_config[2][0] = romb4;
   membank_config[2][1] = romb5;
   membank_config[2][2] = romb6;
   membank_config[2][3] = romb7;

   membank_config[3][0] = romb0;
   membank_config[3][1] = romb3;
   membank_config[3][2] = romb2;
   membank_config[3][3] = romb7;

   membank_config[4][0] = romb0;
   membank_config[4][1] = romb4;
   membank_config[4][2] = romb2;
   membank_config[4][3] = romb3;

   membank_config[5][0] = romb0;
   membank_config[5][1] = romb5;
   membank_config[5][2] = romb2;
   membank_config[5][3] = romb3;

   membank_config[6][0] = romb0;
   membank_config[6][1] = romb6;
   membank_config[6][2] = romb2;
   membank_config[6][3] = romb3;

   membank_config[7][0] = romb0;
   membank_config[7][1] = romb7;
   membank_config[7][2] = romb2;
   membank_config[7][3] = romb3;
}

void ga_memory_manager (void)
{
   dword mem_bank;
   if (CPC.ram_size == 64) { // 64KB of RAM?
      mem_bank = 0; // no expansion memory
      GateArray.RAM_config = 0; // the only valid configuration is 0
   } else {
      mem_bank = (GateArray.RAM_config >> 3) & 7; // extract expansion memory bank
      if (((mem_bank+2)*64) > CPC.ram_size) { // selection is beyond available memory?
         mem_bank = 0; // force default mapping
      }
   }
   if (mem_bank != GateArray.RAM_bank) { // requested bank is different from the active one?
      GateArray.RAM_bank = mem_bank;
      ga_init_banking();
   }
   for (int n = 0; n < 4; n++) { // remap active memory banks
      membank_read[n] = membank_config[GateArray.RAM_config & 7][n];
      membank_write[n] = membank_config[GateArray.RAM_config & 7][n];
   }
   if (!(GateArray.ROM_config & 0x04)) { // lower ROM is enabled?
      if (dwMF2Flags & MF2_ACTIVE) { // is the Multiface 2 paged in?
         membank_read[0] = pbMF2ROM;
         membank_write[0] = pbMF2ROM;
      } else {
         membank_read[0] = pbROMlo; // 'page in' lower ROM
      }
   }
   if (!(GateArray.ROM_config & 0x08)) { // upper/expansion ROM is enabled?
      membank_read[3] = pbExpansionROM; // 'page in' upper/expansion ROM
   }
}

byte z80_IN_handler (reg_pair port)
{
   byte ret_val;

   ret_val = 0xff; // default return value
// CRTC -----------------------------------------------------------------------
   if (!(port.b.h & 0x40)) { // CRTC chip select?
      if ((port.b.h & 3) == 3) { // read CRTC register?
         if ((CRTC.reg_select > 11) && (CRTC.reg_select < 18)) { // valid range?
            ret_val = CRTC.registers[CRTC.reg_select];
         }
         else {
            ret_val = 0; // write only registers return 0
         }
      }
   }
// PPI ------------------------------------------------------------------------
   else if (!(port.b.h & 0x08)) { // PPI chip select?
      byte ppi_port = port.b.h & 3;
      switch (ppi_port) {
         case 0: // read from port A?
            if (PPI.control & 0x10) { // port A set to input?
               if ((PSG.control & 0xc0) == 0x40) { // PSG control set to read?
                  if (PSG.reg_select < 16) { // within valid range?
                     if (PSG.reg_select == 14) { // PSG port A?
                        if (!(PSG.RegisterAY.Index[7] & 0x40)) { // port A in input mode?
                           ret_val = keyboard_matrix[CPC.keyboard_line & 0x0f]; // read keyboard matrix node status
                        } else {
                           ret_val = PSG.RegisterAY.Index[14] & (keyboard_matrix[CPC.keyboard_line & 0x0f]); // return last value w/ logic AND of input
                        }
                     } else if (PSG.reg_select == 15) { // PSG port B?
                        if ((PSG.RegisterAY.Index[7] & 0x80)) { // port B in output mode?
                           ret_val = PSG.RegisterAY.Index[15]; // return stored value
                        }
                     } else {
                        ret_val = PSG.RegisterAY.Index[PSG.reg_select]; // read PSG register
                     }
                  }
               }
            } else {
               ret_val = PPI.portA; // return last programmed value
            }
            break;

         case 1: // read from port B?
            if (PPI.control & 2) { // port B set to input?
               ret_val = bTapeLevel | // tape level when reading
                         (CPC.printer ? 0 : 0x40) | // ready line of connected printer
                         (CPC.jumpers & 0x7f) | // manufacturer + 50Hz
                         (CRTC.flags & VS_flag); // VSYNC status
            } else {
               ret_val = PPI.portB; // return last programmed value
            }
            break;

         case 2: // read from port C?
            byte direction = PPI.control & 9; // isolate port C directions
            ret_val = PPI.portC; // default to last programmed value
            if (direction) { // either half set to input?
               if (direction & 8) { // upper half set to input?
                  ret_val &= 0x0f; // blank out upper half
                  byte val = PPI.portC & 0xc0; // isolate PSG control bits
                  if (val == 0xc0) { // PSG specify register?
                     val = 0x80; // change to PSG write register
                  }
                  ret_val |= val | 0x20; // casette write data is always set
                  if (CPC.tape_motor) {
                     ret_val |= 0x10; // set the bit if the tape motor is running
                  }
               }
               if (!(direction & 1)) { // lower half set to output?
                  ret_val |= 0x0f; // invalid - set all bits
               }
            }
            break;
      }
   }
// ----------------------------------------------------------------------------
   else if (!(port.b.h & 0x04)) { // external peripheral?
      if ((port.b.h == 0xfb) && (!(port.b.l & 0x80))) { // FDC?
         if (!(port.b.l & 0x01)) { // FDC status register?
            ret_val = fdc_read_status();
         } else { // FDC data register
            ret_val = fdc_read_data();
         }
      }
   }
   return ret_val;
}



void z80_OUT_handler (reg_pair port, byte val)
{
// Gate Array -----------------------------------------------------------------
   if ((port.b.h & 0xc0) == 0x40) { // GA chip select?
      switch (val >> 6) {
         case 0: // select pen
            #ifdef DEBUG_GA
            if (dwDebugFlag) {
               fprintf(pfoDebug, "pen 0x%02x\r\n", val);
            }
            #endif
            GateArray.pen = val & 0x10 ? 0x10 : val & 0x0f; // if bit 5 is set, pen indexes the border colour
            if (CPC.mf2) { // MF2 enabled?
               *(pbMF2ROM + 0x03fcf) = val;
            }
            break;
         case 1: // set colour
            #ifdef DEBUG_GA
            if (dwDebugFlag) {
               fprintf(pfoDebug, "clr 0x%02x\r\n", val);
            }
            #endif
            {
               byte colour = val & 0x1f; // isolate colour value
               GateArray.ink_values[GateArray.pen] = colour;
               GateArray.palette[GateArray.pen] = colour_table[colour];
            }
            if (CPC.mf2) { // MF2 enabled?
               int iPen = *(pbMF2ROM + 0x03fcf);
               *(pbMF2ROM + (0x03f90 | ((iPen & 0x10) << 2) | (iPen & 0x0f))) = val;
            }
            break;
         case 2: // set mode
            #ifdef DEBUG_GA
            if (dwDebugFlag) {
               fprintf(pfoDebug, "rom 0x%02x\r\n", val);
            }
            #endif
            GateArray.ROM_config = val;
            GateArray.requested_scr_mode = (void(*)(dword))mode_handler[val & 0x03]; // request a new CPC screen mode
            ga_memory_manager();
            if (val & 0x10) { // delay Z80 interrupt?
               z80.int_pending = 0; // clear pending interrupts
               GateArray.sl_count = 0; // reset GA scanline counter
            }
            if (CPC.mf2) { // MF2 enabled?
               *(pbMF2ROM + 0x03fef) = val;
            }
            break;
         case 3: // set memory configuration
            #ifdef DEBUG_GA
            if (dwDebugFlag) {
               fprintf(pfoDebug, "mem 0x%02x\r\n", val);
            }
            #endif
            GateArray.RAM_config = val;
            ga_memory_manager();
            if (CPC.mf2) { // MF2 enabled?
               *(pbMF2ROM + 0x03fff) = val;
            }
            break;
      }
   }
// CRTC -----------------------------------------------------------------------
   if (!(port.b.h & 0x40)) { // CRTC chip select?
      byte crtc_port = port.b.h & 3;
      if (crtc_port == 0) { // CRTC register select?
         CRTC.reg_select = val;
         if (CPC.mf2) { // MF2 enabled?
            *(pbMF2ROM + 0x03cff) = val;
         }
      }
      else if (crtc_port == 1) { // CRTC write data?
         if (CRTC.reg_select < 16) { // only registers 0 - 15 can be written to
            CRTC.registers[CRTC.reg_select] = val;
            switch (CRTC.reg_select) {
               case 3: // sync width
                  CRTC.hsw = val & 0x0f; // isolate horizontal sync width
                  VDU.hsw = CRTC.hsw - 2; // GA delays HSYNC by 2 chars
                  if (VDU.hsw < 0) { // negative value?
                     VDU.hsw = 0; // no VDU HSYNC
                  }
                  else if (VDU.hsw > 4) { // HSYNC longer than 4 chars?
                     VDU.hsw = 4; // maxium of 4
                  }
                  CRTC.vsw = val >> 4; // isolate vertical sync width
                  if (!CRTC.vsw) {
                     CRTC.vsw = 16; // 0 = width of 16
                  }
                  break;
               case 5: // vertical total adjust
                  CRTC.vt_adjust = val & 0x1f;
                  break;
               case 8: // interlace and skew
                  CRTC.skew = (val >> 4) & 3; // isolate display timing skew
                  if (CRTC.skew == 3) { // no output?
                     CRTC.skew = 0xff;
                  }
                  break;
               case 9: // maximum raster count
                  CRTC.max_raster = val << 3; // modify value for easier handling
                  break;
               case 12: // start address high byte
               case 13: // start address low byte
                  {
                     dword val1 = CRTC.registers[12] & 0x3f;
                     dword val2 = val1 & 0x0f; // isolate screen size
                     val1 = (val1 << 1) & 0x60; // isolate CPC RAM bank
                     val2 |= val1; // combine
                     CRTC.requested_addr = (CRTC.registers[13] + (val2 << 8)) << 1;
                  }
                  break;
            }
         }
         if (CPC.mf2) { // MF2 enabled?
            *(pbMF2ROM + (0x03db0 | (*(pbMF2ROM + 0x03cff) & 0x0f))) = val;
         }
      }
   }
// ROM select -----------------------------------------------------------------
   if (!(port.b.h & 0x20)) { // ROM select?
      GateArray.upper_ROM = val;
      pbExpansionROM = memmap_ROM[val];
      if (pbExpansionROM == NULL) { // selected expansion ROM not present?
         pbExpansionROM = pbROMhi; // revert to BASIC ROM
      }
      if (!(GateArray.ROM_config & 0x08)) { // upper/expansion ROM is enabled?
         membank_read[3] = pbExpansionROM; // 'page in' upper/expansion ROM
      }
      if (CPC.mf2) { // MF2 enabled?
         *(pbMF2ROM + 0x03aac) = val;
      }
   }
// printer port ---------------------------------------------------------------
   if (!(port.b.h & 0x10)) { // printer port?
      CPC.printer_port = val ^ 0x80; // invert bit 7
      if (pfoPrinter) {
         if (!(CPC.printer_port & 0x80)) { // only grab data bytes; ignore the strobe signal
            fputc(CPC.printer_port, pfoPrinter); // capture printer output to file
         }
      }
   }
// PPI ------------------------------------------------------------------------
   if (!(port.b.h & 0x08)) { // PPI chip select?
      switch (port.b.h & 3) {
         case 0: // write to port A?
            PPI.portA = val;
            if (!(PPI.control & 0x10)) { // port A set to output?
               byte psg_data = val;
               psg_write
            }
            break;
         case 1: // write to port B?
            PPI.portB = val;
            break;
         case 2: // write to port C?
            PPI.portC = val;
            if (!(PPI.control & 1)) { // output lower half?
               CPC.keyboard_line = val;
            }
            if (!(PPI.control & 8)) { // output upper half?
               CPC.tape_motor = val & 0x10; // update tape motor control
               PSG.control = val; // change PSG control
               byte psg_data = PPI.portA;
               psg_write
            }
            break;
         case 3: // modify PPI control
            if (val & 0x80) { // change PPI configuration
               PPI.control = val; // update control byte
               PPI.portA = 0; // clear data for all ports
               PPI.portB = 0;
               PPI.portC = 0;
            } else { // bit manipulation of port C data
               if (val & 1) { // set bit?
                  byte bit = (val >> 1) & 7; // isolate bit to set
                  PPI.portC |= bit_values[bit]; // set requested bit
                  if (!(PPI.control & 1)) { // output lower half?
                     CPC.keyboard_line = PPI.portC;
                  }
                  if (!(PPI.control & 8)) { // output upper half?
                     CPC.tape_motor = PPI.portC & 0x10;
                     PSG.control = PPI.portC; // change PSG control
                     byte psg_data = PPI.portA;
                     psg_write
                  }
               } else {
                  byte bit = (val >> 1) & 7; // isolate bit to reset
                  PPI.portC &= ~(bit_values[bit]); // reset requested bit
                  if (!(PPI.control & 1)) { // output lower half?
                     CPC.keyboard_line = PPI.portC;
                  }
                  if (!(PPI.control & 8)) { // output upper half?
                     CPC.tape_motor = PPI.portC & 0x10;
                     PSG.control = PPI.portC; // change PSG control
                     byte psg_data = PPI.portA;
                     psg_write
                  }
               }
            }
            if (CPC.mf2) { // MF2 enabled?
               *(pbMF2ROM + 0x037ff) = val;
            }
            break;
      }
   }
// ----------------------------------------------------------------------------
   if ((port.b.h == 0xfa) && (!(port.b.l & 0x80))) { // floppy motor control?
      FDC.motor = val & 0x01;
      #ifdef DEBUG_FDC
      fputs(FDC.motor ? "\r\n--- motor on" : "\r\n--- motor off", pfoDebug);
      #endif
      FDC.flags |= STATUSDRVA_flag | STATUSDRVB_flag;
   }
   else if ((port.b.h == 0xfb) && (!(port.b.l & 0x80))) { // FDC data register?
      fdc_write_data(val);
   }
   else if ((CPC.mf2) && (port.b.h == 0xfe)) { // Multiface 2?
      if ((port.b.l == 0xe8) && (!(dwMF2Flags & MF2_INVISIBLE))) { // page in MF2 ROM?
         dwMF2Flags |= MF2_ACTIVE;
         ga_memory_manager();
      }
      else if (port.b.l == 0xea) { // page out MF2 ROM?
         dwMF2Flags &= ~MF2_ACTIVE;
         ga_memory_manager();
      }
   }
}

int file_size (int file_num)
{
  struct stat s;

  if (!fstat(file_num, &s)) {
    return s.st_size;
  } else {
    return 0;
  }
}

void tape_eject (void)
{
  free(pbTapeImage);
  pbTapeImage = NULL;
}

int tape_insert (char *pchFileName)
{
   long lFileSize;
   int iBlockLength;
   byte bID;
   byte *pbPtr, *pbBlock;

   tape_eject();
   if ((pfileObject = fopen(pchFileName, "rb")) == NULL) {
      return ERR_FILE_NOT_FOUND;
   }
   fread(pbGPBuffer, 10, 1, pfileObject); // read CDT header
   pbPtr = pbGPBuffer;
   if (memcmp(pbPtr, "ZXTape!\032", 8) != 0) { // valid CDT file?
      fclose(pfileObject);
      return ERR_TAP_INVALID;
   }
   if (*(pbPtr + 0x08) != 1) { // major version must be 1
      fclose(pfileObject);
      return ERR_TAP_INVALID;
   }
   lFileSize = file_size(fileno(pfileObject)) - 0x0a;
   if (lFileSize <= 0) { // the tape image should have at least one block...
      fclose(pfileObject);
      return ERR_TAP_INVALID;
   }
   pbTapeImage = (byte *)malloc(lFileSize+6);
   *pbTapeImage = 0x20; // start off with a pause block
   *(word *)(pbTapeImage+1) = 2000; // set the length to 2 seconds
   fread(pbTapeImage+3, lFileSize, 1, pfileObject); // append the entire CDT file
   fclose(pfileObject);
   *(pbTapeImage+lFileSize+3) = 0x20; // end with a pause block
   *(word *)(pbTapeImage+lFileSize+3+1) = 2000; // set the length to 2 seconds

   #ifdef DEBUG_TAPE
   fputs("--- New Tape\r\n", pfoDebug);
   #endif
   pbTapeImageEnd = pbTapeImage + lFileSize+6;
   pbBlock = pbTapeImage;
   bool bolGotDataBlock = false;
   while (pbBlock < pbTapeImageEnd) {
      bID = *pbBlock++;
      switch(bID) {
         case 0x10: // standard speed data block
            iBlockLength = *(word *)(pbBlock+2) + 4;
            bolGotDataBlock = true;
            break;
         case 0x11: // turbo loading data block
            iBlockLength = (*(dword *)(pbBlock+0x0f) & 0x00ffffff) + 0x12;
            bolGotDataBlock = true;
            break;
         case 0x12: // pure tone
            iBlockLength = 4;
            bolGotDataBlock = true;
            break;
         case 0x13: // sequence of pulses of different length
            iBlockLength = *pbBlock * 2 + 1;
            bolGotDataBlock = true;
            break;
         case 0x14: // pure data block
            iBlockLength = (*(dword *)(pbBlock+0x07) & 0x00ffffff) + 0x0a;
            bolGotDataBlock = true;
            break;
         case 0x15: // direct recording
            iBlockLength = (*(dword *)(pbBlock+0x05) & 0x00ffffff) + 0x08;
            bolGotDataBlock = true;
            break;
         case 0x20: // pause
            if ((!bolGotDataBlock) && (pbBlock != pbTapeImage+1)) {
               *(word *)pbBlock = 0; // remove any pauses (execept ours) before the data starts
            }
            iBlockLength = 2;
            break;
         case 0x21: // group start
            iBlockLength = *pbBlock + 1;
            break;
         case 0x22: // group end
            iBlockLength = 0;
            break;
         case 0x23: // jump to block
   return ERR_TAP_UNSUPPORTED;
            iBlockLength = 2;
            break;
         case 0x24: // loop start
   return ERR_TAP_UNSUPPORTED;
            iBlockLength = 2;
            break;
         case 0x25: // loop end
   return ERR_TAP_UNSUPPORTED;
            iBlockLength = 0;
            break;
         case 0x26: // call sequence
   return ERR_TAP_UNSUPPORTED;
            iBlockLength = (*(word *)pbBlock * 2) + 2;
            break;
         case 0x27: // return from sequence
   return ERR_TAP_UNSUPPORTED;
            iBlockLength = 0;
            break;
         case 0x28: // select block
   return ERR_TAP_UNSUPPORTED;
            iBlockLength = *(word *)pbBlock + 2;
            break;
         case 0x30: // text description
            iBlockLength = *pbBlock + 1;
            break;
         case 0x31: // message block
            iBlockLength = *(pbBlock+1) + 2;
            break;
         case 0x32: // archive info
            iBlockLength = *(word *)pbBlock + 2;
            break;
         case 0x33: // hardware type
            iBlockLength = (*pbBlock * 3) + 1;
            break;
         case 0x34: // emulation info
            iBlockLength = 8;
            break;
         case 0x35: // custom info block
            iBlockLength = *(dword *)(pbBlock+0x10) + 0x14;
            break;
         case 0x40: // snapshot block
            iBlockLength = (*(dword *)(pbBlock+0x01) & 0x00ffffff) + 0x04;
            break;
         case 0x5A: // another tzx/cdt file
            iBlockLength = 9;
            break;

         default: // "extension rule"
            iBlockLength = *(dword *)pbBlock + 4;
      }

      #ifdef DEBUG_TAPE
      fprintf(pfoDebug, "%02x %d\r\n", bID, iBlockLength);
      #endif

      pbBlock += iBlockLength;
   }
   if (pbBlock != pbTapeImageEnd) {
      tape_eject();
      return ERR_TAP_INVALID;
   }

   Tape_Rewind();

/* char *pchTmpBuffer = new char[MAX_LINE_LEN];
   LoadString(hAppInstance, MSG_TAP_INSERT, chMsgBuffer, sizeof(chMsgBuffer));
   snprintf(pchTmpBuffer, _MAX_PATH-1, chMsgBuffer, CPC.tape_file);
   add_message(pchTmpBuffer);
   delete [] pchTmpBuffer; */
   return 0;
}

int tape_insert_voc (char *pchFileName)
{
  long lFileSize, lOffset, lInitialOffset, lSampleLength;
  int iBlockLength;
  byte *pbPtr, *pbTapeImagePtr, *pbVocDataBlock, *pbVocDataBlockPtr;
  bool bolDone;

  tape_eject();
  if ((pfileObject = fopen(pchFileName, "rb")) == NULL) {
    return ERR_FILE_NOT_FOUND;
  }
  fread(pbGPBuffer, 26, 1, pfileObject); // read VOC header
  pbPtr = pbGPBuffer;
  if (memcmp(pbPtr, "Creative Voice File\032", 20) != 0) { // valid VOC file?
    fclose(pfileObject);
    return ERR_TAP_BAD_VOC;
  }
  lOffset =
      lInitialOffset = *(word *)(pbPtr + 0x14);
  lFileSize = file_size(fileno(pfileObject));
  if ((lFileSize-26) <= 0) { // should have at least one block...
    fclose(pfileObject);
    return ERR_TAP_BAD_VOC;
  }

#ifdef DEBUG_TAPE
  fputs("--- New Tape\r\n", pfoDebug);
#endif
  iBlockLength = 0;
  lSampleLength = 0;
  byte bSampleRate = 0;
  bolDone = false;
  while ((!bolDone) && (lOffset < lFileSize)) {
    fseek(pfileObject, lOffset, SEEK_SET);
    fread(pbPtr, 16, 1, pfileObject); // read block ID + size
#ifdef DEBUG_TAPE
    fprintf(pfoDebug, "%02x %d\r\n", *pbPtr, *(dword *)(pbPtr+0x01) & 0x00ffffff);
#endif
    switch(*pbPtr) {
      case 0x0: // terminator
        bolDone = true;
        break;
        case 0x1: // sound data
          iBlockLength = (*(dword *)(pbPtr+0x01) & 0x00ffffff) + 4;
          lSampleLength += iBlockLength - 6;
          if ((bSampleRate) && (bSampleRate != *(pbPtr+0x04))) { // no change in sample rate allowed
            fclose(pfileObject);
            return ERR_TAP_BAD_VOC;
          }
          bSampleRate = *(pbPtr+0x04);
          if (*(pbPtr+0x05) != 0) { // must be 8 bits wide
            fclose(pfileObject);
            return ERR_TAP_BAD_VOC;
          }
          break;
          case 0x2: // sound continue
            iBlockLength = (*(dword *)(pbPtr+0x01) & 0x00ffffff) + 4;
            lSampleLength += iBlockLength - 4;
            break;
            case 0x3: // silence
              iBlockLength = 4;
              lSampleLength += *(word *)(pbPtr+0x01) + 1;
              if ((bSampleRate) && (bSampleRate != *(pbPtr+0x03))) { // no change in sample rate allowed
                fclose(pfileObject);
                return ERR_TAP_BAD_VOC;
              }
              bSampleRate = *(pbPtr+0x03);
              break;
              case 0x4: // marker
                iBlockLength = 3;
                break;
                case 0x5: // ascii
                  iBlockLength = (*(dword *)(pbPtr+0x01) & 0x00ffffff) + 4;
                  break;
      default:
        fclose(pfileObject);
        return ERR_TAP_BAD_VOC;
    }
    lOffset += iBlockLength;
  }
#ifdef DEBUG_TAPE
  fprintf(pfoDebug, "--- %ld bytes\r\n", lSampleLength);
#endif

  dword dwTapePulseCycles = 3500000L / (1000000L / (256 - bSampleRate)); // length of one pulse in ZX Spectrum T states
  dword dwCompressedSize = lSampleLength >> 3; // 8x data reduction
  if (dwCompressedSize > 0x00ffffff) { // we only support one direct recording block right now
    fclose(pfileObject);
    return ERR_TAP_BAD_VOC;
  }
  pbTapeImage = (byte *)malloc(dwCompressedSize+1+8+6);
  if (pbTapeImage == NULL) { // check if the memory allocation has failed
    fclose(pfileObject);
    return ERR_OUT_OF_MEMORY;
  }
  *pbTapeImage = 0x20; // start off with a pause block
  *(word *)(pbTapeImage+1) = 2000; // set the length to 2 seconds

  *(pbTapeImage+3) = 0x15; // direct recording block
  *(word *)(pbTapeImage+4) = (word)dwTapePulseCycles; // number of T states per sample
  *(word *)(pbTapeImage+6) = 0; // pause after block
  *(pbTapeImage+8) = lSampleLength & 7 ? lSampleLength & 7 : 8; // bits used in last byte
  *(dword *)(pbTapeImage+9) = dwCompressedSize & 0x00ffffff; // data length
  pbTapeImagePtr = pbTapeImage + 12;

  lOffset = lInitialOffset;
  bolDone = false;
  dword dwBit = 8;
  byte bByte = 0;
  while ((!bolDone) && (lOffset < lFileSize)) {
    fseek(pfileObject, lOffset, SEEK_SET);
    fread(pbPtr, 1, 1, pfileObject); // read block ID
    switch(*pbPtr) {
      case 0x0: // terminator
        bolDone = true;
        break;
        case 0x1: // sound data
          fread(pbPtr, 3+2, 1, pfileObject); // get block size and sound info
          iBlockLength = (*(dword *)(pbPtr) & 0x00ffffff) + 4;
          lSampleLength = iBlockLength - 6;
          pbVocDataBlock = (byte *)malloc(lSampleLength);
          if (pbVocDataBlock == NULL) {
            fclose(pfileObject);
            tape_eject();
            return ERR_OUT_OF_MEMORY;
          }
          fread(pbVocDataBlock, lSampleLength, 1, pfileObject);
          pbVocDataBlockPtr = pbVocDataBlock;
          for (int iBytePos = 0; iBytePos < lSampleLength; iBytePos++) {
            byte bVocSample = *pbVocDataBlockPtr++;
            dwBit--;
            if (bVocSample > VOC_THRESHOLD) {
              bByte |= bit_values[dwBit];
            }
            if (!dwBit) { // got all 8 bits?
              *pbTapeImagePtr++ = bByte;
              dwBit = 8;
              bByte = 0;
            }
          }
          free(pbVocDataBlock);
          break;
          case 0x2: // sound continue
            fread(pbPtr, 3, 1, pfileObject); // get block size
            iBlockLength = (*(dword *)(pbPtr) & 0x00ffffff) + 4;
            lSampleLength = iBlockLength - 4;
            pbVocDataBlock = (byte *)malloc(lSampleLength);
            if (pbVocDataBlock == NULL) {
              fclose(pfileObject);
              tape_eject();
              return ERR_OUT_OF_MEMORY;
            }
            fread(pbVocDataBlock, lSampleLength, 1, pfileObject);
            pbVocDataBlockPtr = pbVocDataBlock;
            for (int iBytePos = 0; iBytePos < lSampleLength; iBytePos++) {
              byte bVocSample = *pbVocDataBlockPtr++;
              dwBit--;
              if (bVocSample > VOC_THRESHOLD) {
                bByte |= bit_values[dwBit];
              }
              if (!dwBit) { // got all 8 bits?
                *pbTapeImagePtr++ = bByte;
                dwBit = 8;
                bByte = 0;
              }
            }
            free(pbVocDataBlock);
            break;
            case 0x3: // silence
              iBlockLength = 4;
              lSampleLength = *(word *)(pbPtr) + 1;
              for (int iBytePos = 0; iBytePos < lSampleLength; iBytePos++) {
                dwBit--;
                if (!dwBit) { // got all 8 bits?
                  *pbTapeImagePtr++ = bByte;
                  dwBit = 8;
                  bByte = 0;
                }
              }
              break;
              case 0x4: // marker
                iBlockLength = 3;
                break;
                case 0x5: // ascii
                  iBlockLength = (*(dword *)(pbPtr) & 0x00ffffff) + 4;
                  break;
    }
    lOffset += iBlockLength;
  }
  fclose(pfileObject);

  *pbTapeImagePtr = 0x20; // end with a pause block
  *(word *)(pbTapeImagePtr+1) = 2000; // set the length to 2 seconds

  pbTapeImageEnd = pbTapeImagePtr + 3;
/*
#ifdef DEBUG_TAPE
  if ((pfileObject = fopen("./test.cdt", "wb")) != NULL) {
  fwrite(pbTapeImage, (int)((pbTapeImagePtr+3)-pbTapeImage), 1, pfileObject);
  fclose(pfileObject);
}
#endif
*/
  Tape_Rewind();

/* char *pchTmpBuffer = new char[MAX_LINE_LEN];
  LoadString(hAppInstance, MSG_TAP_INSERT, chMsgBuffer, sizeof(chMsgBuffer));
  snprintf(pchTmpBuffer, _MAX_PATH-1, chMsgBuffer, CPC.tape_file);
  add_message(pchTmpBuffer);
  delete [] pchTmpBuffer; */
  return 0;
}

int snapshot_load_open(void *pfileObject)
{
   int n;
   dword dwSnapSize, dwModel;
   char chPath[_MAX_PATH + 1];
   byte val;
   reg_pair port;
   t_SNA_header sh;

   memset(&sh, 0, sizeof(sh));
  if (pfileObject) { //(pfileObject = fopen(pchFileName, "rb")) != NULL) {
    gzread(pfileObject, &sh, sizeof(sh)); // read snapshot header
    if (memcmp(sh.id, "MV - SNA", 8) != 0) { // valid SNApshot image?
         return ERR_SNA_INVALID;
      }
      dwSnapSize = sh.ram_size[0] + (sh.ram_size[1] * 256); // memory dump size
      dwSnapSize &= ~0x3f; // limit to multiples of 64
      if (!dwSnapSize) {
         return ERR_SNA_SIZE;
      }
      if (dwSnapSize > CPC.ram_size) { // memory dump size differs from current RAM size?
         byte *pbTemp;

         pbTemp = new byte [dwSnapSize*1024];
         if (pbTemp) {
            delete [] pbRAM;
            CPC.ram_size = dwSnapSize;
            pbRAM = pbTemp;
         } else {
            return ERR_OUT_OF_MEMORY;
         }
      }
      emulator_reset(false);
      n = gzread(pfileObject, pbRAM, dwSnapSize*1024); // read memory dump into CPC RAM
      if (!n) {
         emulator_reset(false);
         return ERR_SNA_INVALID;
      }

// Z80
      _A = sh.AF[1];
      _F = sh.AF[0];
      _B = sh.BC[1];
      _C = sh.BC[0];
      _D = sh.DE[1];
      _E = sh.DE[0];
      _H = sh.HL[1];
      _L = sh.HL[0];
      _R = sh.R & 0x7f;
      _Rb7 = sh.R & 0x80; // bit 7 of R
      _I = sh.I;
      if (sh.IFF0)
         _IFF1 = Pflag;
      if (sh.IFF1)
         _IFF2 = Pflag;
      _IXh = sh.IX[1];
      _IXl = sh.IX[0];
      _IYh = sh.IY[1];
      _IYl = sh.IY[0];
      z80.SP.b.h = sh.SP[1];
      z80.SP.b.l = sh.SP[0];
      z80.PC.b.h = sh.PC[1];
      z80.PC.b.l = sh.PC[0];
      _IM = sh.IM; // interrupt mode
      z80.AFx.b.h = sh.AFx[1];
      z80.AFx.b.l = sh.AFx[0];
      z80.BCx.b.h = sh.BCx[1];
      z80.BCx.b.l = sh.BCx[0];
      z80.DEx.b.h = sh.DEx[1];
      z80.DEx.b.l = sh.DEx[0];
      z80.HLx.b.h = sh.HLx[1];
      z80.HLx.b.l = sh.HLx[0];
// Gate Array
      port.b.h = 0x7f;
      for (n = 0; n < 17; n++) { // loop for all colours + border
         GateArray.pen = n;
         val = sh.ga_ink_values[n]; // GA palette entry
         z80_OUT_handler(port, val | (1<<6));
      }
      val = sh.ga_pen; // GA pen
      z80_OUT_handler(port, (val & 0x3f));
      val = sh.ga_ROM_config; // GA ROM configuration
      z80_OUT_handler(port, (val & 0x3f) | (2<<6));
      val = sh.ga_RAM_config; // GA RAM configuration
      z80_OUT_handler(port, (val & 0x3f) | (3<<6));
// CRTC
      port.b.h = 0xbd;
      for (n = 0; n < 18; n++) { // loop for all CRTC registers
         val = sh.crtc_registers[n];
         CRTC.reg_select = n;
         z80_OUT_handler(port, val);
      }
      port.b.h = 0xbc;
      val = sh.crtc_reg_select; // CRTC register select
      z80_OUT_handler(port, val);
// ROM select
      port.b.h = 0xdf;
      val = sh.upper_ROM; // upper ROM number
      z80_OUT_handler(port, val);
// PPI
      port.b.h = 0xf4; // port A
      z80_OUT_handler(port, sh.ppi_A);
      port.b.h = 0xf5; // port B
      z80_OUT_handler(port, sh.ppi_B);
      port.b.h = 0xf6; // port C
      z80_OUT_handler(port, sh.ppi_C);
      port.b.h = 0xf7; // control
      z80_OUT_handler(port, sh.ppi_control);
// PSG
      PSG.control = PPI.portC;
      PSG.reg_select = sh.psg_reg_select;
      for (n = 0; n < 16; n++) { // loop for all PSG registers
         SetAYRegister(n, sh.psg_registers[n]);
      }

      if (sh.version > 1) { // does the snapshot have version 2 data?
         dwModel = sh.cpc_model; // determine the model it was saved for
         if (dwModel != CPC.model) { // different from what we're currently running?
            if (dwModel > 2) { // not one of the known models?
               emulator_reset(false);
               return ERR_SNA_CPC_TYPE;
            }
            strncpy(chPath, CPC.rom_path, sizeof(chPath)-2);
            strcat(chPath, "/");
            strncat(chPath, chROMFile[dwModel], sizeof(chPath)-1 - strlen(chPath)); // path to the required ROM image
            FILE *file;
            if ((file = fopen(chPath, "rb")) != NULL) {
               n = fread(pbROMlo, 2*16384, 1, file);
               fclose(file);
               if (!n) {
                  emulator_reset(false);
                  return ERR_CPC_ROM_MISSING;
               }
               CPC.model = dwModel;
            } else { // ROM image load failed
               emulator_reset(false);
               return ERR_CPC_ROM_MISSING;
            }
         }
      }
      if (sh.version > 2) { // does the snapshot have version 3 data?
         FDC.motor = sh.fdc_motor;
         driveA.current_track = sh.drvA_current_track;
         driveB.current_track = sh.drvB_current_track;
         CPC.printer_port = sh.printer_data ^ 0x80; // invert bit 7 again
         PSG.AmplitudeEnv = sh.psg_env_step << 1; // multiply by 2 to bring it into the 0 - 30 range
         PSG.FirstPeriod = false;
         if (sh.psg_env_direction == 0x01) { // up
            switch (PSG.RegisterAY.EnvType)
            {
               case 4:
               case 5:
               case 6:
               case 7:
               case 13:
               case 14:
               case 15:
                  PSG.FirstPeriod = true;
                  break;
            }
         } else if (sh.psg_env_direction == 0xff) { // down
            switch (PSG.RegisterAY.EnvType)
            {
               case 0:
               case 1:
               case 2:
               case 3:
               case 9:
               case 10:
               case 11:
                  PSG.FirstPeriod = true;
                  break;
            }
         }
         CRTC.addr = sh.crtc_addr[0] + (sh.crtc_addr[1] * 256);
         VDU.scanline = sh.crtc_scanline[0] + (sh.crtc_scanline[1] * 256);
         CRTC.char_count = sh.crtc_char_count[0];
         CRTC.line_count = sh.crtc_line_count;
         CRTC.raster_count = sh.crtc_raster_count;
         CRTC.vt_adjust_count = sh.crtc_vt_adjust_count;
         CRTC.hsw_count = sh.crtc_hsw_count;
         CRTC.vsw_count = sh.crtc_vsw_count;
         CRTC.flags = sh.crtc_flags[0] + (sh.crtc_flags[1] * 256);
         GateArray.int_delay = sh.ga_int_delay;
         GateArray.sl_count = sh.ga_sl_count;
         z80.int_pending = sh.z80_int_pending;
      }
   } else {
      return ERR_FILE_NOT_FOUND;
   }

/* char *pchTmpBuffer = new char[MAX_LINE_LEN];
   LoadString(hAppInstance, MSG_SNA_LOAD, chMsgBuffer, sizeof(chMsgBuffer));
   snprintf(pchTmpBuffer, _MAX_PATH-1, chMsgBuffer, CPC.snap_file);
   add_message(pchTmpBuffer);
   delete [] pchTmpBuffer; */
   return 0;
}

int snapshot_save_open(void *pfileObject)
{
   t_SNA_header sh;
   int n;

   memset(&sh, 0, sizeof(sh));
   strcpy(sh.id, "MV - SNA");
   sh.version = 3;
// Z80
   sh.AF[1] = _A;
   sh.AF[0] = _F;
   sh.BC[1] = _B;
   sh.BC[0] = _C;
   sh.DE[1] = _D;
   sh.DE[0] = _E;
   sh.HL[1] = _H;
   sh.HL[0] = _L;
   sh.R = (_R & 0x7f) | (_Rb7 & 0x80);
   sh.I = _I;
   if (_IFF1)
      sh.IFF0 = 1;
   if (_IFF2)
      sh.IFF1 = 1;
   sh.IX[1] = _IXh;
   sh.IX[0] = _IXl;
   sh.IY[1] = _IYh;
   sh.IY[0] = _IYl;
   sh.SP[1] = z80.SP.b.h;
   sh.SP[0] = z80.SP.b.l;
   sh.PC[1] = z80.PC.b.h;
   sh.PC[0] = z80.PC.b.l;
   sh.IM = _IM;
   sh.AFx[1] = z80.AFx.b.h;
   sh.AFx[0] = z80.AFx.b.l;
   sh.BCx[1] = z80.BCx.b.h;
   sh.BCx[0] = z80.BCx.b.l;
   sh.DEx[1] = z80.DEx.b.h;
   sh.DEx[0] = z80.DEx.b.l;
   sh.HLx[1] = z80.HLx.b.h;
   sh.HLx[0] = z80.HLx.b.l;
// Gate Array
   sh.ga_pen = GateArray.pen;
   for (n = 0; n < 17; n++) { // loop for all colours + border
      sh.ga_ink_values[n] = GateArray.ink_values[n];
   }
   sh.ga_ROM_config = GateArray.ROM_config;
   sh.ga_RAM_config = GateArray.RAM_config;
// CRTC
   sh.crtc_reg_select = CRTC.reg_select;
   for (n = 0; n < 18; n++) { // loop for all CRTC registers
      sh.crtc_registers[n] = CRTC.registers[n];
   }
// ROM select
   sh.upper_ROM = GateArray.upper_ROM;
// PPI
   sh.ppi_A = PPI.portA;
   sh.ppi_B = PPI.portB;
   sh.ppi_C = PPI.portC;
   sh.ppi_control = PPI.control;
// PSG
   sh.psg_reg_select = PSG.reg_select;
   for (n = 0; n < 16; n++) { // loop for all PSG registers
      sh.psg_registers[n] = PSG.RegisterAY.Index[n];
   }

   sh.ram_size[0] = CPC.ram_size & 0xff;
   sh.ram_size[1] = (CPC.ram_size >> 8) & 0xff;
// version 2 info
   sh.cpc_model = CPC.model;
// version 3 info
   sh.fdc_motor = FDC.motor;
   sh.drvA_current_track = driveA.current_track;
   sh.drvB_current_track = driveB.current_track;
   sh.printer_data = CPC.printer_port ^ 0x80; // invert bit 7 again
   sh.psg_env_step = PSG.AmplitudeEnv >> 1; // divide by 2 to bring it into the 0 - 15 range
   if (PSG.FirstPeriod) {
      switch (PSG.RegisterAY.EnvType)
      {
         case 0:
         case 1:
         case 2:
         case 3:
         case 8:
         case 9:
         case 10:
         case 11:
            sh.psg_env_direction = 0xff; // down
            break;
         case 4:
         case 5:
         case 6:
         case 7:
         case 12:
         case 13:
         case 14:
         case 15:
            sh.psg_env_direction = 0x01; // up
            break;
      }
   } else {
      switch (PSG.RegisterAY.EnvType)
      {
         case 0:
         case 1:
         case 2:
         case 3:
         case 4:
         case 5:
         case 6:
         case 7:
         case 9:
         case 11:
         case 13:
         case 15:
            sh.psg_env_direction = 0x00; // hold
            break;
         case 8:
         case 14:
            sh.psg_env_direction = 0xff; // down
            break;
         case 10:
         case 12:
            sh.psg_env_direction = 0x01; // up
            break;
      }
   }
   sh.crtc_addr[0] = CRTC.addr & 0xff;
   sh.crtc_addr[1] = (CRTC.addr >> 8) & 0xff;
   sh.crtc_scanline[0] = VDU.scanline & 0xff;
   sh.crtc_scanline[1] = (VDU.scanline >> 8) & 0xff;
   sh.crtc_char_count[0] = CRTC.char_count;
   sh.crtc_line_count = CRTC.line_count;
   sh.crtc_raster_count = CRTC.raster_count;
   sh.crtc_vt_adjust_count = CRTC.vt_adjust_count;
   sh.crtc_hsw_count = CRTC.hsw_count;
   sh.crtc_vsw_count = CRTC.vsw_count;
   sh.crtc_flags[0] = CRTC.flags & 0xff;
   sh.crtc_flags[1] = (CRTC.flags >> 8) & 0xff;
   sh.ga_int_delay = GateArray.int_delay;
   sh.ga_sl_count = GateArray.sl_count;
   sh.z80_int_pending = z80.int_pending;

   if (pfileObject) { // = fopen(pchFileName, "wb")) != NULL) {
     if (gzwrite(pfileObject, &sh, sizeof(sh)) != sizeof(sh)) { // write snapshot header
         return ERR_SNA_WRITE;
      }
      if (gzwrite(pfileObject, pbRAM, CPC.ram_size*1024) != (int)CPC.ram_size*1024) { // write memory contents to snapshot file
         return ERR_SNA_WRITE;
      }
   } else {
      return ERR_SNA_WRITE;
   }

/* char *pchTmpBuffer = new char[MAX_LINE_LEN];
   LoadString(hAppInstance, MSG_SNA_SAVE, chMsgBuffer, sizeof(chMsgBuffer));
   snprintf(pchTmpBuffer, _MAX_PATH-1, chMsgBuffer, CPC.snap_file);
   add_message(pchTmpBuffer);
   delete [] pchTmpBuffer; */
   return 0;
}
void dsk_eject (t_drive *drive)
{
   dword track, side;

   for (track = 0; track < DSK_TRACKMAX; track++) { // loop for all tracks
      for (side = 0; side < DSK_SIDEMAX; side++) { // loop for all sides
         if (drive->track[track][side].data) { // track is formatted?
            free(drive->track[track][side].data); // release memory allocated for this track
         }
      }
   }
   dword dwTemp = drive->current_track; // save the drive head position
   memset(drive, 0, sizeof(t_drive)); // clear drive info structure
   drive->current_track = dwTemp;
}

int dsk_load (char *pchFileName, t_drive *drive, char chID)
{
   int iRetCode;
   dword dwTrackSize, track, side, sector, dwSectorSize, dwSectors;
   byte *pbPtr, *pbDataPtr, *pbTempPtr, *pbTrackSizeTable;

   iRetCode = 0;
   dsk_eject(drive);
   if ((pfileObject = fopen(pchFileName, "rb")) != NULL) {
      fread(pbGPBuffer, 0x100, 1, pfileObject); // read DSK header
      pbPtr = pbGPBuffer;

      if (memcmp(pbPtr, "MV - CPC", 8) == 0) { // normal DSK image?
         drive->tracks = *(pbPtr + 0x30); // grab number of tracks
         if (drive->tracks > DSK_TRACKMAX) { // compare against upper limit
            drive->tracks = DSK_TRACKMAX; // limit to maximum
         }
         drive->sides = *(pbPtr + 0x31); // grab number of sides
         if (drive->sides > DSK_SIDEMAX) { // abort if more than maximum
            iRetCode = ERR_DSK_SIDES;
            goto exit;
         }
         dwTrackSize = (*(pbPtr + 0x32) + (*(pbPtr + 0x33) << 8)) - 0x100; // determine track size in bytes, minus track header
         drive->sides--; // zero base number of sides
         for (track = 0; track < drive->tracks; track++) { // loop for all tracks
            for (side = 0; side <= drive->sides; side++) { // loop for all sides
               fread(pbGPBuffer+0x100, 0x100, 1, pfileObject); // read track header
               pbPtr = pbGPBuffer + 0x100;
               if (memcmp(pbPtr, "Track-Info", 10) != 0) { // abort if ID does not match
                  iRetCode = ERR_DSK_INVALID;
                  goto exit;
               }
               dwSectorSize = 0x80 << *(pbPtr + 0x14); // determine sector size in bytes
               dwSectors = *(pbPtr + 0x15); // grab number of sectors
               if (dwSectors > DSK_SECTORMAX) { // abort if sector count greater than maximum
                  iRetCode = ERR_DSK_SECTORS;
                  goto exit;
               }
               drive->track[track][side].sectors = dwSectors; // store sector count
               drive->track[track][side].size = dwTrackSize; // store track size
               drive->track[track][side].data = (byte *)malloc(dwTrackSize); // attempt to allocate the required memory
               if (drive->track[track][side].data == NULL) { // abort if not enough
                  iRetCode = ERR_OUT_OF_MEMORY;
                  goto exit;
               }
               pbDataPtr = drive->track[track][side].data; // pointer to start of memory buffer
               pbTempPtr = pbDataPtr; // keep a pointer to the beginning of the buffer for the current track
               for (sector = 0; sector < dwSectors; sector++) { // loop for all sectors
                  memcpy(drive->track[track][side].sector[sector].CHRN, (pbPtr + 0x18), 4); // copy CHRN
                  memcpy(drive->track[track][side].sector[sector].flags, (pbPtr + 0x1c), 2); // copy ST1 & ST2
                  drive->track[track][side].sector[sector].size = dwSectorSize;
                  drive->track[track][side].sector[sector].data = pbDataPtr; // store pointer to sector data
                  pbDataPtr += dwSectorSize;
                  pbPtr += 8;
               }
               if (!fread(pbTempPtr, dwTrackSize, 1, pfileObject)) { // read entire track data in one go
                  iRetCode = ERR_DSK_INVALID;
                  goto exit;
               }
            }
         }
         drive->altered = 0; // disk is as yet unmodified
      } else {
         if (memcmp(pbPtr, "EXTENDED", 8) == 0) { // extended DSK image?
            drive->tracks = *(pbPtr + 0x30); // number of tracks
            if (drive->tracks > DSK_TRACKMAX) {  // limit to maximum possible
               drive->tracks = DSK_TRACKMAX;
            }
            drive->random_DEs = *(pbPtr + 0x31) & 0x80; // simulate random Data Errors?
            drive->sides = *(pbPtr + 0x31) & 3; // number of sides
            if (drive->sides > DSK_SIDEMAX) { // abort if more than maximum
               iRetCode = ERR_DSK_SIDES;
               goto exit;
            }
            pbTrackSizeTable = pbPtr + 0x34; // pointer to track size table in DSK header
            drive->sides--; // zero base number of sides
            for (track = 0; track < drive->tracks; track++) { // loop for all tracks
               for (side = 0; side <= drive->sides; side++) { // loop for all sides
                  dwTrackSize = (*pbTrackSizeTable++ << 8); // track size in bytes
                  if (dwTrackSize != 0) { // only process if track contains data
                     dwTrackSize -= 0x100; // compensate for track header
                     fread(pbGPBuffer+0x100, 0x100, 1, pfileObject); // read track header
                     pbPtr = pbGPBuffer + 0x100;
                     if (memcmp(pbPtr, "Track-Info", 10) != 0) { // valid track header?
                        iRetCode = ERR_DSK_INVALID;
                        goto exit;
                     }
                     dwSectors = *(pbPtr + 0x15); // number of sectors for this track
                     if (dwSectors > DSK_SECTORMAX) { // abort if sector count greater than maximum
                        iRetCode = ERR_DSK_SECTORS;
                        goto exit;
                     }
                     drive->track[track][side].sectors = dwSectors; // store sector count
                     drive->track[track][side].size = dwTrackSize; // store track size
                     drive->track[track][side].data = (byte *)malloc(dwTrackSize); // attempt to allocate the required memory
                     if (drive->track[track][side].data == NULL) { // abort if not enough
                        iRetCode = ERR_OUT_OF_MEMORY;
                        goto exit;
                     }
                     pbDataPtr = drive->track[track][side].data; // pointer to start of memory buffer
                     pbTempPtr = pbDataPtr; // keep a pointer to the beginning of the buffer for the current track
                     for (sector = 0; sector < dwSectors; sector++) { // loop for all sectors
                        memcpy(drive->track[track][side].sector[sector].CHRN, (pbPtr + 0x18), 4); // copy CHRN
                        memcpy(drive->track[track][side].sector[sector].flags, (pbPtr + 0x1c), 2); // copy ST1 & ST2
                        dwSectorSize = *(pbPtr + 0x1e) + (*(pbPtr + 0x1f) << 8); // sector size in bytes
                        drive->track[track][side].sector[sector].size = dwSectorSize;
                        drive->track[track][side].sector[sector].data = pbDataPtr; // store pointer to sector data
                        pbDataPtr += dwSectorSize;
                        pbPtr += 8;
                     }
                     if (!fread(pbTempPtr, dwTrackSize, 1, pfileObject)) { // read entire track data in one go
                        iRetCode = ERR_DSK_INVALID;
                        goto exit;
                     }
                  } else {
                     memset(&drive->track[track][side], 0, sizeof(t_track)); // track not formatted
                  }
               }
            }
            drive->altered = 0; // disk is as yet unmodified
         } else {
            iRetCode = ERR_DSK_INVALID; // file could not be identified as a valid DSK
         }
      }
/*    {
         char *pchTmpBuffer = new char[MAX_LINE_LEN];
         LoadString(hAppInstance, MSG_DSK_LOAD, chMsgBuffer, sizeof(chMsgBuffer));
         snprintf(pchTmpBuffer, _MAX_PATH-1, chMsgBuffer, chID, chID == 'A' ? CPC.drvA_file : CPC.drvB_file);
         add_message(pchTmpBuffer);
         delete [] pchTmpBuffer;
      } */
exit:
      fclose(pfileObject);
   } else {
      iRetCode = ERR_FILE_NOT_FOUND;
   }

   if (iRetCode != 0) { // on error, 'eject' disk from drive
      dsk_eject(drive);
   }
   return iRetCode;
}

int dsk_save (char *pchFileName, t_drive *drive, char chID)
{
   t_DSK_header dh;
   t_track_header th;
   dword track, side, pos, sector;

   if ((pfileObject = fopen(pchFileName, "wb")) != NULL) {
      memset(&dh, 0, sizeof(dh));
      strcpy(dh.id, "EXTENDED CPC DSK File\r\nDisk-Info\r\n");
      strcpy(dh.unused1, "Caprice32\r\n");
      dh.tracks = drive->tracks;
      dh.sides = (drive->sides+1) | (drive->random_DEs); // correct side count and indicate random DEs, if necessary
      pos = 0;
      for (track = 0; track < drive->tracks; track++) { // loop for all tracks
         for (side = 0; side <= drive->sides; side++) { // loop for all sides
            if (drive->track[track][side].size) { // track is formatted?
               dh.track_size[pos] = (drive->track[track][side].size + 0x100) >> 8; // track size + header in bytes
            }
            pos++;
         }
      }
      if (!fwrite(&dh, sizeof(dh), 1, pfileObject)) { // write header to file
         fclose(pfileObject);
         return ERR_DSK_WRITE;
      }

      memset(&th, 0, sizeof(th));
      strcpy(th.id, "Track-Info\r\n");
      for (track = 0; track < drive->tracks; track++) { // loop for all tracks
         for (side = 0; side <= drive->sides; side++) { // loop for all sides
            if (drive->track[track][side].size) { // track is formatted?
               th.track = track;
               th.side = side;
               th.bps = 2;
               th.sectors = drive->track[track][side].sectors;
               th.gap3 = 0x4e;
               th.filler = 0xe5;
               for (sector = 0; sector < th.sectors; sector++) {
                  memcpy(&th.sector[sector][0], drive->track[track][side].sector[sector].CHRN, 4); // copy CHRN
                  memcpy(&th.sector[sector][4], drive->track[track][side].sector[sector].flags, 2); // copy ST1 & ST2
                  th.sector[sector][6] = drive->track[track][side].sector[sector].size & 0xff;
                  th.sector[sector][7] = (drive->track[track][side].sector[sector].size >> 8) & 0xff; // sector size in bytes
               }
               if (!fwrite(&th, sizeof(th), 1, pfileObject)) { // write track header
                  fclose(pfileObject);
                  return ERR_DSK_WRITE;
               }
               if (!fwrite(drive->track[track][side].data, drive->track[track][side].size, 1, pfileObject)) { // write track data
                  fclose(pfileObject);
                  return ERR_DSK_WRITE;
               }
            }
         }
      }
      fclose(pfileObject);
   } else {
      return ERR_DSK_WRITE; // write attempt failed
   }

/* char *pchTmpBuffer = new char[MAX_LINE_LEN];
   LoadString(hAppInstance, MSG_DSK_SAVE, chMsgBuffer, sizeof(chMsgBuffer));
   snprintf(pchTmpBuffer, _MAX_PATH-1, chMsgBuffer, chID, chID == 'A' ? CPC.drvA_file : CPC.drvB_file);
   add_message(pchTmpBuffer);
   delete [] pchTmpBuffer; */
   return 0;
}

int dsk_format (t_drive *drive, int iFormat)
{
   int iRetCode = 0;
   drive->tracks = disk_format[iFormat].tracks;
   if (drive->tracks > DSK_TRACKMAX) { // compare against upper limit
      drive->tracks = DSK_TRACKMAX; // limit to maximum
   }
   drive->sides = disk_format[iFormat].sides;
   if (drive->sides > DSK_SIDEMAX) { // abort if more than maximum
      iRetCode = ERR_DSK_SIDES;
      goto exit;
   }
   drive->sides--; // zero base number of sides
   for (dword track = 0; track < drive->tracks; track++) { // loop for all tracks
      for (dword side = 0; side <= drive->sides; side++) { // loop for all sides
         dword dwSectorSize = 0x80 << disk_format[iFormat].sector_size; // determine sector size in bytes
         dword dwSectors = disk_format[iFormat].sectors;
         if (dwSectors > DSK_SECTORMAX) { // abort if sector count greater than maximum
            iRetCode = ERR_DSK_SECTORS;
            goto exit;
         }
         dword dwTrackSize = dwSectorSize * dwSectors; // determine track size in bytes, minus track header
         drive->track[track][side].sectors = dwSectors; // store sector count
         drive->track[track][side].size = dwTrackSize; // store track size
         drive->track[track][side].data = (byte *)malloc(dwTrackSize); // attempt to allocate the required memory
         if (drive->track[track][side].data == NULL) { // abort if not enough
            iRetCode = ERR_OUT_OF_MEMORY;
            goto exit;
         }
         byte *pbDataPtr = drive->track[track][side].data; // pointer to start of memory buffer
         byte *pbTempPtr = pbDataPtr; // keep a pointer to the beginning of the buffer for the current track
         byte CHRN[4];
         CHRN[0] = (byte)track;
         CHRN[1] = (byte)side;
         CHRN[3] = (byte)disk_format[iFormat].sector_size;
         for (dword sector = 0; sector < dwSectors; sector++) { // loop for all sectors
            CHRN[2] = disk_format[iFormat].sector_ids[side][sector];
            memcpy(drive->track[track][side].sector[sector].CHRN, CHRN, 4); // copy CHRN
            drive->track[track][side].sector[sector].size = dwSectorSize;
            drive->track[track][side].sector[sector].data = pbDataPtr; // store pointer to sector data
            pbDataPtr += dwSectorSize;
         }
         memset(pbTempPtr, disk_format[iFormat].filler_byte, dwTrackSize);
      }
   }
   drive->altered = 1; // flag disk as having been modified

exit:
   if (iRetCode != 0) { // on error, 'eject' disk from drive
      dsk_eject(drive);
   }
   return iRetCode;
}

void vdu_init (bool bolInit)
{
   if (bolInit) {
      VDU.hstart = 7;
      VDU.hwidth = CPC_VISIBLE_SCR_WIDTH / 8;
      VDU.vstart = 27;
      VDU.vheight = CPC_VISIBLE_SCR_HEIGHT;
   }
}

int emulator_patch_ROM (void)
{
   char chPath[_MAX_PATH + 1];
   byte *pbPtr;

   strncpy(chPath, CPC.rom_path, sizeof(chPath)-2);
   strcat(chPath, "/");
   strncat(chPath, chROMFile[CPC.model], sizeof(chPath)-1 - strlen(chPath)); // determine the ROM image name for the selected model
   if ((pfileObject = fopen(chPath, "rb")) != NULL) { // load CPC OS + Basic
      fread(pbROMlo, 2*16384, 1, pfileObject);
      fclose(pfileObject);
   } else {
      return ERR_CPC_ROM_MISSING;
   }

   if (CPC.kbd_layout) {
      pbPtr = pbROMlo;
      switch(CPC.model) {
         case 0: // 464
            pbPtr += 0x1d69; // location of the keyboard translation table
            break;
         case 1: // 664
         case 2: // 6128
            pbPtr += 0x1eef; // location of the keyboard translation table
            break;
      }
      if (pbPtr != pbROMlo) {
         memcpy(pbPtr, kbd_layout[CPC.kbd_layout-1], 240); // patch the CPC OS ROM with the chosen keyboard layout
         pbPtr = pbROMlo + 0x3800;
         memcpy(pbPtr, cpc_charset[CPC.kbd_layout-1], 2048); // add the corresponding character set
      }
   }

   return 0;
}

void emulator_reset (bool bolMF2Reset)
{
   int n, val1, val2;

// Z80
   memset(&z80, 0, sizeof(z80)); // clear all Z80 registers and support variables
   _IX =
   _IY = 0xffff; // IX and IY are FFFF after a reset!
   _F = Zflag; // set zero flag
   z80.break_point = 0xffffffff; // clear break point

// CPC
   CPC.cycle_count = CYCLE_COUNT_INIT;
   memset(keyboard_matrix, 0xff, sizeof(keyboard_matrix)); // clear CPC keyboard matrix
   CPC.tape_motor = 0;
   CPC.tape_play_button = 0;
   CPC.printer_port = 0xff;

// CRTC
   memset(&CRTC, 0, sizeof(CRTC)); // clear CRTC data structure
   for (n = 0; n < 14; n++) { // program CRTC with 'valid' data
      CRTC.registers[n] = CRTC_values[(CPC.jumpers & 0x10)>>4][n];
   }
   CRTC.flags = HDT_flag | VDT_flag;
   CRTC.hsw =
   CRTC.hsw_active = CRTC.registers[3] & 0x0f;
   CRTC.vsw = CRTC.registers[3] >> 4;
   CRTC.vt_adjust = CRTC.registers[5] & 0x1f;
   CRTC.skew = (CRTC.registers[8] >> 4) & 3;
   if (CRTC.skew == 3) { // no output?
      CRTC.skew = 0xff;
   }
   CRTC.max_raster = CRTC.registers[9] << 3;
   val1 = CRTC.registers[12] & 0x3f;
   val2 = val1 & 0x0f; // isolate screen size
   val1 = (val1 << 1) & 0x60; // isolate CPC RAM bank
   val2 |= val1; // combine
   CRTC.addr =
   CRTC.requested_addr = (CRTC.registers[13] + (val2 << 8)) << 1;
   CRTC.last_hdisp = 0x28;

// VDU
   memset(&VDU, 0, sizeof(VDU)); // clear VDU data structure
   VDU.hsw =
   VDU.hsw_active = 4;
   VDU.scanline_min = 272; //240;
   vdu_init(true);

// Gate Array
   memset(&GateArray, 0, sizeof(GateArray)); // clear GA data structure
   GateArray.scr_mode =
   GateArray.requested_scr_mode = (void(*)(dword))mode_handler[1]; // set to normal mode 1 handler
   GateArray.scr_border = (void(*)(void))border_handler; // set to normal border handler
   ga_init_banking();

// PPI
   memset(&PPI, 0, sizeof(PPI)); // clear PPI data structure

// PSG
   PSG.control = 0;
   ResetAYChipEmulation();

// FDC
   memset(&FDC, 0, sizeof(FDC)); // clear FDC data structure
   FDC.phase = CMD_PHASE;
   FDC.flags = STATUSDRVA_flag | STATUSDRVB_flag;

// memory
   if (bolMF2Reset) {
      memset(pbRAM, 0, 64*1024); // clear only the first 64K of CPC memory
   } else {
      memset(pbRAM, 0, CPC.ram_size*1024); // clear all memory used for CPC RAM
      if (pbMF2ROM) {
         memset(pbMF2ROM+8192, 0, 8192); // clear the MF2's RAM area
      }
   }
   for (n = 0; n < 4; n++) { // initialize active read/write bank configuration
      membank_read[n] = membank_config[0][n];
      membank_write[n] = membank_config[0][n];
   }
   membank_read[0] = pbROMlo; // 'page in' lower ROM
   membank_read[3] = pbROMhi; // 'page in' upper ROM

// Multiface 2
   dwMF2Flags = 0;
   dwMF2ExitAddr = 0xffffffff; // clear MF2 return address
   if ((pbMF2ROM) && (pbMF2ROMbackup)) {
      memcpy(pbMF2ROM, pbMF2ROMbackup, 8192); // copy the MF2 ROM to its proper place
   }
}

int emulator_init (void)
{
   int iErr, iRomNum;
   char chPath[_MAX_PATH + 1];
   char *pchRomData;

   pbGPBuffer = new byte [128*1024]; // attempt to allocate the general purpose buffer
   pbRAM = new byte [CPC.ram_size*1024]; // allocate memory for desired amount of RAM
   pbROMlo = new byte [32*1024]; // allocate memory for 32K of ROM
   if ((!pbGPBuffer) || (!pbRAM) || (!pbROMlo)) {
      return ERR_OUT_OF_MEMORY;
   }
   pbROMhi =
   pbExpansionROM = pbROMlo + 16384;
   memset(memmap_ROM, 0, sizeof(memmap_ROM[0]) * 256); // clear the expansion ROM map
   ga_init_banking(); // init the CPC memory banking map
   if ((iErr = emulator_patch_ROM())) {
      return iErr;
   }

   for (iRomNum = 2; iRomNum < 16; iRomNum++) { // loop for ROMs 2-15
      if (CPC.rom_file[iRomNum][0]) { // is a ROM image specified for this slot?
         pchRomData = new char [16384]; // allocate 16K
         memset(pchRomData, 0, 16384); // clear memory
         strncpy(chPath, CPC.rom_path, sizeof(chPath)-2);
         strcat(chPath, "/");
         strncat(chPath, CPC.rom_file[iRomNum], sizeof(chPath)-1 - strlen(chPath));
         if ((pfileObject = fopen(chPath, "rb")) != NULL) { // attempt to open the ROM image
            fread(pchRomData, 128, 1, pfileObject); // read 128 bytes of ROM data
            if ((pchRomData[0] == char(0x01)) || (pchRomData[0] == char(0x02))) { // is it a valid CPC ROM image?
               fread(pchRomData+128, 16384-128, 1, pfileObject); // read the rest of the ROM file
               memmap_ROM[iRomNum] = (byte *)pchRomData; // update the ROM map
            } else { // try one more time, as the ROM might have a header
               fread(pchRomData, 128, 1, pfileObject); // read 128 bytes of ROM data
               if ((pchRomData[0] == char(0x01)) || (pchRomData[0] == char(0x02))) { // is it a valid CPC ROM image?
                  fread(pchRomData+128, 16384-128, 1, pfileObject); // read the rest of the ROM file
                  memmap_ROM[iRomNum] = (byte *)pchRomData; // update the ROM map
               } else { // not a valid ROM file
                  fprintf(stderr, "ERROR: %s is not a CPC ROM file - clearing ROM slot %d.\n", CPC.rom_file[iRomNum], iRomNum);
                  delete [] pchRomData; // free memory on error
                  CPC.rom_file[iRomNum][0] = 0;
               }
            }
            fclose(pfileObject);
         } else { // file not found
            fprintf(stderr, "ERROR: The %s file is missing - clearing ROM slot %d.\n", CPC.rom_file[iRomNum], iRomNum);
            delete [] pchRomData; // free memory on error
            CPC.rom_file[iRomNum][0] = 0;
         }
      }
   }
   if (CPC.mf2) { // Multiface 2 enabled?
      if (!pbMF2ROM) {
         pbMF2ROM = new byte [16384]; // allocate the space needed for the Multiface 2: 8K ROM + 8K RAM
         pbMF2ROMbackup = new byte [8192]; // allocate the space needed for the backup of the MF2 ROM
         if ((!pbMF2ROM) || (!pbMF2ROMbackup)) {
            return ERR_OUT_OF_MEMORY;
         }
         memset(pbMF2ROM, 0, 16384); // clear memory
         strncpy(chPath, CPC.rom_path, sizeof(chPath)-2);
         strcat(chPath, "/");
         strncat(chPath, CPC.rom_mf2, sizeof(chPath)-1 - strlen(chPath)); // combine path and file name
         if ((pfileObject = fopen(chPath, "rb")) != NULL) { // attempt to open the ROM image
            fread(pbMF2ROMbackup, 8192, 1, pfileObject);
            if (memcmp(pbMF2ROMbackup+0x0d32, "MULTIFACE 2", 11) != 0) { // does it have the required signature?
               fprintf(stderr, "ERROR: The file selected as the MF2 ROM is either corrupt or invalid.\n");
               delete [] pbMF2ROMbackup;
               delete [] pbMF2ROM;
               pbMF2ROM = NULL;
               CPC.rom_mf2[0] = 0;
               CPC.mf2 = 0; // disable MF2 support
            }
            fclose(pfileObject);
         } else { // error opening file
            fprintf(stderr, "ERROR: The file selected as the MF2 ROM is either corrupt or invalid.\n");
            delete [] pbMF2ROMbackup;
            delete [] pbMF2ROM;
            pbMF2ROM = NULL;
            CPC.rom_mf2[0] = 0;
            CPC.mf2 = 0; // disable MF2 support
         }
      }
   }

   emulator_reset(false);
   CPC.paused &= ~1;

   return 0;
}

void emulator_shutdown (void)
{
   int iRomNum;

   delete [] pbMF2ROMbackup;
   delete [] pbMF2ROM;
   pbMF2ROM = NULL;
   for (iRomNum = 2; iRomNum < 16; iRomNum++) // loop for ROMs 2-15
   {
      if (memmap_ROM[iRomNum] != NULL) // was a ROM assigned to this slot?
         delete [] memmap_ROM[iRomNum]; // if so, release the associated memory
   }

   delete [] pbROMlo;
   delete [] pbRAM;
   delete [] pbGPBuffer;
}

int printer_start (void)
{
   if (!pfoPrinter) {
      if(!(pfoPrinter = fopen(CPC.printer_file, "wb"))) {
         return 0; // failed to open/create file
      }
   }
   return 1; // ready to capture printer output
}

void printer_stop (void)
{
   if (pfoPrinter) {
      fclose(pfoPrinter);
   }
   pfoPrinter = NULL;
}

int audio_init (void)
{
  if (!CPC.snd_enabled)
    return 0;

  /* size is samples * channels * bytes per sample (1 or 2) */
  CPC.snd_buffersize = 1024 * (CPC.snd_stereo ? 2 : 1) * (CPC.snd_bits ? 2 : 1);
  pbSndBuffer = (byte *)malloc(CPC.snd_buffersize); // allocate the sound data buffer
  pbSndBufferEnd = pbSndBuffer + CPC.snd_buffersize;
  memset(pbSndBuffer, 0, CPC.snd_buffersize);
  CPC.snd_bufferptr = pbSndBuffer; // init write cursor

  InitAY();

  for (int n = 0; n < 16; n++)
    SetAYRegister(n, PSG.RegisterAY.Index[n]); // init sound emulation with valid values

  return 0;
}

void audio_shutdown (void)
{
  if (pbSndBuffer) free(pbSndBuffer);
}

void video_init_tables (void)
{
   int idx, n, p1, p2, p3, p4;

   idx = 0;
   for (n = 0; n < 256; n++) { // calculate mode0 byte-to-pixel translation table
      p1 = ((n & 0x80) >> 7) + ((n & 0x08) >> 2) + ((n & 0x20) >> 3) + ((n & 0x02) << 2);
      p2 = ((n & 0x40) >> 6) + ((n & 0x04) >> 1) + ((n & 0x10) >> 2) + ((n & 0x01) << 3);
      mode0_table[idx++] = p1;
      mode0_table[idx++] = p2;
   }

   idx = 0;
   for (n = 0; n < 256; n++) { // calculate mode1 byte-to-pixel translation table
      p1 = ((n & 0x80) >> 7) + ((n & 0x08) >> 2);
      p2 = ((n & 0x40) >> 6) + ((n & 0x04) >> 1);
      p3 = ((n & 0x20) >> 5) +  (n & 0x02);
      p4 = ((n & 0x10) >> 4) + ((n & 0x01) << 1);
      mode1_table[idx++] = p1;
      mode1_table[idx++] = p2;
      mode1_table[idx++] = p3;
      mode1_table[idx++] = p4;
   }
}

int video_set_palette (void)
{
  int n;
  
  /* Initialize colour table */
  if (!CPC.scr_tube)
  {
    for (n = 0; n < 32; n++) 
    {
      dword r = (dword)(colours_rgb[n][0] * (CPC.scr_intensity / 10.0) * 255);
      dword g = (dword)(colours_rgb[n][1] * (CPC.scr_intensity / 10.0) * 255);
      dword b = (dword)(colours_rgb[n][2] * (CPC.scr_intensity / 10.0) * 255);
      dword colour = RGB((r>255)?255:r, (g>255)?255:g, (b>255)?255:b);
      colour_table[n] = colour | colour << 16;
    }
  }
  else 
  {
    for (n = 0; n < 32; n++) 
    {
      dword g = (dword)(colours_green[n] * (CPC.scr_intensity / 10.0) * 255);
      dword colour = RGB(0, (g>255)?255:g, 0);
      colour_table[n] = colour | colour << 16;
    }
  }
  
  switch(CPC.scr_bpp)
  {
  case 16:
    break;
  case 8:
    for (n = 0; n < 32; n++)
    {
      Screen->Palette[n] = colour_table[n];
      colour_table[n] =  n | (n << 8) | (n << 16) | (n << 24);
    }
    Screen->PalSize = 32;
    break;
  }

  for (n = 0; n < 17; n++) // loop for all colours + border
    GateArray.palette[n] = colour_table[GateArray.ink_values[n]];

  return 1;
}

int video_init (void)
{
  CPC.scr_bpp = Screen->Depth;
  dwXScale = dwYScale = 1;

  video_set_palette(); // init CPC colours

  // rendered screen line length (changing bytes to dwords)
  CPC.scr_bps = (Screen->Width * Screen->BytesPerPixel) / 4;
  CPC.scr_line_offs = CPC.scr_bps * dwYScale;
  CPC.scr_base = (dword *)Screen->Pixels;

  /* video_set_style */
  switch(CPC.scr_bpp)
  {
  case 16:
    mode_handler[0] = draw16bpp_mode0_half;
    mode_handler[1] = draw16bpp_mode1_half;
    mode_handler[2] = draw16bpp_mode2_half;
    mode_handler[3] = draw16bpp_mode0_half;
    border_handler = draw16bpp_border_half;
    break;
  case 8:
    mode_handler[0] = draw8bpp_mode0_half;
    mode_handler[1] = draw8bpp_mode1_half;
    mode_handler[2] = draw8bpp_mode2_half;
    mode_handler[3] = draw8bpp_mode0_half;
    border_handler = draw8bpp_border_half;
    break;
  }

  GateArray.scr_mode =
      GateArray.requested_scr_mode = (void(*)(dword))mode_handler[GateArray.ROM_config & 3];
  GateArray.scr_border = (void(*)(void))border_handler;
  /* end */

  vdu_init(true); // initialize the monitor emulation

  return 0;
}
