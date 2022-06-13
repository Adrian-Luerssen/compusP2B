#include <xc.h>
#include "Menu.h"


typedef struct {
    char username [9];
    char password [9];
} user;

static const char LOGINMENU[2][11]= {"1.LOGIN\0","2.REGISTER\0"};
static const char LOGREGSCREEN [2][6] = {"USER:\0","PSWD:\0"};
static const char MAINMENU[4][28] = {"1.PLAY A GAME\0","2.MODIFY TIME\0","3.SHOW GENERAL TOP 5 SCORES \0","4.LOGOUT\0"};
static const char MAINMENUSIZE[4] = {14,14,28,9};
#define LOGIN 1
#define REGISTER 2
static char timer;
static char LCDrow,LCDcol,LCDcolm = 0;
static char val;
static User mUser;
static char mScore;
void initMenu(void){
    timer = TiGetTimer();
}


void menuMotor(void){
    static char state = 1;
    switch (state){
        case 1:
            displayMenu(0,0);
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
                    LcClear();
                    LcGotoXY(0,0);
                    KeSetMode(1);
                    LCDcol = LCDrow = 0;
                    state = 3;
                } 
            }
            break;
        
        case 3:
            displayMenu(1,0);
            if (LCDrow == 2){
                LCDrow = 0;
                LCDcol = 5;
                LcGotoXY(LCDcol,LCDrow);
                LcCursorOn();
                state = 4;
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
                    mUser.username[LCDcol-4] = KeGetCharValue();
                    LcPutChar(mUser.username[LCDcol-4]);
                    LCDcol++;
                    
                    
                    //save char
                } else {
                    state = 5;
                }
            }
            
            break;
        case 5: 
            // save \0 in final position
            mUser.username[LCDcol-4] = '\0';
            LCDrow = 1;
            LCDcol = 5;
            KeSetMode(1);
            LcGotoXY(LCDcol,LCDrow);
            state = 6;
            break;
        case 6:   
            if (isPressed()){
                if (getPresses() == 2){
                    LcGotoXY(LCDcol,LCDrow);
                    LCDcol--;
                }
                
                if (KeGetGenericValue() != '#'){
                    mUser.password[LCDcol-4] = KeGetCharValue();
                    LcPutChar(mUser.password[LCDcol-4]);
                    LCDcol++;
                } else {
                    mUser.password[LCDcol-4] = '\0';
                    state = 7;
                }
            }
            
            break;
        case 7:
            if (val == LOGIN){
                DaFindUser(mUser);
                
            } else {
                DaSaveUser(mUser);
            }
            state = 8;
            break;
        case 8:
            if (DaGetIdle()){
                if (val == LOGIN){
                    KeSetMode(0);
                    if (DaGetStatus() == LOGIN_CORRECT){
                        //goto main menu
                        state = 9;
                        val =0;
                    } else if (DaGetStatus() == LOGIN_FAILED){
                        state = 1;
                        //state = 9;
                    }
                } else {
                    if (DaGetStatus() == REGISTERED_CORRECTLY){
                        KeSetMode(1);
                        state = 3;
                        val = LOGIN;
                    } else if (DaGetStatus() == REGISTER_FAILED){
                        KeSetMode(0);
                        state = 1;
                    }
                }
                
                LcClear();
                LcGotoXY(0,0);
                LCDcol = LCDrow = 0;
            }
            break;
        case 9:
            
            displayMenu(2,val);
            if (LCDrow == 2 || LCDrow+val >= 4){
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
            if (TiGetTics(timer) >= TDISPREFRESH){
                LCDcolm++;
                LCDcolm = LCDcolm % MAINMENUSIZE[2];
                state = 9;
                LcClear();
                LCDcol = LCDrow = 0;
            }else if (JoMoved()){
                if (JoDirection() == 'S' && val < 4){
                    val++;
                } else if(JoDirection() == 'W' && val > 0){
                    val--;
                }
                state = 9;
                LcClear();
                LCDcol = LCDrow = 0;
            } else if (isPressed() && KeGetGenericValue() == '#'){
                if (val == 0){
                    val = 0;
                    state = 12;
                }else {
                   state = (val+1)*20; 
                }
                 
                // play game = state 12
                // modify time = state 40
                // show general top 5 scores = state 60
                // logout = state 80
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
                }
            }
            break;
        case 14:
            //update timer view
            displayMenu(3,0);
            if (LCDrow == 2){
                LcCursorOff();
                state = 15;
            }
            break;
        case 15:
            if (isPressed()){
                val = KeGetGenericValue();
                if (val == '*' || val == '#'){
                    if (val == '*'){
                        state = 17;
                    }
                } else {
                    state = 16;
                }
                
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
            displayMenu(3,0);
            if(LCDrow == 2){
                state = 20;
            }
            break;
        case 20:
            if (TiGetTics(timer) >= TSECOND*3){
                state = 22;
                LcClear();
                LCDcol = LCDrow = 0;
                LcGotoXY(0,0);
            }else if (SiRecievedByte()){
                val = SiReadByte();
                state = 21;
            }
            break;
        case 21:
            if (SiRecievedByte()){
                LCDcolm = SiReadByte();
                state = 19;
                LcClear();
                LCDcol = LCDrow = 0;
                LcGotoXY(0,0);
            }
            break;
        case 22:
            displayMenu(4,0);
            if(LCDrow == 2){
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
                LcClear();
                LCDcol = LCDrow = val = LCDcolm = 0;
                LcGotoXY(0,0);
            }
            break;
    }
}



void displayMenu (char menuMode, char row){
    if (menuMode == 0){
        if (LOGINMENU[LCDrow][LCDcol] != '\0'){
            LcPutChar(LOGINMENU[LCDrow][LCDcol]);
            LCDcol++;
        } else {
            LCDrow++;
            LcGotoXY(0,1);
            LCDcol = 0;
        }        
    } else if (menuMode == 1) {
        if (LOGREGSCREEN[LCDrow][LCDcol] != '\0'){
            LcPutChar(LOGREGSCREEN[LCDrow][LCDcol]);
            LCDcol++;
        } else {
            LCDrow++;
            LcGotoXY(0,1);
            LCDcol = 0;
        }
    } else if (menuMode == 2){
        if (LCDrow+row < 4){
            if (MAINMENUSIZE[LCDrow+row] > 16){
                if (LCDcol < 16){
                    LcPutChar(MAINMENU[LCDrow+row][(LCDcol+LCDcolm) % MAINMENUSIZE[LCDrow+row]]);
                    LCDcol++;
                }else {
                    LCDrow++;
                    LcGotoXY(0,1);
                    LCDcol = 0;
                }
              
            }else if (MAINMENU[LCDrow+row][LCDcol] != '\0'){
                LcPutChar(MAINMENU[LCDrow+row][LCDcol]);
                LCDcol++;
            } else {
                LCDrow++;
                LcGotoXY(0,1);
                LCDcol = 0;
            }
        }
           
    }
    
    
}