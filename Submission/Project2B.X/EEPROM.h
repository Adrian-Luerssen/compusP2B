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

void DaSaveScore(char userScore);

void saveEEPROM(char ad, char data);

char readEEPROM(char address);

void readUserData (void);

void DaFindUser(User* logUser);
//PRE: DaGetIdle()
//POST: checks if the username and password are correct
//		status can be gotten from DaGetStatus()

char DaGetUserNumber(void);
//PRE: --
//POST: returns the current UserNum

void DaSaveUser(User* regUser);
//PRE: DaGetIdle()
//POST: saves the regUser to the EEPROM if username doesnt exist yet
//		check status to see if successful or not DaGetStatus()

char DaGetStatus(void);
//PRE: --
//POST: returns the most recent status saved in the EEPROM
//		 LOGIN_CORRECT 0
//		 LOGIN_FAILED 1
//		 IN_PROCESS 2
//		 REGISTERED_CORRECTLY 3
//		 REGISTER_FAILED 4

char DaGetIdle(void);
//PRE: --
//POST: returns 1 if in idle state, returns 0 otherwise

void DaSaveScore(char userScore);
//PRE: DaGetIdle()
//POST: saves the userScore to the EEPROM if it is better than a currently saved score

char DaGetNumScores(void);
//PRE: --
//POST: returns the number of scores saved

void incrementPosition(void);
//PRE: --
//POST: increments the position pointer

void displayScoresMarquee(void);
//PRE: DaGetIdle()
//POST: puts score[positon]'s and score[positon+1] score and username into the LCD

void resetPosition(void);
//PRE: --
//POST: sets the position pointer to 0

#endif	/* XC_HEADER_TEMPLATE_H */

