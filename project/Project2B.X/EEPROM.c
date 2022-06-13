

#include <xc.h>
#include "EEPROM.h"
#include "LcTLCD.h"
#define BYTES_PER_USER 24
#define LOGIN_CORRECT 0
#define LOGIN_FAILED 1
#define IN_PROCESS 2
#define REGISTERED_CORRECTLY 3
#define REGISTER_FAILED 4
#define IDLE 5

static char state;
static char userNum;
static char position;
static char status;
static User mUser;
static User users[8];
static Score topScores[5];
static char lastUserPointer;
static char saveScore;

void initData(void) {
    state = 0;
    for (userNum = 0; userNum < 8; userNum++){
        readUserData();
    }
    for (position = 0; position < 10; position = position+2){
        EECON1bits.EEPGD = 0;
        EECON1bits.CFGS = 0;
        EEADR = (SCORE_POSITION)+position;
        EECON1bits.RD = 1;
        topScores[position].score = EEDATA;
        EECON1bits.EEPGD = 0;
        EECON1bits.CFGS = 0;
        EEADR = (SCORE_POSITION)+position+1;
        EECON1bits.RD = 1;
        while (EECON1bits.RD == 1){}
        topScores[position].userNum = EEDATA;
    }
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EEADR = POINTER_POSITION;
    EECON1bits.RD = 1;
    while (EECON1bits.RD == 1){}
    lastUserPointer = EEDATA;
    if (lastUserPointer == 0xFF){
        lastUserPointer = 0;
    }
}

void dataMotor(void){
    switch(state){
        case 0:
            //idle
            break;
        case 1:
            status = IN_PROCESS;
            if (userNum != 8){
                state = 2;
                //LcGotoXY(10,0);
            } else {
                status = LOGIN_FAILED;
                state = 0;
            }
            break;
            
        case 2:
            if (users[userNum].username[position] == mUser.username[position]){
                //LcPutChar(mUser.username[position]);
                if (mUser.username[position] == '\0'){
                    state = 3;
                    LcGotoXY(10,1);
                    position = 0;
                }else {
                    position++;
                }
                
            } else {
                state = 4;
                
            }
            break; 
            
        case 3:
            // if character different state = 0 login unsuccessful

            if (users[userNum].password[position] == mUser.password[position]){
                //LcPutChar(mUser.password[position]);
                if (mUser.password[position] == '\0'){
                    state = 0;
                    status = LOGIN_CORRECT;
                }else {
                    position++;
                }
            } else {
                state = 4;
            }
                // login successful state = 0;
            
            break;
            
        case 4:
            //GOTO NEXT USER IN MEMORY
            position = 0;
            userNum++;
            state = 1;
            break; 
        case 5:
            if (userNum != 8){
                state = 6;
                //LcGotoXY(10,0);
            } else {
                state = 10;
            }
            break;
        case 6:
            if (users[userNum].username[position] == mUser.username[position]){
                //LcPutChar(mUser.username[position]);
                if (mUser.username[position] == '\0'){
                    state = 0;
                    status = REGISTER_FAILED;
                }else {
                    position++;
                }
                
            } else {
                state = 7;
                
            }
            break;
        case 7:
            position = 0;
            userNum++;
            state = 5;
            break;
        case 10:
            EEADR = (lastUserPointer*BYTES_PER_USER)+position;
            EEDATA = mUser.username[position];
            users[lastUserPointer].username[position] = mUser.username[position];
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.WREN = 1;
            INTCONbits.GIE = 0;
            EECON2 = 0x55;
            EECON2 = 0xAA;
            EECON1bits.WR = 1;
            INTCONbits.GIE = 1;
            EECON1bits.WREN = 0;
            state = 11;
            break;
        case 11:
            if(EECON1bits.WR == 0){
                if(mUser.username[position] != '\0'){
                    position++;
                    state = 10;
                } else{
                    position = 0;
                    state = 12;
                }
            }
            break;
        case 12:
            EEADR = (lastUserPointer*BYTES_PER_USER)+position+9;
            EEDATA = mUser.password[position];
            users[lastUserPointer].password[position] = mUser.password[position];
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.WREN = 1;
            INTCONbits.GIE = 0;
            EECON2 = 0x55;
            EECON2 = 0xAA;
            EECON1bits.WR = 1;
            INTCONbits.GIE = 1;
            EECON1bits.WREN = 0;
            state = 13;
            break;
        case 13:
            if(EECON1bits.WR == 0){
                if(mUser.password[position] != '\0'){
                    position++;
                    state = 12;
                } else{
                    position = 0;
                    state = 14;
                }
            }
            break;
        case 14:
            lastUserPointer = (lastUserPointer+1)%8;
            EEADR = POINTER_POSITION;
            EEDATA = lastUserPointer;
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.WREN = 1;
            INTCONbits.GIE = 0;
            EECON2 = 0x55;
            EECON2 = 0xAA;
            EECON1bits.WR = 1;
            INTCONbits.GIE = 1;
            EECON1bits.WREN = 0;
            state = 15;
            break;
        case 15:
            if(EECON1bits.WR == 0){
                status = REGISTERED_CORRECTLY;
                state = 0;
            }
            break;
        case 20:
            if (position == 5){
                if (topScores[status].score < saveScore){
                    topScores[status].score = saveScore;
                    topScores[status].userNum = userNum;
                    state = 21;
                } else{
                    state = 0;
                }
            }else {
                if (topScores[position].score < topScores[status].score){
                    status = position;
                }
                position++;
            }
            break;
        case 21:
            EEADR = SCORE_POSITION + (status*2);
            EEDATA = saveScore;
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.WREN = 1;
            INTCONbits.GIE = 0;
            EECON2 = 0x55;
            EECON2 = 0xAA;
            EECON1bits.WR = 1;
            INTCONbits.GIE = 1;
            EECON1bits.WREN = 0;
            state = 22;
            break;
        case 22:
            if(EECON1bits.WR == 0){
                state = 23;
            }
            break;
        case 23:
            EEADR = SCORE_POSITION + (status*2)+1;
            EEDATA = userNum;
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.WREN = 1;
            INTCONbits.GIE = 0;
            EECON2 = 0x55;
            EECON2 = 0xAA;
            EECON1bits.WR = 1;
            INTCONbits.GIE = 1;
            EECON1bits.WREN = 0;
            state = 24;
            break;
        case 24:
            if(EECON1bits.WR == 0){
                state = 0;
            }
            break;
    }
}


void DaFindUser(User logUser){
    state = 1;
    userNum = 0;
    mUser = logUser;
    status = IN_PROCESS;
    LcGotoXY(10,0);
}

char DagetUserNumber(void){
    return userNum;
}



void DaSaveUser(User regUser){
    state = 5;
    position = 0;
    userNum = 0;
    mUser = regUser;
    status = IN_PROCESS;
}

char DaGetStatus(void){
    return status;
}

char DaGetIdle(void){
    return state == 0;
}

void DaSaveScore(char userScore) {
    saveScore = userScore;
    position = 0;
    status = 0;
    state = 20;
}

void readUserData (void) {
    for (position = 0; position < 9; position++){
        EECON1bits.EEPGD = 0;
        EECON1bits.CFGS = 0;
        EEADR = (userNum*BYTES_PER_USER)+position;
        EECON1bits.RD = 1;
        while (EECON1bits.RD == 1){}
        users[userNum].username[position] = EEDATA;
    }
    for (position = 0; position < 9; position++){
        EECON1bits.EEPGD = 0;
        EECON1bits.CFGS = 0;
        EEADR = (userNum*BYTES_PER_USER)+9+position;
        EECON1bits.RD = 1;
        while (EECON1bits.RD == 1){}
        users[userNum].password[position] = EEDATA;
    }
}