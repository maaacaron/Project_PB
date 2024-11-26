#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include "player.h"

//플레이어 입력 
int scan_playerID()
{
    int PID;
    printf("플레이어의 아이디를 입력하세요 : ");
    scanf("%d", &PID);

    return PID;
}

int main()
{
    int playerID = scan_playerID();
    int shmid;
    key_t key;
    int pid;
    struct player* shmaddr;

    key = ftok("keyServerShm_1", 1);
    shmid = shmget(key, 1, IPC_CREAT|0644);
    if(shmid == -1)
    {
        perror("shmget");
        exit(1);
    }

    shmaddr = (struct player*)shmat(shmid, NULL, 0);
    
    while(1)
    {
        if(shmaddr->flag != 1)
        {
            if(shmaddr->isServerRunning != 1)   //서버가 실행되고 있지 않으면
            {
                shmaddr->isServerRunning = 1;   //서버 실행중 업데이트
                if((pid = fork()) == 0)
                {
                    execl("./server", "server", NULL);  //서버 실행
                    exit(1);
                }
            } 
            shmaddr->flag = 1;                  //서버에 값 넘겨주는 중 표시
            shmaddr->playerID = playerID;
            shmaddr->processID = getpid();
            shmaddr->isMyTurn = 0;
            break;
        }
    }

    char playerIDStr[10];

    pid = fork();
    if (pid == 0)
    {
        printf("자식프로세스에서 main.c 실행");
        execl("./main", "main", playerIDStr, NULL);
    }


}