#include <xc.h>
#include "Menu.h"


typedef struct {
    char username [9];
    char password [9];
} user;

static const char LOGINMENU[2][11]= {"1.LOGIN\0","2.REGISTER\0"};
static const char LOGREGSCREEN [2][6] = {"USER:\0","PSWD:\0"};
static const char MAINMENU[4][28] = {"1.PLAY A GAME\0","2.MODIFY TIME\0","3.SHOW GENERAL\0 TOP 5 SCORES\0","4.LOGOUT\0"};
#define LOGIN 1
#define REGISTER 2
static char timer;
static char LCDrow,LCDcol = 0;
static char val;
static User mUser;

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
                    if (DaGetStatus() == LOGIN_CORRECT){
                        //goto main menu
                        state = 9;
                        val =0;
                    } else if (DaGetStatus() == LOGIN_FAILED){
                        KeSetMode(0);
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
            }
            break;
        case 11:
            if (JoMoved()){
                if (JoDirection() == 'S' && val < 4){
                    val++;
                } else if(JoDirection() == 'W' && val > 0){
                    val--;
                }
                state = 9;
                LcClear();
                LCDcol = LCDrow = 0;
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
            if (MAINMENU[LCDrow+row][LCDcol] != '\0'){
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