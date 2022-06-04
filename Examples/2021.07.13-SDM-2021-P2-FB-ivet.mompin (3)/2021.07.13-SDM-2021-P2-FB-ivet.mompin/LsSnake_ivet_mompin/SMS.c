#include <xc.h>
#include "SMS.h"


static const char SMS[8][7] = {"2abc \0",
                             "3def \0",
                             "4ghi \0",
                             "5jkl \0",
                             "6mno \0",
                             "7pqrs \0",
                             "8tuv \0",
                             "9wxyz \0"};
static const char new[21]={"Enter Name: \0"};
static const char new2[21]={"Enter New Name: \0"};
static const char enterHour[25]={"Enter New Time (HH:MM): \0"};
char key_aux,key_2,final_key,keyGot;
char num_user,stateSMS;
char key_SMS,first;
char timer_SMS;

void initSMS(){
    timer_SMS=TiGetTimer();
    total_users=0;
    num_user=0;
    SMS_Reset();
    digit=0;
  
} 

void motor_SMS(times_pressed){
    
    switch(stateSMS){
        case 0:
            if(times_pressed==8){
                num_user=total_users;
                
            }
            LcCursorOn();
            stateSMS=1;
            pointer=0;
            break;
            
        case 1:
            if(times_pressed!=10){
            LcGotoXY(rowi,0);
            
            if(times_pressed==8){
             SMS_Print(new[pointer]);
            
                if(new[pointer]=='\0'){
                    SMS_PrintReset();
                    stateSMS=2;
                    
                }
            }
            else if(times_pressed==7){
               SMS_Print(new2[pointer]);  
               
                if(new2[pointer]=='\0'){
                    SMS_PrintReset();
                    stateSMS=2;
                   
                }
            }
            }else {
                
               SMS_Print(enterHour[pointer]);
               
                if(enterHour[pointer]=='('){
                    SMS_PrintReset();
                }else if(enterHour[pointer]=='\0'){
                    stateSMS=2;
                    
                }
            }
            
            break;
        
        case 2:
           
                if(PRESSED()){
                    
                    stateSMS=3;
                    if(((key_2!=keyGot)||(TiGetTics(timer_SMS)>=1000))&&!first&&keyGot!='*'&&keyGot!='#'){
                        if(times_pressed==8){
                            users[num_user].username[digit]=final_key;
                         }else if(times_pressed==7){
                            users[user_selected].username[digit]=final_key;
                         }
                        rowi=rowi+1;
                        digit=digit+1;
                        if(digit==10){
                            digit=0;
                            rowi=0;
                        }
                        key_SMS=0;
                        stateSMS=2;
                    }
                    TiResetTics(timer_SMS);
                    LcGotoXY(rowi,1);
                    
                }else{
                  key_2=keyGot;
                    }
                
            break;
        
        case 3:
            keyGot=selectChar(column,row);
            if(first){
                key_2=keyGot;
                first=0;
            }
            if(!PRESSED()){
               key_aux=getKey(keyGot);
                if(keyGot != '#' && keyGot !='*' ){
                  if(keyGot !='0' &&keyGot!='1' && times_pressed!=10){
                      final_key=SMS[key_aux][key_SMS];
                      LcPutChar(final_key);
                      key_SMS=key_SMS+1;
                    if(SMS[key_aux][key_SMS]=='\0'){
                        key_SMS=0;
                    }
                      
                  }else{      
                     LcPutChar(keyGot);
                     final_key=keyGot;
                     
                     if(times_pressed==10){
                            if(rowi==9){
                                hours[1]= (keyGot-'0');
                            }else if(rowi==10){
                                hours[0]= (keyGot-'0');
                                rowi=rowi+1;
                                PrintData(rowi,1,':',0);
                               
                                
                            }else if(rowi==12){
                                minutes[1]=(keyGot-'0');
                            }else if(rowi==13){
                                minutes[0]=(keyGot-'0');
                            }
                            seconds=0;
                     }
                     
                  }
               
                stateSMS=2;
                }else{
                    LcCursorOff();
                    if(keyGot=='*'){
                        if(times_pressed==8){
                            
                            users[num_user].username[digit+1]='\0';
                            total_users=total_users+1;
                            user_selected=num_user;
                            num_user=num_user+1;
                            stateSMS=0;
                            start();
                            
                      
                        }else if(times_pressed==7){
                            users[user_selected].username[digit+1]='\0';
                           
                            stateSMS=1;
                        }else if(times_pressed==10){
                            time[3]=hours[1];
                            time[2]=hours[0];
                            time[1]=minutes[1];
                            time[0]=minutes[0];
                            stateSMS=1;
                        }
                        
                    }else if(keyGot=='#'){
                        LcClear();
                        times_pressed=0;
                        stateSMS=0;                      
                  }  
                    SMS_Reset();
                    SMS_PrintReset();
                }   
                }
    
            break;
            
       
    }
   
}

char getKey(char key){
    static char key_on=0;
            if(key=='2'){
                key_on=0;
            }else if(key=='3'){
                key_on = 1;
            }else if(key=='4'){
                key_on = 2;
            }else if(key=='5'){
                key_on = 3;
            }else if(key=='6'){
                key_on = 4;
            }else if(key=='7'){
                key_on = 5;
            }else if(key=='8'){
                key_on = 6;
            }else if(key=='9'){
                key_on = 7;
            }
        return key_on;
       
}
void SMS_PrintReset(){
    rowi=0;
    first=1;
    LcGotoXY(rowi,1);
    TiResetTics(timer_SMS);
}
void SMS_Print(char chari){
    LcPutChar(chari);
    UpdateCursor();
}
void SMS_Reset(){
    key_SMS=0;
    key_aux=0;
    rowi=0;
    digit=0;
    pointer=0;
    keyGot=0;
    stateSMS=0;
}
