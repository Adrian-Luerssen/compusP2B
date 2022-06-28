#include <pic18f4321.h>
#include <xc.h>
#include "TTIMER.h"
#include "keypad.h"
#include "LcTLCD.h"
#include "Menu.h"
#include "Joystick.h"
#include "SIO.h"
#include "Audio.h"
#include "EEPROM.h"
#include "time.h"
//#include "TPORTB.h"
//#include "TSIO.h"
//#include "TADC.h"

#pragma config OSC = HSPLL //INTIO2
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


void main(void){
    TiInitTimer();
    initJoystick();
	initKeypad();
    LcInit(2,16);
    initSIO();
    initMenu();
    initAudio();
    initTime();
    initData();
	while(1){
        audioMotor();
        SMSMotor();
        KeypadMotor();
        menuMotor();
        joystickMotor();
        dataMotor();
        timeMotor();
        btMotor();
	}				
}


