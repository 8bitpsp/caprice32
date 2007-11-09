/* Caprice32 - Amstrad CPC Emulator
   (c) Copyright 1997-2005 Ulrich Doewich

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


   Oct 11, 2000 - 22:12 preliminary IN/OUT handlers; started with the GA OUT register write routines
   Oct 12, 2000 - 23:31 added GA ROM select to z80_OUT_handler
   Oct 14, 2000 - 13:58 added PPI IN/OUT handlers
   Oct 15, 2000 - 15:33 added CRTC IN/OUT handlers
   Oct 20, 2000 - 23:23 fixed some IN/OUT handler bugs
   Oct 27, 2000 - 17:39 added reset_CPC
   Oct 30, 2000 - 21:05 found the problem with the streched display: scr_bps needs to be in dwords, not bytes!
   Nov 01, 2000 - 23:43 aargh! found the BASIC 'reset' bug: the pbROMhi variable was pointing to the wrong location!
   Nov 03, 2000 - 18:03 added keyboard_interrupt handler
   Nov 03, 2000 - 19:10 added preliminary PSG register write handlers
   Nov 03, 2000 - 23:37 fixed the PPI port C handler; CPC keyboard works now!
   Dec 12, 2000 - 18:14 changed load_dsk() to work with a C based disk info structure
   Jan 24, 2001 - 22:28 loading from disk images works (again)!
   Apr 06, 2001 - 12:52 fixed the keyboard translation table (Allegro WIP 3.9.34 added 3 more keys to the array)

   Jun 13, 2001 - 17:25 added DirectDraw and DirectInput8 init routines for the new Windows port of Caprice32
   Jun 13, 2001 - 20:50 keyboard emualtion works now via the DI8 action mapping
   Jun 21, 2001 - 00:30 changed DDraw code to work in windowed mode; added routines to render CPC screen in 32bpp; added drag'n drop functionality for CPC drive A
   Jun 23, 2001 - 16:15 only the visible portion of the CPC screen is rendered to the frame buffer; Windows window is now sized to the proper CPC screen dimensions
   Jun 28, 2001 - 18:42 fixed the extended DSK parsing; loading DSK images via common dialog file selector; handling of joystick axis data
   Jun 29, 2001 - 20:16 speed throttling to original CPC speed; fixed the joystick mapping in the CPC keyboard matrix; implemented more menu commands
   Jun 30, 2001 - 22:57 settings stored in and retrieved from the registry; updated emulator_init and emulator_shutdown; file selector remembers path and selected file
   Jul 01, 2001 - 17:54 ROM slot cofiguration is stored in and retrieved from the registry; automatically loads last DSK images on startup
   Jul 04, 2001 - 00:34 colour tables for 16bpp (555 & 565) and 8bpp PC video modes; DDraw init updated with bit depth dependant code
   Jul 04, 2001 - 22:06 fixed 16bpp colour tables; updated VDU screen centering code to work with all bit depths
   Jul 06, 2001 - 00:35 implemented pause option; emulation is halted when application becomes inactive
   Jul 06, 2001 - 19:34 zip decompression for DSK and SNA loading
   Jul 07, 2001 - 00:58 fixed setting of GA colour table in load_snapshot; last DSK image load on startup supports ZIP files
   Jul 18, 2001 - 18:40 DirectSound init code finally works correctly!
   Jul 20, 2001 - 18:17 found the problem with the sound emulation: snd_multiplier calculation was done with integer arithmetic! grrrr...
   Jul 25, 2001 - 00:48 *finally* got a clean sound code that won't break up anymore!
   Aug 01, 2001 - 17:38 sound configuration is pulled from and stored in the registry
   Aug 02, 2001 - 18:31 floppy drive LED is now shown on the CPC screen during data transfers
   Aug 03, 2001 - 19:26 added About box
   Aug 03, 2001 - 23:49 altered load_dsk() to allocate memory on a track by track basis (in preparation of fdc_writeID() support)
   Aug 06, 2001 - 18:42 started work on the Options dialog
   Aug 10, 2001 - 19:10 finished the General Options property sheet
   Aug 11, 2001 - 17:29 up to 576KB of RAM supported
   Aug 12, 2001 - 21:43 extracted psg_reg_write() from the psg_write #define; fixed the PSG setup in load_snapshot()
   Aug 16, 2001 - 00:49 load_snapshot() now adjusts RAM size, and loads the correct CPC ROM as necessary
   Aug 20, 2001 - 00:59 added save_snapshot(); header contains preliminary v3 info
   Aug 20, 2001 - 16:10 updated load_snapshot() with processing of the 'official' v3 info
   Aug 22, 2001 - 23:26 added Audio Options property sheet to control PSG emulation
   Oct 06, 2001 - 13:22 removed the "experimental" joystick 2 support from the action map code - did not work as expected
   Oct 07, 2001 - 20:27 added the save_dsk() routine
   Oct 13, 2001 - 18:51 completed rewrite of zip handling; users can now choose images from multiple file archives
   Nov 08, 2001 - 19:08 bug fix: the zip_dir routine caused a crash if a zip archive would not open properly
   Nov 09, 2001 - 00:04 the ROM loading routine now checks the validity of the image, and skips a possible 128 bytes header

   May 09, 2002 - 19:07 removed the DDSCAPS_3DDEVICE flag from the InitDirectDraw code; modified UpdateBounds to prevent strechblits

   Jun 12, 2002 - 18:24 started converting DirectX specific stuff to Allegro
   Jun 14, 2002 - 17:49 emulator runs via GDI blits; re-implemented Allegro keyboard handler
   Jun 22, 2002 - 17:34 finally got the windowed DirectX mode working using the new Allegro CVS version
   Jun 25, 2002 - 18:41 added timer based speed throttling
   Jun 27, 2002 - 18:08 CPC screen blits correctly now: source coordinates were being mangled
   Jul 14, 2002 - 17:42 rewrote the PC sound playback routine to dynamically adjust the rate at which the buffer is filled
   Jul 24, 2002 - 22:16 fixed a possible crash on startup if a zipped DSK in drive A/B had been (re)moved between sessions
   Jul 27, 2002 - 16:45 added processing of a "language" file
   Jul 27, 2002 - 19:14 changed LoadConfiguration and SaveConfiguration to use the Allegro configuration rountines
   Jul 29, 2002 - 22:56 traced the 'access violation' problem back to the sound pause/resume functions
   Aug 02, 2002 - 22:37 added some error condition checks and modified sub-routines to report them properly

   Aug 24, 2002 - 23:00 determined Allegro to be unsuitable - reverting everything back to DirectX; implemented DirectInput keyboard translation
   Aug 26, 2002 - 22:51 fixed the initial application window size to use the correct dimensions
   Aug 27, 2002 - 18:23 streamlined the code to be more Windows friendly; app now goes to sleep when minimized
   Sep 07, 2002 - 18:28 added fullscreen mode; corrected 8bpp palette init; GDI calls (e.g. file selector) still work
   Sep 08, 2002 - 16:06 rewrote PCVideo init routines to support windowed/fullscreen mode switching
   Sep 12, 2002 - 00:02 fixed 50Hz timer by using QueryPerformanceCounter
   Sep 20, 2002 - 18:19 re-implemented DirectSound support; fixed timing problems - sound playback should no longer pop
   Sep 21, 2002 - 15:35 LoadConfiguration & SaveConfiguration now use the cap32.cfg file instead of the Registry
   Sep 25, 2002 - 22:49 full screen resolution is selectable in the Options dialog; auto-sizes visible portion; allows mode change while being fs
   Sep 26, 2002 - 22:26 added scanline and line doubling (in software) rendering modes
   Sep 28, 2002 - 17:38 added a speed slider to the General page of the Options dialog
   Sep 29, 2002 - 19:31 added colour monitor and green screen option; switched to QueryPerformanceCounter for the FPS counter
   Sep 29, 2002 - 23:03 added the 15 and 16 bpp green monitor colour maps - thanks to Ralf's Excel sheet!
   Oct 02, 2002 - 23:43 added fault condition checks to zip_dir, zip_extract and load_snapshot
   Oct 04, 2002 - 18:29 added fault condition checks to save_snapshot, load_dsk and save_dsk; added altered DSK check on exit
   Oct 04, 2002 - 22:34 added disk drive activity indicator
   Oct 07, 2002 - 17:48 fixed switching CPC monitor type "on-the-fly"; fixed surface restore in fullscreen mode
   Oct 07, 2002 - 21:58 added line doubling (in hardware) rendering mode
   Oct 16, 2002 - 22:18 added enumeration and initilization of all installed keyboards and joysticks
   Oct 17, 2002 - 23:40 replaced the controls property page in the options dialog
   Oct 28, 2002 - 19:50 added support for custom controls: CPC joystick functions can now be mapped to any attached input device(s)
   Nov 01, 2002 - 15:54 added custom print routine for on-screen messages (e.g. FPS counter)
   Nov 02, 2002 - 16:44 mouse cursor now auto-hides with no mouse movement for 2 seconds
   Nov 10, 2002 - 17:38 added ROM patching to support the french and spanish CPC keyboard layouts
   Nov 10, 2002 - 21:31 fixed PPI port C handler
   Nov 12, 2002 - 22:08 changes to the OUT handler: GA and CRTC cannot be accessed at the same time; fixed the GA pen selection
   Nov 13, 2002 - 22:48 tweaked the IN/OUT handlers a bit more
   Dec 05, 2002 - 00:06 fixed the problem with the tape startup delay: the PPI Port C bit manipulation only considered 4 instead of 8 bits
   Dec 08, 2002 - 22:05 updated the Z80_OUT_handler to accept FDC data on ports 0x7f and 0x7e, as per Kevin's I/O port document
   Dec 10, 2002 - 23:50 print now draws double spaced in scanline mode: don't have to remove the text again this way
   Dec 19, 2002 - 23:28 added 48 & 96 kHz audio options; added sanity checks for LoadConfiguration
   Dec 21, 2002 - 15:50 PCAudio_init reports if selected sample rate is not supported; added empty path handling to LoadConfiguration
   Dec 22, 2002 - 17:21 found a typo in my keyboard lookup table: CPC key 8 was returning the same as CPC key 0
   Jan 12, 2003 - 01:05 added the ability to search for multiple extensions to zip_dir
   Jan 12, 2003 - 15:57 completed processing of dropped dsk, sna and cdt files
   Jan 15, 2003 - 17:30 added the removal of the temp file to process_drop when uncompressing zip archives
   Jan 21, 2003 - 18:53 changed the window handle in all MessageBox calls from NULL to hAppWnd: errors display correctly in full-screen mode now
   Jan 24, 2003 - 00:44 altered the validity check on loading expansion ROMs, as some may not contain a jump instruction at offset 0x06
   Jan 25, 2003 - 16:49 fixed the FDC port handling in z80_in_handler: "Famous 5" loads now!
   Jan 27, 2003 - 18:47 updated the resource file to support themed Windows XP controls
   Jan 28, 2003 - 19:18 added insert_voc: converts a sound sample into a CDT direct recording block
   Jan 30, 2003 - 20:37 added 24bpp display mode support
   Jan 31, 2003 - 16:50 CPC speed slider range is now 50% to 800%, and can be set in 25% increments
   Jan 31, 2003 - 22:42 AMSDOS is placed in ROM slot 7 if the config file does not yet exist
   Feb 23, 2003 - 14:48 fixed the Windows system palette problems in 256 colour mode
   Mar 04, 2003 - 21:04 removed the background brush in the app window class; replaced "return 0L" with "break" in WindowProc
   Mar 11, 2003 - 22:38 emulation can now continue to run when focus is lost; added more invalid object checks to ensure clean exits
   Mar 12, 2003 - 18:08 had to add the background brush again: fullscreen mode would otherwise not update the unused area
   Mar 12, 2003 - 19:02 added the 'auto-pause on loss of focus' feature to the Options dialog
   Mar 12, 2003 - 22:07 added the display of HRESULT codes for DirectDraw function calls
   Mar 17, 2003 - 22:40 added a check_drvX call to the eject disk menu functions
   Mar 22, 2003 - 18:39 added the ability to insert blank disks in either CPC drive; the new Disk page in the Options dialog allows you to choose the format; implemented the Flip Disk option to reverse sides
   Mar 23, 2003 - 21:41 custom disk formats can now be specified in the config file; modified LoadConfiguration and SaveConfiguration to support this feature
   Mar 29, 2003 - 16:40 tape motor is not turned on if there is no tape in the drive
   Apr 02, 2003 - 21:31 changes to the Options dialog: moved the ROM slots to their own page; some cosmetic changes; remembers last page user was on
   Apr 03, 2003 - 22:12 added the option to capture printer output to file; aborting a save dialog for a changed DSK now drops back to the emulation without taking any further action
   Apr 07, 2003 - 19:09 modified the z80_OUT_handler to capture port writes for the MF2; added all the necessary bits for MF2 emulation, but for now it doesn't display the MF2 menu correctly (text is invisible!)...
   Apr 07, 2003 - 21:59 doh! fixed the MF2 problem: the MF2 page-out port always set RAM bank0 instead if checking if the lower ROM was active
   Apr 09, 2003 - 15:49 MF2 ROM is now restored at every reset to ensure the ROM area has not been corrupted by memory writes
   Apr 13, 2003 - 16:18 added code to display the logo in the About box with a transparent background
   Apr 16, 2003 - 16:18 joystick emulation can be toggled on/off; keyboard control assignments now override regular CPC keyboard actions; all dialogs now feature the "Caprice32 - " string to make identification easier; ZIP selector shows ZIP file name
   Apr 26, 2003 - 14:03 added CaptureScreenshot to save the screen contents to a PNG file
   Apr 26, 2003 - 16:25 finished screen capture implementation: added file selector and path/file storage to config file
   May 05, 2003 - 22:46 updated the Audio page in the Options dialog with controls to choose the sample size and adjust the volume
   May 06, 2003 - 18:59 load_snapshot and save_snapshot now use the v3 PSG information + the printer port data
   May 13, 2003 - 17:41 fixed the file selector problem on drag and drop: the exit condition check was reversed
   May 18, 2003 - 01:01 moved Gate Array memory handling to ga_memory_manager; save_snapshot correctly stores current RAM bank now
   May 21, 2003 - 00:31 changed the colour palette generation to support an intensity level slider
   May 21, 2003 - 14:42 added the Intensity slider control to the Video options
   May 26, 2003 - 20:06 emulation loop needs to keep running to produce proper key events: fixes the Pause problem
   May 28, 2003 - 13:59 modified the shadow of the print routine to make it more readable on a white background
   May 29, 2003 - 15:09 implemented the info message display system and added strings for most emulator actions and keyboard shortcuts
   May 30, 2003 - 12:17 added the tape PLAY/STOP button control for proper tape operation
   Jun 02, 2003 - 15:05 if the CreateSurface call fails with video memory, it now attempts to allocate it in system memory
   Jun 04, 2003 - 19:38 added the 'on printer port' drop-down to the Audio options page
   Aug 10, 2003 - 14:35 z80_IN_handler: CRTC write only registers return 0; load_snapshot: fixed broken snapshot support (PPI values were written to the wrong ports!); digiblaster/soundplayer combined into one - changed from a drop down to a check box

   May 19, 2004 - 23:13 removed all DirectX/Windows specific parts and replaced them (wherever possible) with the SDL equivalent
   May 23, 2004 - 17:48 dropped double buffer scheme in favour of a back buffer/custom blit operation; fixed colour palette init in 8bpp mode; added support for half size render mode; back buffer is cleared in video_init(); replaced the SDL_Flip operation with an SDL_BlitSurface; initial ExitCondition is now EC_FRAME_COMPLETE to ensure we have a valid video memory pointer to start with
   May 24, 2004 - 00:49 reintroduced snapshot_load & snapshot_save; modified vdu_init to update the two SDL_Rect structures to center or crop the CPC screen; introduced the dwXScale var to support the half size render mode
   May 29, 2004 - 18:09 reintroduced tape_eject, tape_insert and tape_insert_voc; added sound support via the native SDL audio routines
   Nov 11, 2007 - 17:22 moved system-dependent stuff to cap32_sdl.cpp (Akop Karapetyan)
*/

#include <zlib.h>

#include "cap32.h"
#include "crtc.h"
#include "tape.h"
#include "z80.h"

#define VERSION_STRING "v4.2.0"

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

#define MSG_SNA_LOAD             32
#define MSG_SNA_SAVE             33
#define MSG_DSK_LOAD             34
#define MSG_DSK_SAVE             35
#define MSG_JOY_ENABLE           36
#define MSG_JOY_DISABLE          37
#define MSG_SPD_NORMAL           38
#define MSG_SPD_FULL             39
#define MSG_TAP_INSERT           40
#define MSG_SDUMP_SAVE           41
#define MSG_PAUSED               42
#define MSG_TAP_PLAY             43
#define MSG_TAP_STOP             44

#define MAX_LINE_LEN 256

#define MIN_SPEED_SETTING 2
#define MAX_SPEED_SETTING 32
#define DEF_SPEED_SETTING 4



extern byte bTapeLevel;
extern t_z80regs z80;

extern dword *ScanPos;
extern dword *ScanStart;
extern word MaxVSync;
extern t_flags1 flags1;
extern t_new_dt new_dt;

dword dwTicks, dwTicksOffset, dwTicksTarget, dwTicksTargetFPS;
dword dwFPS, dwFrameCount;
dword dwXScale, dwYScale;
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
byte keyboard_matrix[16];

static byte *membank_config[8][4];

FILE *pfileObject;
FILE *pfoPrinter;

#ifdef DEBUG
dword dwDebugFlag = 0;
FILE *pfoDebug;
#endif

double colours_rgb[32][3] = {
   { 0.5, 0.5, 0.5 }, { 0.5, 0.5, 0.5 },{ 0.0, 1.0, 0.5 }, { 1.0, 1.0, 0.5 },
   { 0.0, 0.0, 0.5 }, { 1.0, 0.0, 0.5 },{ 0.0, 0.5, 0.5 }, { 1.0, 0.5, 0.5 },
   { 1.0, 0.0, 0.5 }, { 1.0, 1.0, 0.5 },{ 1.0, 1.0, 0.0 }, { 1.0, 1.0, 1.0 },
   { 1.0, 0.0, 0.0 }, { 1.0, 0.0, 1.0 },{ 1.0, 0.5, 0.0 }, { 1.0, 0.5, 1.0 },
   { 0.0, 0.0, 0.5 }, { 0.0, 1.0, 0.5 },{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 1.0 },
   { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 },{ 0.0, 0.5, 0.0 }, { 0.0, 0.5, 1.0 },
   { 0.5, 0.0, 0.5 }, { 0.5, 1.0, 0.5 },{ 0.5, 1.0, 0.0 }, { 0.5, 1.0, 1.0 },
   { 0.5, 0.0, 0.0 }, { 0.5, 0.0, 1.0 },{ 0.5, 0.5, 0.0 }, { 0.5, 0.5, 1.0 }
};

double colours_green[32] = {
   0.5647, 0.5647, 0.7529, 0.9412,
   0.1882, 0.3765, 0.4706, 0.6588,
   0.3765, 0.9412, 0.9098, 0.9725,
   0.3451, 0.4078, 0.6275, 0.6902,
   0.1882, 0.7529, 0.7216, 0.7843,
   0.1569, 0.2196, 0.4392, 0.5020,
   0.2824, 0.8471, 0.8157, 0.8784,
   0.2510, 0.3137, 0.5333, 0.5961
};

byte bit_values[8] = {
   0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};

#define MOD_CPC_SHIFT   (0x01 << 8)
#define MOD_CPC_CTRL    (0x02 << 8)
#define MOD_EMU_KEY     (0x10 << 8)

typedef enum {
   CAP32_EXIT = MOD_EMU_KEY,
   CAP32_FPS,
   CAP32_FULLSCRN,
   CAP32_JOY,
   CAP32_LOADDRVA,
   CAP32_LOADDRVB,
   CAP32_LOADSNAP,
   CAP32_LOADTAPE,
   CAP32_MF2RESET,
   CAP32_MF2STOP,
   CAP32_OPTIONS,
   CAP32_PAUSE,
   CAP32_RESET,
   CAP32_SAVESNAP,
   CAP32_SCRNSHOT,
   CAP32_SPEED,
   CAP32_TAPEPLAY
} CAP32_KEYS;

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

static dword cpc_kbd[3][149] = {
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

#define MAX_ROM_MODS 2
#include "rom_mods.c"

char chAppPath[_MAX_PATH + 1];
char chROMSelected[_MAX_PATH + 1];
char chROMFile[3][14] = {
   "cpc464.rom",
   "cpc664.rom",
   "cpc6128.rom"
};

t_CPC CPC;
t_CRTC CRTC;
t_FDC FDC;
t_GateArray GateArray;
t_PPI PPI;
t_PSG PSG;
t_VDU VDU;

t_drive driveA;
t_drive driveB;

t_zip_info zip_info;

#define MAX_DISK_FORMAT 8
#define DEFAULT_DISK_FORMAT 0
#define FIRST_CUSTOM_DISK_FORMAT 2
t_disk_format disk_format[MAX_DISK_FORMAT] = {
   { "178K Data Format", 40, 1, 9, 2, 0x52, 0xe5, {{ 0xc1, 0xc6, 0xc2, 0xc7, 0xc3, 0xc8, 0xc4, 0xc9, 0xc5 }} },
   { "169K Vendor Format", 40, 1, 9, 2, 0x52, 0xe5, {{ 0x41, 0x46, 0x42, 0x47, 0x43, 0x48, 0x44, 0x49, 0x45 }} }
};



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



int file_size (int file_num)
{
   struct stat s;

   if (!fstat(file_num, &s)) {
      return s.st_size;
   } else {
      return 0;
   }
}



int zip_dir (t_zip_info *zi)
{
   int n, iFileCount;
   long lFilePosition;
   dword dwCentralDirPosition, dwNextEntry;
   word wCentralDirEntries, wCentralDirSize, wFilenameLength;
   byte *pbPtr;
   char *pchStrPtr;
   dword dwOffset;

   iFileCount = 0;
   if ((pfileObject = fopen(zi->pchZipFile, "rb")) == NULL) {
      return ERR_FILE_NOT_FOUND;
   }

   wCentralDirEntries = 0;
   wCentralDirSize = 0;
   dwCentralDirPosition = 0;
   lFilePosition = -256;
   do {
      fseek(pfileObject, lFilePosition, SEEK_END);
      if (fread(pbGPBuffer, 256, 1, pfileObject) == 0) {
         fclose(pfileObject);
         return ERR_FILE_BAD_ZIP; // exit if loading of data chunck failed
      }
      pbPtr = pbGPBuffer + (256 - 22); // pointer to end of central directory (under ideal conditions)
      while (pbPtr != (byte *)pbGPBuffer) {
         if (*(dword *)pbPtr == 0x06054b50) { // check for end of central directory signature
            wCentralDirEntries = *(word *)(pbPtr + 10);
            wCentralDirSize = *(word *)(pbPtr + 12);
            dwCentralDirPosition = *(dword *)(pbPtr + 16);
            break;
         }
         pbPtr--; // move backwards through buffer
      }
      lFilePosition -= 256; // move backwards through ZIP file
   } while (wCentralDirEntries == 0);
   if (wCentralDirSize == 0) {
      fclose(pfileObject);
      return ERR_FILE_BAD_ZIP; // exit if no central directory was found
   }
   fseek(pfileObject, dwCentralDirPosition, SEEK_SET);
   if (fread(pbGPBuffer, wCentralDirSize, 1, pfileObject) == 0) {
      fclose(pfileObject);
      return ERR_FILE_BAD_ZIP; // exit if loading of data chunck failed
   }

   pbPtr = pbGPBuffer;
   if (zi->pchFileNames) {
      free(zi->pchFileNames); // dealloc old string table
   }
   zi->pchFileNames = (char *)malloc(wCentralDirSize); // approximate space needed by using the central directory size
   pchStrPtr = zi->pchFileNames;

   for (n = wCentralDirEntries; n; n--) {
      wFilenameLength = *(word *)(pbPtr + 28);
      dwOffset = *(dword *)(pbPtr + 42);
      dwNextEntry = wFilenameLength + *(word *)(pbPtr + 30) + *(word *)(pbPtr + 32);
      pbPtr += 46;
      char *pchThisExtension = zi->pchExtension;
      while (*pchThisExtension != '\0') { // loop for all extensions to be checked
         if (strncasecmp((char *)pbPtr + (wFilenameLength - 4), pchThisExtension, 4) == 0) {
            strncpy(pchStrPtr, (char *)pbPtr, wFilenameLength); // copy filename from zip directory
            pchStrPtr[wFilenameLength] = 0; // zero terminate string
            pchStrPtr += wFilenameLength+1;
            *(dword *)pchStrPtr = dwOffset; // associate offset with string
            pchStrPtr += 4;
            iFileCount++;
            break;
         }
         pchThisExtension += 4; // advance to next extension
      }
      pbPtr += dwNextEntry;
   }
   fclose(pfileObject);

   if (iFileCount == 0) { // no files found?
      return ERR_FILE_EMPTY_ZIP;
   }

   zi->iFiles = iFileCount;
   return 0; // operation completed successfully
}



int zip_extract (char *pchZipFile, char *pchFileName, dword dwOffset)
{
   int iStatus, iCount;
   dword dwSize;
   byte *pbInputBuffer, *pbOutputBuffer;
   FILE *pfileOut, *pfileIn;
   z_stream z;

   tmpnam(pchFileName); // generate a unique (temporary) file name for the decompression process
   if (!(pfileOut = fopen(pchFileName, "wb"))) {
      return ERR_FILE_UNZIP_FAILED; // couldn't create output file
   }
   pfileIn = fopen(pchZipFile, "rb"); // open ZIP file for reading
   fseek(pfileIn, dwOffset, SEEK_SET); // move file pointer to beginning of data block
   fread(pbGPBuffer, 30, 1, pfileIn); // read local header
   dwSize = *(dword *)(pbGPBuffer + 18); // length of compressed data
   dwOffset += 30 + *(word *)(pbGPBuffer + 26) + *(word *)(pbGPBuffer + 28);
   fseek(pfileIn, dwOffset, SEEK_SET); // move file pointer to start of compressed data

   pbInputBuffer = pbGPBuffer; // space for compressed data chunck
   pbOutputBuffer = pbInputBuffer + 16384; // space for uncompressed data chunck
   z.zalloc = (alloc_func)0;
   z.zfree = (free_func)0;
   z.opaque = (voidpf)0;
   iStatus = inflateInit2(&z, -MAX_WBITS); // init zlib stream (no header)
   do {
      z.next_in = pbInputBuffer;
      if (dwSize > 16384) { // limit input size to max 16K or remaining bytes
         z.avail_in = 16384;
      } else {
         z.avail_in = dwSize;
      }
      z.avail_in = fread(pbInputBuffer, 1, z.avail_in, pfileIn); // load compressed data chunck from ZIP file
      while ((z.avail_in) && (iStatus == Z_OK)) { // loop until all data has been processed
         z.next_out = pbOutputBuffer;
         z.avail_out = 16384;
         iStatus = inflate(&z, Z_NO_FLUSH); // decompress data
         iCount = 16384 - z.avail_out;
         if (iCount) { // save data to file if output buffer is full
            fwrite(pbOutputBuffer, 1, iCount, pfileOut);
         }
      }
      dwSize -= 16384; // advance to next chunck
   } while ((dwSize > 0) && (iStatus == Z_OK)) ; // loop until done
   if (iStatus != Z_STREAM_END) {
      return ERR_FILE_UNZIP_FAILED; // abort on error
   }
   iStatus = inflateEnd(&z); // clean up
   fclose(pfileIn);
   fclose(pfileOut);

   return 0; // data was successfully decompressed
}



int snapshot_load (char *pchFileName)
{
   int n;
   dword dwSnapSize, dwModel, dwFlags;
   char chPath[_MAX_PATH + 1];
   byte val;
   reg_pair port;
   t_SNA_header sh;

   memset(&sh, 0, sizeof(sh));
   if ((pfileObject = fopen(pchFileName, "rb")) != NULL) {
      fread(&sh, sizeof(sh), 1, pfileObject); // read snapshot header
      if (memcmp(sh.id, "MV - SNA", 8) != 0) { // valid SNApshot image?
         fclose(pfileObject);
         return ERR_SNA_INVALID;
      }
      dwSnapSize = sh.ram_size[0] + (sh.ram_size[1] * 256); // memory dump size
      dwSnapSize &= ~0x3f; // limit to multiples of 64
      if (!dwSnapSize) {
         fclose(pfileObject);
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
            fclose(pfileObject);
            return ERR_OUT_OF_MEMORY;
         }
      }
      emulator_reset(false);
      n = fread(pbRAM, dwSnapSize*1024, 1, pfileObject); // read memory dump into CPC RAM
      fclose(pfileObject);
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
            if ((pfileObject = fopen(chPath, "rb")) != NULL) {
               n = fread(pbROMlo, 2*16384, 1, pfileObject);
               fclose(pfileObject);
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
         if (VDU.scanline > MaxVSync) {
            VDU.scanline = MaxVSync; // limit to max value
         }
         CRTC.char_count = sh.crtc_char_count[0];
         CRTC.line_count = sh.crtc_line_count & 127;
         CRTC.raster_count = sh.crtc_raster_count & 31;
         CRTC.hsw_count = sh.crtc_hsw_count & 15;
         CRTC.vsw_count = sh.crtc_vsw_count & 15;
         dwFlags = sh.crtc_flags[0] + (sh.crtc_flags[1] * 256);
         CRTC.flag_invsync = dwFlags & 1 ? 1 : 0; // vsync active?
         if (dwFlags & 2) { // hsync active?
            flags1.inHSYNC = 0xff;
            CRTC.flag_hadhsync = 1;
            if ((CRTC.hsw_count >= 3) && (CRTC.hsw_count < 7)) {
               CRTC.flag_inmonhsync = 1;
            }
         }
         CRTC.flag_invta = dwFlags & 0x80 ? 1 : 0; // in vertical total adjust?
         GateArray.hs_count = sh.ga_int_delay & 3;
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



int snapshot_save (char *pchFileName)
{
   t_SNA_header sh;
   int n;
   dword dwFlags;

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
   sh.crtc_hsw_count = CRTC.hsw_count;
   sh.crtc_vsw_count = CRTC.vsw_count;
   dwFlags = 0;
   if (CRTC.flag_invsync) { // vsync active?
      dwFlags |= 1;
   }
   if (flags1.inHSYNC) { // hsync active?
      dwFlags |= 2;
   }
   if (CRTC.flag_invta) { // in vertical total adjust?
      dwFlags |= 0x80;
   }
   sh.crtc_flags[0] = dwFlags & 0xff;
   sh.crtc_flags[1] = (dwFlags >> 8) & 0xff;
   sh.ga_int_delay = GateArray.hs_count;
   sh.ga_sl_count = GateArray.sl_count;
   sh.z80_int_pending = z80.int_pending;

   if ((pfileObject = fopen(pchFileName, "wb")) != NULL) {
      if (fwrite(&sh, sizeof(sh), 1, pfileObject) != 1) { // write snapshot header
         fclose(pfileObject);
         return ERR_SNA_WRITE;
      }
      if (fwrite(pbRAM, CPC.ram_size*1024, 1, pfileObject) != 1) { // write memory contents to snapshot file
         fclose(pfileObject);
         return ERR_SNA_WRITE;
      }
      fclose(pfileObject);
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

   if (CPC.keyboard) {
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
         memcpy(pbPtr, cpc_keytrans[CPC.keyboard-1], 240); // patch the CPC OS ROM with the chosen keyboard layout
         pbPtr = pbROMlo + 0x3800;
         memcpy(pbPtr, cpc_charset[CPC.keyboard-1], 2048); // add the corresponding character set
      }
   }

   return 0;
}



void emulator_reset (bool bolMF2Reset)
{
   int n;

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

// VDU
   memset(&VDU, 0, sizeof(VDU)); // clear VDU data structure
   VDU.flag_drawing = 1;

// CRTC
   crtc_reset();

// Gate Array
   memset(&GateArray, 0, sizeof(GateArray)); // clear GA data structure
   GateArray.scr_mode =
   GateArray.requested_scr_mode = 1; // set to mode 1
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

   for (iRomNum = 0; iRomNum < 16; iRomNum++) { // loop for ROMs 0-15
      if (CPC.rom_file[iRomNum][0]) { // is a ROM image specified for this slot?
         pchRomData = new char [16384]; // allocate 16K
         memset(pchRomData, 0, 16384); // clear memory
         strncpy(chPath, CPC.rom_path, sizeof(chPath)-2);
         strcat(chPath, "/");
         strncat(chPath, CPC.rom_file[iRomNum], sizeof(chPath)-1 - strlen(chPath));
         if ((pfileObject = fopen(chPath, "rb")) != NULL) { // attempt to open the ROM image
            fread(pchRomData, 128, 1, pfileObject); // read 128 bytes of ROM data
            word checksum = 0;
            for (int n = 0; n < 0x43; n++) {
               checksum += pchRomData[n];
            }
            if (checksum == ((pchRomData[0x43] << 8) + pchRomData[0x44])) { // if the checksum matches, we got us an AMSDOS header
               fread(pchRomData, 128, 1, pfileObject); // skip it
            }
            if (!(pchRomData[0] & 0xfc)) { // is it a valid CPC ROM image (0 = forground, 1 = background, 2 = extension)?
               fread(pchRomData+128, 16384-128, 1, pfileObject); // read the rest of the ROM file
               memmap_ROM[iRomNum] = (byte *)pchRomData; // update the ROM map
            } else { // not a valid ROM file
               fprintf(stderr, "ERROR: %s is not a CPC ROM file - clearing ROM slot %d.\n", CPC.rom_file[iRomNum], iRomNum);
               delete [] pchRomData; // free memory on error
               CPC.rom_file[iRomNum][0] = 0;
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

   crtc_init();
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



void video_set_style(int half_pixels)
{
   switch (half_pixels) {
      case 1:
         dwXScale = 1;
         dwYScale = 1;
         CPC.scr_prerendernorm = (void(*)(void))prerender_normal_half;
         CPC.scr_prerenderbord = (void(*)(void))prerender_border_half;
         CPC.scr_prerendersync = (void(*)(void))prerender_sync_half;
         break;
      case 2:
         dwXScale = 2;
         dwYScale = 1;
         CPC.scr_prerendernorm = (void(*)(void))prerender_normal;
         CPC.scr_prerenderbord = (void(*)(void))prerender_border;
         CPC.scr_prerendersync = (void(*)(void))prerender_sync;
         break;
   }

   switch(CPC.scr_bpp)
   {
      case 32:
               CPC.scr_render = (void(*)(void))render32bpp;
               break;

      case 24:
               CPC.scr_render = (void(*)(void))render24bpp;
               break;

      case 16:
      case 15:
               CPC.scr_render = (void(*)(void))render16bpp;
               break;

      case 8:
               CPC.scr_render = (void(*)(void))render8bpp;
               break;
   }
}



int getConfigValueInt (char* pchFileName, char* pchSection, char* pchKey, int iDefaultValue)
{
   FILE* pfoConfigFile;
   char chLine[MAX_LINE_LEN + 1];
   char* pchToken;

   if ((pfoConfigFile = fopen(pchFileName, "r")) != NULL) { // open the config file
      while(fgets(chLine, MAX_LINE_LEN, pfoConfigFile) != NULL) { // grab one line
         pchToken = strtok(chLine, "[]"); // check if there's a section key
         if((pchToken != NULL) && (pchToken[0] != '#') && (strcmp(pchToken, pchSection) == 0)) {
            while(fgets(chLine, MAX_LINE_LEN, pfoConfigFile) != NULL) { // get the next line
               pchToken = strtok(chLine, "\t =\n\r"); // check if it has a key=value pair
               if((pchToken != NULL) && (pchToken[0] != '#') && (strcmp(pchToken, pchKey) == 0)) {
                  char* pchPtr = strtok(NULL, "\t =#\n\r"); // get the value if it matches our key
                  if (pchPtr != NULL) {
                     return (strtol(pchPtr, NULL, 0)); // return as integer
                  } else {
                     return iDefaultValue; // no value found
                  }
               }
            }
         }
      }
      fclose(pfoConfigFile);
   }
   return iDefaultValue; // no value found
}



void getConfigValueString (char* pchFileName, char* pchSection, char* pchKey, char* pchValue, int iSize, char* pchDefaultValue)
{
   FILE* pfoConfigFile;
   char chLine[MAX_LINE_LEN + 1];
   char* pchToken;

   if ((pfoConfigFile = fopen(pchFileName, "r")) != NULL) { // open the config file
      while(fgets(chLine, MAX_LINE_LEN, pfoConfigFile) != NULL) { // grab one line
         pchToken = strtok(chLine, "[]"); // check if there's a section key
         if((pchToken != NULL) && (pchToken[0] != '#') && (strcmp(pchToken, pchSection) == 0)) {
            while(fgets(chLine, MAX_LINE_LEN, pfoConfigFile) != NULL) { // get the next line
               pchToken = strtok(chLine, "\t =\n\r"); // check if it has a key=value pair
               if((pchToken != NULL) && (pchToken[0] != '#') && (strcmp(pchToken, pchKey) == 0)) {
                  char* pchPtr = strtok(NULL, "\t=#\n\r"); // get the value if it matches our key
                  if (pchPtr != NULL) {
                     strncpy(pchValue, pchPtr, iSize); // copy to destination
                     return;
                  } else {
                     strncpy(pchValue, pchDefaultValue, iSize); // no value found, return the default
                     return;
                  }
               }
            }
         }
      }
      fclose(pfoConfigFile);
   }
   strncpy(pchValue, pchDefaultValue, iSize); // no value found, return the default
}






void splitPathFileName(char *pchCombined, char *pchPath, char *pchFile)
{
   char *pchPtr;

   pchPtr = strrchr(pchCombined, '/'); // start from the end and find the first path delimiter
   if (!pchPtr) {
      pchPtr = strrchr(pchCombined, '\\'); // try again with the alternate form
   }
   if (pchPtr) {
      pchPtr++; // advance the pointer to the next character
      if (pchFile) {
         strcpy(pchFile, pchPtr); // copy the filename
      }
      char chOld = *pchPtr;
      *pchPtr = 0; // cut off the filename part
      if (pchPath != pchCombined) {
         if (pchPath) {
            strcpy(pchPath, pchCombined); // copy the path
         }
         *pchPtr = chOld; // restore original string
      }
   } else {
      if (pchFile) {
         *pchFile = 0; // no filename found
      }
      if (pchPath != pchCombined) {
         if (pchPath) {
            strcpy(pchPath, pchCombined); // copy the path
         }
      }
   }
}



