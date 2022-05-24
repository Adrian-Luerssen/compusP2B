#include "TMotorAVG.h"

#define MAX_SAMPLES 69
#define SEC1 1000

static char how_many = 0;
static int samples[MAXSAMPLES];
static char timer;

void initAVG(void) {
	char i;
	for (i = 0; i <MAXSAMPLES; i++){
		samples[i] = 0;
	}
}

void AVGPush (int data){
	// if the queue is full then we will not store the value
	if (how_many < MAXSAMPLES) {
		sample[how_many++] = data;
	}
	timer = TiGetTimer();
	TiResetTics(timer);
}


void motorAVG (void) {
	static char state = 0;
	static long acum = 0;
	static char count = 0;

	switch (state) {
		case 0:
			if (how_many == MAXSAMPLES && TiGetTics(timer) >= SEC1){
				TiResetTics(timer);
				count = 0;
				acum = 0;
				state = 1;
			}
		break;
		case 1:
			if (count < MAXSAMPLES){
				acum += samples[count++]; 
				// worst case 69 * 1023 = 70587 an int is 16 bits
				// 2^16 = 65536 therefore int is not enough so we need a long
			}else { //if (count == MAXSAMPLES){ can be removed as we want to save flash memory

				acum = acum/MAXSAMPLES; // very slow as we dont have dedicated instructions to divide
				state = 2;
			}
		break;

		case 2:
			if (SiIsAvailable() == 1){
				SiSendChar((char) (acum >> 8) & 0x03);
				state = 3;
			}
		break;

		case 3:
			if (SiIsAvailable() == 1){
				SiSendChar((char) (acum) & 0xFF);
				how_many = 0;
				state = 0;
			}
		break;
	}
}

