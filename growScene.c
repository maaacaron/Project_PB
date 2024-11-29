#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "event.h"
#include "player.h"
#define TOTAL_GROWING_DATE 4

// 성장 씬 함수 
struct player *shmaddr;

void add_skill(int playerID, struct price add)
{
    struct monsterSkill *rec = &shmaddr[playerID].selectedMonster.skills;
    if (rec->skill_2_ID = -1) rec->skill_2_ID = add.skill;
    else if (rec->skill_3_ID = -1) rec->skill_3_ID = add.skill;
    else if (rec->skill_4_ID = -1) rec->skill_4_ID = add.skill;
}

void add_price(int playerID, struct price add)      //보상 추가
{
    if(add.skill != -1)
    {
        add_skill(playerID, add);
        return;
    }
    struct monsterStats *rec = &shmaddr[playerID].selectedMonster.stats;
    rec->attackPower += add.AP;
    rec->defensePower += add.DP;
    rec->HP += add.HP;
    rec->speed += add.SP;
}

void trigger_event(FILE* fp, int playerID, int eid)       
{
    struct event rec;
    int selected;

    fseek(fp, eid * sizeof(rec), SEEK_SET);
    fread(&rec, sizeof(rec),  1,  fp);
    printf("%s\n", rec.story);

    for(int i = 0; i < rec.choice_num; i++)
    {
        if(i == 0) printf("%s\n", rec.select_1.choice);
        else if(i == 1) printf("%s\n", rec.select_2.choice);
        else if(i == 2) printf("%s\n", rec.select_3.choice);
        else if(i == 3) printf("%s\n", rec.select_4.choice);
        else if(i == 4) printf("%s\n", rec.select_5.choice);
    }

    printf("원하는 선택지를 고르세요(1 ~ %d) : ", rec.choice_num);
    scanf("%d", &selected);
    if(selected == 1) 
    {
        printf("%s\n", rec.select_1.result);
        add_price(playerID, rec.select_1.prices);
    }
    else if(selected == 2) 
    {
        printf("%s\n", rec.select_2.result);
        add_price(playerID, rec.select_2.prices);
    }
    else if(selected == 3)
    {
        printf("%s\n", rec.select_3.result);
        add_price(playerID, rec.select_3.prices);
    }
    else if(selected == 4)
    {
        printf("%s\n", rec.select_1.result);
        add_price(playerID, rec.select_4.prices);
    }
    else if(selected == 5)
    {
        printf("%s\n", rec.select_1.result);
        add_price(playerID, rec.select_5.prices);
    }
}

void* make_shared_memory()          //공유 메모리 생성 및 연결 
{
    int shmid;
    key_t key;

    key = ftok("main", 10597);
    shmid = shmget(key, sizeof(struct player) * 4, 0);
    if(shmid == -1)
    {
        perror("shmget");
        exit(1);
    }

    printf("shmid : %d", shmid);
    shmaddr = (struct player*)shmat(shmid, NULL, 0);
}

int main(int argc, char* argv[])
{
    int eid[TOTAL_GROWING_DATE];
    int specialEventCount = 0;
    int receivedPlayerID = atoi(argv[1]);
    
    FILE* fp;

    if ((fp = fopen("eventDex", "rb")) == NULL ) {
        fprintf(stderr, "파일 열기 오류\n");
        exit(1);
    }

     ();

    for(int i = 0; i < TOTAL_GROWING_DATE; i++)
    {
        eid[i] = rand() % MAX_NORMALEVENT_NUM;
        for(int j = 0; j < i; j++)
        {
            if(eid[i] == eid[j]) 
            {
                if(specialEventCount < 4) eid[i] = MAX_NORMALEVENT_NUM + ++specialEventCount;      //같은 이벤트가 나오면 스페셜 이벤트로 전환(최대 7회)
            }
        }
        
        trigger_event(fp, receivedPlayerID, eid[i]);
    }


}