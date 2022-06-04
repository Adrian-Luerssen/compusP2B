#include "hour.h"

static char keyHour;
static char timerHour;

static const char showHour[15]={"Current Time: \0"};

void HourInit(){
    minutes[0] = 0;
	minutes[1] = 0;
	hours[0] = 0;
	hours[1] = 0;
	seconds = 0;
	timerHour = TiGetTimer();
    TiResetTics(timerHour);
    rowi=0;
    stateHour=0;
    pointer=0;
}
void motor_hour(char mode){
    
    switch(stateHour){
        case 0:
            
            if(mode!=12){
                stateHour=1;
                
            }else{
                
                LcGotoXY(rowi,0);
                LcPutChar(showHour[pointer]);
                UpdateCursor();
               
                if(showHour[pointer]=='\0'){
                    stateHour=1; 
                    PrintCouples(mode,0,1,1,hours[1],hours[0],1);
                     PrintData(2,1,':',0);
                    PrintCouples(mode,3,4,1,minutes[1],minutes[0],1);
                    
                }else{
                    stateHour=0;
                }
            }
            break;
        case 1:
            if(!PRESSED()){
                if(seconds!=60){
                    if (TiGetTics(timerHour) >= 2000) {
                        
                        TiResetTics(timerHour);
                        seconds=seconds+1;
                       
                    }

                }else{
                    seconds = 0;
                    minutes[0]=minutes[0]+1;
                    if(mode==12){
                    PrintData(4,1,minutes[0],1);
                    }
                    stateHour=2;
                }
            }else{
                
                stateHour=5;
            }
        break;
        
        case 2:
			if (minutes[0] == 10) {
				minutes[0] = 0;
                minutes[1]=minutes[1]+1;
                PrintCouples(mode,4,3,1,0,minutes[1],1);
                stateHour=3;
            }else{
                stateHour=1;
            }
        break;
        case 3:
            if (minutes[1] == 6) {
                minutes[1] = 0;
                hours[0]=hours[0]+1;
                PrintCouples(mode,3,1,1,0,hours[0],1);
                stateHour=4;
            }else{
                stateHour=1;
            }
        break;
        case 4:
            if (hours[0] == 10) {
                  hours[0] = 0;
                  hours[1]=hours[1]+1;
                  PrintCouples(mode,1,0,1,0,hours[1],1);
                  stateHour=1;
            }else{
                stateHour=1;
            }if (hours[0] == 4 && hours[1] == 2) {
                  hours[0] = 0;
                  hours[1] = 0;
                  PrintCouples(mode,0,1,1,0,0,1);
                  stateHour=1;
				}
            
        break;
        case 5:
            keyHour=selectChar(column,row);
            if(!PRESSED()){
                if(keyHour=='#'){
                    if(mode==12){
                        stateHour=0;
                    }else{
                        stateHour=1;
                    }
                    LcClear();
                    times_pressed=0;
                    mode=0;
                }else{
                    stateHour=0;
                }
            }
        break;
        }
      }

void PrintCouples(char mode, char col1, char col2, char row, char character1, char character2, char yes){
    if(mode==12){
          PrintData(col1,row,character1,yes);
          PrintData(col2,row,character2,yes);
     }
}