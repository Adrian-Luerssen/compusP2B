#include <xc.h>
#include "TTimer.h"
#include "LcTLCD.h"
#include "Joystick.h"
#include "Keyboard.h"
#include "Time.h"
#include "Bluetooth.h"
#include "EUSART.h"
#include "Speaker.h"
#include "Logic.h"
#include "EEPROM.h"

#pragma config OSC = HSPLL
#pragma config WDT = OFF
#pragma config PBADEN = DIG
#pragma config LVP = OFF
#pragma config DEBUG = OFF

void __interrupt() my_interrupts(void){
    _TiRSITimer();
}

void main(void) {
    initJoystk();
    TiInitTimer();
    LcInit(2,16);
    //LcClear();
    initKeyboard();
    initTimeADT();
    initBluetooth();
    initEUSART();
    initSpeaker();
    initLogic();
    initROM();
    
    //SPPlay();//DEBUG
    TRISAbits.TRISA3 = 0;//DEBUG
    while(1){
        LATAbits.LA3 = 1;//DEBUG
        
        motorJoystk();
        motorKeyboard();
        motorTime();
        motorEUSART();
        motorSpeaker();
        motorLogic();
        motorROM();
    }
    return;
}
