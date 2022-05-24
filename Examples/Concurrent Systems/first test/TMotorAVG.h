#ifndef T_AVG_H
#define T_AVG_H

#include "TEusart.h"
#include "TTimer.h"

void initAVG(void);
//pre:--
//post: fills the queue with 0's

void AVGPush (int data);
//pre:--
//post: stores the data in the queue of samples

void motorAVG(void);
//pre:--
//post: gets samples from the ADC and store them in a queue


#endif