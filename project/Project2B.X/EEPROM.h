#ifndef EEPROM_H
#define	EEPROM_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "Menu.h"
typedef struct {
    char username [9];
    char password [9];
} User;

typedef struct {
    char score;
    char userNum;
} Score;



//0x00  start user 9bytes username 9 bytes password = 18 bytes per user * 8 users
//0x90  start of scores 5 scores 1 byte for score 1 byte for user number = 10bytes total
//0x9A  last user number

#define USER_POSITION 0x00
#define SCORE_POSITION 0x90
#define SCORE_USER_POSITION 0xA0
#define POINTER_POSITION 0xFF
#define LASTSCORE_POINTER 0xFE

#define BYTES_PER_USER 16
#define LOGIN_CORRECT 0
#define LOGIN_FAILED 1
#define IN_PROCESS 2
#define REGISTERED_CORRECTLY 3
#define REGISTER_FAILED 4
#define IDLE 5

void initData(void);

void dataMotor(void);

void DaFindUser(User* logUser);

char DaGetUserNumber(void);

void DaSaveUser(User* regUser);

char DaGetStatus(void);

char DaGetIdle(void);

void readUserData (void);

void DaSaveScore(char userScore);

void saveEEPROM(char ad, char data);
char DaGetScoreMarquee(char LCDcol,char LCDrow,char LCDcolm);
char readEEPROM(char address);
char DaGetNumScores(void);
void incrementPosition(void);
void displayScoresMarquee(void);
void resetPosition(void);
#endif	/* XC_HEADER_TEMPLATE_H */

