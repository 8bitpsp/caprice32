#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pspkernel.h>

#include "audio.h"
#include "video.h"
#include "psp.h"
#include "ctrl.h"

#include "psp/menu.h"

PSP_MODULE_INFO(PSP_APP_NAME, 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

static void ExitCallback(void* arg)
{
  ExitPSP = 1;
}

int main(int argc, char **argv)
{
  /* Initialize PSP */
  pspInit(argv[0]);
  pspAudioInit(1024, 1);
  pspCtrlInit();
  pspVideoInit();

  /* Initialize callbacks */
  pspRegisterCallback(PSP_EXIT_CALLBACK, ExitCallback, NULL);
  pspStartCallbackThread();

  /* Show the menu */
  if (InitMenu())
  {
	  DisplayMenu();
	  TrashMenu();
	}

  /* Release PSP resources */
  pspAudioShutdown();
  pspVideoShutdown();
  pspShutdown();

  return(0);
}
