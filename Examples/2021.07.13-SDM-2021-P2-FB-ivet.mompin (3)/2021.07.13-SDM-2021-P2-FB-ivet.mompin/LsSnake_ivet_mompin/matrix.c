
#include "TMatrix.h"

#define BOUNCES 40
#define MAXROWS 4
#define MAXCOLS 3



static const char TABLE[MAXROWS][MAXCOLS]= {'1','2','3',
                                            '4','5','6',
                                            '7','8','9',
                                            '*','0','#'};
static char timer,key;

void initMatrix(void){
	TRISBbits.TRISB5 = 1;
    TRISBbits.TRISB4= 1;
    TRISBbits.TRISB3 = 1;
    TRISDbits.TRISD3 = 0;
    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD0 = 0;
	timer = TiGetTimer();
    key='#';
}

void motorMatrix(void){

static char state = 0;
switch(state){
    
	case 0:
       
        if (!PRESSED()){
            LATDbits.LATD3=1;
            LATDbits.LATD2=0;
            LATDbits.LATD1=0;
            LATDbits.LATD0=0;
            state=1;
            row=0;
        }else{
            state=4;
        }
        break;
    case 1:
       if (!PRESSED()){
            LATDbits.LATD3=0;
            LATDbits.LATD2=1;
            LATDbits.LATD1=0;
            LATDbits.LATD0=0;
            state=2;
            row=1;
           
        }else{
            state=4;
        }
       break;
    case 2:
        if (!PRESSED()){
            LATDbits.LATD3=0;
            LATDbits.LATD2=0;
            LATDbits.LATD1=1;
            LATDbits.LATD0=0;
            state=3;
            row=2;
        }else{
            state=4;
        }
       break;
    case 3:
        if (!PRESSED()){
            LATDbits.LATD3=0;
            LATDbits.LATD2=0;
            LATDbits.LATD1=0;
            LATDbits.LATD0=1;
            row=3;
            state=0;
        }else{
            state=4;
        }
       break;
    case 4:
        
        TiResetTics(timer);
        state=5;
        break;
        
    case 5:
        if (TiGetTics(timer)>= BOUNCES){
            if(PRESSED()){
                
                state=6;
                
            }else{
                state=0;
            }
        }
        break;
        
    case 6:
        
            if(PORTBbits.RB3){  
                column=0; 
               
            }
            else if(PORTBbits.RB4){
               
                column=1;  
            }
            else if(PORTBbits.RB5){
              
                column=2;  
            }
            state=7;
            key=selectChar(column,row);
            
            if(times_pressed==15){
                if(key=='2'){
                   direction='U';
                }else if(key=='8'){
                   direction='D';
                }else if(key=='4'){
                    direction='L';
                }else if(key=='6'){
                    direction='R';
                }
            }
            //LcPutChar(key);
            
        
            
    break;
    case 7:
        if (!PRESSED()) {
            TiResetTics(timer);
            state=8;
			}
    break;
    case 8:
			if (TiGetTics(timer) >= BOUNCES){
                if(!PRESSED()) {
				state = 0;
                }
                else {
				state = 7;
                }
            }
		break;
}

}
char selectChar(char column, char row){
   
   key = TABLE[row][column];
   return key; 
}
