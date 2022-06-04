
#include <xc.h>
#include "EUSART.h"
#include "TTimer.h"
#include "LCD.h"
#include "TMatrix.h"
#include "SMS.h"
#include "TData.h"
#include "menu.h"
#include "Joystick.h"
#include "speaker.h"
#include "hour.h"
#include "marqueeMenu.h"
#include "queueUser.h"



#pragma config OSC = HS
#pragma config PBADEN = DIG
#pragma config MCLRE = ON
#pragma config DEBUG = OFF
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config WDT = OFF
#pragma config LVP = OFF


void HighInterruptRSI(void);
void main(void);
void Initports(void);


//Important: We CANNOT call interrupting functions from our code 
//The returns from the interrupts are different than the returns from the rest of the functions.

//Defining a High Priority Interrupt. 

void __interrupt() RSI_High(void){
	//void __interrupt() RSI_High(void){
	_TiRSITimer();
}
//Defining a LowPriority Interrupt.. 
void __interrupt(low_priority) RSI_Low(void) {
	//void __interrupt(low_priority) RSI_Low(void){

	
}

void main(void){
	TiInitTimer();
    initMatrix();
    LcInit(2,16);
    LcCursorOff();
    initSMS();
    initMenu();
    JoyInit();
    initData();
    HourInit();
    EusInit();
    initUser();
    InitSpeaker();
	while(1){
           
           motorMatrix(); 
           motor_hour(times_pressed);
         if(times_pressed==15){
             
              motorJoyStick();
              motorEusart();
          }        
           else if(times_pressed<=2||times_pressed==9){
                motorMenu();
           }  
          else if(times_pressed==8||times_pressed==7||times_pressed==10){   
             
              motor_SMS(times_pressed);   
            }
            else if(times_pressed==11||times_pressed>=16){             
              motor_data(times_pressed);
            }
         
           else if(times_pressed==14||times_pressed==13){
              motorUser();
          }  
	}
}



