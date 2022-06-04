#ifndef AUDIO_H
#define	AUDIO_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "TTimer.h"
#define AUDIO_ON()          LATCbits.LATC0=1;
#define AUDIO_OFF()         LATCbits.LATC0=0;


void initAudio(void);

void audioMotor(void);

void startSong(void);

void stopSong(void);

#endif	/* XC_HEADER_TEMPLATE_H */

