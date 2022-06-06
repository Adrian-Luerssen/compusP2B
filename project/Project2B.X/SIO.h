
#ifndef SIO_H
#define	SIO_H



#include <xc.h> // include processor files - each processor file is guarded.  
void initSIO(void);
char SiIsAvailable(void);

void SiSendChar(char myByte);


#endif	

