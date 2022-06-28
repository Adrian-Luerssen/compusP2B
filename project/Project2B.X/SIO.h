
#ifndef SIO_H
#define	SIO_H



#include <xc.h> // include processor files - each processor file is guarded.  
#include "TTimer.h"
void initSIO(void);
char SiIsAvailable(void);

void SiSendChar(char myByte);
char SiRecievedByte(void);
char SiReadByte(void);

void btMotor (void);
char btAvailable(void);
void btSendByte(char byte);
#endif	

