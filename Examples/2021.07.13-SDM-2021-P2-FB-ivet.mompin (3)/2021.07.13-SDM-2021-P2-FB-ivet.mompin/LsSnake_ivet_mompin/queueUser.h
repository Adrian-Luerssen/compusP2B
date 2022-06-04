
#ifndef XC_QUEUE_USER_H
#define	XC_QUEUE_USER_H

#include <xc.h> 
#include "TData.h"
#include "marqueeMenu.h"
#include "EUSART.h"


static char m,n,times4,num_users3;
void initUser();
void motorUser();
void marqueeUsers(char times_pressed);
void userReset();
void PutSpaces(char j);
void place_char(char character,char row);

#endif 

