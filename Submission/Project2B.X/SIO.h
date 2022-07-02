
#ifndef SIO_H
#define	SIO_H



#include <xc.h> // include processor files - each processor file is guarded.  
#include "TTimer.h"
void initSIO(void);
void btMotor (void);


char btAvailable(void);
//PRE: --
//POST: returns 1 if additional serial channel is ready to send, otherwise returns 0

void btSendByte(char byte);
//PRE: btAvailable()
//POST: sends the byte through additional serial channel

void setResetTXRX(void);
//PRE: --
//POST: negates the enable of the additional serial channel

char SiIsAvailable(void);
//PRE: --
//POST: returns 1 if SIO is ready to send, otherwise returns 0

void SiSendChar(char myByte);
//PRE: SiIsAvailable()
//POST: Sends buffer using the EUSART

char SiRecievedByte(void);
//PRE: --
//POST: returns 1 if SIO has received, otherwise returns 0

char SiReadByte(void);
//PRE: SiRecievedByte()
//POST: returns whatever was received.
//WARNING: destructive read
#endif	

