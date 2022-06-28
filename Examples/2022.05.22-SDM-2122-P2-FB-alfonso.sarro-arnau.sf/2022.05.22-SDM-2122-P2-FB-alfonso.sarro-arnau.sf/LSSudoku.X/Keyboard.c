#include "Keyboard.h"
#include "TTimer.h"
#include "LcTLCD.h"
#include "Logic.h"

#define PRESSED 1
#define NOT_PRESSED 0

#define KEYPRESSED (PORTBbits.RB5 == PRESSED || PORTBbits.RB4 == PRESSED || PORTBbits.RB3 == PRESSED || PORTBbits.RB2 == PRESSED)
#define SAMEKEY (prev_row == row && prev_column == column)

#define DEBOUNCE_T 10//16 ms ^= 20
#define ONE_SECOND 1200//1 s ^= 1200

#define PINS_ROWS 
#define PINS_COLUMNS TRISBbits.TRISB2 = TRISBbits.TRISB3 = TRISBbits.TRISB4 = TRISBbits.TRISB5
#define FALSE 0
#define TRUE 1


static char sms;
static char row;
static char column;
static char prev_row;
static char prev_column;
static char tDebounce;
static char tSMS;
static char numerical;

static const char table[4][3][6] = { //scratch rascar: possible to put in ram deleting const; ram is more scarce though 
	{
		{'1', '1', '\0'},
		{'2', 'A', 'B', 'C', '\0'},
		{'3', 'D', 'E', 'F', '\0'}
	},
	{
		{'4', 'G', 'H', 'I', '\0'},
		{'5', 'J', 'K', 'L', '\0'},
		{'6', 'M', 'N', 'O', '\0'}
	},
	{
		{'7', 'P', 'Q', 'R', 'S', '\0'},
		{'8', 'T', 'U', 'V', '\0'},
		{'9', 'W', 'X', 'Y', 'Z', '\0'},
	},
	{
		{'*', '\0'},
		{'0', ' ', '\0'},
		{'#', '#'}
	}
};

void shiftColumns(void) {
    column++;
    if (column>2) column = 0;
    LATE = 0x04 >> column; //DEBUG gotta test   // b'1000' >> column
}

void initKeyboard(void) {
	sms = 1;
	row = prev_row = 0;
	column = prev_column = 0;
	tDebounce = TiGetTimer();
	tSMS = TiGetTimer();
    TRISE = 0x08;
    PINS_COLUMNS = 1;
    numerical = TRUE;
}

//TODO: special call to Logic when HASH #### is pressed methinks

void motorKeyboard(void) {
	static char state = 0;
    
	switch(state) {
		case 0:
			if (KEYPRESSED) {
                if (PORTBbits.RB5 == 1) row = 0;
                else if (PORTBbits.RB4 == 1) row = 1;
                else if (PORTBbits.RB3 == 1) row = 2; //se puede rascar SCRATCH MY BACK
                else { row = 3; }
				TiResetTics(tDebounce);
                prev_row = row;
                prev_column = column;
				state++; //state 1
			}
            else {
                shiftColumns();
            }
			break;
		case 1:
			if (TiGetTics(tDebounce) >= DEBOUNCE_T && !KEYPRESSED) {
				state--; //state 0
			}
			else if (TiGetTics(tDebounce) >= DEBOUNCE_T && KEYPRESSED) {
				TiResetTics(tSMS);
				state++; //state 2
			}
			break;
		case 2:
			if (!KEYPRESSED) {
                if(!numerical) {
                    //LcPutChar(table[row][column][0]);
                    if(table[row][column][0] != '#') LcPutChar(table[row][column][sms]);
                }
                state++; //state 3
                
			}
			break;
		case 3:
            shiftColumns();
            if(numerical) {
               LoGotKey(table[prev_row][prev_column][0]);
               state = 0;
            }
            else {
                if (TiGetTics(tSMS) >= ONE_SECOND) {
                    LoGotKey(table[prev_row][prev_column][sms]);
                    sms = 1;
                    state = 0;
                }

                if (KEYPRESSED) {
                    if (PORTBbits.RB5 == 1) row = 0;
                    else if (PORTBbits.RB4 == 1) row = 1;
                    else if (PORTBbits.RB3 == 1) row = 2; //se puede rascar SCRATCH MY BACK
                    else { row = 3; }

                    if (!SAMEKEY) { 
                        if(numerical) {
                            LoGotKey(table[prev_row][prev_column][0]);
                        }
                        else {
                            LoGotKey(table[prev_row][prev_column][sms]);
                        }
                        sms = 1;
                    }
                    else {
                        if((table[row][column][sms+1] == '\0') || table[row][column][0] == '*' || table[row][column][0] == '#') {//Scratch Rascar
                            sms = 0;
                        } else {
                            sms++;
                        }
                        //falta delit crio
                        LcDelete();
                        //LcPutChar(table[row][column][sms]);//DEBUG temporal character

                    }
                    state = 0;
                }
            }
			break;
	}
}

void KeEnableSMS() {
    numerical = FALSE;
}

void KeDisableSMS() {
    numerical = TRUE;
}
