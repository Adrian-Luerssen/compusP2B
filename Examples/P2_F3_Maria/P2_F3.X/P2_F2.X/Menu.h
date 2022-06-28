#ifndef MENU_H
#define	MENU_H

#include <xc.h>

//#define MAX_POSITIONS 8
#define LOGIN 1 //1
#define REGISTER 2 //2



static char accessMenuString1[8] = {"1. LOGIN"};
static char accessMenuString2[11] = {"2. REGISTER"};

static char *menuStrings[] = {{"1. Play a Game  "},
                              {"2. Modify time  "},
                              {"3. Show general Top 5 scores "},
                              {"4. Show time    "},
                              {"5. Logout       "}};

static char enterUserString[5] = {"USER:"};
static char enterPasswordString[5] = {"PSWD:"};


static char timeRemainingString[15] = {"TIME REMAINING:"};
static char timeLeftString[10]= {"TIME LEFT:"};
static char scoreString[6] = {"SCORE:"};
static char errorsString[7] = {"ERRORS:"};
static char modifyTimeString[12] = {"MODIFY TIME:"};
static char byeString[3] = {"BYE"};
static char currentTimeString[13] = {"CURRENT TIME:"};

void initMenu(void);
void menuMotor(void);
char getGame();
#endif	