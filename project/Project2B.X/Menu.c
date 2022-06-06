#include <xc.h>
#include "Menu.h"

typedef struct {
    char username [9];
    char password [9];
} user;

static const char LOGINMENU[2][11]= {"1.LOGIN\0","2.REGISTER\0"};
static const char LOGREGSCREEN [2][6] = {"USER:\0","PSWD:\0"};
static const char MAINMENU[4][28] = {"1.PLAY A GAME\0","2.MODIFY TIME\0","3.SHOW GENERAL TOP 5 SCORES\0","4.LOGOUT\0"};
#define LOGIN 1
#define REGISTER 2
static char timer;
static char LCDrow,LCDcol = 0;
static char val;

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
            if (LCDcol >= 12 && TiGetTics(timer) >= 1000){
                state = 5;
            }else if (isPressed()){
                TiResetTics(timer);
                if (getPresses() == 2){
                    LcGotoXY(LCDcol--,LCDrow);
                }
                if (KeGetGenericValue() != '#'){
                    LcPutChar(KeGetCharValue());
                    LCDcol++;
                    
                    if (val == LOGIN){ //val tells us if we are logging in or registering
                        
                    } else {
                        
                    }
                    //save char
                } else {
                    state = 5;
                }
            }
            
            break;
        case 5: 
            // save \0 in final position
            LCDrow = 1;
            LCDcol = 5;
            KeSetMode(1);
            LcGotoXY(LCDcol,LCDrow);
            state = 6;
            break;
        case 6:   
            if (isPressed()){
                if (getPresses() == 2){
                    LcGotoXY(LCDcol--,LCDrow);
                }
                
                if (KeGetGenericValue() != '#'){
                    LcPutChar(KeGetCharValue());
                    LCDcol++;
                    if (val == LOGIN){//val tells us if we are logging in or registering
                        
                    } else {
                        
                    }
                }
            }
            
            break;
    }
}



void displayMenu (char menuMode,char row){
    if (menuMode == 0){
        if (LOGINMENU[LCDrow+row][LCDcol] != '\0'){
            LcPutChar(LOGINMENU[LCDrow+row][LCDcol]);
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
    }
    
    
}