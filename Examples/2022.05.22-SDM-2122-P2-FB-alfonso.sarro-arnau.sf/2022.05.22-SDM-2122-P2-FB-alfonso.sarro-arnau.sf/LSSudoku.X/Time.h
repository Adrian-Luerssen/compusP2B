/* 
 * File:   Time.h
 * Author: alfon
 *
 * Created on 11 May 2022, 20:26
 */

#ifndef TIME_H
#define	TIME_H

void initTimeADT(void);

void motorTime(void);


void TimUpdateTime(void);
//PRE:
void TimEnableTime(void);

void TimDisableTime(void);

void TimModifyTime(char newTime, char position);

char TimGetEndTime(char position);

void TimResetTime(void);
#endif	/* TIME_H */

