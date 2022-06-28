#include <xc.h>
#include "EUSART.h"
#include "TTimer.h"

#define BUSY 0
#define FREE 1
#define BAUDRATE_TICS 1
#define EUTX LATCbits.LATC4

static char flag;
static char state;
static char vit;
static char tEUSART;
static char index;

void initEUSART() {
    TRISCbits.TRISC4 = 0;
    //TRISCbits.TRISC5 = 1;//SCRATCH MY BACK raski ruski rascar se puede yes we can
    index = 0;
    state = 0;
    flag = FREE;
    EUTX = 1;
    tEUSART = TiGetTimer();
}

void motorEUSART() {
    switch (state) {
        case 0:
            if (flag == BUSY && TiGetTics(tEUSART) >= BAUDRATE_TICS) {
                state++; 
                EUTX = 0;
                TiResetTics(tEUSART);
            }
            break;
        case 1:
            if (TiGetTics(tEUSART) >= BAUDRATE_TICS) {
                TiResetTics(tEUSART);
                EUTX = ((vit & 0x01 << index) != 0);
                index++;
                if (index > 7) state++;
            }
            break;
        case 2:
            if (TiGetTics(tEUSART) >= BAUDRATE_TICS) {
                TiResetTics(tEUSART);
                EUTX = 1;
                state++;
                index = 0;
            }
            break;
        case 3:
            if (TiGetTics(tEUSART) >= BAUDRATE_TICS) {
                state = 0;
                flag = FREE;
            }
            break;
    }
}

char EUIsAvailable(void) {
    return flag;
}

void EUSendChar (char data) {
    vit = data;
    flag = BUSY;
    TiResetTics(tEUSART);
}