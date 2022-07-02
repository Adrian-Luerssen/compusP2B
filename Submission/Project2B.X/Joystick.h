
#ifndef JOYSTICK_H
#define	JOYSTICK_H



#include <xc.h> // include processor files - each processor file is guarded.  
#include "SIO.h"

#define LEFT 10
#define RIGHT 240
#define CENTRE_H 150
#define CENTRE_L 100

void joystickMotor(void);
void initJoystick(void);

char JoMoved(void);
//PRE: --
//POST: returns 1 if joystick has been moved, returns 0 otherwise

char JoDirection(void);
//PRE: JoMoved()
//POST: returns the character for the direction it has moved
//		- UP - 'W'
// 		- DOWN - 'S'
//		- LEFT - 'A'
// 		- RIGHT - 'D'

void JoSetMode(char mode);
//PRE: 0 <= mode <= 1
//POST: mode 1 means the joystick sends its direction through the SIO
// 		mode 0 means the joystick does not
#endif	

