//DICTIONARY


// INTERFACE 0 (TTimer)
void TiResetTics(char Handle);
//Pre: 0<Handle<MAXTIMERS.
//Post: Writes in the tics of the Handle timer the universal tics of the system.

int TiGetTics(char Handle);
//Pre: 0<Handle<MAXTIMERS.
//Post: Less than 32767 tics have passed since the last TiResetTics.
//Post: Returns the number of tics from the last TiResetTics for the Handle timer.

char TiGetTimer(void);
//Pre: There are free timers.
//Post: Returns the Handler of a timer and marks it as busy.
//Post:	If there are not free timers left, returns a -1.



// INTERFACE 1 (Audio)
void startSong(void);
//PRE: --
//POST: begins playing the 5 note song, and starts the 60 second timer

void stopSong(void);
//PRE: --
//POST turns the speaker off, resets the 60 second timer, and moves the motor to idle



// INTERFACE 2 (Joystick)
char JoMoved(void);
//PRE: --
//POST: returns 1 if joystick has been moved, returns 0 otherwise

char JoDirection(void);
//PRE: JoMoved()
//POST: returns the character for the direction it has moved
//		- UP - 'W'
// 		- DOWN - 'S'
//		- LEFT - 'A'
// 		- RIGHT - 'D'

void JoSetMode(char mode);
//PRE: 0 <= mode <= 1
//POST: mode 1 means the joystick sends its direction through the SIO
// 		mode 0 means the joystick does not



//INTERFACE 3 (Keypad)
char isPressed(void);
//PRE: -- 
//POST: returns 1 if a key has been pressed, returns 0 otherwise

char KeGetCharValue(void);
//PRE: isPressed()
//POST: returns the SMS character corresponding to the key pressed and number of times pressed

char getPresses(void);
//PRE: KeSetMode(1) and isPressed()
//POST: returns <= 1 if a new press is detected on the SMS keypad and >= 2 for a repeating press

char KeGetGenericValue(void);
//PRE: isPressed()
//POST: returns the generic character corresponding to the key pressed

void KeSetMode(char menuMode);
//PRE:  0 <= menuMode <= 1
//POST: mode 1 means the keypad acts as SMS
// 		mode 0 means the keypad acts as a number keypad



//INTERFACE 4 (Serial)
char btAvailable(void);
//PRE: --
//POST: returns 1 if additional serial channel is ready to send, otherwise returns 0

void btSendByte(char byte);
//PRE: btAvailable()
//POST: sends the byte through additional serial channel

void setResetTXRX(void);
//PRE: --
//POST: negates the enable of the additional serial channel

char SiIsAvailable(void);
//PRE: --
//POST: returns 1 if SIO is ready to send, otherwise returns 0

void SiSendChar(char myByte);
//PRE: SiIsAvailable()
//POST: Sends buffer using the EUSART

char SiRecievedByte(void);
//PRE: --
//POST: returns 1 if SIO has received, otherwise returns 0

char SiReadByte(void);
//PRE: SiRecievedByte()
//POST: returns whatever was received.
//WARNING: destructive read



//INTERFACE 5 (EEPROM)
void DaFindUser(User* logUser);
//PRE: DaGetIdle()
//POST: checks if the username and password are correct
//		status can be gotten from DaGetStatus()

char DaGetUserNumber(void);
//PRE: --
//POST: returns the current UserNum

void DaSaveUser(User* regUser);
//PRE: DaGetIdle()
//POST: saves the regUser to the EEPROM if username doesnt exist yet
//		check status to see if successful or not DaGetStatus()

char DaGetStatus(void);
//PRE: --
//POST: returns the most recent status saved in the EEPROM
//		 LOGIN_CORRECT 0
//		 LOGIN_FAILED 1
//		 IN_PROCESS 2
//		 REGISTERED_CORRECTLY 3
//		 REGISTER_FAILED 4

char DaGetIdle(void);
//PRE: --
//POST: returns 1 if in idle state, returns 0 otherwise

void DaSaveScore(char userScore);
//PRE: DaGetIdle()
//POST: saves the userScore to the EEPROM if it is better than a currently saved score

char DaGetNumScores(void);
//PRE: --
//POST: returns the number of scores saved

void incrementPosition(void);
//PRE: --
//POST: increments the position pointer

void displayScoresMarquee(void);
//PRE: DaGetIdle()
//POST: puts score[positon]'s and score[positon+1] score and username into the LCD

void resetPosition(void);
//PRE: --
//POST: sets the position pointer to 0



// INTERFACE 6 (LCD)
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

void LcScroll(void);
//PRE: --
//POST: shifts the display contents to the left