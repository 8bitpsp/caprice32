Caprice32 PSP
=============

Amstrad CPC emulator

&copy; 2007 Akop Karapetyan  
&copy; 1997-2004 Ulrich Doewich

New Features
------------

#### Version 4.2.0.2 (November 28 2007)

*   ZIP file support for disk and tape images
*   Disk images can be saved, ejected and loaded without resetting the system (System tab)
*   Save state autoloading – any save state slot can be designated as the autoload slot (Options tab, disabled by default). Saved games in the autoload slot are loaded any time a game is loaded from the Game tab
*   CPC model is now correctly reset when modified via the System menu

Installation
------------

Unzip `caprice32.zip` into `/PSP/GAME/` folder on the memory stick.

Controls
--------

During emulation:

| PSP controls                    | Emulated controls            |
| ------------------------------- | ---------------------------- |
| D-pad up/down/left/right        | Keyboard up/down/left/right  |
| Analog stick up/down/left/right | Joystick up/down/left/right  |
| [ ] (square)                    | Joystick button A            |
| X (cross)                       | Joystick button B            |
| O (circle)                      | Spacebar                     |
| [R]                             | Show virtual keyboard        |
| [L] + [R]                       | Return to the emulator menu  |

When the virtual keyboard is on:

| PSP controls                    | Function                 |
| ------------------------------- | ------------------------ |
| Directional pad                 | select key               |
| [ ] (square)                    | press key                |
| O (circle)                      | hold down/release key    |
| ^ (triangle)                    | release all held keys    |

Only certain keys can be held down.

Keyboard mappings can be modified for each game. Button configuration changes are not automatically retained after a mapping is modified. To save changes, press X (cross) after desired mapping is configured. To set the mapping as the default mapping, press [ ] (square).

Autoloading
-----------

Caprice32 PSP can be configured to automatically load a saved game whenever a new game is loaded via the Game tab. To do this, specify one of the save state slots as the Autoload slot (Options tab). If a saved game exists in the specified slot, it will be automatically loaded.

Loading Disk and Tape Images
----------------------------

To load a disk image or a tape image without resetting the system, use the _Disk drive_ / _Tape drive_ option listed in the System tab.

Disk Image Saving
-----------------

To save a modified save state image, navigate to the System tab, highlight the _Disk drive_ option and press [ ] (square). Only uncompressed (unzipped) disk images can be updated.

Known Issues
------------

1.  While the version of the emulator is 4.2.0, the rendering routines are from version 4.1.0. The latest renderer is unoptimized and performs poorly on the PSP. When the renderer is updated, Caprice32 PSP will be updated to reflect the change.

Compiling
---------

To compile, ensure that [zlib](svn://svn.pspdev.org/psp/trunk/zlib) and [libpng](svn://svn.pspdev.org/psp/trunk/libpng) are installed, and run make:

`make -f Makefile.psp`

Before compiling Caprice32, compile the PSP library by entering the `psp/mypsp` directory and running `make`.

Version History
---------------

#### 4.2.0.0 (November 21 2007)

*   Initial release

Credits
-------

Ulrich Doewich (Caprice32)
