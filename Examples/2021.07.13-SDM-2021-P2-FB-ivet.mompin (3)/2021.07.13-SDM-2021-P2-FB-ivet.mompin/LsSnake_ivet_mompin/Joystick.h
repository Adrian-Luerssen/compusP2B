#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <xc.h>
#include "EUSART.h"

    void JoyInit(void);
    char calculate_dist(char num);
    void motorJoyStick(void);
    
#endif