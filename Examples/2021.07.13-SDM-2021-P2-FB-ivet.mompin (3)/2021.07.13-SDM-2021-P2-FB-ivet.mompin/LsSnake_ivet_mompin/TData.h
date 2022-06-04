
#ifndef T_DATA_H
#define	T_DATA_H

#include "LCD.h"
#include "TTimer.h"
#include "TMatrix.h"
#include "marqueeMenu.h"

typedef struct{
    char username[12];
    int score; 
}user;

user users[20];
user auxiliar;
char total_users,pointer,rowi,times_pressed,modify,startGame,user_selected;
static char times_8,stateHour,timer_menu;
static signed char num_users;
int current_score[4];
char minutes[2];
char hours[2];
char time[4];
char seconds,digit;

void initData();
void motor_data();
void DivideScore(int score);
void UpdateCursor();
void PrintData(char col,char f, char characters,char yes);
int scoreReduce(int score);
char UpDownUsers(char fila);
char SelectUser(char num_users);
void start();
void return_to_main();
char stopModify(char modify);
char checkIfModify(char times_pressed,char key);
#endif 
