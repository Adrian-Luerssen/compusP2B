/******** DICTIONARY ********/
//INT0, INT5, INT6, INT7 and INT13 (ones using TIMER0 functions)
char TiGetTimer(void);
//PRE: --
//POST: returns the handler of a free timer and marks it as busy.
//      Returns -1 if there are no free timers.

void TiResetTics(char handler);
//PRE: handler has been returned by TiGetTimer().
//POST: sets the tics of the handler's timer to the universal tics.

unsigned int TiGetTics(char handler);
//PRE: handler has been returned by TiGetTimer();
//POST: returns the amount of tics since the last TiResetTics(handler);


//INT1 Joystick -> Logic
void LoMoveUp(void);
//PRE: --
//POST: Moves the menu or in the game up depending on where the user's at.

void LoMoveDown(void);
//PRE: --
//POST: Moves the menu or in the game down depending on where the user's at.

void LoMoveRight(void);
//PRE: User is playing.
//POST: Moves in game to the right depending on where the user's at.

void LoMoveLeft(void);
//PRE: User is playing.
//POST: Moves in game to the left depending on where the user's at.


//INT2 Logic -> Speaker
void SPPlay(void);
//PRE: --
//POST: starts the 5 seconds - notes melody on the speaker enabling it.


//INT3 Logic -> Bluetooth
void SiSendChar(char my_byte);
//PRE: SIO is available for sending
//POST: Sends buffer using the EUSART

char SiIsAvailable(void);
//PRE: --
//POST: returns 1 if SIO is ready to send, otherwise returns 0

char SiHasReceived(void);
//PRE: --
//POST: returns 1 if has received; otherwise 0.

char SiGetChar(void);
//PRE: Sio has received -> SioHasReceived() == 1
//POST: returns whatever was received.


//INT4 Keyboard -> Logic
void LoGotInput(char input);
//PRE: keyboard has been pressed and debounced.
//POST: gives input to logic


//INT8 Time -> Logic
void LoEndGame(void);
//PRE: game has started and time has finished.
//POST: ends the game and makes Logic.

void LoPlaySound(void);
//PRE: game has started and a minute has passed since last time it was called.
//POST: tells Logic to tell Speaker to start playing the melody again.


//INT9 Logic -> EUSART
void EUSendChar(char my_byte);
//PRE: SIO is available for sending
//POST: Sends buffer using the EUSART

char EUIsAvailable(void);
//PRE: --
//POST: returns 1 if SIO is ready to send, otherwise returns 0


//INT10 Logic -> EEPROM
char readFromROM(void);
//PRE: address is set to correct value.
//POST: reads the value in the previously set address and returns it as a char.

void storeUser(char value, char position);
//PRE: --
//POST: stores the character in the user array in the index of position.

void storePass(char value, char position);
//PRE: --
//POST: stores the character in the password array in the index of position.

void startWriteUser(void);
//PRE: user and password array have been filled.
//POST: writes user info in EEPROM.

char doneWritingROM(void);
//PRE: was writing user info to EEPROM.
//POST: return a 1 if has finished writhing user info; otherwise a 0.

char login(char userInput[8], char passInput[8]);
//PRE: --
//POST: returns, after enough calls, whether or not the user credentials are valid.

char signup(char userInput[8]);
//PRE: --
//POST: returns, after enough calls, whether or not the user can be registered given their credentials.

char getFlag(void);
//PRE: a flag altering function has been called
//POST: return the value of the flag at call-time.

char getNumUsers(void);
//PRE: --
//POST: returns number of registered users.

void startReadingUser(void);
//PRE: --
//POST: sets the address and necessary flags to appropiate values in order to start reading users.

char readUser(void);
//PRE: startReadingUser() has been the last different called function.
//POST: returns the user char by char.

void startReadingScore(void);
//PRE: --
//POST: sets the address and necessary flags to appropiate values in order to start reading scores.

char readScore(void);
//PRE: startReadingScores() has been the last different called function except anotherScore().
//POST: returns the score information char by char.

void anotherScore(void);
//PRE: a score will be read after it.
//POST: prepares address for another score to be read.

void storeScore(char score, char value, char pos);
//PRE: a player has finished a game.
//POST: stores the score and the player info in the EEPROM.


//INT11 Logic -> LCD
void setLine();
//PRE: --
//POST: moves cursor to selected row without moving it on the X axis.

char LcPrintLine(char *c);
//PRE: string is not longer than 255 characters. (length and shift would overflow)
//POST: prints the string on the LCD cooperatively 
//(needs to be called repetitively from motor and control return value)

char LcPrintSecondLine (char * c);
//PRE: string is not longer than 255 characters. (length and shift would overflow)
//POST: prints the string on the second row of the LCD cooperatively. Meant to be used as
//a marquee simultaneously with LcPrintLine() and setLine().
//(needs to be called repetitively from motor and control return value)

void LcDelete(void);
// Pre: A character has been printed in current row.
// Post: Deletes last character and puts cursor back 1 place.

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

void LcGotoXY(char Column, char Row);
// Pre : Column between 0 and 39, row between 0 and 3. 
// Post: Sets the cursor to those coordinates. 
// Post: The next order can last up to 40us. 

void LcClear(void);
// Post: Clears the display and sets the cursor to its previous state. 
// Post: The next order can last up to 1.6ms. 

void LcCursorOn(void);
// Post: Turn on the cursor
// Post: The next order can last up to 40us. 

void LcCursorOff(void);
// Post: Turns off the cursor
// Post: The next order can last up to 40us. 


//INT12 Time -> LCD
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


//INT14 Time -> Speaker
void SPOneSec(void);
//PRE: speaker is enabled
//POST: Changes the note that's playing on the speaker if it is playing