#include <xc.h>

#include "Timer.h"
#include "LcTLCD.h"
#include "Keypad.h"
#include "Data.h"
#include "Menu.h"
#include "SIO.h"
#include "Joystick.h"
#include "EEPROM.h"
#include "Music.h"
#include "TX.h"

#pragma config OSC = HSPLL
#pragma config PBADEN = DIG
#pragma config MCLRE = ON
#pragma config DEBUG = OFF
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config WDT = OFF
#pragma config LVP = OFF

char timerGeneral;
void main(void);
void HighInterruptRSI(void);

void __interrupt() RSI_High(void){
	_TiRSITimer();
}

void main(void) {
    timerGeneral = TiGetTimer();
    TiInitTimer();
    initKeypad();
    initData();
    initSIO();
    initJoystick();
    LcInit(2, 16);
    TRISBbits.TRISB6 = 0;
    TRISBbits.TRISB7 = 0;
    initMenu();
    TRISAbits.TRISA3= 0;
    LATAbits.LATA3 = 0;
    TRISAbits.TRISA4= 0;
    LATAbits.LATA4 = 0;
    EeInitAction();
    initMusic();
    initTX();
    TiResetTics(timerGeneral);
    LcClear();
    LcGotoXY(0,0);
    
    while (1){
        menuMotor();
        motorJoystick();
        motorKey();
        motorEeprom();
        motorMusic();
        motorRX();
        motorTX();
        motorTimer();
    }
}