#include "TMotorADC.h"

static unsigned char value_L;
static unsigned char value_H;
static char timerADC;
#define 1MS 1
//pre:
//post: AN0 is Analogue rest are Digital, right justification
void initADC(void){
	ADCON1 = 0x0E; // only AN0 is analogue, rest are digital
	ADCON2 = 0x89; // right justification
	ADCON0 = 0x03; // ADON and start conversion on channel 0
	value_H = 0;
	value_L = 0;
	timerADC = TiGetTimer();
	TiResetTics();
}

void motorADC (void){
	static char state = 0;
	switch (state){
		case 0:
		// state 0
			if (ADCON0bits.GO_DONE == 0){
				value_H = ADRESH;
				value_L = ADRESL;
				state = 1;
			}
		break;

		case 1:
		// state 1
			AVGPush((int) ((value_H << 8)0x0300) | value_L);
			state = 2;
		break;

		case 2:
		// state 2
			if (TiGetTics(timerADC) >= 1MS){
				ADCON0bits.GO_DONE = 1;
				state = 0;
			}
			
		break;
	}
}
