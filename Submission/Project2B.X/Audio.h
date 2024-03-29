#ifndef AUDIO_H
#define	AUDIO_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "TTimer.h"
#define AUDIO_ON()          LATCbits.LATC3=1;
#define AUDIO_OFF()         LATCbits.LATC3=0;


void initAudio(void);

void audioMotor(void);

void startSong(void);
//PRE: --
//POST: begins playing the 5 note song, and starts the 60 second timer

void stopSong(void);
//PRE: --
//POST turns the speaker off, resets the 60 second timer, and moves the motor to idle


#endif	/* XC_HEADER_TEMPLATE_H */

