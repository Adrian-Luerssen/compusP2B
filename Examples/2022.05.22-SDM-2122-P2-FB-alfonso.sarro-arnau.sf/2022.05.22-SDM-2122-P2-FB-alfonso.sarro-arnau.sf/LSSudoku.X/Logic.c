#include <xc.h>
#include "Logic.h"
#include "TTimer.h"
#include "Speaker.h"
#include "Bluetooth.h"
#include "Time.h"
#include "Keyboard.h"
#include "LcTLCD.h"
#include "EEPROM.h"

#define STANDBY 0
#define STARTED 1
#define REGISTER 3
#define LOGIN 2

#define USER 0
#define PSWD 1

static char tLogic;
static char gameStarted;
static char key;
static unsigned char state;
static unsigned char position;
static char user[8];
static char pass[8];
static char writing;
static char menuWrite;
static char menuPos;
static char endTime[4];
static char score;
static char printScore[2];
static char flag;
static char scoreFlag;
static char userIndex;
static char skippedUser;
static char bufferSelect;
static char buffer[2][81];
static char readAux[8];

void initLogic(void) {
    tLogic = TiGetTimer();
    gameStarted = STANDBY;
    key = '0';
    state = 0;
    position = 0;
    flag = CMP_BUSY;
//    buffer1[80] = '\0';
//    buffer2[80] = '\0';
}

void LoStartGame(void) {
    gameStarted = STARTED;
}

void LoPlaySound(void) {
    if(gameStarted == STARTED) {
        SPPlay(); //TODO: uncomment
    }
}

void LoEndGame(void) {
    SiSendChar('F');
    TimDisableTime();
    LcClear();
    menuWrite = 0;
    menuPos = 6;
    position = 0;
    state = 16;
    gameStarted = STANDBY;
}

void LoGotKey(char pressed) {
    key = pressed;
    if(state == 5) {
        if(key != '#') {
            if(writing == USER) {
                user[position] = key;
                storeUser(user[position], position);
                position++;
            }
            else {
                pass[position] = key;
                storePass(pass[position], position);
                position++;
            }            
        }
    }
}

void LoMoveUp() {
    if (menuPos == 16) menuPos = 8;
    if(state == 11 || state == 8) {
        menuPos--;
        menuWrite = 0;
        if(menuPos == 5) {
            menuPos = 6;
        }
        LcClear();
        state = menuPos;
    }
    else if(gameStarted == STARTED) {
        if(SiIsAvailable()) {
            SiSendChar('W');
        }
    }
}

void LoMoveDown() {
    if (menuPos == 16) menuPos = 8;
    if(state == 11 || state == 8) {
        menuPos++;
        menuWrite = 0;
        if(menuPos == 11) {
            menuPos = 10;
        }
        LcClear();
        state = menuPos;
    }
    else if(gameStarted == STARTED) {
        if(SiIsAvailable()) {
            SiSendChar('S');
        }
    }
}

void LoMoveLeft() {
    if(gameStarted == STARTED) {
        if(SiIsAvailable()) {
            SiSendChar('A');
        }
    }
}

void LoMoveRight() {
    if(gameStarted == STARTED) {
        if(SiIsAvailable()) {
            SiSendChar('D');
        }
    }
}

void motorLogic(void) {
    switch(state) {
        //MAIN LOGIN/REGISTER MENU
        case 0:
            if(LcPrintLine("1.LOGIN\n") == PRINT_COMPLETED) {state++; LcCursorOff();}
            break;
        case 1:
            if(LcPrintLine("2.REGISTER") == PRINT_COMPLETED) {state++; writing = USER;}
            break;
        case 2:
            if (key == '2' || key == '1') { //SCRATCH RASCA RASCA GANA PREMIO 4 BYTES!!!!!
                state++;
                gameStarted = key - '0' + 1;
                LcClear();
                LcCursorOn();
            }
            /*if(key == '1') { 
                state++;
                gameStarted = LOGIN;
                LcClear();
                LcCursorOn();
            }
            else if(key == '2') {
                state++;
                gameStarted = REGISTER;
                LcClear();
                LcCursorOn();
            }*/
            key = '\0';
             
            break;
        case 3:
            if(LcPrintLine("USER:\n") == PRINT_COMPLETED) state++;
            break;
        case 4:
            if(LcPrintLine("PSWD:\n") == PRINT_COMPLETED) {state++; LcGotoXY(6, 0); KeEnableSMS(); position = 0;}
            break;
        case 5:
            if(position >= 8) {
                key = '#';
            }
            if(key == '#') { 
                key = '\0';
                position = 0;
                if(writing == USER) {
                    writing = PSWD;
                    LcGotoXY(6,1);
                }
                else{
                    LcClear();
                    menuPos = 6;
                    menuWrite = 0;
                    KeDisableSMS();
                    if(gameStarted == REGISTER) {
                        state = 86;
                    }
                    else {
                        //COMPROBAR Q EXISTE USER
                        state = 203;
                        position = 0;
                        LcCursorOff();
                        gameStarted = LOGIN; 
                    }
                    writing = USER; //FOR THE NEXT LOGIN/REGISTER
                    
                }
            }
            
            break;
        case 86:
            if(signup(user) != CMP_BUSY) {
                flag = getFlag();
                if (flag == CMP_EQUAL){
                    state = 100;}
                if (flag == CMP_NOT_EQUAL) {
                    startWriteUser();
                    state = 87;}
            }
            break;
        case 87:
            if(doneWritingROM() == 1) {
                state = 100;//cambio nau
                position = 0;
            }
            break;
        
         //LOGIN
        case 203:
            if (login(user, pass) != CMP_BUSY) {
                flag = getFlag();
                if(flag == CMP_EQUAL) {
                    state = 6;
                    gameStarted = STANDBY;
                }
                else if(flag == CMP_NOT_EQUAL) {
                    state = 100;
                }
            }
            break;
        case 100:
            if(position < 8) {
                user[position] = '\0';
                pass[position] = '\0';
                storeUser('\0', position);
                position++;
            }
            else {
                position = 0;
                if(gameStarted == LOGIN) {
                    if(flag == CMP_EQUAL) {
                        state = 3;
                    }
                    else /*if (flag == CMP_NOT_EQUAL)*/ { //modified mod nau to enable led (free flash): flag already has been controlled to be either equal or not_equal before
                        state = 0;
                    }
                }
                else if(gameStarted == REGISTER) {
                    if(flag == CMP_EQUAL) {
                        state = 0;
                    }
                    else /*if(flag == CMP_NOT_EQUAL)*/ {
                        gameStarted = LOGIN;
                        state = 3;
                    }
                }
            }
            //if(user[position] == )
            break;
        case 6:
            if(LcPrintLine("1.PLAY A GAME\n") == PRINT_COMPLETED) {state++;}
            break;
        case 7:
            if(LcPrintLine("2.MODIFY TIME\n") == PRINT_COMPLETED) {if(menuPos == 7) {state++;} else {state = 11;}}
            break;
        case 8:
            if(menuPos == 7 || menuPos == 16) {
                if (key != '#') {
                    LcPrintLine("3.SHOW GENERAL TOP 5 SCORES \n"); }
                else {
                    state = 11;}   
            }
            else {
                LcGotoXY(0,1);
                state++;
            }
            break;
        case 9:
            if(LcPrintLine("4.LOGOUT\n") == PRINT_COMPLETED) {
                if(menuPos == 9) {state++;}
                else {LcGotoXY(0,0); menuPos = 16; state--;}
            }
            break;
        case 10:
            if(LcPrintLine("7.SHOW ALL USERS\n") == PRINT_COMPLETED) {
                state++;}
            break;
        case 11:
            if(key == '#') {
                key = '\0';
                LcClear();
                if(menuPos == 6) {
                    state++;
                }
                else if(menuPos == 7) {
                    position = 0;
                    state = 26;
                }
                else if(menuPos == 8 || menuPos == 16) { //16 is 8 but with the hack to show marquee properly
                    //KeEnableSMS(); //DEBUGGING SMS
                    position = 0;
                    state = 40; //show top 5 scores
                }
                else if(menuPos == 9) {
                    state = 29;
                }
                else if(menuPos == 10) {
                    position = 0;
                    state = 40;
                    //SHOW ALL USERS
                }
            }            
            break;
        //PLAY GAME
        case 12:
            if(LcPrintLine("TIME REMAINING:\n") == PRINT_COMPLETED) {
                position = 0;
                state++;
            }
            break;
        case 13:
            if(position < 8) {
                if(SiIsAvailable()) {
                    SiSendChar(user[position]);
                    if(user[position] == '\0') {
                        position = 0;
                        if(SiHasReceived()) {
                            state = SiGetChar();
                        }
                        state = 14;
                    }
                    position++;
                }
            }
            else {
                if(SiIsAvailable()) {
                    SiSendChar('\0');
                    position = 0;
                    if(SiHasReceived()) {
                        state = SiGetChar();
                    }
                    state = 14;
                }
            }
            break;
        case 14:
            if(SiHasReceived()) {
                if(SiGetChar() == 'K') {
                    TimEnableTime();
                    gameStarted = STARTED;
                    state++;
                }
            }
            break;
        case 15:
            if(key == '*') {
                key = '\0';
                LoEndGame();
            }
            else if(key != '\0') {
                if(SiIsAvailable()) {
                    SiSendChar(key);
                    key = '\0';
                }
            }
            break;
        case 16:
            if(position < 4) {
                endTime[position] = TimGetEndTime(position);
                position++;
            }
            else {
                position = 0;
                state++;
            }
            break;
        case 17:
            if(SiHasReceived()) {
                score = SiGetChar();
                printScore[1] = score % 10;
                printScore[0] = score / 10;
                key = '\0';
                TiResetTics(tLogic);
                state++; 
            }
            break;
        case 18:
            if(SiHasReceived()) {
                TiResetTics(tLogic);
                LcClear();
                state++;
            }
            else if(TiGetTics(tLogic) >= 3600) {
                LcClear();
                state = 69;
            }
            break;
        case 19:
            if(LcPrintLine("ERRORS:\n") == PRINT_COMPLETED) { 
                state++; 
            }
            break;
        case 20:
            if(SiHasReceived()) {
                LcPutChar(SiGetChar());
                TiResetTics(tLogic);
            }
            else if(TiGetTics(tLogic) >= 1000) {
                LcGotoXY(0,1);
                if(TiGetTics(tLogic) >= 3600) {
                    TiResetTics(tLogic);
                    LcClear();
                    position = 0;
                    state = 69;
                }
            }
            break;
            //STORE SCORE
        case 69:
            if(position < 8) {
                storeScore(score, user[position], position);
                position++;
            }
            else {
                position = 0;
                state = 21;
            }
            break;
        case 21:
            if(LcPrintLine("TIME LEFT: ") == PRINT_COMPLETED) {
                position = 0;
                state++; 
            }
            break;
        case 22:
            if(position < 4) {
                LcPutChar(endTime[position]);
                if(position == 1) {
                    LcPutChar(':');
                }
                position++;
            }
            else {
                position = 0;
                LcGotoXY(0,1);
                state++;
            }
            break;
        case 23:
            if(LcPrintLine("SCORE: ") == PRINT_COMPLETED) { 
                state++; 
            }
            break;
        case 24:
            if(position < 2) {
                LcPutChar(printScore[position] + '0');
                position++;
            }
            else {
                position = 0;
                state++;
            }
            break;
        case 25:
            if(key == '#') {
                LcClear(); 
                key = '\0';
                menuWrite = 0;
                menuPos = 6;
                state = 6;
            }
            break;
        //MODIFY TIME
        case 26:
            if(LcPrintLine("MODIFY TIME:\n") == PRINT_COMPLETED) { 
                state++; 
            }
            break;
        case 27:
            if(key == '#') {
                LcClear(); 
                key = '\0';
                menuWrite = 0;
                menuPos = 6;
                state = 6;
            }
            else{
                if(position < 4) {
                    if(key != '\0') {
                        LcPutChar(key);
                        TimModifyTime(key, position);
                        key = '\0';
                        position++;
                    }
                }
            }
            break;
        //SHOW TOP 5 SCORES
        case 28:
            //check if there is score :?
            anotherScore();
            if (readFromROM() == 0xFF) {
                //NO MORE SCORES
                //SHOW
                state = 43;
            }
            else {
                //MORE SCORES
                state = 45;
            }
            break;
        case 45:
            if (position < 16) {
                if (position < 2) {
                    char aux = readScore();
                    buffer[1][position+(userIndex*16)] = aux;
                    position++;
                }
                else {
                    buffer[1][position+(userIndex*16)] = ' ';
                    position++;
                }
            }
            else {
                position = 0;
                scoreFlag = 0;
                state++;
            }
            break; 
        case 46: //SHOW: maybe can reuse show all scores state?? methinks yes
            if (position < 16) {
                if (position < 8) {
                    char aux = readScore();
                    if (aux == '\0') {
                        if(scoreFlag == 0) {
                            scoreFlag = 1; 
                            aux=':';} 
                        else {
                            aux = ' ';}}
                    buffer[0][position+(userIndex*16)] = aux;
                    position++;
                }
                else {
                    if (scoreFlag == 0) {
                        scoreFlag = 1;
                        buffer[0][position+(userIndex*16)] = ':';
                    } 
                    else {
                        buffer[0][position+(userIndex*16)] = ' ';
                    }
                    position++;
                }
            }
            else {
                position = 0;
                scoreFlag = 0;
                userIndex++;
                state = 28;
            }
            break; 
        case 29:
            if(LcPrintLine("BYE BYE ") == PRINT_COMPLETED) { 
                state++;  
            }
            break;
        case 30:
            if(LcPrintLine(user) == PRINT_COMPLETED) {
                TiResetTics(tLogic);
                state++;  
            }
            break;
        case 31:
            if(TiGetTics(tLogic) >= 2400) {
                LcClear();
                position = 0;
                state++;
            }
            break;
        case 32:
            if(position < 8) {
                user[position] = '\0';
                pass[position] = '\0';
                storeUser('\0', position);
                storePass('\0', position);
                position++;
            }
            else {
                TimResetTime();
                position = 0;
                state = 0;
            }
            break;
        case 40:
            if (position < 81) {
                buffer[0][position] = '\0';
                buffer[1][position] = '\0';
                position++;
            }
            else {
                position = 0;
                userIndex = 0;
                if (menuPos == 10) {//show all users
                    startReadingUser();
                    skippedUser = 0;
                    bufferSelect = 0;
                    state = 49;
                } else if (menuPos == 8 || menuPos == 16) {
                    startReadingScore();
                    state = 28;
                }
            }
            break;
        case 49: //primero
            if (position < 16) {
                if (position < 8) {
                    if(user[position] == '\0') {
                        buffer[bufferSelect][position] = ' ';
                    }
                    else {
                        buffer[bufferSelect][position] = user[position];
                    }
                    position++;
                }
                else { 
                    buffer[bufferSelect][position] = ' ';
                    position++;
                }
            }
            else {
                position = 0;
                if(getNumUsers() > 1) {state = 47;}
                else {state = 44;}
            }
            break;
//        case 48:
//            if (position < 8) {
//                readAux[position] = user[position];
//                position++;
//            } else {
//                state = 41;
//                position = 0;
//            }
//            break;
        case 47:
            if (position < 8) {
                char x = readUser();
                if (x != READ_COMPLETED) {
                    readAux[position] = x;
                    position++;
                }
            }
            else {
                //if (skippedUser == 0) {
                    scoreFlag = compare(readAux, user);
                    if(scoreFlag != CMP_BUSY) {
                        position = 0;
                        if (scoreFlag == CMP_EQUAL) {
                            skippedUser = 1;
                        }
                        else if (scoreFlag == CMP_NOT_EQUAL) {
                            state = 41;
                            bufferSelect = 1 - bufferSelect;
                        }
                    }
//                }
//                else {
//                    position = 0;
//                }
            }
            break;
        case 41:
            if (position < 16) {
                if (position < 8) {
                    if(readAux[position] == '\0') {
                        buffer[bufferSelect][position+((userIndex)*16)] = ' ';
                    }
                    else {
                        buffer[bufferSelect][position+((userIndex)*16)] = readAux[position];
                    }
                    position++;
                }
                else { 
                    buffer[bufferSelect][position+((userIndex)*16)] = ' ';
                    position++;
                }
            }
            else {
                position = 0;
                if (bufferSelect == 1) userIndex++;
                if (userIndex*2+(1-bufferSelect) >= getNumUsers()) {
                    if (bufferSelect == 0) {
                        state = 44;
                    }
                    else {
                        state = 43;
                    } 
                } else {
                    state = 47;
                }
            }
            break;
        case 43:
            //SHOWING SCORES OR ALL USERS
            if (key != '#') {
                setLine();
                LcPrintLine(buffer[0]);
                LcPrintSecondLine(buffer[1]);
            } else {
                LcClear(); 
                key = '\0';
                menuWrite = 0;
                position = 0;
                menuPos = 6;
                state = 6;
            }
            break;
        case 44: 
            if (position < 16) {
                buffer[1][position+(userIndex*16)] = ' ';
                position++;
            } else {state = 43;}
            break;
    }
}
