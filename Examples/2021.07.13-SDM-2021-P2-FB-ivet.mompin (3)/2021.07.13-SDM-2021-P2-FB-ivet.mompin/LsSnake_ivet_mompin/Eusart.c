#include "EUSART.h"


char data,times3,seconds_aux,timerEusart,count;
char keyEus,stateEus;
char sec[2];
char min[2];

static const char score[10]={"Score: \0"};

void EusInit(){
    SPBRG=64;
    TRISC |= 0xC0; //1100 0000, the 6 LSB are not modified.
	TXSTA = 0b00100100; // 0b00100000 BRG = 0; SYNC = 0;
	RCSTA = 0b10010000;
	BAUDCONbits.BRG16 = 0;
    TRISCbits.TRISC6=1;
    TRISCbits.TRISC7=1;
    
    timerEusart=TiGetTimer();
    TiResetTics(timerEusart);
}

void motorEusart(){
    
    switch(stateEus){
        case 0:
            
            LcClear();
            if(startGame=='Y'){
              if(TXSTAbits.TRMT==1){
                  
                  if(!times3){
                    TXREG=startGame;
                    times3=times3+1;
                    
                  }
                    stateEus=1;
                    digit=0;
                    rowi=0;
                    
              }
              TiResetTics(timerEusart);
            }
            
            break;
        case 1:
            
            LcGotoXY(rowi,0);
            if(users[user_selected].username[rowi]!='\0'){
                LcPutChar(users[user_selected].username[rowi]);
                rowi=rowi+1;
             
            }else if(users[user_selected].username[rowi]=='\0'){
                PrintData(0,1,'T',0);
                PrintData(2,1,min[1],1);
                PrintData(3,1,min[0],1);
                PrintData(4,1,':',0);
                PrintData(5,1,sec[1],1);
                PrintData(6,1,sec[0],1);
                PrintData(8,1,'|',0);
                PrintData(10,1,'S',0);
                PrintData(12,1,current_score[2],1);
                PrintData(13,1,current_score[1],1);
                PrintData(14,1,current_score[0],1);
                  stateEus=2;  
                  rowi=0;
            }
            break;
        case 2:
            if(users[user_selected].username[rowi]!='\0'){
                if(TXSTAbits.TRMT==1){
                    TXREG=users[user_selected].username[rowi];   
                    rowi=rowi+1;
                }
            }else{ 
                  stateEus=3;
                  seconds_aux=0;
                  times3=0;
                  
            }
            break;
        case 3:
           motorSpeaker();
            if (TiGetTics(timerEusart) >= 2000) {
                 
                        times3=0;
                        TiResetTics(timerEusart);
                   
                        seconds_aux='+';
                        
                            sec[0]=sec[0]+1;
                            PrintData(6,1,sec[0],1);
                            if(sec[0]==10){
                                sec[0]=0;
                                sec[1]=sec[1]+1;
                              
                            }if(sec[1]==6){
                                sec[1]=0;
                                min[0]=min[0]+1;
  
                            }
                            if(min[0]==10){
                                min[0]=0;
                                min[1]=min[1]+1;
                                
                            }
                            if(min[1]==6){
                                sec[1]=0;
                                sec[0]=0;
                                min[1]=0;
                                min[0]=0;
                                
                            }
                            PrintData(2,1,min[1],1);
                            PrintData(3,1,min[0],1);
                            PrintData(5,1,sec[1],1);
                            PrintData(6,1,sec[0],1);
                            if(TXSTAbits.TRMT==1){
                               if(!times3){
                                TXREG=(seconds_aux);
                                times3=times3+1;
                                seconds_aux=0;
                               }
                            }
                          
            }else{
                stateEus=4;
                digit=0;
            }
            
        break;   
        case 4:
            if(TXSTAbits.TRMT==1){
                TXREG=direction;
                direction=0;
                stateEus=3;
            }
            else if(PIR1bits.RCIF==1){
                data=(int)(RCREG);
                if(data!=250){
                    if(data<=200){
                        if(data>users[user_selected].score){
                            users[user_selected].score = data;
                            num_users=0;
                           
                        }
                        ScoreDigits(data,12,13,14,1);
                    }
                    
                    stateEus=3;
                    
                }else{
                    stateEus=5;
                    digit=0;
                    rowi=0;
                    
                }
            }else{
                stateEus=3;
            }
        break;
        case 5:
            
            LcGotoXY(rowi,0);
            if(score[rowi]!='\0'){
                LcPutChar(score[rowi]);
                rowi=rowi+1;
            }else if(score[rowi]=='\0'){
                PrintData(7,0,current_score[2],1);
                PrintData(8,0,current_score[1],1);
                PrintData(9,0,current_score[0],1);
                stateEus=6;
                rowi=0;
                
            }
        break;
        case 6:
            LcGotoXY(rowi,1);
            if(HighScore[rowi]!='\0'){
                LcPutChar(HighScore[rowi]);
                rowi=rowi+1;
            }else if(HighScore[rowi]=='\0'){
                ScoreDigits(users[user_selected].score,12,13,14,1);
                stateEus=7;
                LcGotoXY(rowi,1);
                rowi=0;
            }
            break;
        case 7:
            if(PRESSED()){
                stateEus=8;
            }
           
            break;
        case 8:
            
            keyEus=selectChar(column,row);
            if(!PRESSED()){
                
                if(keyEus=='#'){
                    LcClear();
                    times_pressed=0;
                    stateEus=0;
                    InitSpeaker();
                }
            }
            break;
            
    }
}

void EusReset(){
    digit=0;
    rowi=0;
    stateEus=0;
    seconds_aux=0;
    sec[1]=0;
    sec[0]=0;
    min[1]=0;
    min[0]=0;
    current_score[2]=0;
    current_score[1]=0;
    current_score[0]=0;
    startGame=0;
    pointer=0;
    times3=0;
    num_users=0;
    direction=0;
}
void ScoreDigits(char data2, char col1, char col2,char col3,char row){
    DivideScore(data2);
    PrintData(col1,row,current_score[2],1);
    PrintData(col2,row,current_score[1],1);
    PrintData(col3,row,current_score[0],1);
}
