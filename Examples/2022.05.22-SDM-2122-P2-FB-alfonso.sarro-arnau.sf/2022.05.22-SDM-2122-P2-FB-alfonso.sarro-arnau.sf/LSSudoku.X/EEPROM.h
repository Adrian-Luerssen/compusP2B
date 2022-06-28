/* 
 * File:   EEPROM.h
 * Author: alfon
 *
 * Created on 16 May 2022, 19:49
 */

#ifndef EEPROM_H
#define	EEPROM_H

#define CMP_BUSY 0
#define CMP_EQUAL 1
#define CMP_NOT_EQUAL 2

#define READ_START 4
#define READ_READY 3
#define READ_BUSY 2
#define READ_COMPLETED 1

void initROM(void);

void motorROM(void);

char readFromROM(void);

void writeToROM(char value);

void storeUser(char value, char position);

void storePass(char value, char position);

void startWriteUser(void);

char compare(char a[8], char b[8]);

char login(char userInput[8], char passInput[8]);

char signup(char userInput[8]);

char getFlag(void);

char getNumUsers(void);

char readUser(void);

void startReadingUser(void);

void startReadingScore(void);

char readScore(void);

void anotherScore(void);
//PRE: a score has been read.
//POST: prepares address for another score to be read.

void storeScore(char score, char value, char pos);

char doneWritingROM(void);


#endif	/* EEPROM_H */

