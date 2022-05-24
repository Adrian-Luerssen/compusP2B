
#include "TKeypad.h"
#define TBOUNCES 25;
#define PRESSED 1;
#define COLS ~((PORTB >> 1)| 0xF8);
#define SECOND 200;
#define IDLE 999;
// ~ negates bit by bit

// PORTB [XXXX C C C S]
// PORTB >> 1 [XXXXX C C C]
// PORTB >> 1 | 0xF8 [11111 C C C] negative logic
// ~((PORTB >> 1) | 0xF8) [00000 !C !C !C] positive logic

static char row;
static char timerBounces;
static char timer1s;
static char lastPress;

void initKeyboard(void){
	timerBounces = TiGetTimer();
	timer1s = TiGetTimer();
	row = 0;
	TRISBbits.TRISB0 = TRSIBbits.TRISB1 = RISBbits.TRISB1 = TRSIBbits.TRISB2 = 1;
	RISBbits.TRISB3 = TRSIBbits.TRIS42 = RISBbits.TRISB5 = TRSIBbits.TRISB6 = RISBbits.TRISB1 = 0;
}


void keyboardMotor(void) {
	static char state = 0;
	initKeyboard();
	switch(state) {
		
		case 0:
			if (TiGetTics(timer1s) < SECOND || lastPress == IDLE){
				if (keyPressed() != PRESSED) {
					row = 1 - row ;
					// row = 1 or row = 0;
					LATB = (row+1) << 4;
				}else {
					TiResetTics(timerBounces);
					state = 2;
				}
			} else {
				lastPress = IDLE;
				nextLetter();
			}
			
		break;
		case 1:
			if (TiGetTics(timerBounces) >= TBOUNCES){
				decodeKey(row,COLS);
				state = 2;
			}
		break;
		case 2:
			if (keyPressed() != PRESSED) {
				TiResetTics(timerBounces);
				state = 3;
			}
		break;
		case 3:
			if (TiGetTics(timerBounces)>=TBOUNCES) {
				TiResetTics(timer1s); 
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

	if (lastPress != col + (row*3)){
		nextLetter();
		lastPress = col + (row*3);
	} else {
		if (lastPress == 8 || lastPress == 6){
			numPresses =(numPresses + 1)%5;
		} else {
			numPresses =(numPresses + 1)%4;
		}
		
	}
	switch(col + (row*3)) {
		case 0:
			sendChar('1');
		break;
		case 1:
			if (numPresses == 3){
				sendChar('2');
			} else {
				sendChar('a'+numPresses);
			}
		break;
		case 2:
			if (numPresses == 3){
				sendChar('3');
			} else {
				sendChar('d'+numPresses);
			}
		break;
		case 3:
			if (numPresses == 3){
				sendChar('4');
			} else {
				sendChar('g'+numPresses);
			}
		break;
		case 4:
			if (numPresses == 3){
				sendChar('5');
			} else {
				sendChar('j'+numPresses);
			}
		break;
		case 5:
			if (numPresses == 3){
				sendChar('6');
			} else {
				sendChar('m'+numPresses);
			}
		break;
		case 6:
			if (numPresses == 4){
				sendChar('7');
			} else {
				sendChar('q'+numPresses);
			}
		break;
		case 7:
			if (numPresses == 3){
				sendChar('8');
			} else {
				sendChar('t'+numPresses);
			}
		break;
		case 8:
			if (numPresses == 4){
				sendChar('9');
			} else {
				sendChar('w'+numPresses);
			}
		break;
		case 9:
			sendChar('*');
		break;
		case 10:
			sendChar('0');
		break;
		case 11:
			sendChar('#');
		break;

	}
}

char keyPressed(void) {
	//check columns return 1 if pressed
	return !(PORTBbits.RB1 && PORTBbits.RB2 && PORTBbits.RB3);
}

char nextLetter(void){
	numPresses = 0;
}