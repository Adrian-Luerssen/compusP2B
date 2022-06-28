/*
#include "Menu.h"
#include "LcTLCD.h"
#include "Data.h"
#include "Timer.h"
#include "Keypad.h"
#include "Joystick.h"
#include "EEPROM.h"
#include "Music.h"
#include "TX.h"

static char menuState; //marqueeState;
static char pointer;//, pointerMarquee, auxPM, pointerM;
char pointerUser;
char option;
char keyValue;
static char minutes;
static char seconds;
static char minutesCountdown;
static char secondsCountdown;
static unsigned int timerMenu;
char aux;
char maxScore;
char nameMaxScore[9];
static char gameON;
char error[2];
void initMenu(void){
    timerMenu = TiGetTimer();
	menuState = 100;
	pointer = 0;
	minutes=30;
	seconds=0;
	gameON = 0;
    LcClear();
    LcCursorOn();
    LcGotoXY(0,0);
    maxScore = 0;
    option=0;
    TiResetTics(timerMenu);
    numUsers = 0;
    LATAbits.LATA3 = 0;
    readEeprom(0);
    LATAbits.LATA4 = 0;
    
}
void menuMotor(void){
    switch(menuState){
        
		case 0://IMPRIMIM "1. LOGIN"
            LcPutChar(accessMenuString1[pointer]);
            ++pointer;
            if(pointer == 8){
                ++menuState;
                pointer=0;
                LcGotoXY(0,1);
            }
			break;

		case 1://IMPRIMIM "2. REGISTER"
			LcPutChar(accessMenuString2[pointer]);
			++pointer;
			if(pointer == 11){
				++menuState;
                LcCursorOff();
			}
			break;

		case 2://ESPEREM A QUE HI HAGI UN PRESSING PER CONTINUAR, SI HI HA PRESSING, MIREM SI ES 1 O 2, SI NO ES CAP, IGNOREM
			if(isPressed()){
				option = KeyGetValue();
				pointer = 0;
			}
			//LOGIN
			if(option == LOGIN || option == REGISTER){
                ++menuState;
                LcClear();
                LcGotoXY(0,0); 
                TiResetTics(timerMenu);
			}
			break;

		case 3://IMPRIMIM "USER: "
            LcPutChar(enterUserString[pointer]);
            ++pointer;
            if(pointer == 5){
                LcGotoXY(++pointer,0);
                ++menuState;
                pointer=0;
                KeyResetValue();
            }
			break;

		case 4://IMPRIMIM EL QUE ENS VAN ESCRIVINT PEL KEYPAD (USERNAME)
            if(isPressed()){
                keyValue = KeyGetSMS();
                if(KeyGetPress() > 1){
                    LcGotoXY(--pointer+6 ,0);
                }
                if(keyValue!='#' && pointer<8){
                    LcPutChar(keyValue);
                    users[numUsers].username[pointer]=keyValue;
                    ++pointer;
                    TiResetTics(timerMenu);
                }
                else{
                    users[numUsers].username[pointer]='\0';
                    LcGotoXY(0,1);
                    pointer = 0;
                    menuState++;
                }
            }
			break;            

		case 5://IMPRIMIM "PSWD: "
			LcPutChar(enterPasswordString[pointer]);
			++pointer;
			if(pointer == 5){
				LcGotoXY(++pointer,1);
				++menuState;
				pointer=0;
                TiResetTics(timerMenu);
                KeyResetValue();
			}
			break;

		case 6://IMPRIMIM EL QUE ENS VAN ESCRIVINT PEL KEYPAD (PASSWORD)
            if(isPressed()){
                keyValue = KeyGetSMS();
                if(keyValue!='#'){
                    LcPutChar(keyValue);
                    users[numUsers].password[pointer]=keyValue;
                    ++pointer;
                    TiResetTics(timerMenu);
                }else{
                    users[numUsers].password[pointer]='\0';
                    pointer = 0;
                    if(option == LOGIN){//ENS ANEM A COMPROVAR LOGIN
                        ++menuState;
                        pointerUser=0;
                    }else{
                        pointerUser=0;
                        menuState = 9;
                    }
                }
            }
			break;
            
		case 7://COMPROVEM LOGIN
            if(numUsers!=pointerUser)
                if(!(users[pointerUser].username[pointer]==users[numUsers].username[pointer])){//NO ESTA REGISTRAT
                    ++pointerUser;
                    if(pointerUser==numUsers) menuState=0;
                    else pointer = 0;
                }
                else
                    if(users[pointerUser].username[pointer]==users[numUsers].username[pointer])//DE MOMENT SONT IGUALS
                        if(users[numUsers].username[pointer]=='\0'){//DEFINITIU IGUALS -> COMPROVEM CONTRASEÑA
                            pointer = 0;
                            ++menuState;
                        }
                        else ++pointer;
                    
            else{
                menuState=0;
                pointer=0;
                option = 0;
            }
			break;
        
        case 8:
            if(!(users[pointerUser].password[pointer]==users[numUsers].password[pointer])) menuState=0;
			else
				if(users[pointerUser].password[pointer]==users[numUsers].password[pointer])//DE MOMENT SONT IGUALS
					if(users[numUsers].password[pointer]=='\0'){//DEFINITIU IGUALS -> L'USUARI POT ACCEDIR
                        currentUser = pointerUser;
						LcClear();
						LcGotoXY(0,0);
						pointer = 0;
						menuState=10;
                        option = 0;
					}
					else ++pointer;
			break;
        
		case 9://COMPROVEM REGISTER
            if(numUsers!=pointerUser){
                if(!(users[numUsers].username[pointer]==users[pointerUser].username[pointer])){//NO ESTA REGISTRAT
                    ++pointerUser;
                    if(pointerUser==numUsers){
                        menuState = 99;
                    }
                    else pointer = 0;
                }
                else
                    if(users[numUsers].username[pointer]==users[pointerUser].username[pointer])//DE MOMENT SONT IGUALS
                        if(users[numUsers].username[pointer]=='\0') menuState = 0;//DEFINITIU IGUALS -> L'USUARI NO POT ACCEDIR
                        else ++pointer;
            }else{
                menuState = 99;
            }
			break;
            
        case 99:
            writeEeprom(users[numUsers],numUsers);
            ++numUsers;
            LcClear();
            option = LOGIN;
            pointer = 0;
            LcGotoXY(0,0);
            menuState = 3;
            break;

		//L'USUARI HA ACCEDIT -> IMPRIMIMT TOT EL MENU
		case 10:
            LcPutChar(menuStrings[option][pointer]);
            ++pointer;
            if(pointer == 16){
                pointer=0;
                LcGotoXY(0,1);
                ++option;
                ++menuState;
                if(option==5) menuState = 12;
            }
            
			break;
            
		case 11:
            LcPutChar(menuStrings[option][pointer]);
			++pointer;
            if(pointer == 28){
                ++menuState;
                TiResetTics(timerMenu);
            }
			break;
            
		case 12:
            if(TiGetTics(timerMenu) > 500){
                if(isPressed()){
                    if(KeyGetValue()== '#'){
                        if(option==1){//OPTION 1 SELECTED
                            menuState = 13;
                        }else if(option==2){//OPTION 2 SELECTED
                            KeyResetValue();
                            menuState = 27;
                            LcClear();
                            LcGotoXY(0,0);
                            pointer=0;
                        }else if(option==3){//OPTION 3 SELECTED
                            aux = 255;
                            pointerUser=0;
                            pointer=0;
                            //menuState = 25;
                        }else if(option==4){//OPTION 4 SELECTED
                            aux = 0;
                            pointerUser=currentUser;
                            pointer=0;
                            //menuState;
                        }else if(option==5){//OPTION 5 SELECTED
                            LcClear();
                            LcGotoXY(0,0);
                            pointer=0;
                            aux = 0;
                            menuState = 35;
                        }
                        pointer=0;
                    }
                }else if(getPositionJoystick() == 'S'){//MOVEM EL MENU ABAIX
                    menuState = 10;
                    pointer=0;
                    LcClear();
                    LcGotoXY(0,0);
                    if(option==5) --option;
                }else if(getPositionJoystick() == 'W'){//MOVEM EL MENU ADALT
                    menuState = 10;
                    pointer=0;
                    LcClear();
                    LcGotoXY(0,0);
                    if(option==1) --option;
                    else option-=2; 
                }
            }
			
			break;

		//OPTION 1 SELECTED 
		case 13: //SEND NAME USER
			if(users[pointerUser].username[pointer] != '\0'){
				if(TXbusy()==1){//PODEM ENVIAR
                    //sendMyTX(users[pointerUser].username[pointer]);
					setTX(users[pointerUser].username[pointer]);
					++pointer;
				}
            }
            else{
                pointer =0;
                ++menuState;
            }
            break;
            
        case 14:
            if(getRX()=='K'){
                //sendMyTX('K');
                setTX('K');
                minutesCountdown = minutes;
                secondsCountdown = seconds;
                LcClear();
                LcGotoXY(0,0);
                ++menuState;
                gameON = 1;
                pointer =0;
                startMusic();
            }
            break;
            
		case 15:
			LcPutChar(timeRemainingString[pointer]);
            ++pointer;
            if(pointer > 14){
                ++menuState;
                pointer=0;             
                
			}
            break;
		case 16:
            LcGotoXY(0,1);
			TiResetTics(timerMenu);
            LcPutTime(minutesCountdown);
            LcPutChar(':');
            LcPutTime(secondsCountdown);
			++menuState;
			break;

		case 17:
			if(TiGetTics(timerMenu) == 1200){
                if(secondsCountdown==0){
                    --minutesCountdown;
                    if(minutesCountdown==0 && secondsCountdown==0){
                        ++menuState;
                    }
                    else{
                        secondsCountdown= 59;
                    }
                }
                else{
                    --secondsCountdown;
                }
                LcGotoXY(0,1);
				--menuState;
			}
			if(KeyGetValue()== '*'){//END OF THE SUDOKU GAME
                ++menuState;
			}
			break;
            
		case 18:
            if(TXbusy()==1){
                endMusic();
                //sendMyTX('F');
                setTX('F');
                TiResetTics(timerMenu);
                ++menuState; 
            }
			break;	
                
        case 19:
			if(RXdata()==1){
                aux = getRX();
                TiResetTics(timerMenu);
                ++menuState;
            } //missing write to eeprom
            break;
            
        case 20:
            if (TiGetTics(timerMenu) < 3*ONESEC){
                if(RXdata()==1){
                    LcClear();
                    error[0] = getRX();
                    ++menuState;
                    TiResetTics(timerMenu);
                }
            }else{
                pointer = 0;
                LcClear();
                LcGotoXY(0,0);
                menuState = 23;
            }
            break;
        case 21:
            if(RXdata()==1){
                error[1] = getRX();
                pointer = 0;
                LcClear;
                LcGotoXY(0,0);
                menuState = 34;
            }
            break;
            
        case 22:
            LcPutChar(errorsString[pointer]);
            ++pointer;
            if(pointer == 7){
                writeEeprom(users[pointerUser],pointerUser);
                menuState = 20;
                pointer=0;
                LcGotoXY(0,1);
                LcPutChar(error[0]);
                LcPutChar(error[1]);
                TiResetTics(timerMenu);
            }
			break;
            
        case 23:
            LcPutChar(timeLeftString[pointer]);
            ++pointer;
            if(pointer == 10){
                ++menuState;
                LcGotoXY(++pointer,0);                
            }
			break;
            
        case 24:
            LcPutTime(minutesCountdown);
            LcPutChar(':');
            LcPutTime(secondsCountdown);
            LcGotoXY(0,1);
            pointer = 0;
            ++menuState;
            break;
            
        case 25:
            LcPutChar(scoreString[pointer]);
            ++pointer;
            if(pointer == 6){
                ++menuState;
                LcGotoXY(++pointer,1);    
                LcPutScore(aux);
                LcClear();
                LcGotoXY(0,0);
            }
			break;
            
        case 26:
            if(isPressed()){
				if(KeyGetSMS()== '#'){
                    option = 0;
                    LcClear();
                    pointer = 0;
                    LcGotoXY(0,0);
                    menuState = 10;
                }
            }
            break;
        
        //OPTION 2 SELECTED: MODIFY TIME
        case 27: //Write MODIFY TIME string
            LcPutChar(modifyTimeString[pointer]);
			++pointer;
			if(pointer == 12){
				LcGotoXY(0,1);
				pointer=0;
                minutes =0;
                TiResetTics(timerMenu);
                aux = 28;
                menuState = 33;
                keyValue = KeyGetNumber();
			}
			break;
            
        case 28:
            if((KeyGetValue() || Pressed()==1 && KeyGetValue()==0) && TiGetTics(timerMenu)> BOUNCES){
                TiResetTics(timerMenu);
                keyValue = KeyGetNumber();
                minutes = keyValue*10;
                LcPutChar(keyValue + '0');
                aux = 29;
                menuState = 33;
            }
            break;
            
        case 29:
            if(KeyGetValue() && TiGetTics(timerMenu)> BOUNCES){
                TiResetTics(timerMenu);
                keyValue = KeyGetNumber();
                minutes += keyValue;
                LcPutChar(keyValue + '0');
                aux = 30;
                menuState = 33;
            }
            break;
            
        case 30:
             if(KeyGetValue() && TiGetTics(timerMenu)> BOUNCES){
                LcPutChar(':');
                TiResetTics(timerMenu);
                keyValue = KeyGetNumber();
                seconds = keyValue * 10;
                LcPutChar(keyValue + '0');
                aux = 31;
                menuState = 33;
            }
            break;
        
        case 31://HEM ACABAT UPDATE TIME
             if(KeyGetValue() && TiGetTics(timerMenu)> BOUNCES){
                TiResetTics(timerMenu);
                keyValue = KeyGetNumber();
                seconds += keyValue;
                LcPutChar(keyValue + '0');
                aux = 32;
                menuState = 33;
            }
            break;
            
        case 32:
            if(Pressed()==1)
                if(KeyGetSMS()=='#'){
                    LcClear();
                    LcGotoXY(0,0);
                    option = 0;
                    pointer = 0;
                    menuState = 10;
                }
			break;
            
        case 33:
            if(TiGetTics(timerMenu)>BOUNCES && Pressed()==0){
                menuState = aux;
                TiResetTics(timerMenu);
            }
            break;
            
            
        case 34: //continue part 1
            if(pointer < 5){
                if(users[pointerUser].score[pointer] > aux){
                    ++pointer;
                }
                else{
                    option = users[pointerUser].score[pointer];
                    users[pointerUser].score[pointer] = aux;
                    aux = option;
                    ++pointer;
                }
            }
            else{
                pointer = 0;
                menuState = 22;
            }
            
            break;
            
        case 35: //OPTION 5 selected //pointer ==0, aux ==0
            if(aux < 2){
                LcPutChar(byeString[pointer]);
                ++pointer;
                if(pointer == 3){
                    ++aux;
                    pointer = 0;
                    LcPutChar(' ');
                }
            }else{
                TiResetTics(timerMenu);
                ++menuState;
                pointer = 0;
                
            }
			break;
            
        case 36:
            LcPutChar(users[pointerUser].username[pointer]);
            ++pointer;
            if(users[pointerUser].username[pointer] == '\0'){
                ++menuState;
            }
			break;
        
        case 37:
            if(TiGetTics(timerMenu)> 2*ONESEC){
                pointer = 0;
                minutes=30;
                seconds=0;
                gameON = 0;
                LcClear();
                LcCursorOn();
                LcGotoXY(0,0);
                maxScore = 0;
                option=0;
                menuState = 0;
            }
            break;
            
            
            //NO TOCAR PUNTER POINTERUSER
            
        case 39: //OPTION 4 selected //pointerUser=0; pointer=0;  LcGotoXY(0, 0);
            if(pointerUser<numUsers){
                LcPutChar(users[pointerUser].username[pointer]);
                ++pointer;
                if(users[pointerUser].username[pointer]!='\0'){
                    LcGotoXY(0,1);
                    ++menuState;
                }
            }
            break;
            
        case 40:
            if(pointerUser<numUsers){
                LcPutChar(users[pointerUser].username[pointer]);
                ++pointer;
                if(users[pointerUser].username[pointer]!='\0'){
                    LcGotoXY(0,0);
                    ++menuState;
                }
            }
            break;
            
            
        
            
            
            
            
            
            
        /*
        //OPTION 3 SELECTED: SHOW TOP 5 GENERAL SCORES
        case 25:
            if(pointerUser < MAXUSERS){
                if(pointer < 5){
                    if(users[pointerUser].score[pointer] > maxScore.score && users[pointerUser].score < aux) maxScore = users[pointerUser].score[pointer];
                }else pointer = 0;
            }
                
            else{
                ++menuState;
                pointerUser=0;
                pointer=0;
                LcGotoXY(0,0);
            }
            if(Pressed()==1)
                if(KeyGetValue()=='#')menuState = 10;//tornar a menu
            
            break;
            
        case 26:
            if(nameMaxScore.username[pointer]!='\0'){
                LcPutChar(nameMaxScore[pointer]);
                ++pointer;
            }
            else{
                LcPutScore(maxScore);
                TiResetTics(timerMenu);
                ++menuState;
            }
            break;
            
        case 27: //FALTA LA MARQUEE
            maxScore=0;
            if(TiGetTics(timerMenu)>1200) menuState = 25;
            if(Pressed()==1)
                if(KeyGetValue()=='#') menuState = 10;//tornar a menu
            break;
        
            
        case 28: //OPTION 4 selected //pointerUser=0; pointer=0;
            if(users[currentUser].username[pointer]!='\0'){
                LcPutChar(users[pointerUser].username[pointer]);
                ++pointer;
            }
            else{
                ++aux;
                pointer=0;
                pointerUser = numUsers-1;
                if(pointerUser == currentUser) --pointerUser;
                ++menuState;
            }
            break;
        
        case 29: //OPTION 4 selected //pointerUser=0; pointer=0;
            if(pointerUser<numUsers){
                LcGotoXY(pointer, aux%2);
                if(users[pointerUser].username[pointer]!='\0'){
                    LcPutChar(users[pointerUser].username[pointer]);
                    ++pointer;
                }
                else{
                    ++aux;
                    pointer=0;
                    ++pointerUser;
                    if(pointerUser == currentUser){
                        --pointerUser;
                    }
                }
            }
            else ++menuState;
            break;
*/
/*case 0:
            if((pointer) < 16){
                LcPutChar(laMevaString[pointer+pointerMarquee]);
                pointer++;
                
                if(pointer+pointerMarquee > 21){
                    auxPM = pointerMarquee;
                    pointer=0;
                    pointerMarquee=0;
                }
            }
            else{
                ++pointerMarquee;
                ++menuState;
                TiResetTics(timerMenu);
            }
            break;
            
        case 1:
            if(TiGetTics(timerMenu) > 1200){
                if(auxPM){
                    pointerMarquee = ++auxPM;
                    auxPM=0;
                }else ++pointerMarquee;
                
                pointer=0;
                LcClear();
                LcGotoXY(0,0);
                menuState--;
            }
            break;
 *//*
            
        case 100:
            if(getFirstDirection(numUsers) == 0xFF){
                menuState=0;
            }else{
                readEeprom(numUsers);
                ++menuState;
            }
            break;
            
        case 101:        
            if(getDoneCharging()==1){
                users[numUsers] = getUser();
                ++numUsers;
                menuState = 100;
            }
            break;
    }
}
char getGame(){
    return gameON;
}
*/