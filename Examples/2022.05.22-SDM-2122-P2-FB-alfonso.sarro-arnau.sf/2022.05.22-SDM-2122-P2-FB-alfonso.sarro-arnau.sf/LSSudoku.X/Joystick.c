#include "Joystick.h"
#include "LcTLCD.h"
#include "EUSART.h"
#include "Logic.h"

#define BUFFER_SIZE 10
#define YAXIS 0x01
#define XAXIS 0x05
#define DONE 0
#define GO 1

static char state;
static char times;

void initJoystk(void) {
	state = 0;
	ADCON0 = 0x01; //b'00000001'
	ADCON1 = 0x0C; //b'00001101'
	ADCON2 = 0x89; //b'10001001'
	TRISAbits.RA0 = 1;
    times = 0;
}


void motorJoystk(void) {
	switch(state) {
		case 0:
			if(ADCON0bits.GODONE == DONE) {
				if (ADRESH == 0x00 || ADRESH == 0x03) {
                    //DEBUGGING: change nested if for LoMoveMenu(ADRESH);
                    if(times == BUFFER_SIZE) {
                        if(ADRESH == 0x00) {
                            if(ADCON0 == YAXIS) {
                                LoMoveUp();
                            }
                            else {
                                LoMoveRight();
                            }
                        } else {
                            if(ADCON0 == YAXIS) {
                                LoMoveDown();
                            }
                            else {
                                LoMoveLeft();
                            }
                        }
                        state++;
                    }
                    else {
                        times++;
                    }
                    //REAL CODE
                    /*
					LoMoveMenu(ADRESH);
					state++; //GOTO state 1
                    */
				}
                else {
                    state = 2;
                    times = 0;//TEST
                }
                ADCON0bits.GODONE = 1;
			}
            
            break;
		case 1:
			if(ADCON0bits.GODONE == DONE) {
				if (ADRESH == 0x02 || ADRESH == 0x01) {
                    times = 0;
					state--; //GOTO state 0
				}
                ADCON0bits.GODONE = 1;
			}
            break;
        case 2:
            if(ADCON0 == YAXIS) {
                ADCON0 = XAXIS;
            }
            else {
                ADCON0 = YAXIS;
            }
            state--;
            break;
	}
}
