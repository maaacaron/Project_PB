#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "monster.h" // 몬스터 스텟 구조 저장 헤더파일 
#include "player.h"


void checkMySkillInfo(int receivedPlayerID, struct player* shmaddr)
{
    int pid;
    int status;
    char skill1Str[10];
    char skill2Str[10];
    char skill3Str[10];
    char skill4Str[10];
    printf("-----------------------------------------------------\n");
    printf("스킬\n");
    if (shmaddr[receivedPlayerID].selectedMonster.skills.skill_1_ID == -1)
    {
        printf("없음\n");
    }
    else
    {
        if ((pid = fork()) == 0)
        {
            sprintf(skill1Str, "%d", shmaddr[receivedPlayerID].selectedMonster.skills.skill_1_ID);
            execl("./checkMySkill", "checkMySkill", skill1Str, NULL);
        }
        else
        {
            wait(&status);
        }
    }
    if (shmaddr[receivedPlayerID].selectedMonster.skills.skill_2_ID == -1)
    {
        printf("스킬 2: 없음\n");
    }
    else
    {
        if ((pid = fork()) == 0)
        {
            sprintf(skill2Str, "%d", shmaddr[receivedPlayerID].selectedMonster.skills.skill_2_ID);
            execl("./checkMySkill", "checkMySkill", skill2Str, NULL);
        }
        else
        {
            wait(&status);
        }
    }
    if (shmaddr[receivedPlayerID].selectedMonster.skills.skill_3_ID == -1)
    {
        printf("스킬 3: 없음\n");
    }
    else
    {
        if ((pid = fork()) == 0)
        {
            sprintf(skill3Str, "%d", shmaddr[receivedPlayerID].selectedMonster.skills.skill_3_ID);
            execl("./checkMySkill", "checkMySkill", skill3Str, NULL);
        }
        else
        {
            wait(&status);
        }
    }
    if (shmaddr[receivedPlayerID].selectedMonster.skills.skill_4_ID == -1)
    {
        printf("스킬 4: 없음\n");
    }
    else
    {
        if ((pid = fork()) == 0)
        {
            sprintf(skill4Str, "%d", shmaddr[receivedPlayerID].selectedMonster.skills.skill_4_ID);
            execl("./checkMySkill", "checkMySkill", skill4Str, NULL);
        }
        else
        {
            wait(&status);
        }
    }
    printf("-----------------------------------------------------\n");
}

void checkMyMonster(int receivedPlayerID)
{
    key_t key = ftok("main", 10597);
    int shmid = shmget(key, sizeof(struct player) * 4, IPC_CREAT | 0644);
    if (shmid == -1)
    {
        perror("shmget 실패");
        exit(1);
    }

    struct player* shmaddr = (struct player*)shmat(shmid, NULL, 0);
    if (shmaddr == (void*)-1)
    {
        perror("shmat 실패");
        exit(1);
    }
    printf("\n");
    printf("이름  속성\n");
    printf("-----------------------------------------------------\n");
    printf("%-5s %-5s\n", shmaddr[receivedPlayerID].selectedMonster.monster_name, shmaddr[receivedPlayerID].selectedMonster.property);
    printf("레벨    경험치    체력    공격력    방어력    스피드\n");
    printf("-----------------------------------------------------\n");
    printf("%-8d %-8d %-8d %-8d %-8d %-8d\n", shmaddr[receivedPlayerID].selectedMonster.stats.level, shmaddr[receivedPlayerID].selectedMonster.stats.exp,
        shmaddr[receivedPlayerID].selectedMonster.stats.HP, shmaddr[receivedPlayerID].selectedMonster.stats.attackPower,
        shmaddr[receivedPlayerID].selectedMonster.stats.defensePower, shmaddr[receivedPlayerID].selectedMonster.stats.speed);
    checkMySkillInfo(receivedPlayerID, shmaddr);
}


int main(int argc, char* argv[])
{
    int receivedPlayerID = atoi(argv[1]);

    checkMyMonster(receivedPlayerID);

    exit(0);
}
