#include <xc.h>
#include "menu.h"

static char key2,keyMenu;

void initMenu(){
   menuReset();
    option3=0;
    option4=1;
    timer_menu=TiGetTimer();
    times_pressed=0;
}
void motorMenu(){
    
    static unsigned char state_menu=0;
    
    switch(state_menu){
        
        case 0:
            if(!PRESSED()){
                MenuDoMarquee(timer_menu,option3,option4,sub_opt);
              
            }else{
                state_menu=1;
                LcClear();
                InitMarquee();
            }
            
            break;
        case 1:
            keyMenu=selectChar(column,row);
           
                 if(!PRESSED()){
                    if(keyMenu=='2'){
                            
                        if(option3<6&&option4<6){
                            option3=option3+1;
                            option4=option4+1;
                            
                        }
                        if(option4==6){
                            option3=5;
                            option4=0;
                        }
                        if(option3==6){
                            option3=0;
                            option4=1;
                        }
                        if(key2=='*'){
                            
                            sub_opt=0;
                            if(times_pressed==9){
                                option3=8;
                                option4=9;
                            }else if(times_8!=0){
                                option3=6;
                                option4=7;
                                
                            }
                            times_8=0;
                        }

                       state_menu=0;
                    }
                    else if(keyMenu=='8'){
                        
                        if((option3>-1&&option4>-1)&&key2!='*'){
                            option3=option3-1;
                            option4=option4-1;
                        }
                        if(option3==-1){
                            option3=5;
                            option4=0;      
                        }
                        if(option4==-1){
                            option3=4;
                            option4=5;      
                        }
                        if(times_pressed==9){
                            sub_opt=1;
                            option3=8;
                            option4=9;      
                        }
                         state_menu=0;
                        if(key2=='*'){
                            
                            times_8=times_8+1;
                            sub_opt=1;
                            if(option3==6){
                                
                                option3=6;
                                option4=7;
                                SMS_Reset();
                            }
                            
                        }
                        
                         state_menu=0;
                    }else if(keyMenu=='*'){
                        
                        if(times_pressed==9){
                            if(sub_opt==0){
                            LcClear();
                            times_pressed=7;
                            
                            }else{
                              
                                times_pressed=17;
                               
                            }
                           state_menu=2; 
                        }else{
                        times_pressed=times_pressed+1;
                        
                        key2=keyMenu;
                        
                        if(times_pressed==1){
                            
                                if(option3==0){
                                    option3=6;
                                    option4=7;
                                     state_menu=0;
                                }else{
                                    
                                    if (option3==1){
                                        userReset();
                                        times_pressed=13;
                                    }else if(option3==2){
                                        userReset();
                                        times_pressed=14;
                                    }else if(option3==3){
                                        times_pressed=16;
                                        rowi=2;
                                    }else if(option3==4){
                                        times_pressed=12;
                                    }else if(option3==5){
                                        times_pressed=10;
                                    }else if(option3==6){
                                        EusReset();
                                        times_pressed=15;
                                    }
                                    state_menu=2;
                                }
                                
                        }else if(times_pressed==2){
                           
                            if(times_8==1){
                                if(option3==6){
                                times_pressed=8;
                                }
                                }
                            else if(times_8==0){
                                if(option3==6){
                                   times_pressed=11;
                                   rowi=2;
                                }
                            }  
                            state_menu=2;
                        }
                        
                        }  
                        }else{
                            state_menu=2;
                          }
                          }
                           
            break;
        case 2:
      
                LcCursorOff();
                state_menu=0; 
                if(times_pressed!=9 ){
                option3=0;
                option4=1;
                }else{
                    option3=8;
                    option4=9;
                    
                }
                menuReset();
                SMS_Reset();
                return_to_main();
                EusReset();
                
            break;
    } 
    
}
void menuReset(){
    TiResetTics(timer_menu);
    times_8=0;
    InitMarquee();  
    key2=0;
    sub_opt=0;
    keyMenu=0;
}
