#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
//#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "player.h"

int main()                          //마지막 플레이어까지 접속하여 정보를 받아올 때까지 무한반복하며 대기
{
    struct player* shmaddr;
    key_t key;
    int shmid;

    int fd;
    int player[4][2];               //플레이어 아이디 + 프로세스 아이디

    int accessCount = 0;

    key = ftok("/home/g_202111097/teamProject/keyServerShm_1", 1);
    shmid = shmget(key, 1, IPC_CREAT|0644);
    if(shmid == -1)
    {
        perror("shmget");
        exit(1);
    }

    shmaddr = (char*)shmat(shmid, NULL, 0);


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
    
    unlink("serverPipe");
    fd = open("serverPipe", O_RDONLY);
}