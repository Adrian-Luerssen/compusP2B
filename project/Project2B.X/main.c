#include <pic18f4321.h>
#include <xc.h>
#include "TTIMER.h"
#include "keypad.h"
#include "LcTLCD.h"
#include "Menu.h"
#include "Joystick.h"
#include "SIO.h"
//#include "TPORTB.h"
//#include "TSIO.h"
//#include "TADC.h"

#pragma config OSC = HS //INTIO2
#pragma config PBADEN = DIG
#pragma config MCLRE = OFF
#pragma config DEBUG = OFF
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config WDT = OFF
#pragma config LVP = OFF

#define u 1
#define zero 0

void HighInterruptRSI(void);
void main(void);
void InitPorts(void);

void __interrupt() RSI_High(void){
	//void __interrupt() RSI_High(void){
	_TiRSITimer();
}
int tick_count;
char tmr;
//Important: We CANNOT call interrupting functions from our code 
//The returns from the interrupts are different than the returns from the rest of the functions.

//Defining a High Priority Interrupt. 
void InitPorts(void){
    OSCCON = 0x00;
    OSCTUNE = 0x04;
    OSCTUNEbits.PLLEN =1;
    TRISAbits.TRISA3 = 0;
}

void main(void){
    InitPorts();
    TiInitTimer();
    tmr = TiGetTimer();
    initJoystick();
	initKeypad();
    LcInit(2,16);
    initSIO();
	while(1){
        if (TiGetTics(tmr) >= 1000){
            LATA = ~LATA;
            TiResetTics(tmr);
        }
        //if (MeGetKeypadMode == 0){
            SMSMotor();
        //} else {
            //KeyPadGameMotor();
        //}
        KeypadMotor();
        menuMotor();
        joystickMotor();
	}				
}


