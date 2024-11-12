#include <stdio.h>
#include <stdlib.h>
#include "skill.h"
#define START_ID 0

int main(int argc, char* argv[])
{

    struct attackSkill rec_AS;
    struct buffSkill rec_BS;
    struct debuffSkill rec_DS;
    struct healSkill rec_HS;

    char c;
    int skill_type;

	FILE *fp;
	if(argc != 2){
		fprintf(stderr, "사용법: %s 파일이름\n", argv[0]);
		exit(1);
	}
	fp = fopen(argv[1], "wb");

    do{
        printf("스킬 종류 (0 : 공격, 1 : 버프, 2 : 디버프, 3 : 힐) : ");
        scanf("%d", skill_type);

        if(skill_type == 0){        //공격 스킬
            printf("%3s %15s\n", "스킬ID", "스킬 이름");
            if(scanf("%d %s", &rec_AS.SID, &rec_AS.skill_name) != 2) break;

            printf("데미지  속성\n");
            if(scanf("%d %s", &rec_AS.skill_damage, &rec_AS.property) != 2) break;

            fseek(fp, rec_AS.SID * sizeof(rec_AS), SEEK_SET);
            fwrite(&rec_AS, sizeof(rec_AS), 1, fp);
        }


        else if(skill_type == 1){        //버프 스킬
            printf("스킬ID  스킬 이름\n");
            if(scanf("%d %s", &rec_BS.SID, &rec_BS.skill_name) != 2) break;

            printf("버프 타입 (1: 공격력, 2: 방어력, 3: 스피드, 4: 공격력 + 방어력, 5: 공격력 + 스피드, 6: 방어력 + 스피드, 7: 전부)\n");   //원래 스피드 = 4, 공+방 = 3이므로 이후에 둘 변환
            if(scanf("%d", &rec_BS.buff_type) != 1) break;
             
            if(rec_BS.buff_type == ATTACK){
                printf("공격력 증가량 입력 :");
                if(scanf("%d", &rec_BS.attack_up_value) != 1) break;
            }
            else if(rec_BS.buff_type == DEFENSE){
                printf("방어력 증가량 입력 :");
                if(scanf("%d", &rec_BS.defense_up_value) != 1) break;
            }
            else if(rec_BS.buff_type == 3){
                rec_BS.buff_type = SPEED;
                printf("스피드 증가량 입력 :");
                if(scanf("%d", &rec_BS.speed_up_value) != 1) break;
            }
            else if(rec_BS.buff_type == 4){
                rec_BS.buff_type = ATTACK + DEFENSE;
                printf("버프 수치 입력\n%5d %5d\n", "공격력", "방어력");
                if(scanf("%d %d", &rec_BS.attack_up_value, &rec_BS.defense_up_value) != 2) break;
            }
            else if(rec_BS.buff_type == ATTACK + SPEED){
                printf("버프 수치 입력\n%5d %5d\n", "공격력", "스피드");
                if(scanf("%d %d", &rec_BS.attack_up_value, &rec_BS.speed_up_value) != 2) break;
            }
            else if(rec_BS.buff_type == DEFENSE + SPEED){
                printf("버프 수치 입력\n%5d %5d", "방어력", "스피드");
                if(scanf("%d %d", &rec_BS.defense_up_value, &rec_BS.speed_up_value) != 2) break;
            }
            else if(rec_BS.buff_type == ATTACK + DEFENSE + SPEED){
                printf("버프 수치 입력\n%5d %5d %5d", "공격력", "방어력", "스피드");
                if(scanf("%d %d %d", &rec_BS.attack_up_value, &rec_BS.defense_up_value, &rec_BS.speed_up_value) != 3) break;
            }

            fseek(fp, START_BUFFSKILL_ID * sizeof(rec_AS), SEEK_SET);                   //위치 +100(공격스킬 크기)
            fseek(fp, (rec_BS.SID - START_BUFFSKILL_ID) * sizeof(rec_BS), SEEK_CUR);    //위치 +(SID - 100)(버프스킬 크기)
            fwrite(&rec_BS, sizeof(rec_BS), 1, fp);
        }

        else if(skill_type == 2){        //디버프 스킬
            printf("스킬ID  스킬 이름\n");
            if(scanf("%d %s", &rec_DS.SID, &rec_DS.skill_name) != 2) break;

            printf("디버프 타입 (1: 공격력, 2: 방어력, 3: 스피드, 4: 공격력 + 방어력, 5: 공격력 + 스피드, 6: 방어력 + 스피드, 7: 전부)\n");
            if(scanf("%d", &rec_DS.debuff_type) != 1) break;
                
            if(rec_DS.debuff_type == ATTACK){
                printf("공격력 감소량 입력 : ");
                if(scanf("%d", &rec_DS.attack_down_value) != 1) break;
            }
            else if(rec_DS.debuff_type == DEFENSE){
                printf("방어력 감소량 입력 : ");
                if(scanf("%d", &rec_DS.defense_down_value) != 1) break;
            }
            else if(rec_DS.debuff_type == 3){
                rec_DS.debuff_type = SPEED;
                printf("스피드 감소량 입력 : ");
                if(scanf("%d", &rec_DS.speed_down_value) != 1) break;
            }
            else if(rec_DS.debuff_type == 4){
                rec_DS.debuff_type = ATTACK + DEFENSE;
                printf("디버프 수치 입력\n%5d %5d", "공격력", "방어력");
                if(scanf("%d %d", &rec_DS.attack_down_value, &rec_DS.defense_down_value) != 2) break;
            }
            else if(rec_DS.debuff_type == ATTACK + SPEED){
                printf("디버프 수치 입력\n%5d %5d", "공격력", "스피드");
                if(scanf("%d %d", &rec_DS.attack_down_value, &rec_DS.speed_down_value) != 2) break;
            }
            else if(rec_DS.debuff_type == DEFENSE + SPEED){
                printf("디버프 수치 입력\n%5d %5d", "방어력", "스피드");
                if(scanf("%d %d", &rec_DS.defense_down_value, &rec_DS.speed_down_value) != 2) break;
            }
            else if(rec_DS.debuff_type == ATTACK + DEFENSE + SPEED){
                printf("디버프 수치 입력\n%5d %5d %5d", "공격력", "방어력", "스피드");
                if(scanf("%d %d %d", &rec_DS.attack_down_value, &rec_DS.defense_down_value, &rec_DS.speed_down_value) != 3) break;
            }

            fseek(fp, START_BUFFSKILL_ID * sizeof(rec_AS), SEEK_SET);                           //위치 +100(공격스킬 크기)
            fseek(fp, (START_DEBUFFSKILL_ID - START_BUFFSKILL_ID) * sizeof(rec_BS), SEEK_CUR);  //위치 +100(버프스킬 크기)
            fseek(fp, (rec_BS.SID - START_DEBUFFSKILL_ID) * sizeof(rec_DS), SEEK_CUR);    //위치 +(SID - 200)(디버프스킬 크기)
            fwrite(&rec_DS, sizeof(rec_DS), 1, fp);
        }

        else if(skill_type == 3){        //힐 스킬
            printf("스킬ID  스킬 이름\n");
            if(scanf("%d %s", &rec_HS.SID, &rec_HS.skill_name) != 2) break;

            printf("힐량 입력 : ");
            if(scanf("%d", &rec_HS.heal_value) != 1) break;

            fseek(fp, START_BUFFSKILL_ID * sizeof(rec_AS), SEEK_SET);                           //위치 +100(공격스킬 크기)
            fseek(fp, (START_DEBUFFSKILL_ID - START_BUFFSKILL_ID) * sizeof(rec_BS), SEEK_CUR);  //위치 +100(버프스킬 크기)
            fseek(fp, (START_HEALSKILL_ID - START_DEBUFFSKILL_ID) * sizeof(rec_DS), SEEK_CUR);  //위치 +100(디버프스킬 크기)
            fseek(fp, ((rec_HS.SID - START_HEALSKILL_ID) * sizeof(rec_HS)), SEEK_CUR);
            fwrite(&rec_HS, sizeof(rec_HS), 1, fp);
        }
        
        else printf("입력 오류");

		printf("계속하겠습니까? (Y/N)");
		scanf(" %c", &c);
	} while (c == 'Y');

	fclose(fp);
	exit(0);
}
