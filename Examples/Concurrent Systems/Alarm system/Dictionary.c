//dictionary
//incterfaces 2 and 4 (Timer interfaces)

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

//Interface 1

typedef struct {
	char code;
	char dif;
}info;

void QupushByte(char current_dif, char current_channel);
void QupushByte(info my_info);
//pre: !QuFull(); 
//post: saves the byte in the queue, 
//if queue is full deletes the first thing in the queue
// we are not able to process 8 consectutive streams
// two times in a row as it takes 11ms to get an input
// data for each channel, and it takes 16s to output all of them
// to Tout all the values from the Queue

info QuPop(void);
//Pre: the array is not empty
//post: returns an element from the circular queue.
//warning destructive reading.

char QuIsEmpty (void);
//Pre: --
//post: returns 1 if the queue is empty, otherwise 0

char QuFull (void);
//Pre: --
//post: returns 1 if the queue is full, otherwise 0