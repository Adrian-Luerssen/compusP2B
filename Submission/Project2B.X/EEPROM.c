

#include <xc.h>
#include "EEPROM.h"
#include "LcTLCD.h"
//#include "SIO.h"
#define LOGIN_CORRECT 0
#define LOGIN_FAILED 1
#define IN_PROCESS 2
#define REGISTERED_CORRECTLY 3
#define REGISTER_FAILED 4
#define IDLE 5

static char numScores;
static char position;

static char state;
static char userNum;
static char status;
static User* mUser;
static User users[8];
static Score topScores[5];

static char lastUserPointer;
static char saveScore;
static char saveState;
static char wrongState;

void initData(void) {
    state = 0;
    for (userNum = 0; userNum < 8; userNum++){
        readUserData();
    }
    for (position = 0; position < 5;){
        topScores[position].score = readEEPROM((SCORE_POSITION)+position);
        topScores[position].userNum = readEEPROM((SCORE_USER_POSITION)+position);
        position++;
    }
    lastUserPointer = readEEPROM(POINTER_POSITION);
    numScores = readEEPROM(LASTSCORE_POINTER);
    if (lastUserPointer == 0xFF){
        lastUserPointer = 0;
    }
    if (numScores == 0xFF){
        numScores = 0;
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
                //LcClear();
                //LcPutChar(userNum+'0');
                //LcGotoXY(10,0);
            } else {
                status = LOGIN_FAILED;
                state = wrongState;
                position = 0;
            }
            
            break;
            
        case 2:
            
            if (users[userNum].username[position] == (*mUser).username[position]){
                //LcPutChar((*mUser).username[position]);
                if ((*mUser).username[position] == '\0'){
                    state = saveState;
                    position = 0;
                } else {
                    position++;
                }
            } else {
                position = 0;
                userNum++;
                state = 1;
            }    
            break; 
            
        case 3:
            // if character different state = 0 login unsuccessful

            if (users[userNum].password[position] == (*mUser).password[position]){
                //LcPutChar((*mUser).password[position]);
                if ((*mUser).password[position] == '\0'){
                    state = 0;
                    status = LOGIN_CORRECT;
                }else {
                    position++;
                }
            } else {
                
                status = LOGIN_FAILED;
                state = 0;
            }
                // login successful state = 0;
            
            break;
            
        
        /*case 5:
            if (userNum != 8){
                state = 6;
                //LcGotoXY(10,0);
            } else {
                state = 10;
            }
            break;
        case 6:
            if (users[userNum].username[position] == (*mUser).username[position]){
                //LcPutChar((*mUser).username[position]);
                if ((*mUser).username[position] == '\0'){
                    state = 0;
                    status = REGISTER_FAILED;
                }else {
                    position++;
                }
                
            } else {
                state = wrongState;
                
            }
            break;
        case 7:
            position = 0;
            userNum++;
            state = 5;
            break;*/
        case 10:
            
            saveEEPROM((lastUserPointer*16)+position ,(*mUser).username[position]);
            users[lastUserPointer].username[position] = (*mUser).username[position];
            if(position < 7){
                position++;
                saveState = 10;
            } else{
                position = 0;
                saveState = 12;
            }
            state = 11;
            break;
        case 11:
            if(EECON1bits.WR == 0){
                state = saveState;
            }
            break;
        case 12:
            users[lastUserPointer].password[position] = (*mUser).password[position];
            saveEEPROM((lastUserPointer*16)+position+8,(*mUser).password[position]);
            if(position < 7){
                position++;
                saveState = 12;
            } else{
                position = 0;
                saveState = 14;
            }
            state = 11;
            break;
        case 14:
            if (position >= numScores){
                state = 16;
                position = 0;
            } else if (topScores[position].userNum == lastUserPointer){
                topScores[position].userNum = 0xFF;
                topScores[position].score = 0xFF;
                numScores--;
                state = 15;
            }else{
                position++;
            }
            break;
        case 15:
            if (position < numScores){
                topScores[position].userNum = topScores[position+1].userNum;
                topScores[position].score = topScores[position+1].score;
                position++;
            } else {
                position = 0;
                state = 14;
            }
            break;
        case 16:
            if (position < 5){
                saveEEPROM(SCORE_POSITION + position, topScores[position].score);
                saveState = 17;
                state = 11;
            }else {
                state = 18;
            }
            break;
        case 17:
            
            saveEEPROM(SCORE_USER_POSITION + position, topScores[position].userNum);
            saveState = 16;
            state = 11;
            position++;
            
            break;
        case 18:
            
            saveEEPROM(LASTSCORE_POINTER,numScores);

            saveState = 19;
            state = 11;
            break;
        case 19:
            lastUserPointer = (lastUserPointer+1)%8;
            saveEEPROM(POINTER_POSITION,lastUserPointer);
            saveState = 0;
            status = REGISTERED_CORRECTLY;
            state = 11;
            break;
            
        case 20:
            if (numScores < 5){
                status = numScores;
                numScores++;
                saveEEPROM(LASTSCORE_POINTER,numScores);
                
                saveState = 21;
                state = 11;
                
            }else if (position == 5){
                if (topScores[status].score < saveScore){
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
            topScores[status].score = saveScore;
            topScores[status].userNum = userNum;
            state = 22;
            break;
        case 22:
            saveEEPROM(SCORE_POSITION + status,saveScore);
            saveState = 23;
            state = 11;
            break;
        case 23:
            saveEEPROM(SCORE_USER_POSITION + status,userNum);
            saveState = 0;
            state = 11;
            break;
        case 50:
            if (users[topScores[position].userNum].username[status] != '\0'){
                LcPutChar(users[topScores[position].userNum].username[status]);
                status++;
            } else {
                LcPutChar(':');
                state = 51;
                LcGotoXY(0,1);
                status = 0;
            }
            break;
        case 51:
            LcPutChar((topScores[position].score/10)+'0');
            LcPutChar((topScores[position].score%10)+'0');
            LcGotoXY(16,0);
            if (position == numScores){ position = 0;}
            state = 52;
            break;
        case 52:
            saveState = position+1 == numScores? 0:position+1;
            if (users[topScores[saveState].userNum].username[status] != '\0'){
                LcPutChar(users[topScores[saveState].userNum].username[status]);
                status++;
            } else {
                LcPutChar(':');
                state = 53;
                LcGotoXY(16,1);
            }
            break;
        case 53:
            LcPutChar((topScores[saveState].score/10)+'0');
            LcPutChar((topScores[saveState].score%10)+'0');
            state = 0;
            break;
        
    }
}

void displayScoresMarquee(void){
    state = 50;
    status = 0;
}
void DaFindUser(User* logUser){
    state = 1;
    userNum = 0;
    mUser = logUser;
    saveState = 3;
    wrongState = 0;
    status = IN_PROCESS;
    LcGotoXY(10,0);
}

char DagetUserNumber(void){
    return userNum;
}



void DaSaveUser(User* regUser){
    state = 1;
    saveState = 0;
    wrongState = 10;
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
    LcClear();
    LcPutChar((saveScore/10)+'0');
    LcPutChar((saveScore%10)+'0');
    position = 0;
    status = 0;
    state = 20;
}

void readUserData (void) {
    for (position = 0; position < 8; position++){
        users[userNum].username[position] = readEEPROM((userNum*16)+position);
    }
    users[userNum].username[8] = '\0';
    for (position = 0; position < 8; position++){
        users[userNum].password[position] = readEEPROM((userNum*16)+8+position);
    }
    users[userNum].password[8] = '\0';
}

void saveEEPROM(char ad, char data){
    EEADR = ad;
    EEDATA = data;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    INTCONbits.GIE = 1;
    EECON1bits.WREN = 0;
}




char readEEPROM(char address){
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EEADR = address;
    EECON1bits.RD = 1;
    return EEDATA;
}

char DaGetNumScores(void){
    return numScores;
}
void incrementPosition(void){
    position++;
}

void resetPosition(void){
    position = 0;
}