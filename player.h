// 플레이어 정보 구조체 

#ifndef PLAYER_H
#define PLAYER_H

#include "monster.h"

struct player
{
    int flag;
    int playerID;
    int processID;
    int isServerRunning;
<<<<<<< Updated upstream
    int isMyTurn;
=======
    struct monster selectedMonster;
>>>>>>> Stashed changes
};