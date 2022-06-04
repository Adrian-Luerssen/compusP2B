#include <xc.h>
#include "Menu.h"

typedef struct {
    char username [9];
    char password [9];
} users;

static const char LOGINMENU[2][11]= {"1.LOGIN\0","2.REGISTER\0"};
static const char LOGREGSCREEN [2][6] = {"USER:\0","PSWD:\0"};
static const char MAINMENU[4][28] = {"1.PLAY A GAME\0","2.MODIFY TIME\0","3.SHOW GENERAL TOP 5 SCORES\0","4.LOGOUT\0"};


static char LCDrow,LCDcol = 0;
static char val;
void menuMotor(void){
    static char state = 0;
    switch (state){
        case 0:
            displayMenu(1,0);
            if (LCDrow == 2){
                LCDrow = 0;
                LCDcol = 5;
                LcGotoXY(LCDcol,LCDrow);
                LcCursorOn();
                state = 1;
            }
            break;
        case 1: 
            if (LCDcol >= 12 && getPresses() <= 1){
                LCDrow = 1;
                LCDcol = 5;
                LcGotoXY(LCDcol,LCDrow);
                state = 2;
            }else if (isPressed()){
                if (getPresses() > 1){
                    LcGotoXY(LCDcol--,LCDrow);
                }
                if (KeGetGenericValue() != '#'){
                    LcPutChar(KeGetCharValue());
                    LCDcol++;
                    //save char
                } else {
                    LCDrow = 1;
                    LCDcol = 5;
                    LcGotoXY(LCDcol,LCDrow);
                    state = 2;
                }
            }
            
            break;
            
        case 2:   
            if (isPressed()){
                if (getPresses() > 1){
                    LcGotoXY(LCDcol--,LCDrow);
                }
                
                if (KeGetGenericValue() != '#'){
                    LcPutChar(KeGetCharValue());
                    LCDcol++;
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