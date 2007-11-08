#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pspkernel.h>

#include "psp/menu.h"
#include "emulate.h"

#include "video.h"
#include "image.h"
#include "ui.h"
#include "ctrl.h"
#include "psp.h"
#include "util.h"
#include "init.h"
#include "file.h"

static char ResumeEmulation;

int InitMenu()
{
  if (!InitEmulation())
    return 0;

  return 1;
}

void DisplayMenu()
{

  /* Menu loop */
  do
  {
    ResumeEmulation = 0;
ResumeEmulation = 1;

    /* Set normal clock frequency */
    pspSetClockFrequency(222);
    /* Set buttons to autorepeat */
    pspCtrlSetPollingMode(PSP_CTRL_AUTOREPEAT);

    if (!ExitPSP)
    {
      /* Set clock frequency during emulation */
//      pspSetClockFrequency(Options.ClockFreq);
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

void TrashMenu()
{
  /* Free emulation-specific resources */
  TrashEmulation();
}
