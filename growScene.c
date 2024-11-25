#include <stdio.h>
#include <stdlib.h>
#include "event.h"
#define TOTAL_GROWING_DATE 4

// 성장 씬 함수

void init_price(struct price p)     //보상 스탯 초기화
{
    p.AP = 0;
    p.DP = 0;
    p.exp = 0;
    p.HP = 0;
    p.skill = 0;
    p.SP = 0;
}

void add_price(struct price rec, struct price add)      //보상 추가
{
    rec.AP += add.AP;
    rec.DP += add.DP;
    rec.exp += add.exp;
    rec.HP += add.HP;
    rec.skill += add.skill;
    rec.SP += add.SP;
}

void trigger_event(FILE* fp, int eid, struct price p)       
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
        add_price(p, rec.select_1.prices);
    }
    else if(selected == 2) 
    {
        printf("%s\n", rec.select_2.result);
        add_price(p, rec.select_2.prices);
    }
    else if(selected == 3)
    {
        printf("%s\n", rec.select_3.result);
        add_price(p, rec.select_3.prices);
    }
    else if(selected == 4)
    {
        printf("%s\n", rec.select_1.result);
        add_price(p, rec.select_4.prices);
    }
    else if(selected == 5)
    {
        printf("%s\n", rec.select_1.result);
        add_price(p, rec.select_5.prices);
    }


    //공유메모리에 포켓몬 정보 업데이트
}

int main()
{
    struct price prices;
    int eid[TOTAL_GROWING_DATE];
    int specialEventCount = 0;
    
    FILE* fp;

    if ((fp = fopen("eventDex", "rb")) == NULL ) {
        fprintf(stderr, "파일 열기 오류\n");
        exit(1);
    }

    init_price(prices);

    //공유메모리 연결

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
        
        trigger_event(fp, eid[i], prices);
    }


}