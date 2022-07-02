#include <xc.h>
#include "Menu.h"


typedef struct {
    char username [9];
    char password [9];
} user;

static char LOGINMENU[2][11]=           {"1.LOGIN\0","2.REGISTER\0"};
static char LOGREGSCREEN [2][6] =       {"USER:\0","PSWD:\0"};
static const char MAINMENU[5][28] =     {"1.PLAY A GAME\0"
                                        ,"2.MODIFY TIME\0"
                                        ,"3.SHOW GENERAL TOP 5 SCORES \0"
                                        ,"4.LOGOUT\0"
                                        ,"5.SHOW TIME\0"};
static char MAINMENUSIZE[5] =           {14,14,28,9,12};
static char ERROR[9] =                  {"ERRORS: \0"};
static char TIME[16] =                  {"TIME REMAINING: \0"};
static char RESULTS[2][11] =            {"TIME LEFT:\0","SCORE:\0"};
static char LOGOUTMESSAGE[9] =          {"BYE BYE \0"};
static char modifyTimeString[13] =      {"MODIFY TIME:\0"};
static char sysTime[14] =               {"CURRENT TIME:\0"};

#define LOGIN 1
#define REGISTER 2
static char timer;
static char LCDrow,LCDcol,LCDcolm = 0;
static char val;
static char pos;
static User mUser;
static char mScore;
void initMenu(void){
    timer = TiGetTimer();
    val = 0;
}


void menuMotor(void){
    static char state = 1;
    switch (state){
        case 0:
            mUser.username[pos] = '\0';
            mUser.password[pos] = '\0';
            pos++;
            if (val == 9){
                state = 1;
                resetDisplay();
                pos = 0;
            }
        case 1:
            displayString(&LOGINMENU[LCDrow]);
            if (LCDrow == 2){
                LcCursorOff();
                KeSetMode(0);
                state = 2;
            }
            break;
        case 2:
            if (isPressed()){
                val = KeGetGenericValue() - '0';
                if (val == LOGIN||val == REGISTER){
                    resetDisplay();
                    KeSetMode(1);
                    state = 3;
                } 
            }
            break;
        
        case 3:
            displayString(&LOGREGSCREEN[LCDrow]);
            if (LCDrow == 2){
                LCDrow = 0;
                LCDcol = 5;
                LcGotoXY(LCDcol,LCDrow);
                LcCursorOn();
                state = 4;
                pos = 0;
            }
            break;
        case 4: 
            if (LCDcol >= 12 && TiGetTics(timer) >= TSECOND){
                state = 5;
            }else if (isPressed()){
                TiResetTics(timer);
                if (getPresses() == 2){
                    LcGotoXY(LCDcol,LCDrow);
                    LCDcol--;
                }
                if (KeGetGenericValue() != '#'){
                    mUser.username[pos] = KeGetCharValue();
                    LcPutChar(mUser.username[pos]);
                    LCDcol++;
                    pos++;
                    
                    //save char
                } else {
                    state = 5;
                }
            }
            
            break;
        case 5: 
            // save \0 in final position
            mUser.username[pos] = '\0';
            LCDrow = 1;
            LCDcol = 5;
            LcGotoXY(LCDcol,LCDrow);
            state = 6;
            pos = 0;
            break;
        case 6:   
            if (isPressed()){
                if (getPresses() == 2){
                    LcGotoXY(LCDcol,LCDrow);
                    LCDcol--;
                }
                
                if (KeGetGenericValue() != '#'){
                    mUser.password[pos] = KeGetCharValue();
                    LcPutChar(mUser.password[pos]);
                    LCDcol++;
                    pos++;
                } else {
                    mUser.password[pos] = '\0';
                    pos = 0;
                    state = 7;
                }
            }
            
            break;
        case 7:
            if (val == LOGIN){
                DaFindUser(&mUser);
                
            } else {
                DaSaveUser(&mUser);
            }
            state = 8;
            break;
        case 8:
            if (DaGetIdle()){
                if (val == LOGIN){
                    resetDisplay();
                    KeSetMode(0);
                    if (DaGetStatus() == LOGIN_CORRECT){
                        //goto main menu
                        state = 9;
                        val =0;
                    } else if (DaGetStatus() == LOGIN_FAILED){
                        state = 0;
                        //state = 9;
                    }
                } else {
                    if (DaGetStatus() == REGISTERED_CORRECTLY){
                        KeSetMode(1);
                        state = 3;
                        val = LOGIN;
                    } else if (DaGetStatus() == REGISTER_FAILED){
                        KeSetMode(0);
                        state = 0;
                    }
                }
                
                resetDisplay();
            }
            break;
        case 9:
            mainMenu();
            if (LCDrow == 2 || LCDrow+val >= 5){
                LcCursorOff();
                state = 10;
            }
            break;
        case 10:
            if (!JoMoved()){
                state = 11;
                TiResetTics(timer);
            }
            break;
        case 11:
            if (val < 4 && val > 0){ 
                if (TiGetTics(timer) >= TDISPREFRESH){
                    LCDcolm++;
                    LCDcolm = LCDcolm % MAINMENUSIZE[2];
                    state = 9;
                    resetDisplay();
                }
            }else {
                LCDcolm = 0;
            }
            if (JoMoved()){
                if (JoDirection() == 'S' && val < 4){
                    val++;
                } else if(JoDirection() == 'W' && val > 0){
                    val--;
                }
                state = 9;
                resetDisplay();
            } else if (isPressed() && KeGetGenericValue() == '#'){
                resetDisplay();
                state = (val+1)*20;
                resetPosition();
                if (val == 0){
                    state = 12;
                }
                LCDcolm = 0;
                // play game = state 12
                // modify time = state 40
                // show general top 5 scores = state 60
                // logout = state 80
                // show sys time = state 100;
            }
            
            break;
        case 12:
            if (SiIsAvailable()){
                SiSendChar(mUser.username[val]);
                if (mUser.username[val] == '\0'){
                    state = 13;
                } else {
                    val++;
                }
            } 
            break;
        case 13:
            if (SiRecievedByte()){
                val = SiReadByte();
                if (val == 'K'){
                    state = 14;
                    JoSetMode(1);
                    resetDisplay();
                }
            }
            break;
        case 14:
            //update timer view
            displayString(&TIME);
            if (LCDrow == 1){
                LcCursorOff();
                resetTimer();
                startTimer();
                state = 15;
            }
            break;
        case 15:
            if (timerOver()){
                JoSetMode(0);
                state = 17;
            } else if (isPressed()){
                val = KeGetGenericValue();
                if (val == '*' || val == '#'){
                    if (val == '*'){
                        JoSetMode(0);
                        state = 17;
                        stopTimer();
                    }
                } else {
                    state = 16;
                }
                
            } else if (SiRecievedByte()){
                mScore = SiReadByte();
                state = 20;
                TiResetTics(timer);
            }
            break;
        case 16:
            if (SiIsAvailable()){
                SiSendChar(val);
                state = 15;
            }
            break;
        case 17:
            if (SiIsAvailable()){
                SiSendChar('F');
                state = 18;
            }
            break;
        case 18:
            if (SiRecievedByte()){
                mScore = SiReadByte();
                state = 20;
                TiResetTics(timer);
            }
            break;
        case 19:
            displayError();
            if(LCDrow == 2){
                state = 20;
                TiResetTics(timer);
            }
            break;
        case 20:
            if (TiGetTics(timer) >= T3SECOND){
                state = 22;
                resetDisplay();
            }else if (SiRecievedByte()){
                val = SiReadByte();
                state = 21;
            }
            break;
        case 21:
            if (SiRecievedByte()){
                LCDcolm = SiReadByte();
                state = 19;
                resetDisplay();
            }
            break;
        case 22:
            displayResults();
            if(LCDrow == 2){
                LcGotoXY(7,1);
                LcPutChar((mScore/10)+'0');
                LcPutChar((mScore%10)+'0');
                displayTimeRemaining();
                state = 23;
            }
            break;
        case 23:
            if (isPressed() && KeGetGenericValue() == '#'){
                DaSaveScore(mScore);
                state = 24;
            }
            break;
        case 24:
            if (DaGetIdle()){
                state = 9;
                resetDisplay();
                val = LCDcolm = 0;
                
            }
            break;
        case 40:
            displayString(&modifyTimeString);
            if(LCDrow == 1){
                modifyTime();
                state = 41;
            }
            break;
        case 41:
            if (isPressed()){
                if(KeGetGenericValue() == '*' ){
                    resetDisplay();
                    LCDcolm = 0;
                    state = 9;
                    val =0;
                    resetTimer();
                } else if (KeGetGenericValue() == '#'){
                    if (val >= 4){
                        resetDisplay();
                        LCDcolm = 0;
                        state = 9;
                        val =0;
                    }
                } else {
                    val++;
                }
            } 
            break;
            
        case 60:
            if (DaGetNumScores() == 0){
                state = 9;
            }else{
                
                displayScoresMarquee();
                state = 61;
                if (DaGetNumScores() == 1) state+=2;
            }
            
            break;
        case 61:
            if (DaGetIdle()){
                state = 62;
                TiResetTics(timer);
            }
            break;
        case 62:
            state = 63;
            if (TiGetTics(timer) >= TDISPREFRESH){
                CantaIR(BITS_8 | DISPLAY_CONTROL);
                LCDcolm++;
                if (LCDcolm == 16){
                    resetDisplay();
                    LCDcolm = 0;
                    state = 60;
                    incrementPosition();
                }
                TiResetTics(timer);
            }
            
            break;
        case 63:
            if (DaGetNumScores() > 1) state = 62;
            if (isPressed() && KeGetGenericValue() == '#'){
                resetDisplay();
                LCDcolm = 0;
                state = 9;
                val =0;
            }
            break;
            
            
        case 80:
            displayLogOut();
            if (LCDrow == 1){
                state = 81;
                TiResetTics(timer);
            }
            break;
        case 81:
            if (TiGetTics(timer) >= T3SECOND){
                state = 0;
                resetDisplay();
                LCDcolm = 0;
                val =0;
            }
            break;
        case 100:
            displayString(&sysTime);
            if(LCDrow == 1){
                displaySysTime();
                state = 101;
            }
            break;
        case 101:
            if (isPressed() && KeGetGenericValue() == '#'){
                resetDisplay();
                LCDcolm = 0;
                state = 9;
                stopTimer();
                val =0;
            }
            break;
    }
}

void resetDisplay(void){
    LcClear();
    LCDcol = LCDrow = 0;
}

void displayString(char* string){
    if (string[LCDcol+LCDcolm] != '\0'){
        LcPutChar(string[LCDcol+LCDcolm]);
        LCDcol++;
    } else {
        nextRow();
    }
}


void mainMenu(void){
    if (LCDrow+val < 5){
        if (MAINMENUSIZE[LCDrow+val] > 16){
            if (LCDcol < 16){
                LcPutChar(MAINMENU[LCDrow+val][(LCDcol+LCDcolm) % MAINMENUSIZE[LCDrow+val]]);
                LCDcol++;
            }else {
                nextRow();
            }

        }else if (MAINMENU[LCDrow+val][LCDcol] != '\0'){
            LcPutChar(MAINMENU[LCDrow+val][LCDcol]);
            LCDcol++;
        } else {
            nextRow();
        }
    }
}

void displayError(void){
    if (LCDrow == 0){
       if (ERROR[LCDcol] != '\0'){
            LcPutChar(ERROR[LCDcol]);
            LCDcol++;
        } else {
        nextRow();
        } 
    } else {
        LcPutChar(val);
        LcPutChar(LCDcolm);
        LCDrow++;
    }
}
void displayResults(void){
    if (RESULTS[LCDrow][LCDcol] != '\0'){
        LcPutChar(RESULTS[LCDrow][LCDcol]);
        LCDcol++;
    } else {
        nextRow();
    }
}

void displayLogOut(void){
    if (LOGOUTMESSAGE[LCDcol] != '\0'){
        LcPutChar(LOGOUTMESSAGE[LCDcol]);
        LCDcol++;
    } else if (mUser.username[LCDcolm] != '\0' && LCDcolm < 8) {
        LcPutChar(mUser.username[LCDcolm]);
        LCDcolm++;
    } else {
        nextRow();
    }
    
}
void nextRow(void){
    LCDrow++;
    LcGotoXY(0,1);
    LCDcol = 0;
}

