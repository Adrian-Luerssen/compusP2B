#include "TData.h"


 char fila,times_2,out,state_data;
 char key_data;
void initData(){
    return_to_main();
    key_data=0;
    for(char m=0;m<20;m++){
        users[m].score=0;
    }
    state_data=0;
}
void motor_data(){
    
    switch(state_data){
        case 0:
            
            if(times_pressed !=17){
            sub_opt=0;
            marqueePlaceArrow(fila);
            num_users=0;
            state_data=1;
            LcGotoXY(rowi,fila);
            key_data=0;
            
            }else{
                state_data=5;
                num_users=user_selected;
            }
            break;
            
        case 1:
            
                user_selected=num_users;
            
                if(users[num_users].username[pointer]!='\0'){
                    
                    LcPutChar(users[num_users].username[pointer]);
                    UpdateCursor(); 
                    LcGotoXY(rowi,fila);
                }else{
                     fila=fila+1;
                     rowi=2;
                     LcGotoXY(rowi,fila);
                     pointer=0;
                     state_data=2;
                    
                }
            break;
        case 2:
            if(users[num_users+1].username[pointer]!='\0'){
                         LcPutChar(users[num_users+1].username[pointer]);
                         UpdateCursor();
                         LcGotoXY(rowi,fila);
                   
            }else{
               state_data=3;
               pointer=0;
            }
            break;
        case 3:
            if(PRESSED()){
                LcClear();
                state_data=4;
            }else{
                //
            }
            break;
        case 4:
         
            key_data=selectChar(column,row);
            
            if(!PRESSED()){
                if(key_data=='#'){
                    return_to_main();
                     times_pressed=0;
                     key_data=0;
                     state_data=0;
                     user_selected=0;
                }else if(key_data=='2'){
                    fila=UpDownUsers(fila);
                    num_users=num_users-1;
                    sub_opt=0;
                    rowi=2;
                    if(num_users==-2){
                        num_users=total_users-1;
                    }
                    state_data=1;
                }
                 else if(key_data=='8'){
                     fila=UpDownUsers(fila);
                     num_users=num_users+1;
                     sub_opt=1;
                     rowi=2;
                      if(num_users==total_users){
                            num_users=0;
                        }
                     
                      state_data=1;
                 }
                
                 else if(key_data=='*'){ 
                     if(times_pressed==11){
                         start();
                         key_data=0;
                     }if(times_pressed==16){
                        
                         times_pressed=9;
                     }
                     return_to_main(); 
                     state_data=0;
                 }
                 
                }
             
            
        break;
        case 5:
            
            if(num_users<=total_users){
                users[num_users]=users[num_users+1];
                num_users=num_users+1;  
            }else{
                total_users=total_users-1;
                state_data=0;
                times_pressed=18;
               
            }
            break;
    }
    
}



int scoreReduce(int score){
    current_score[digit]=(int)(score%10);
    score= (int)(score/10);
    return score;
}
void DivideScore(int score){
        digit=0;
        score=scoreReduce(score);
        digit=digit+1;
        score=scoreReduce(score);
        digit=digit+1;
        score=scoreReduce(score);
        digit=digit+1;
}
void UpdateCursor(){
    pointer=pointer+1;
    rowi=rowi+1;
}
void PrintData(char col,char f, char characters,char yes){
    LcGotoXY(col,f);
    if(yes){
    LcPutChar(characters+'0');
    }else{
        LcPutChar(characters);
    }
}
char UpDownUsers(char fila){
    fila=0;
    marqueePlaceArrow(fila);
    LcGotoXY(2,fila);
    return fila;
}
void start(){
    startGame = 'Y';
    times_pressed=15;
}
void return_to_main(){
    pointer=0;
    fila=0;
    rowi=2;
    num_users=0;
    
}
