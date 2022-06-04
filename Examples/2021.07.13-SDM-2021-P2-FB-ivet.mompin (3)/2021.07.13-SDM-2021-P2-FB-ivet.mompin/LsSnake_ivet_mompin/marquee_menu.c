#include "marqueeMenu.h"      

static const char STRINGS[10][30] = {"1. New Game \0",
                                   "2. Show top 5 scores \0",
                                   "3. Show Users \0",
                                   "4. Modify Users \0",
                                   "5. Show Time \0",
                                   "6. Modify Time \0",
                                   "1. Select User \0",
                                   "2. New User \0",
                                   "1. Edit Name \0",
                                   "2. Delete User \0"};

static char chars[10]={12,21,15,13,15,14,13,14,13,15};

void MenuDoMarquee(int timer, char option3, char option4,char sub_opt){
   
    if(chars[option3]>=16){
                if(j[0]<21&&TiGetTics(timer)>=2000){
                    if(sub_opt!=1){
                        marqueePlaceArrow(sub_opt);
                    }
                    LcGotoXY(j[0],r[0]);
                    LcPutChar(STRINGS[option3][k[0]]);

                j[0]=j[0]+1;
                k[0]=k[0]+1;
               
                if(STRINGS[option3][k[0]]=='\0'){
                    k[0]=0;
                    TiResetTics(timer);
                }
                
                }
                if(j[0]>=21){
                    i[0]=i[0]+1;
                    
                    if(STRINGS[option3][i[0]]=='\0'){
                        i[0]=0;
                    }
                    if(sub_opt!=1){
                        marqueePlaceArrow(sub_opt);
                        j[0]=2;
                    }
                    k[0]=i[0];
                }
                }else{
                    if(sub_opt!=1){
                        marqueePlaceArrow(sub_opt);
                    }
                    LcGotoXY(j[0],r[0]);
                    if(STRINGS[option3][k[0]]!='\0'){
                    LcPutChar(STRINGS[option3][k[0]]);
                    j[0]=j[0]+1;
                    k[0]=k[0]+1;
                     
                    }
                   
                }
                 
                if(chars[option4]>=16){
                
                    if(j[1]<21&&TiGetTics(timer)>=2000){
                    if(sub_opt==1){
                        marqueePlaceArrow(sub_opt);
                    }
                    LcGotoXY(j[1],r[1]);
                    LcPutChar(STRINGS[option4][k[1]]);
                    j[1]=j[1]+1;
                    k[1]=k[1]+1;
                
                    if(STRINGS[option4][k[1]]=='\0'){
                        k[1]=0;
                        TiResetTics(timer);
                    }
                    
                    }

                    if(j[1]>=21){
                        i[1]=i[1]+1;
                        
                        if(STRINGS[option4][i[1]]=='\0'){
                            i[1]=0;
                        }if(sub_opt==1){
                           marqueePlaceArrow(sub_opt);
                            j[1]=2;
                        }else{
                            j[1]=0;
                        }
                        k[1]=i[1];
                        
                    }
                    
                }else {
                    if(sub_opt==1){
                        marqueePlaceArrow(sub_opt);
                    }
                    LcGotoXY(j[1],r[1]);
                    if(STRINGS[option4][k[1]]!='\0'){
                    LcPutChar(STRINGS[option4][k[1]]);
                    j[1]=j[1]+1;
                    k[1]=k[1]+1;
                    }
                }   
              
            
}
            

void marqueePlaceArrow(char sub_opt){
            LcGotoXY(0,sub_opt);
            LcPutChar('-');
            LcPutChar('>');
}
void InitMarquee(){
     j[0]=0;
     j[1]=0;
     r[0]=0;
     r[1]=1;
     i[0]=0;
     i[1]=0;
     k[0]=0;
     k[1]=0;

}