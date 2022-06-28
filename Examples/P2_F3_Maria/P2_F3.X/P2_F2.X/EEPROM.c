#include <xc.h>
#include "EEPROM.h"
#include "LcTLCD.h"

static char pointer;
static char pointerEeprom;
static char doneCharging;
static char eepromState, aux;
User user; 
TopScore topScore;
char numAddress;

void EeInitAction(){
    pointer = 0;
    pointerEeprom = 0;
    eepromState = 10;
}

void motorEeprom() {
    switch(eepromState){
        //WRITE
        case 0:
            EEADR = numAddress*18+10 + pointerEeprom;
            EEDATA = user.username[pointer];
            //LcPutChar(user.username[pointer]);
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.WREN = 1;
            INTCONbits.GIE = 0;
            EECON2 = 0x55;
            EECON2 = 0xAA;
            EECON1bits.WR = 1;
            INTCONbits.GIE = 1;
            EECON1bits.WREN = 0;
            ++pointerEeprom;
            if(user.username[pointer] != '\0'){
                ++pointer;
                aux = 0;
                eepromState = 4;
            }
            else{
                pointer = 0;
                aux = 1;
                eepromState = 4;

            }
            break;
            
        case 1:
            EEADR = numAddress*18+10 + pointerEeprom;
            EEDATA = user.password[pointer];
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.WREN = 1;
            INTCONbits.GIE = 0;
            EECON2 = 0x55;
            EECON2 = 0xAA;
            EECON1bits.WR = 1;
            INTCONbits.GIE = 1;
            EECON1bits.WREN = 0;
            ++pointerEeprom;
            if(user.password[pointer] != '\0'){
                ++pointer;
                aux = 1;
                eepromState = 4;
            }
            else{
                eepromState = 10;
                doneCharging = 1;
            }
            break;
            
        case 2:
            EEADR = numAddress;
            EEDATA = topScore.score;
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.WREN = 1;
            INTCONbits.GIE = 0;
            EECON2 = 0x55;
            EECON2 = 0xAA;
            EECON1bits.WR = 1;
            INTCONbits.GIE = 1;
            EECON1bits.WREN = 0;
            ++pointerEeprom;
            aux = 3;
            eepromState = 4;
            break;

        case 3:
            EEADR = numAddress + 1;
            EEDATA = topScore.user;
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.WREN = 1;
            INTCONbits.GIE = 0;
            EECON2 = 0x55;
            EECON2 = 0xAA;
            EECON1bits.WR = 1;
            INTCONbits.GIE = 1;
            EECON1bits.WREN = 0;
            ++pointerEeprom;
            eepromState = 10;
            doneCharging = 1;
            break;
            
        case 4:
            if(EECON1bits.WR == 0){
                eepromState = aux;
            }
            break;
            
        //READ
        case 5:
            EEADR = numAddress*18+10 + pointerEeprom;
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.RD = 1;
            ++pointerEeprom;
            if(EEDATA != 0){
                user.username[pointer] = EEDATA;
                ++pointer;
                aux = 5;
                eepromState = 9;
            }
            else{
                pointer = 0;
                aux = 6;
                eepromState = 9;
            }
            break;
            
        case 6:
            EEADR = numAddress*18+10 + pointerEeprom;
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.RD = 1;
            user.password[pointer] = EEDATA;
            ++pointerEeprom;
            if(user.password[pointer] != 0){
                ++pointer;
                aux = 6;
                eepromState = 9;
            }
            else{
                eepromState = 10;
                doneCharging = 1;
            }
            break;
            
        case 7:
            EEADR = numAddress;
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.RD = 1;
            topScore.score = EEDATA;
            aux = 8;
            eepromState = 9;
            break;
            
        case 8:
            EEADR = numAddress +1;
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.RD = 1;
            topScore.user = EEDATA;
            eepromState = 10;
            doneCharging = 1;
            break;
            
            
        case 9:
            if(EECON1bits.RD == 0){
                eepromState = aux;
            }
            break;
            
        //WAIT STATE
        case 10:
            doneCharging = 0;
            break;
            
        case 11:
            EEADR = numAddress;
            EEDATA = 0xFF;
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.WREN = 1;
            INTCONbits.GIE = 0;
            EECON2 = 0x55;
            EECON2 = 0xAA;
            EECON1bits.WR = 1;
            INTCONbits.GIE = 1;
            EECON1bits.WREN = 0;
            ++numAddress;
            if(numAddress < 170){            
                aux = 11;
                eepromState = 4;
            }else{
                --eepromState;
                doneCharging = 1;
            }
            break;
            
    }
}
char getDoneCharging(){
    return doneCharging;
}
void writeEeprom(User us, char num){
    eepromState = 0;
    user = us;
    numAddress = num;
    pointer = 0;
    pointerEeprom = 0;
    doneCharging = 0;
    
}
void writeEepromScores(TopScore score, char num){
    pointerEeprom=0;
    eepromState = 2;
    topScore = score;
    numAddress = num*2;
    doneCharging = 0;
    
}
void readEeprom(char num){
    eepromState = 5;
    numAddress = num;
    pointer = 0;
    pointerEeprom = 0;
    doneCharging = 0;
}

void readEepromScores(char num){
    eepromState = 7;
    numAddress = num;
    doneCharging = 0;
}


User getUser(void){
    return user;
}

TopScore getScore(void){
    return topScore;
}

char getFirstDirection(char numAddress){
    EEADR = numAddress*18+10;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.RD = 1;
    return EEDATA;
}
char getFirstDirectionScores(char numAddress){
    EEADR = numAddress*2;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.RD = 1;
    return EEDATA;
}
void cleanEeprom(){
    eepromState = 11;
    numAddress = 10;
    doneCharging = 0;
}