

//Interface 0 -> Motors to TIMER0

char TiGetTimer(void);
//Pre: There are free timers.
//Post: Returns the Handler of a timer and marks it as busy.
//Post:	If there are not free timers left, returns a -1.
void TiResetTics(char Handle);
//Pre: 0<Handle<MAXTIMERS.
//Post: Writes in the tics of the Handle timer the universal tics of the system.
int TiGetTics(char Handle);
//Pre: 0<Handle<MAXTIMERS.
//Post: Less than 32767 tics have passed since the last TiResetTics.
//Post: Returns the number of tics from the last TiResetTics for the Handle timer.


//Interface 1 -> Motors to MATRIX


char selectChar(column,row);
//Pre: a key has been pressed
//Post: Returns the char that matches the column and the row pressed


//Interface 2 -> Motors to LCD

void LcClear(void);
// Post: Erases the display and sets the cursor to its previous state. 
// Post: The next order can last up to 1.6ms. 

void LcGotoXY(char Column, char Row);
//Pre: A column and a row for the LCD have been obtained
//Post: moves the cursor to the indicated cell in the screen.


//Interface 3 -> Menu to LCD
   
void LcCursorOn(void);
// Post: Turn on the cursor
// Post: The next order can last up to 40us. 

void LcCursorOff(void);
// Post: Turns off the cursor
// Post: The next order can last up to 40us. 

void LcClear(void);
// Post: Erases the display and sets the cursor to its previous state. 
// Post: The next order can last up to 1.6ms. 

void LcGotoXY(char Column, char Row);
//Pre: A column and a row for the LCD have been obtained
//Post: moves the cursor to the indicated cell in the screen.


//Interface 4 -> Menu to Marquee

void MenuDoMarquee(int timer, char option3, char option4,char sub_opt);
//Pre: a timer, the options 3 and 4 and the place of the arrow have been set.
//Post: marquee for the menu is performed at a rate of 1 second.


//Interface 5 Menu -> EUSART

void EusReset();
//Pre: --
//Post: it reinitialises all for the next time we use EUSART


//Interface 6 Menu -> Data

void return_to_main();
//Pre: --
//Post: it reinitialises all for the next time we use data


//Interface 7 Hour -> Data

void PrintData(char col,char f, char characters,char yes);
//Pre: new data has to be printed in the LCD
//Post: prints data in the LCD and adds '0' if needed depending on yes
//1 = needed, 0=not needed.

void UpdateCursor()
//Pre:--
//Post: moves the pointer of the string and the cursor of the LCD to the next
//position.

//Interface 8 QueueUser->Data

void DivideScore(int score);
//Pre: new score has arrived
//Divides the integer value of score into a string of 3 digits 

void PrintData(char col,char f, char characters,char yes);
//Pre: new data has to be printed in the LCD
//Post: prints data in the LCD and adds '0' if needed depending on yes
//1 = needed, 0=not needed.


//Interface 9 EUSART -> Joystick

//There is no concrete function for this interface because direction is a global 
//variable but basically gets the direction of the joystick.

//Interface 10 Menu -> QueueUser

void userReset();
//Pre: --
//Post: it reinitialises all for the next time we use QueueUser


//Interface 11 EUSART-> Speaker

void motorSpeaker();
//Pre:
//Post:performs the music of the game. It should be included in the main.
//Nevertheless, since we want it only to perform before receiving the end number
//of the RCREG, we will include it inside the EUSART.