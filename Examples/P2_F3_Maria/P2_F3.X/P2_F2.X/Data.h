 
#ifndef DATA_H

#define	DATA_H

#include <xc.h>


#define MAXUSERS 2


typedef struct{
	char username[9];
	char password[9];
}User;

typedef struct{
	char score;
	char user;
}TopScore;

User users[MAXUSERS + 1];
TopScore topScores[5];
static char numUsers;
static char numScores;
static char currentUser;

void initData();


#endif