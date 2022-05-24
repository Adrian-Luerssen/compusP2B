#include <p18f4321.h>
#include <xc.h>
#include "TMotorADC.h"
#include "TMotorAVG.h"
#include "TEusart.h"

#pragma config OSC = HS // external OSC of 10Mhz
#pragma config PBADEN = DIG
#pragma config MCLRE = OFF
#pragma config DEBUG = OFF
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config WDT = OFF
#pragma config LVP = OFF


void main(void){
	initAVG();
	initSIO();
	initADC();

	while(1){
		//we call all the motors. the order of these does not matter. (if the order of these matters then we have done something wrong)
		motorAVG();
		motorADC();
		// the time it takes to complete one cucle of all of the motors is called cooperative uncertainty.
	}				
}



