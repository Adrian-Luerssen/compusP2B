#ifndef T_EUSART_H
#define T_EUSART_H

//post: configuring a baudrate of 9600 and FOSC of 10Mhz
void initSIO (void);

//pre:SIO is available to send
//post: sense my byte with the eusart
void SiSendChar(char my_byte);

//pre:
//post: returns 1 if the sio is ready to send, returns 0 otherwise
char SiIsAvailable (void);

#endif