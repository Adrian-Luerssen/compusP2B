#ifndef T_ADC_H
#define T_ADC_H

#include "TTimer.h"
#include "TMotorAVG.h"

void initADC(void);
//pre:
//post: AN0 is Analogue rest are Digital, right justification


void motorADC(void);
//pre:--
//post: gets samples from the ADC and store them in a queue

#endif