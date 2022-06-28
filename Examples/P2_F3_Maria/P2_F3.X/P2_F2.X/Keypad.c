#include "Keypad.h"

char const textSMS[10][5] =  { //KEY X PRESSED
        {'0', ' ', '%', '%', '%'},
        {'1', '%', '%', '%', '%'},
        {'A', 'B', 'C', '2', '%'},
        {'D', 'E', 'F', '3', '%'},
        {'G', 'H', 'I', '4', '%'},
        {'J', 'K', 'L', '5', '%'},
        {'M', 'N', '0', '6', '%'},
        {'P', 'Q', 'R', 'S', '7'},
        {'T', 'U', 'V', '8', '%'},
        {'W', 'X', 'Y', 'Z', '9'}
    };

char const text[4][3] = { //ROWS X COLUMS
    {1, 2, 3},
    {'*', 0, '#'},
    {7, 8, 9},
    {4, 5, 6}

};


static char timerKeypad;
static char keypadState;
static char rows, cols; 
static char press, rotating;
static char value, valueOld;
static char SMS;

void initKeypad(){
    SetRowsOutput();
    SetColumnsInput();
    ClrRows();
    keypadState = 0;
    timerKeypad = TiGetTimer();
    TiResetTics(timerKeypad);
    press = 0;
    valueOld=10;
    rows=0;
    INTCON2bits.RBPU = 0;
    rotating = 0;
    
}
			
void motorKey(){
    switch(keypadState){    
        case 0:
            rows = (rows+1) % 4;
            switch(rows){
                case 0: 
                    ClrRows();
                    ROW1 = 0;
                    break;
                    
                case 1:
                    ClrRows();
                    ROW2 = 0;
                    break;
                    
                case 2:
                    ClrRows();
                    ROW3 = 0;
                    break;
                    
                case 3:
                    ClrRows();
                    ROW0 = 0;
                    break;                    
            }
            if(TiGetTics(timerKeypad)>ONESEC) KeyResetValue();
            if (Pressed()==1){
                ++keypadState;
                TiResetTics(timerKeypad);
            }
            break;
            
        case 1:
            if(TiGetTics(timerKeypad) > BOUNCES)
                if(Pressed()==1){
                    if(COL0 == 0) cols=0;
                    else if(COL1 == 0) cols=1;
                    else if(COL2 == 0) cols=2;
                    ++keypadState;
                }
                else --keypadState;
            break;
        case 2:
            valueOld = value;
            value = text[rows][cols];
            if(value == valueOld)++press;
            if (value == '#'||value == '*') SMS = value;
            else{
                SMS = textSMS[value][press%5];
                if( SMS == '%'|| press==5){
                    rotating = 2;
                    press=0;
                    SMS = textSMS[value][press];
                }
            }
            ++keypadState;
            break;
        case 3://Wait for release
            if (Pressed()==0){
                TiResetTics(timerKeypad);
                keypadState=0;
            }
            break;
    }
}
void KeyResetValue(void) {
    TiResetTics(timerKeypad);
    SMS = 0;
    press=0;
    rotating = 0;
    valueOld = 10;    
}
char KeyGetValue(void) { 
    return value;
}
char KeyGetSMS(void) { 
    char auxSMS = SMS;
    SMS = 0;
    return auxSMS;
}
char isPressed(){
    return SMS;
}
char KeyGetNumber(void){
    char auxValue = value;
    value = 0;
    return auxValue;
}
char KeyGetPress(){
    return (press + rotating);
}
