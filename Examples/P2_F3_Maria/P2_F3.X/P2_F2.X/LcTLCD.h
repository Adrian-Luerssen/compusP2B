#ifndef LCTLCD_H
#define	LCTLCD_H

#include <xc.h>


#define SetD4_D7Sortida()		(TRISBbits.TRISB0 = TRISDbits.TRISD2 = TRISDbits.TRISD1 = TRISDbits.TRISD0 = 0)
#define SetD4_D7Entrada()		(TRISBbits.TRISB0 = TRISDbits.TRISD2 = TRISDbits.TRISD1 = TRISDbits.TRISD0 = 1)
#define SetControlsSortida()    (TRISCbits.TRISC2 = TRISCbits.TRISC1 = TRISCbits.TRISC0 = 0)
#define SetD4(On)				(LATDbits.LATD0 = (On))
#define SetD5(On)				(LATDbits.LATD1 = (On))
#define SetD6(On)				(LATDbits.LATD2 = (On))
#define SetD7(On)				(LATBbits.LATB0 = (On))
#define GetBusyFlag()           (PORTBbits.RB0)
#define RSUp()					(LATCbits.LATC0 = 1)
#define RSDown()				(LATCbits.LATC0 = 0)
#define RWUp()					(LATCbits.LATC1 = 1)
#define RWDown()				(LATCbits.LATC1 = 0)
#define EnableUp()				(LATCbits.LATC2 = 1)
#define EnableDown()                            (LATCbits.LATC2 = 0)
// -------------------------------END--HARDWARE---AREA--------------------


void LcInit(char rows, char columns);
// Pre: Rows = {1, 2, 4}  Columns = {8, 16, 20, 24, 32, 40 }
// Pre: There is a free timer
// Pre: It needs 40ms of tranquility between the VCC raising and this constructor being called.
// Post: This routine can last up to 100ms
// Post: The display remains cleared, the cursor turned off and at the position 0, 0.

void LcEnd(void);
// The destructor

void LcClear(void);
// Post: Clears the display and sets the cursor to its previous state. 
// Post: The next order can last up to 1.6ms. 

void LcCursorOn(void);
// Post: Turn on the cursor
// Post: The next order can last up to 40us. 

void LcCursorOff(void);
// Post: Turns off the cursor
// Post: The next order can last up to 40us. 

void LcGotoXY(char Column, char Row);
// Pre : Column between 0 and 39, row between 0 and 3. 
// Post: Sets the cursor to those coordinates. 
// Post: The next order can last up to 40us. 

void LcPutChar(char c);
// Post: Paints the char in the actual cursor position and increments 
// its position. If the column gets to 39 it returns to 0.
// The row of the LCD is increased when this happens until the second
// row and then it is reset back to row 0 if it has 2 rows total. 
// If the LCD has 4 rows it will reset back to row 0 when it
// reaches row 4 and the columns will go till 39 before reseting to 0.
// The one row LCDs returns to 0 when a column gets to 39. 
// The row is never increased. 
	// The char is written

void LcPutTime(char time);
void LcPutScore(char score);
void LcScroll();


#endif	/* LCTLCD_H */

