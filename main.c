#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
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

void callGrowScene()
{
    int child, status, pid
    pid = fork();
    if (pid == 0)
    {
        execl("./growScene", "growScene", NULL); //성장 씬 실행
        perror("execl 실패");
        exit(1);
    }
    else //부모 프로세스
    {
        child = wait(&status);
        printf("자식프로세스 %d 종료. 성장씬이 종료되었습니다./n", child);
    }
}

void callBattleScene()
{
    int child, status, pid;
    pid = fork();
    if (pid == 0)
    {
        execl("./battleScene.c", "battleScene", NULL);
        perror("execl 실패");
        exit(1);
    }
    else //부모 프로세스
    {
        child = wait(&status);
        printf("자식프로세스 %d 종료. 배틀씬이 종료되었습니다. 수고하셨습니다./n", child);
    }
}

int main()
{
    //포켓몬 선택
    printf("포켓몬 선택 후 성장 씬으로 이동합니다./n");

    //포켓몬 선택이 끝났을 때
    printf("포켓몬 선택이 완료되었습니다./n");

    //성장씬으로
    callGrowScene();

    printf("배틀 씬으로 이동합니다./n");

    //배틀씬으로
    callBattleScene();

    //두번째 성장씬으로
    callGrowScene();

    printf("마지막 배틀!/n");
    callBattleScene();

    printf("게임 종료");
    return 0;
    
    //성장씬으로 넘어감 growScene 호출 fork()한뒤 자식 프로세스 끝날때까지 wait
    //배틀씬 호출 fork() 자식 프로세스 끝날때까지 wait
    //승자 패자 구분하는 거 구현해 승자만 남겨두고 패자는 exit 승자만 다시 fork한뒤 성장씬 그리고 다시 돌아와 마지막 배틀씬
}