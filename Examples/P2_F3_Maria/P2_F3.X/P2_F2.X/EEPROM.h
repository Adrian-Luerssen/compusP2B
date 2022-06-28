#ifndef EEPROM_H
#define	EEPROM_H

#include <xc.h>
#include "Data.h"

void EeInitAction();

void motorEeprom();

char getDoneCharging();

void writeEeprom(User us, char num);

void writeEepromScores(TopScore score, char num);

void readEeprom(char num);

void readEepromScores(char num);

User getUser(void);

TopScore getScore(void);

char getFirstDirection(char numAddress);

char getFirstDirectionScores(char numAddress);

void cleanEeprom();

#endif	