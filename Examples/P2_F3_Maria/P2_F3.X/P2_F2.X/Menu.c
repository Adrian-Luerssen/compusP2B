#include "Menu.h"
#include "LcTLCD.h"
#include "Data.h"
#include "Timer.h"
#include "Keypad.h"
#include "Joystick.h"
#include "EEPROM.h"
#include "Music.h"
#include "TX.h"

static char menuState;//marqueeState;
static char pointer, pointerMarquee, auxPM;//, pointerM;
char pointerUser, auxPU;
char option;
char keyValue;
static char minutes;
static char seconds;
static char minutesCountdown;
static char secondsCountdown;
static unsigned char timerMenu;
char aux;
char maxScore, maxIndex;
char prevMaxIndex, prevMaxScore;
static char gameON;
char error[2];

void initMenu(void){
    timerMenu = TiGetTimer();
	menuState = 100;//100
	minutes=30;
	seconds=0;
	gameON = 0;
    maxScore = 0;
    TiResetTics(timerMenu);
    numUsers = 0;
    numScores = 0;
    readEeprom(0);
    auxPU = 0;
}
void menuMotor(void){   
    switch(menuState){
        case 100:
            if(getFirstDirectionScores(numScores) == 0xFF){
                menuState=102;
            }else{
                readEepromScores(numScores);
                ++menuState;
            }
            break;
            
        case 101:        
            if(getDoneCharging()==1){
                topScores[numScores] = getScore();
                ++numScores;
                if(numScores < 5) --menuState;
                else ++menuState;
            }
            break;
            
        case 102:
            if(getFirstDirection(numUsers) == 0xFF){
                menuState=104;
            }else{
                readEeprom(numUsers);
                ++menuState;
            }
            break;
            
        case 103:        
            if(getDoneCharging()==1){
                users[numUsers] = getUser();
                ++numUsers;
                --menuState;
            }
            break;
            
        case 104:
            LcClear();
            LcGotoXY(0,0);
            menuState=0;
            pointer=0;
            option = 0;
            break;
            
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
                if(KeyGetPress() > 1){
                    LcGotoXY(--pointer+6 ,0);
                }
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
                    if(pointerUser==numUsers) menuState=104;
                    else pointer = 0;
                }
                else
                    if(users[pointerUser].username[pointer]==users[numUsers].username[pointer])//DE MOMENT SONT IGUALS
                        if(users[numUsers].username[pointer]=='\0'){//DEFINITIU IGUALS -> COMPROVEM CONTRASEÑA
                            pointer = 0;
                            ++menuState;
                            
                        }
                        else ++pointer;
                    
                    else menuState = 104;
            else menuState = 104;
			break;
        
        case 8:
            if(!(users[pointerUser].password[pointer]==users[numUsers].password[pointer])) menuState = 104;
			else
				if(users[pointerUser].password[pointer]==users[numUsers].password[pointer])//DE MOMENT SONT IGUALS
					if(users[numUsers].password[pointer]=='\0'){//DEFINITIU IGUALS -> L'USUARI POT ACCEDIR
                        currentUser = pointerUser;
						LcClear();
						LcGotoXY(0,0);
						pointer = 0;
						menuState=10;
                        option = 0;
                        pointer = 0;
                        
					}
					else ++pointer;
			break;
        
		case 9://COMPROVEM REGISTER
            if(numUsers!=pointerUser){
                if(!(users[numUsers].username[pointer]==users[pointerUser].username[pointer])){//NO ESTA REGISTRAT
                    ++pointerUser;
                    if(pointerUser==numUsers){
                        if(numUsers < MAXUSERS){
                            menuState = 56;//FALTA ACABR
                        }
                        else{
                            pointer = 0;
                            cleanEeprom();
                            menuState = 57;//FALTA ACABAR
                        }
                        
                    }
                    else pointer = 0;
                }
                else
                    if(users[numUsers].username[pointer]==users[pointerUser].username[pointer])//DE MOMENT SONT IGUALS
                        if(users[numUsers].username[pointer]=='\0'){//DEFINITIU IGUALS -> L'USUARI NO POT ACCEDIR
                            menuState = 104;
                        } 
                        else ++pointer;
            }else{
                menuState = 56; //PERSIST DATA IN EEPROM
            }
			break;
         
        case 57:
            if(getDoneCharging()==1){
                menuState = 52;
            }
            break;
            
        case 52: //LOOP UNTIL SAVE ALL THE DATA OF THE NEW POSITIONS
            if (pointer < MAXUSERS){
                users[pointer] = users[pointer+1];
                writeEeprom(users[pointer],pointer);
                ++menuState;
            }else{
                --numUsers;
                pointer = 0;
                menuState += 2;
            }
            break;
            
        case 53: // WAIT FOR EACH DATA TO BE SAVED
            if(getDoneCharging()==1){
                ++pointer;
                --menuState;
            }
            break;
            
        case 54: //NOW CHECK topScores
            if(pointer < 5){
                if(topScores[pointer].user == 0 && topScores[pointer].score > 0){
                    topScores[pointer] = topScores[aux+1];
                    --pointer;
                }else{
                    --topScores[pointer].user;
                }
                writeEepromScores(topScores[pointer], pointer);
                ++menuState;
            }else{
                --numScores;
                menuState += 2;
            }
            break;
        
        case 55:
            if(getDoneCharging()==1){
                ++pointer;
                --menuState;
            }
            break;
            
        case 56:
            writeEeprom(users[numUsers],numUsers);
            ++numUsers;
            LcClear();
            option = LOGIN;
            pointer = 0;
            LcGotoXY(0,0);
            menuState = 3;
            break;
            
        
        case 10:
            if(option == 2){
                aux = 0;
                pointer=0;
                LcGotoXY(0,1);
                ++option;
                ++menuState;
            }else{
                LcPutChar(menuStrings[option][pointer]);
                ++pointer;
                if(pointer == 16){
                    pointer=0;
                    LcGotoXY(0,1);
                    ++option;
                    ++menuState;
                    if(option==5) menuState = 12;
                }
            }
			break;
            
		case 11:
            if(option==2){
                aux = 1;
                LcGotoXY(0,1);
                menuState = 110; 
                pointer=0;
            }else{
                LcPutChar(menuStrings[option][pointer]);
                ++pointer;
                if(pointer == 16){
                    ++menuState;
                    TiResetTics(timerMenu);
                    KeyResetValue();
                    if(option == 3){
                        pointer = 0;
                        --option;
                        menuState = 110;
                    }
                }
                
            }
			break;    
            
        case 110:
            if((pointer) < 16){
                LcPutChar(menuStrings[option][pointer+pointerMarquee]);
                pointer++;
                
                if(pointer+pointerMarquee > (29 -1)){ //size - 1
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
            
        case 111:
            if(TiGetTics(timerMenu) > 1200){
                if(auxPM){
                    pointerMarquee = ++auxPM;
                }else ++pointerMarquee;
                pointer=0;
                if(aux == 0) LcGotoXY(0,0);
                else LcGotoXY(0,1);
                menuState--;
                if(auxPM == 28){ 
                    menuState = 110;
                    pointer = 0;
                    pointerMarquee = 0;
                    auxPM = 0;
                }else{
                    auxPM=0;
                }
            }else{
                if(aux == 0) ++option;
                menuState = 12;
            }
            break;
            
		case 12:
            if(TiGetTics(timerMenu) > 500){
                if(isPressed()){
                    if(KeyGetValue()== '#'){
                        if(option==1){//OPTION 1 SELECTED
                            pointer = 0;
                            menuState = 13;
                        }else if(option==2){//OPTION 2 SELECTED
                            KeyResetValue();
                            menuState = 27;
                            LcClear();
                            LcGotoXY(0,0);
                            pointer=0;
                        }else if(option==3){//OPTION 3 SELECTED
                            aux = 0;
                            pointerUser=0;
                            pointer=0;
                            pointerMarquee = 0;
                            LcClear();
                            LcGotoXY(0,0);
                            menuState = 48;
                            TiResetTics(timerMenu);
                        }else if(option==4){//OPTION 4 SELECTED
                            pointer=0;
                            menuState = 45;
                            LcClear();
                            LcGotoXY(0,0);
                        }else if(option==5){//OPTION 5 SELECTED
                            LcClear();
                            LcGotoXY(0,0);
                            pointer=0;
                            aux = 0;
                            menuState = 35;
                        }
                        pointer=0;
                    }
                    
                }else if(getPositionJoystick() == 'S' && getCentreJoystick()==0){//MOVEM EL MENU ABAIX
                    resetPositionJoystick();
                    menuState = 10;
                    pointer=0;
                    LcClear();
                    LcGotoXY(0,0);
                    if(option==5) --option;
                    
                }else if(getPositionJoystick() == 'W' && getCentreJoystick()==0){//MOVEM EL MENU ADALT
                    resetPositionJoystick();
                    menuState = 10;
                    pointer=0;
                    LcClear();
                    LcGotoXY(0,0);
                    if(option==1) --option;
                    else option-=2; 
                }else if (option == 3 || option == 2){
                    if(aux == 0) --option;
                    menuState = 111;
                }
                
            }else if (option == 3 || option == 2){
                if(aux == 0) --option;
			    menuState = 111;
            }
			
			break;

		//OPTION 1 SELECTED 
		case 13: //SEND NAME USER
			if(users[pointerUser].username[pointer] != '\0'){
				if(TXbusy()==1){//PODEM ENVIAR
					setTX(users[pointerUser].username[pointer]);
					++pointer;
				}
            }
            else{
                setTX(users[pointerUser].username[pointer]);
                pointer =0;
                ++menuState;
            }
            break;
            
        case 14:
            if(getRX()=='K'){
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
			menuState = 39;
			break;
            
        case 39:
            if(TXgetState() == 0){ // we can send
                sendMyTX(minutesCountdown/10 + '0');
                ++menuState;
            }
            
            break;
         
        case 40:
            if(TXgetState() == 0){ // we can send
                sendMyTX(minutesCountdown%10 + '0');
                ++menuState;
            }
            break;
            
        case 41:
            if(TXgetState() == 0){
                sendMyTX(':');
                ++menuState;
            }
            break;
            
        case 42:
            if(TXgetState() == 0){
                sendMyTX(secondsCountdown/10 + '0');
                ++menuState;
            }
            break;
            
        case 43:
            if(TXgetState() == 0){
                sendMyTX(secondsCountdown%10 + '0');
                ++menuState;
            }
            break;
            
        case 44:
            if(TXgetState() == 0){
                sendMyTX(' ');
                menuState = 17;
            }
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
            if (isPressed()){
                if(TXgetState()==0){
                    keyValue = KeyGetNumber();
                    setTX(keyValue+'0');
                }
                KeyResetValue();
            }
            
			break;
            
		case 18:
            if(TXbusy()==1){
                endMusic();
                setTX('F');
                TiResetTics(timerMenu);
                ++menuState; 
            }
			break;	
                
        case 19:
			if(RXdata()==1){
                auxPM = getRX();
                aux = auxPM;
                TiResetTics(timerMenu);
                ++menuState;
            } 
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
                auxPU = pointerUser;
                menuState = 34;
                pointer = 0;
                LcClear();
                LcGotoXY(0,0);
            }
            break;
            
        case 34: //continue part 1
            if(pointer < 5){
                if(topScores[pointer].score == 0){
                    topScores[pointer].score = aux;
                    topScores[pointer].user = pointerUser;
                    ++numScores;
                    writeEepromScores(topScores[pointer], pointer);
                    menuState = 23;
                }
                else if(topScores[pointer].score > aux){
                    ++pointer;
                }
                else{
                    prevMaxScore = topScores[pointer].score;
                    prevMaxIndex = topScores[pointer].user;
                    topScores[pointer].score = aux;
                    topScores[pointer].user = auxPU;
                    if(topScores[pointer].score != 0)writeEepromScores(topScores[pointer], pointer);
                    aux = prevMaxScore;
                    auxPU = prevMaxIndex;
                    ++pointer;
                    ++numScores;
                    if(numScores > 5) numScores = 5;
                }
            }
            else{
                menuState = 23;
            }
            
            break;
            
        case 21:
            if(RXdata()==1){
                error[1] = getRX();
                pointer = 0;
                LcClear();
                LcGotoXY(0,0);
                ++menuState;
            }
            break;
            
        case 22:
            LcPutChar(errorsString[pointer]);
            ++pointer;
            if(pointer == 7){
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
                LcPutScore(auxPM);
                ++menuState;
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
                keyValue = KeyGetNumber();
				pointer=0;
                minutes =0;
                TiResetTics(timerMenu);
                aux = 28;
                menuState = 33;
                
			}
			break;
        case 28:
            if(isPressed() && TiGetTics(timerMenu)> BOUNCES){
                TiResetTics(timerMenu);
                keyValue = KeyGetNumber();
                LcPutChar(keyValue + '0');
                minutes = keyValue*10;
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
            
        //OPTION 4: SHOW TIME
            
        case 45:
            LcPutChar(currentTimeString[pointer]);
            ++pointer;
            if(pointer == 13){
                showTimerPic();
                ++menuState;
                TiResetTics(timerMenu);
                KeyResetValue();
            }
            break;
        
        case 46:
            if(isPressed() && TiGetTics(timerMenu)> ONESEC){
				if(KeyGetSMS()== '#'){
                    stopTimerPic();
                    option = 0;
                    LcClear();
                    pointer = 0;
                    LcGotoXY(0,0);
                    menuState = 10;
                }
            }
            break;
            
        //OPTION 3 : SHOW TOP 5
        case 48:
            KeyResetValue();
            if(numScores == 0) menuState = 10;
            else{
                aux =0;
                ++menuState;
                LcGotoXY(0,0);
            }
            break;
            
        case 49:
            if(users[topScores[aux].user].username[pointer] != '\0'){
                LcPutChar(users[topScores[aux].user].username[pointer]);
                ++pointer;
            }else{
                LcPutChar(':');
                LcGotoXY(0,1);
                LcPutScore(topScores[aux].score);
                LcGotoXY(16,0);
                ++aux;
                if(numScores == aux) aux = 0;
                ++menuState;
                pointer = 0;
                //else menuState = //FALTA ACABAR;
            }
            break;
            
        case 50:
            if(users[topScores[aux].user].username[pointer] != '\0'){
                LcPutChar(users[topScores[aux].user].username[pointer]);
                ++pointer;
            }else{
                LcPutChar(':');
                LcGotoXY(16,1);
                LcPutScore(topScores[aux].score);
                ++menuState;
                TiResetTics(timerMenu);
                //++aux;
                pointer = 0;
            }
            break;
            
        case 51:
            if (TiGetTics(timerMenu) > ONESEC){
                LcScroll();
                TiResetTics(timerMenu);
                ++pointerMarquee;
                if(pointerMarquee == 16){
                    LcClear();
                    pointer = 0;
                    menuState = 49;
                    LcGotoXY(0,0);
                    pointerMarquee=0;
                }
                
            }
            else if(isPressed()){
                if(KeyGetSMS()== '#'){
                    option = 0;
                    LcClear();
                    pointer = 0;
                    LcGotoXY(0,0);
                    menuState = 10;
                }
            }
            break;
  
    }
}
char getGame(){
    return gameON;
}
