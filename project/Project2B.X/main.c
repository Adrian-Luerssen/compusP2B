#include <pic18f4321.h>
#include <xc.h>
#include "TTIMER.h"
#include "keypad.h"
#include "LcTLCD.h"
#include "Menu.h"
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
void Initports(void);

void __interrupt() RSI_High(void){
	//void __interrupt() RSI_High(void){
	_TiRSITimer();
}
int tick_count;
//Important: We CANNOT call interrupting functions from our code 
//The returns from the interrupts are different than the returns from the rest of the functions.

//Defining a High Priority Interrupt. 
void InitPorts(void);

void main(void){
    TiInitTimer();
	initKeypad();
    LcInit(2,16);
	while(1){
        //if (MeGetKeypadMode == 0){
            SMSMotor();
        //} else {
            //KeyPadGameMotor();
        //}
        KeypadMotor();
        menuMotor();
	}				
}


