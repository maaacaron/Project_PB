#include <stdio.h>
#include <stdlib.h>
#include "event.h"
#define START_EID 0


int scan_event(FILE* fp)
{
    struct event rec;
    int choice_num;

    printf("이벤트 ID : ");
    scanf("%d", &rec.event_id);

    //이벤트 스토리 불러와서 rec.story에 저장
    


    printf("선택지 개수 입력 : ");
    scanf("%d", &choice_num);

    for(int i = 1; i <= choice_num; i++)        //선택지 및 보상 입력
    {
        if(i == 1)
        {
            printf("선택지 %d 스토리 입력 : ", i);      //띄어쓰기 있는 경우도 알아서 잘 넣도록 패치 필요
            scanf("%s", &rec.select_1.choice);
            printf("선택지 %d 보상 입력\n", i);
            printf("exp  HP  공격력  방어력  속도  스킬id(얻는 스킬이 없는 경우 -1)");
            if(scanf("%d %d %d %d %d %d", &rec.select_1.prices.exp, &rec.select_1.prices.HP, &rec.select_1.prices.AP, 
                                        &rec.select_1.prices.DP, &rec.select_1.prices.SP, &rec.select_1.prices.skill) != 6) return 0;
        }

        else if(i == 2)
        {
            printf("선택지 %d 스토리 입력 : ", i);
            scanf("%s", &rec.select_2.choice);
            printf("선택지 %d 보상 입력\n", i);
            printf("exp  HP  공격력  방어력  속도  스킬id(얻는 스킬이 없는 경우 -1)");
            if(scanf("%d %d %d %d %d %d", &rec.select_2.prices.exp, &rec.select_2.prices.HP, &rec.select_2.prices.AP, 
                                        &rec.select_2.prices.DP, &rec.select_2.prices.SP, &rec.select_2.prices.skill) != 6) return 0;
        }
        
        else if(i == 3)
        {
            printf("선택지 %d 스토리 입력 : ", i);
            scanf("%s", &rec.select_3.choice);
            printf("선택지 %d 보상 입력\n", i);
            printf("exp  HP  공격력  방어력  속도  스킬id(얻는 스킬이 없는 경우 -1)");
            if(scanf("%d %d %d %d %d %d", &rec.select_3.prices.exp, &rec.select_3.prices.HP, &rec.select_3.prices.AP, 
                                        &rec.select_3.prices.DP, &rec.select_3.prices.SP, &rec.select_3.prices.skill) != 6) return 0;
        }

        else if(i == 4)
        {
            printf("선택지 %d 스토리 입력 : ", i);
            scanf("%s", &rec.select_4.choice);
            printf("선택지 %d 보상 입력\n", i);
            printf("exp  HP  공격력  방어력  속도  스킬id(얻는 스킬이 없는 경우 -1)");
            if(scanf("%d %d %d %d %d %d", &rec.select_4.prices.exp, &rec.select_4.prices.HP, &rec.select_4.prices.AP, 
                                        &rec.select_4.prices.DP, &rec.select_4.prices.SP, &rec.select_4.prices.skill) != 6) return 0;
        }

        else if(i == 5)
        {
            printf("선택지 %d 스토리 입력 : ", i);
            scanf("%s", &rec.select_5.choice);
            printf("선택지 %d 보상 입력\n", i);
            printf("exp  HP  공격력  방어력  속도  스킬id(얻는 스킬이 없는 경우 -1)");
            if(scanf("%d %d %d %d %d %d", &rec.select_5.prices.exp, &rec.select_5.prices.HP, &rec.select_5.prices.AP, 
                                        &rec.select_5.prices.DP, &rec.select_5.prices.SP, &rec.select_5.prices.skill) != 6) return 0;
        }
    }
    fseek(fp, rec.event_id * sizeof(rec), SEEK_SET);
    fwrite(&rec, sizeof(rec), 1, fp);

    return 1;
}

int main(int argc, char *argv[])
{
    char c;

    FILE *fp;
    if (argc != 2) {
        fprintf(stderr, "사용법: %s 파일이름\n", argv[0]);
        exit(1);
    }

    if ((fp = fopen(argv[1], "wb")) == NULL ) {
        fprintf(stderr, "파일 열기 오류\n");
        exit(2);
    }


    do{
        if(scan_event(fp) == 0) break;
        
        printf("계속하겠습니까? (Y/N)");
		scanf(" %c", &c);
    } while(c == 'Y');
}