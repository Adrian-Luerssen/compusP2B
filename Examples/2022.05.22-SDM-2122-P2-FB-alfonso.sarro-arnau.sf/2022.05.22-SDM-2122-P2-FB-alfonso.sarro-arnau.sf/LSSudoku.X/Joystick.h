/* 
 * File:   Joystick.h
 * Author: alfon
 *
 * Created on 06 May 2022, 11:00
 */

#ifndef JOYSTICK_H
#define	JOYSTICK_H

#include <xc.h>

void initJoystk(void);
//PRE: --
//POST: initializes all the variables and registers used for the joystick.

void motorJoystk(void);
//PRE: --
//POST: does the ADC conversion and detects if the joystick has moved up or down

#endif	/* JOYSTICK_H */

