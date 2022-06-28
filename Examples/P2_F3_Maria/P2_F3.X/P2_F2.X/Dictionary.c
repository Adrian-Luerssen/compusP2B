//INT 0
void TiInitTimer(void);
//Pre: --
//Post: Initializes the timer0 to interrupt each 1ms.

void TiResetTics(char Handle);
//Pre: 0<Handle<MAXTIMERS.
//Post: Writes in the tics of the Handle timer the universal tics of the system.

unsigned int TiGetTics(char Handle);
//Pre: 0<Handle<MAXTIMERS.
//Post: Less than 32767 tics have passed since the last TiResetTics.
//Post: Returns the number of tics from the last TiResetTics for the Handle timer.

char TiGetTimer(void);
//Pre: There are free timers.
//Post: Returns the Handler of a timer and marks it as busy.
//Post:	If there are not free timers left, returns a -1.

void TiFreeTimer (char Handle);
//Pre: 0<Handle<MAXTIMERS.
//Post: The Handle timer is marked as free.

void showTimerPic();
//PRE: --
//POST: enables the visualization of the time the pic has been turned on.

void stopTimerPic();
//PRE: --
//POST: disables the visualization of the time the pic has been turned on.

//INT 1
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
//PRE:--
//POST:writes in the LCD the time entered in char format

void LcPutScore(char score);
//PRE:--
//POST:writes in the LCD the score entered in char format

void LcScroll();
//PRE:--
//POST: move the lcd text one position to the left.

//INT 2
void initData();
//PRE: --
//POST: initialises de array of users and the array of scores

//INT 3
void EeInitAction();
//PRE:--
//POST: Initialises de Eeprom for when its needed to read/write.

char getDoneCharging();
//PRE: a read or write has been demanded.
//POST: return a 1 when action is completed.

void writeEeprom(User us, char num);
//PRE: a User and its identifier is provided to persist its data in the Eeprom.
//POST: the data is written in the Eeprom in the corresponding addresses.

void writeEepromScores(TopScore score, char num);
//PRE: a Score and its identifier is provided to persist its data in the Eeprom.
//POST: the data is written in the Eeprom in the corresponding addresses.

void readEeprom(char num);
//PRE: an address has been provided to access to its data of type User.
//POST: the data is read and stored.

void readEepromScores(char num);
//PRE: an address has been provided to access to its data of type Score.
//POST: the data is read and stored.

User getUser(void);
//PRE:  a User read has been demanded.
//POST: returns the variable containing its information.

TopScore getScore(void);
//PRE:  a Score read has been demanded.
//POST: returns the variable containing its information.

char getFirstDirection(char numAddress);
//PRE:  an address has been provided to check the data in it.
//POST: returns  the data in the address.

//INT 4
void initJoystick();
//PRE:--
//POST: initialises the registers and timer needed.

char getPositionJoystick();
//PRE:--
//POST: returns the position of the joystick.

/*Positions:
 * 
 *      W
 * A        D
 *      S
 * 
 */

void resetPositionJoystick();
//PRE:--
//POST: sets the position of the joystick (0).

//INT 5
char TXbusy();
//PRE:--
//POST: returns a 1 if the port TX is free, and a 0 if is busy sending.

char RXdata();
//PRE:--
//POST: returns a 1 if something has been sent to the PIC, and a 0 if it is free

void setTX (char text);
//PRE: TXbusy() == 1
//POST: sends the data 'text'

char getRX();
//PRE:RXdata() == 1
//POST: returns the character received

//INT 6
void initTX();
//PRE:--
//POST: initialises the output and timer needed.

char TXgetState();
//PRE:--
//POST: returns a 1 if the port TX is busy, and a 0 if is free sending.

void sendMyTX(char send);
//PRE:--
//POST: sends the data 'send'

//INT 7
void initKeypad(void);
//PRE:--
//POST: initialises the ports and timer needed

char isPressed();
//PRE:--
//POST: return an int>0 if there has been a pressing

void KeyResetValue(void);
//PRE:--
//POST: sets the value storing the reading to 0

char KeyGetValue(void);
//PRE: isPressed() > 0
//POST: returs the literal value of the keypad

char KeyGetNumber(void);
//PRE: isPressed() > 0
//POST: returs the number pressed

char KeyGetSMS(void);
//PRE: isPressed() > 0
//POST: returs the character pressed in SMS mode

char KeyGetPress();
//PRE:--
//POST: returns int>0 if the SMS mode is rotating

//INT 8
void initMusic();
//PRE:--
//POST: initialises the output and timer needed

void startMusic();
//PRE:--
//POST: starts the sound of the msuic

void endMusic();
//PRE:--
//POST: ends the play of the msuic

void playNote(char note);
//PRE: startMusic()
//POST: toggles the value of the speaker causing the desired frequence.
