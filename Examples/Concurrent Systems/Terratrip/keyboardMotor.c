
#include "TKeypad.h"
#define TBOUNCES 25;
#define PRESSED 1;
#define COLS ~((PORTB >> 1)| 0xF8);
// ~ negates bit by bit

// PORTB [XXXX C C C S]
// PORTB >> 1 [XXXXX C C C]
// PORTB >> 1 | 0xF8 [11111 C C C] negative logic
// ~((PORTB >> 1) | 0xF8) [00000 !C !C !C] positive logic

static char row;
static char timer;

void initKeyboard(void){
	timer = TiGetTimer();
	row = 0;
	TRISBbits.TRISB0 = TRSIBbits.TRISB1 = RISBbits.TRISB1 = TRSIBbits.TRISB2 = 1;
	RISBbits.TRISB3 = TRSIBbits.TRIS42 = RISBbits.TRISB5 = TRSIBbits.TRISB6 = RISBbits.TRISB1 = 0;
}


void keyboardMotor(void) {
	static char state = 0;
	initKeyboard();
	switch(state) {
		
		case 0:
			if (keyPressed() != PRESSED) {
				row = 1 - row ;
				// row = 1 or row = 0;
				LATB = (row+1) << 4;
			}else {
				TiResetTics(timer);
				state = 2;
			}
		break;
		case 1:
			if (TiGetTics(timer) >= TBOUNCES) {
				decodeKey(row,COLS);
				state = 2;
			}
		break;
		case 2:
			if (keyPressed() != PRESSED) {
				TiResetTics(timer);
				state = 3;
			}
		break;
		case 3:
			if (TiGetTics(timer)>=TBOUNCES) {
				state = 0;
			}
		break;
	}
}

void decodeKey(char row, char col){
	//pre: row == 1 or row == 0
	//pre: col == 0 or col == 1 or col == 2

	//const char table[][] = {{0,1,2},{3,4,5}};
	// if (rows == 0 && cols == 0)
	// ...
	switch(col + (row*3)) {
		case 0:
			StChangeChannel(1);
		break;
		case 1:
			StClearTrip();
		break;
		case 2:
			StChangeDirection();
		break;
		case 3:
			StChangeChannel(0);
		break;
		case 4:
			StTimeTrip();
		break;
		case 5:
			SsActivateOrDisactivate();
		break;

	}
}

char keyPressed(void) {
	//check columns return 1 if pressed
	return !(PORTBbits.RB1 && PORTBbits.RB2 && PORTBbits.RB3);
}