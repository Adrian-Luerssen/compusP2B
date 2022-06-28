#include "Timer.h"
#include "LcTLCD.h"

#define TI_NUMTIMERS 9
#define TI_MAXTICS 30000
#define TI_FALSE 0
#define TI_TRUE 1
#define TMRL 0x73
#define TMRH 0xDF

struct Timer {
	unsigned int h_initialTics;
	unsigned char b_busy;
} s_Timers[TI_NUMTIMERS];

static unsigned int  h_Tics=0;
static char counter;
static char timerPic;
static char timerState;
static char minutes;
static char seconds;
static char show;
void TiInitTimer(void) {
    //Pre: --
    //Post: Initializes the timer0 to interrupt each 1ms.
    RCONbits.IPEN = 0; //No priorities
    INTCONbits.GIE_GIEH = 1;
    INTCONbits.PEIE_GIEL = 1;
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    T0CON = 0x08; //16 bits without prescaler
    TMR0H = TMRH;
    TMR0L = TMRL;
    T0CONbits.TMR0ON = 1; //Start timer

    for (counter=0;counter<TI_NUMTIMERS;counter++) {
	    s_Timers[counter].b_busy=TI_FALSE;
    }
    timerPic = TiGetTimer();
    TiResetTics(timerPic);
    timerState = 0;
    minutes = 0;
    seconds = 0;
}

void _TiRSITimer (void) {
//Timer Interrupt Service Routine
    //@ 40MHz (Tinst = 100nS), we want 1ms/Tinst=10.000 tics 2*16-10.000=0xD8F0
    
    TMR0H = TMRH;
    TMR0L = TMRL;

    INTCONbits.TMR0IF = 0;
    h_Tics++;

    if (h_Tics>=TI_MAXTICS) {
        //We reset them before they overflow (every 30s aprox)
        for (counter=0;counter<TI_NUMTIMERS;counter++){
            if (s_Timers[counter].b_busy==TI_TRUE){
                s_Timers[counter].h_initialTics -= h_Tics;
            }
        }
        h_Tics=0;
    }

}
void TiResetTics(char Handle) {
//Pre: 0<Handle<MAXTIMERS.
//Post: Writes in the tics of the Handle timer the universal tics of the system.

    s_Timers[Handle].h_initialTics=h_Tics;
}
unsigned int TiGetTics(char Handle) {
//Pre: 0<Handle<MAXTIMERS.
//Post: Less than 32767 tics have passed since the last TiResetTics.
//Post: Returns the number of tics from the last TiResetTics for the Handle timer.
    volatile unsigned int actual;
    actual=h_Tics; 
      return (actual-(s_Timers[Handle].h_initialTics));

}
char TiGetTimer(void) {
//Pre: There are free timers.
//Post: Returns the Handler of a timer and marks it as busy.
//Post:	If there are not free timers left, returns a -1.
    counter=0;
    while (s_Timers[counter].b_busy==TI_TRUE) {
        counter++;
	if (counter == TI_NUMTIMERS) return -1;
    }
    s_Timers[counter].b_busy=TI_TRUE;
    return (counter);
}
void TiFreeTimer (char Handle) {
//Pre: 0<Handle<MAXTIMERS.
//Post: The Handle timer is marked as free.
    s_Timers[Handle].b_busy=TI_FALSE;
}

void motorTimer(){
    switch (timerState){
        case 0:
			if(TiGetTics(timerPic) == 1200){
                TiResetTics(timerPic);
                ++timerState;
                if(seconds==59){
                    ++minutes;
                    seconds = 0;
                    
                }
                else{
                    ++seconds;
                }
			}
            break;
        case 1:
            if(show ==1){
                LcGotoXY(0,1);
                LcPutTime(minutes);
                LcPutChar(':');
                LcPutTime(seconds);
            }
            --timerState;
            break;
		
    }
}
void showTimerPic(){
    show = 1;
}
void stopTimerPic(){
    show = 0;
}