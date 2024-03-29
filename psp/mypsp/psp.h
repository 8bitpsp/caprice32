/** PSP helper library ***************************************/
/**                                                         **/
/**                          psp.h                          **/
/**                                                         **/
/** This file contains declarations for core PSP management **/
/** routines, such as program startup/shutdown, callbacks,  **/
/** etc...                                                  **/
/**                                                         **/
/** Copyright (C) Akop Karapetyan 2007                      **/
/**     You are not allowed to distribute this software     **/
/**     commercially. Please, notify me, if you make any    **/
/**     changes to this file.                               **/
/*************************************************************/
#ifndef _PSP_H
#define _PSP_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
  PSP_EXIT_CALLBACK
} CallbackType;

extern int  ExitPSP;

void        pspInit(char *app_path);
void        pspShutdown();
const char* pspGetAppDirectory();
void        pspSetClockFrequency(int freq);
int         pspGetBatteryTime();
int         pspGetBatteryPercent();

/* Callbacks */
int pspRegisterCallback(CallbackType type, void (*func)(void *param), void *param);
int pspStartCallbackThread();

#ifdef __cplusplus
}
#endif

#endif // _PSP_H
