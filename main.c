#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "player.h"
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

<<<<<<< Updated upstream
=======
void printMonsterNames()
{
    FILE* fp;
    struct monster rec;

    if ((fp = fopen("monsterDex", "rb")) == NULL)
    {
        perror("파일 열기 오류");
        exit(1);
    }
    printf("포켓몬 이름 목록: \n");

    while (fread(&rec, sizeof(struct monster), 1, fp) > 0)
    {
        printf("%s\n", rec.monster_name);
    }

    fclose(fp);
}

int findMonsterByName(const char* name, struct monster* selectedMonster)
{
    FILE* fp;
    struct monster rec;

    if ((fp = fopen("monsterDex", "rb")) == NULL)
    {
        perror("파일 열기 오류");
        return -1;
    }
    while (fread(&rec, sizeof(struct monster), 1, fp) > 0)
    {
        if (strcmp(rec.monster_name, name) == 0)
        {
            *selectedMonster = rec;
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);
    return -1;
}

// 공유 메모리에 선택한 몬스터 저장
void saveMonsterToSharedMemory(int playerID, struct monster* selectedMonster) {
    key_t key = ftok("main", 10597);
    int shmid = shmget(key, sizeof(struct player) * 4, IPC_CREAT | 0644);
    if (shmid == -1) {
        perror("shmget 실패");
        exit(1);
    }

    struct player* shmaddr = (struct player*)shmat(shmid, NULL, 0);
    if (shmaddr == (void*)-1) {
        perror("shmat 실패");
        exit(1);
    }

    // 선택한 몬스터 데이터를 공유 메모리에 저장
    shmaddr[playerID].selectedMonster = *selectedMonster;
    shmaddr[playerID].flag = 1; // 데이터 저장 완료 플래그
    printf("플레이어 %d의 몬스터 데이터가 공유 메모리에 저장되었습니다.\n", playerID);

    if (shmdt(shmaddr) == -1) {
        perror("공유 메모리 분리 실패");
        exit(1);
    }
}

>>>>>>> Stashed changes
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

int main(int argc, char* argv[])
{
<<<<<<< Updated upstream
=======
    int receivedPlayerID = atoi(argv[1]);
    struct monster selectedMonster;
    char inputName[50];


    printf("Player ID : %d\n", receivedPlayerID);
    //포켓몬 선택
    printf("포켓몬 선택 후 성장 씬으로 이동합니다./n");
    printMonsterNames();

    //포켓몬 선택
    printf("선택할 포켓몬의 이름을 입력하세요 : ");
    scanf("%s", inputName);

    if (findMonsterByName(inputName, &selectedMonster) == 0) {
        printf("선택된 포켓몬: %s\n", selectedMonster.monster_name);
        printf("속성: %s\n", selectedMonster.property);
        printf("HP: %d, 공격력: %d, 방어력: %d, 속도: %d\n",
            selectedMonster.stats.HP, selectedMonster.stats.attackPower,
            selectedMonster.stats.defensePower, selectedMonster.stats.speed);

        // 공유 메모리에 몬스터 데이터 저장
        saveMonsterToSharedMemory(receivedPlayerID, &selectedMonster);
    }
    else {
        printf("'%s' 포켓몬을 찾을 수 없습니다.\n", inputName);
        return 1;
    }

>>>>>>> Stashed changes


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