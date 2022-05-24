//Dictionary

//INT0 and INT1 (the ones that use the timer functions)
char TiGetTimer(void);
//PRE: --
//POST returns the handler of a free timer 
//and marks it as busy. 
//Returns -1 if there are no free timers.

void TiResettics(char handler);
//Pre: 
//Post: sets the tics of the handlers timer 
//to the universal tics

unsigned int TiGetTics(char handler);
//pre: handler has been returned by the TiGetTimer();
//post: returns the number of tics of the handlers timer

//INT 2 (from AVG to Display)
char DiStopShow(void);
//pre: --
//post: returns the current value from the 7segments 
//and shows a 0.

//INT 3 (from AVG to EUSART)
char SiIsAvailable(void);
//PRE: --
//POST: returns 1 if SIO is ready to send
// otherwise returns 0

void SiSendChar(char my_byte);
//PRE: SiIsAvailable() == 1
//POST: sends the byte

char SiHasRecieved(void);
//PRE:--
//POST: returns 1 if eusart has recieved. otherwise 0.
char SiGetChar(void);
//pre: SiHasRecieved() == 1
//post returns the first received byte
//WARNING: destructive reading

//INT 4 (from AVG to Circular QUEUE)

void QupushByte(char my_byte);
//pre: -- 
//post: saves the byte in the queue, 
//if queue is full deletes the first thing in the queue

char QuGetFromQueue(void);
//Pre: the array is not empty
//post: returns an element from the circular queue.
//warning destructive reading.

char QuIsEmpty (void);
//Pre: --
//post: returns 1 if the queue is empty, otherwise 0

//INT5  (from AVG to PWM)
void PwResetduty(void);
//pre: --
//post: generates a period with the set duty cycle

//INT 7 (from pushbutton to PWM)
void PwIncrementDuty(void);
//pre: -- 
//post: increments the duty cyle of the PWM by 2ms.
// if it reaches the max time, resets the duty cycle.

//INT 6 
void DisIncrementNumber(void);
//pre: --
//post: increments value on the display. 
//if we are at max, reset the display