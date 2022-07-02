/*
 * File:   keypad.c
 * Author: adril
 *
 * Created on 25 May 2022, 17:04
 */



#include <xc.h>
#include <pic18f4321.h>
#include "keypad.h"

#define TBOUNCES 25
#define PRESSED !(PORTBbits.RB0 && PORTBbits.RB1 && PORTBbits.RB2)
static const char SMSTABLE[9][6] = {"1\0","ABC2\0","DEF3\0","GHI4\0","JKL5\0","MNO6\0","PQRS7\0","TUV8\0","WXYZ9\0"};
static const char TABLE[12]= {'1','2','3','4','5','6','7','8','9','*','0','#'};

static char row;
static char timerBounces,timer_SMS;
static char currentKey;
static char previous;
static char pressed;
static char numPresses;
static char available = 0;
static char state = 0;
static char stateSMS = 0;

void initKeypad(void){
    INTCON2bits.RBPU = 0;
  
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB6 = 0;
    TRISAbits.TRISA4 = 0;
    timerBounces = TiGetTimer();
    timer_SMS = TiGetTimer();
    //LcCursorOn();
}


void KeypadMotor(void){ 
    
    switch (state){
        case 0:
       
            if (!PRESSED){
                LATBbits.LATB3=0;
                LATBbits.LATB4=1;
                LATBbits.LATB5=1;
                LATBbits.LATB6=1;
                state=1;
                row=0;
            }else{
                state=4;
            }
            break;
        case 1:
           if (!PRESSED){
                LATBbits.LATB3=1;
                LATBbits.LATB4=0;
                LATBbits.LATB5=1;
                LATBbits.LATB6=1;
                state=2;
                row=1;

            }else{
                state=4;
            }
           break;
        case 2:
            if (!PRESSED){
                LATBbits.LATB3=1;
                LATBbits.LATB4=1;
                LATBbits.LATB5=0;
                LATBbits.LATB6=1;
                state=3;
                row=2;
            }else{
                state=4;
            }
           break;
        case 3:
            if (!PRESSED){
                LATBbits.LATB3=1;
                LATBbits.LATB4=1;
                LATBbits.LATB5=1;
                LATBbits.LATB6=0;
                row=3;
                state=0;
            }else{
                state=4;
            }
           break;
        case 4:
            state=5;
            TiResetTics(timerBounces);
            break;
        case 5:
            if (TiGetTics(timerBounces) >= TBOUNCES){
                if (!PRESSED){
                    state = 0;
                } else{
                    pressed = 1;
                    state = 6;
                }
            }
            break;
        case 6:
            if (!PRESSED){
                TiResetTics(timerBounces);
                state = 7;
            }
            break;
        case 7:
            if (TiGetTics(timerBounces) >= TBOUNCES){
                if (!PRESSED){
                    pressed = 0;
                    state = 0;
                } else {
                    state = 6;
                }
            }
            break;
    }
}


void SMSMotor(void){
    switch(stateSMS){
        //LATAbits.LATA4 = pressed;
        case 0:
            //LATC = available;
            
            if(pressed){
                stateSMS=1;
                currentKey = KeGetGenericValue();
                available = 2;
                if(numPresses != 0 && (previous != currentKey ||TiGetTics(timer_SMS)>=TSECOND)){
                    //next
                    available = 1; // reset but new value;

                    previous = currentKey;
                    //stateSMS=0;
                    numPresses = 0;
                }


                //TiResetTics(timer_SMS);

            }else{
              previous=currentKey;
            }
            


            break;

        case 1:
            //currentKey = getGenericKey(row,getColumn());
            //available = 1;
            //LcGotoXY(0,0);
            //LcPutChar(KeGetCharValue());
            numPresses++;
            stateSMS = 2;
            break;

        case 2:
            if (!pressed){
                TiResetTics(timer_SMS);

                stateSMS = 0;
            }
            break;
            
        case 3:
            if(pressed){
                stateSMS = 4;
            }
            
            break;
        case 4:
            stateSMS = 5;
            break;
        case 5:
            if (!pressed){

                stateSMS = 3;
            }
            break;
    }

        
   
}


char isPressed(void) {
	//check columns return 1 if pressed
	return pressed && (stateSMS == 1||stateSMS == 4);
}
char getColumn (void){
    if (!PORTBbits.RB0) return 0;
    if (!PORTBbits.RB1) return 1;
    return 2;
}


char KeGetCharValue(void){
    return getFullValue(KeGetGenericValue());
}
char KeGetGenericValue(void){
    return TABLE[(row*3)+getColumn()];
}



char getFullValue(char generic){
    
    if (generic - '0' >= 2 && generic - '0' <= 9){
        if (SMSTABLE[generic - '0' - 1][(numPresses-1)] == '\0'){
            numPresses = 1;
        }
        return SMSTABLE[generic - '0' - 1][(numPresses-1)];
    }
    return generic;
    
}

char getPresses(void) {
    return available;
}


void KeSetMode(char menuMode){
    if (menuMode == 1){
        previous = 0;
        numPresses = 0;
        available = 0; // reset
        stateSMS = 2;
        TiResetTics(timer_SMS); 
    } else if (menuMode == 0){
        stateSMS = 4;
    }
    
}