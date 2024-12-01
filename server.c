#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "player.h"
#include "monster.h"

int main()                          //마지막 플레이어까지 접속하여 정보를 받아올 때까지 무한반복하며 대기 
{
    struct player* shmaddr;
    key_t key;
    int shmid;

    int fd;
    int player[4][2];               //플레이어 아이디 + 프로세스 아이디

    int accessCount = 0;

    key = ftok("main", 1);
    shmid = shmget(key, sizeof(struct player) * 5, IPC_CREAT | 0644);   //플레이어별 메모리 크기 설정
    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }


    shmaddr = (struct player*)shmat(shmid, NULL, 0);

    //플레이어 정보 입력
    /* 원래 있던 코드
    while(accessCount < 4)
    {
        if(shmaddr->flag == 1)          //다른 프로세스에서 공유메모리에 값을 넣었으면
        {
            sleep(1);                   //공유메모리에 값을 다 넣기 전에 실행 되는 것 방지
            for(int i = 0; i < 4; i++)
            {
                if(player[i][0] == NULL)
                {
                    player[i][0] = shmaddr->playerID;           //플레이어의 아이디
                    player[i][1] = shmaddr->processID;          //해당 프로세스의 아이디
                    accessCount++;                              //접근한 프로세스 수 +1
                }
            }


            shmaddr->playerID = 0;
            shmaddr->processID = 0;
            shmaddr->flag = 0;          //서버가 값 받기 완료
        }
    }
    */

    printf("플레이어 데이터를 수집중입니다...\n");

    //플레이어 정보 입력
    while (accessCount < 4)
    {
        for (int i = 0; i < 4; i++)
        {
            if (shmaddr[i].flag == 1)
            { // 새로운 데이터가 들어온 슬롯만 처리
                shmaddr[i].selectedMonster.stats.HP,
                shmaddr[i].selectedMonster.stats.attackPower,
                shmaddr[i].selectedMonster.stats.defensePower,
                shmaddr[i].selectedMonster.stats.speed;
                shmaddr[i].flag = 0; // 처리 완료 표시
                accessCount++;       // 플레이어 수 증가
            }
        }
    }

    int dead1_flag = 0;
    int dead2_flag = 0;
    int dead3_flag = 0;
    int dead4_flag = 0;

    // 종료 방지
    while ((dead1_flag == 0 && dead2_flag == 0) && (dead3_flag == 0 && dead4_flag == 0))
    {
        if (shmaddr[1].is_dead != 0)
        {
            dead1_flag = 1;
        }

        if (shmaddr[1].is_dead != 0)
        {
            dead2_flag = 1;
        }

        if (shmaddr[1].is_dead != 0)
        {
            dead3_flag = 1;
        }

        if (shmaddr[1].is_dead != 0)
        {
            dead4_flag = 1;
        }
    }
}