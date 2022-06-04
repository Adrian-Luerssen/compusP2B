#include "queueUser.h"



static char timer_user,key_user;
char tops[5][4];
void initUser(){
    timer_user=TiGetTimer();
    userReset();
    times4=0;
}
void motorUser(){
    static unsigned char state_user=0;
    
    switch(state_user){
        case 0:
                if(times_pressed==14){
                state_user=1;
                }else if(times_pressed==13){
                    state_user=3;
                }
                userReset();
            break;
        case 1:
            DivideScore(users[num_users].score);
            if(PRESSED()){
                state_user=2;
            }else{
                
                marqueeUsers(times_pressed);
                
                
            }
            break;
        
        case 2:
            key_user=selectChar(column,row);
            if(!PRESSED()){
                if(key_user=='#'){
                    state_user=0;
                    times_pressed=0;
                 
                }else{
                    state_user=1;
                }
            }
            break;
        case 3:
           
            if(m<(total_users-1)){
                state_user=4;
                n=m+1;
            }else{
                state_user=1;
                num_users=0;
                pointer=0;
            }
            break;
        case 4:
            if(n<total_users){
                    if(users[n].score > users[m].score){
                        auxiliar = users[n];
                        users[n] = users[m];
                        users[m] = auxiliar;
                    }
                    n=n+1;
                }else{
                state_user=3;
                m=m+1;
                }
            break;
        
    }
}

void marqueeUsers(char times_pressed){
                    if(num_users>=total_users){
                        num_users=0;
                        num_users3=1; 
                    }
        if(j[0]<16){
             LcGotoXY(j[0],r[0]);
            if(TiGetTics(timer_user)>=100){
                LcGotoXY(j[0],r[0]);
                place_char(users[num_users].username[k[0]],0);
                LcGotoXY(j[0],r[1]);
              if(times_pressed==14){
                  place_char(users[num_users3].username[k[0]],1); 
              }else if(times_pressed==13){
                  if(HighScore[k[0]]!='\0'){
                    LcPutChar(HighScore[k[0]]);              
                  }else{
                    PrintData(15-i[0],0,' ',0);
                            PrintData(11-i[0],1,' ',0);
                            PrintData(12-i[0],1,current_score[2],1);
                            PrintData(13-i[0],1,current_score[1],1);
                            PrintData(14-i[0],1,current_score[0],1);
                            PrintData(15-i[0],1,' ',0);
                      
                      
                  }
                  
              }
                TiResetTics(timer_user);
                  j[0]=j[0]+1;
                  k[0]=k[0]+1;
            }
             
        }

        if(j[0]>=16){
           
            PrintData(j[0],0,' ',0);
            PrintData(j[0],1,' ',0);
            
            i[0]=i[0]+1;

            if(i[0]>=16){
                if(times_pressed==13){
                     num_users=num_users+1;
                     
                }else if(times_pressed==14){
                     num_users3=num_users3+1;
                     num_users=num_users+1;
                }
                else if(num_users>5 && times_pressed==13){
                   num_users=0;
                }
                      i[0]=0;
                      k[0]=0;
            }
            
            j[0]=0;          
            k[0]=i[0];
        }
    
}
void userReset(){
    times4=0;
    InitMarquee();
    r[1]=1;
    r[0]=0;
    TiResetTics(timer_user);
    m=0;
    n=1;
}
void PutSpaces(char j){
    if(j<16){
       LcPutChar(' ');  
    }
}
void place_char(char character,char row){
    if(character!='\0'){
         LcPutChar(character);              
    }else{
       PrintData(j[0],row,' ',0);
    }
}