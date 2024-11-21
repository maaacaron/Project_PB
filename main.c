#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "monster.h"

char* make_shared_memory()          //공유 메모리 생성 및 연결
{
    int shmid;
    key_t key;
    char *shmaddr;

    key = ftok("main", 10597);
    shmid = shmget(key, 1024, IPC_CREAT|0644);
    if(shmid == -1)
    {
        perror("shmget");
        exit(1);
    }

    printf("shmid : %d", shmid);
    shmaddr = (char*)shmat(shmid, NULL, 0);

    return shmaddr;
}

int main()
{
    int playerID;
    
    printf("사용자의 아이디(숫자)를 입력해주세요 : ");
    scanf("%d", &playerID);


}