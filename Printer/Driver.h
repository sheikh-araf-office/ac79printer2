

#ifndef __DRIVER_H
#define __DRIVER_H

#include "Driver.h"
#include "McuInt.h"
#include "UserInterface.h"


extern void USBReturnWifi(unsigned char Step); // ×î¶à64×Ö½Ú
/*extern void Cmd0dPrint(void);
extern void Cmd1bClass(unsigned char data);
extern void Cmd1b0a(unsigned char data);
extern void Cmd1b2aDots(unsigned char data);
extern void Cmd1b4aPAP(unsigned char data);
extern void Cmd1b55PrtDir(unsigned char data);
extern void Cmd1b7a(unsigned char data);
extern void Cmd1b89(unsigned char data);
extern void Cmd1b9c(unsigned char data);
extern void Cmd1dClass(unsigned char data);
extern void Cmd1ba0(unsigned char data);
extern void Cmd1d56CutPaper(unsigned char data);*/
extern void DealData(unsigned char data);


#endif


