#include <xc.h>
#include "EEPROM.h"

static char address;
static char users;
static char userR[8] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
static char passR[8] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
static char user[8] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
static char pass[8] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
static char userInputROM[8] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
static char passInputROM[8] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
static char startUser; //SCRATCH: can delete variable and make function go to state 1 directly; also avoids possible bugs if variable is not propperly controlled.
static char saveScore;
static char position;
static char state;
static char checkUser;
static char flagReading;
static char flagReading;
static char pointer;
static char score;
static char scoreAux;
static char empty;
static char done;
static char newestUserAddress;

void initROM(void) {
    flagReading = CMP_EQUAL;
    flagReading = READ_COMPLETED;
    pointer = 0;
    address = 0;
    users = readFromROM();
    if(users == 0xFF) users = 0;
    startUser = 0;
    address = 16 + users * 16;
    checkUser = 0;
    done = 1;
}

void writeToROM(char value) {
    EEADR = address;
    EEDATA = value;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    INTCONbits.GIE = 1;
    EECON1bits.WREN = 0;
}

char readFromROM() {
    EEADR = address;
    EECON1bits.CFGS = 0;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;
    return EEDATA;
}

void motorROM() {
    switch(state) {
        case 0:            
            if(startUser == 1) {
                state++;
                position = 0;
                address = ((users * 16) % 0x80) +16 ;
                if(address == 0x00) {
                    address = 0x10;
                }
                users = users % 0x90;
                done = 0;
            }
            else if(checkUser == 1 || checkUser == 3){
                address = 0x10;
                position = 0;
                state = 3;
            }
            else if(saveScore == 1){
                address = 0x90;
                position = 0;
                state = 8;
            }
            break;
        case 1:
            if(position < 8) {
                if(EECON1bits.WR == 0) {
                    writeToROM(user[position]);
                    user[position] = '\0'; //DEBUG
                    position++;
                    address++;
                }
            }
            else {
                position = 0;
                state++;
            }
            break;
        case 2:
            if(position < 8) {
                if(EECON1bits.WR == 0) {
                    writeToROM(pass[position]);
                    pass[position] = '\0';
                    position++;
                    address++;
                }
            }
            else {
                if(EECON1bits.WR == 0) {
                    users++;
                    position = address;
                    address = 0;
                    writeToROM(users);
                    address++;
                    state = 100;
                    startUser = 0;
                }
            }
            break;
        case 100:
            if(EECON1bits.WR == 0) {
                writeToROM(position-16);
                address = position;
                position = 0;
                state = 0;
                done = 1;
            }
            break;
        case 3:
            if(position < 8) {
                userR[position] = readFromROM();
                address++;
                position++;
            }
            else {
                position = 0;
                state++;
            }
            break;
        case 4:
            if(position < 8) {
                passR[position] = readFromROM();
                address++;
                position++;
            }
            else {
                position = 0;
                flagReading = CMP_BUSY;
                pointer = 0;
                state++;
            }
            break;
        case 5:
            if(compare(userInputROM, userR) != CMP_BUSY) {
                if(flagReading == CMP_EQUAL) {
                    flagReading = CMP_BUSY;
                    pointer = 0;
                    if(checkUser == 1)state++;
                    if(checkUser == 3) {
                        //checkUser = 2;
                        flagReading = CMP_EQUAL;
                        state = 7;}
                }
                else if(flagReading == CMP_NOT_EQUAL) {
                    if(((address - 16) / 16) >= users) {
                        state = 7;
                    }
                    else {
                        flagReading = CMP_BUSY;
                        pointer = 0;
                        state = 3;
                    }
                }
            }
            break;
        case 6:
            if(compare(passInputROM, passR) != CMP_BUSY) {
                if(flagReading == CMP_EQUAL) {
                    //TA WENO
                    state = 7;
                }
                else if(flagReading == CMP_NOT_EQUAL) {
                    if(((address - 16) / 16) >= users) {
                        state = 7;
                    }
                    else {
                        flagReading = CMP_BUSY;
                        pointer = 0;
                        state = 3;
                    }
                }
            }
            break;
        case 7:
            if (position < 8) {
                userInputROM[position] = '\0';
                passInputROM[position] = '\0';
                userR[position] = '\0';
                passR[position] = '\0';
                position++;
            } else {
                position = 0;
                state = 0;
                checkUser = 2;
            }
            break;
        //Check if address is empty
        case 8:
            if(readFromROM() == 0xFF) {
                empty = 1;
                state = 9;
            }
            else {
                if(address < 0xD0) {
                    address = address + 16;
                }
                else {
                    address = 0x90;
                    state = 11;
                }
                
            }
            break;
        //WRITE SCORE VALUE
        case 9:
            if(EECON1bits.WR == 0) {
                writeToROM(score);
                address++;
                position = 0;
                state++;
            }
            break;
        //WRITE USER THAT GOT THE SCORE
        case 10:
            if(position < 8) {
                if(EECON1bits.WR == 0) {
                    writeToROM(user[position]);
                    address++;
                    position++;
                }
            }
            else {
                position = 0;
                if(empty == 1) {
                    saveScore = 0;
                    empty = 0;
                    state = 0;
                }
                else {
                    address = address + 7;
                    state = 12;
                }
            }
            break;
        //CHECK IF SCORE IS LOWER
        case 11:
            scoreAux = readFromROM();
            if(scoreAux < score) {
                state = 9;
            }
            else {
                if(address < 0xD0) {
                    address = address + 16;
                }
                else {
                    saveScore = 0;
                    state = 0;
                }
            }
            break;
        case 12:
            score = scoreAux;
            state--;
            break;
    }
}

void storeUser(char value, char positionUser) {
    user[positionUser] = value;
}

void storePass(char value, char positionPass) {
    pass[positionPass] = value;
}

void startWriteUser() {
    startUser = 1;
}

char login(char userInput[8], char passInput[8]) {
    if(position < 8 && checkUser == 0) {
        userInputROM[position] = userInput[position];
        passInputROM[position] = passInput[position];
        position++;
    }
    else {
        if (flagReading != CMP_BUSY && checkUser == 0) {
            checkUser = 1;
        }
        else {
            if (checkUser == 2) {
                checkUser = 0; 
                return flagReading;
            }
        }
    }

    return CMP_BUSY;
}

char signup(char userInput[8]) {
    if(position < 8 && checkUser == 0) {
        userInputROM[position] = userInput[position];
        position++;
    }
    else {
        if (flagReading != CMP_BUSY && checkUser == 0) {
            checkUser = 3;
        }
        else {
            if (checkUser == 2) {
                checkUser = 0; 
                return flagReading;
            }
        }
    }

    return CMP_BUSY;
}

char getFlag() {
    return flagReading;
}

char getNumUsers(){
    if (users > 8) return 8;
    return users;
}

char compare (char a[8], char b[8]) {
	if (flagReading != CMP_BUSY) {
		flagReading = CMP_BUSY;
		pointer = 0;
	}

	/*if (pointer == 8) {
		flag = CMP_EQUAL;
	}
	else {*/
		if (a[pointer] == b[pointer]) {
			pointer++;
            if(pointer == 8) flagReading = CMP_EQUAL;
		}
		else {
			flagReading = CMP_NOT_EQUAL;
		}
	//}
	return flagReading;
}

void storeScore(char scoreIn, char userLetter, char pos) {
    score = scoreIn;
    user[pos] = userLetter;
    if(pos == 7) {
        saveScore = 1;
    }
}

char doneWritingROM() {
    return done;
}

void startReadingUser() {
    address = 0x01;
    newestUserAddress = readFromROM();
    address = newestUserAddress;
    flagReading = READ_READY;
}

char readUser() {
    char toRead;
    if (flagReading == READ_READY) {
        flagReading = READ_BUSY;
        position = 0;
    } 
    
    if (address == 0x18) {
            address = 0x80;
            if (readFromROM() == 0xFF) {
                flagReading = READ_COMPLETED;
                toRead = READ_COMPLETED;
            }
    } else if (position < 8) {
        userR[position] = readFromROM();
        toRead = userR[position];
        userR[position] = '\0';
        address++;
        position++;
    }
    
    if (position == 8) {
        position = 0;
        if (address != 0x18 && address != newestUserAddress + 24) {
            address = address - 24;
        } else if (address == newestUserAddress + 24) {
            flagReading = READ_COMPLETED;
        }
        //flagReading = READ_COMPLETED;
    }
    
    return toRead;
}

void startReadingScore() {
    address = 0x89;
    //flagReading = READ_READY;
}

void anotherScore() {
    address = address + 7;
    flagReading = READ_READY;
}

char readScore () {
    char aux;
    aux = readFromROM();
    if (flagReading == READ_READY) {
        flagReading = READ_START;
        return (aux / 10) + '0';
    } else if (flagReading == READ_START) {
        flagReading = READ_BUSY;
        address++;
        return (aux % 10) + '0';
    } else if (flagReading == READ_BUSY) {
        address++;
        return aux;
    }
}
