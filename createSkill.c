#include <stdio.h>
#include <stdlib.h>
#include "skill.h"
#define START_ID 0

int main(int argc, char* argv[])
{

    struct skill rec;
    size_t rec_size;
    char c;

	FILE *fp;
	if(argc != 2){
		fprintf(stderr, "사용법: %s 파일이름\n", argv[0]);
		exit(1);
	}
	fp = fopen(argv[1], "wb");

    do{
        printf("스킬 종류 (0 : 공격, 1 : 버프, 2 : 디버프, 3 : 힐) : ");
        scanf("%d", &rec.skill_type);

        rec_size = sizeof(rec.skill_name) + sizeof(rec.skill_type);

        if(rec.skill_type == 0){        //공격 스킬
            rec_size += sizeof(rec.as);
            printf("%3s %15s\n", "스킬ID", "스킬 이름");
            if(scanf("%d %s", &rec.SID, &rec.skill_name) != 2) break;

            printf("데미지  속성\n");
            if(scanf("%d %s", &rec.as.skill_damage, &rec.as.property) != 2) break;

            fseek(fp, (rec.SID * rec_size), SEEK_SET);
            fwrite(&rec.as, rec_size, 1, fp);
        }



        //내일의 나에게
        //레코드 사이즈 지금 문제있음
        //저장한 위치랑 불러오는 위치랑 다름
        //잘 조정해야됨







        else if(rec.skill_type == 1){        //버프 스킬
            rec_size += sizeof(rec.bs);
            printf("스킬ID  스킬 이름\n");
            if(scanf("%d %s", &rec.SID, &rec.skill_name) != 2) break;

            printf("버프 타입 (1: 공격력, 2: 방어력, 3: 스피드, 4: 공격력 + 방어력, 5: 공격력 + 스피드, 6: 방어력 + 스피드, 7: 전부)\n");   //원래 스피드 = 4, 공+방 = 3이므로 이후에 둘 변환
            if(scanf("%d", &rec.bs.buff_type) != 1) break;
             
            if(rec.bs.buff_type == ATTACK){
                printf("공격력 증가량 입력 :");
                if(scanf("%d", &rec.bs.attack_up_value) != 1) break;
            }
            else if(rec.bs.buff_type == DEFENSE){
                printf("방어력 증가량 입력 :");
                if(scanf("%d", &rec.bs.defense_up_value) != 1) break;
            }
            else if(rec.bs.buff_type == 3){
                rec.bs.buff_type = SPEED;
                printf("스피드 증가량 입력 :");
                if(scanf("%d", &rec.bs.speed_up_value) != 1) break;
            }
            else if(rec.bs.buff_type == 4){
                rec.bs.buff_type = ATTACK + DEFENSE;
                printf("버프 수치 입력\n%5d %5d\n", "공격력", "방어력");
                if(scanf("%d %d", &rec.bs.attack_up_value, &rec.bs.defense_up_value) != 2) break;
            }
            else if(rec.bs.buff_type == ATTACK + SPEED){
                printf("버프 수치 입력\n%5d %5d\n", "공격력", "스피드");
                if(scanf("%d %d", &rec.bs.attack_up_value, &rec.bs.speed_up_value) != 2) break;
            }
            else if(rec.bs.buff_type == DEFENSE + SPEED){
                printf("버프 수치 입력\n%5d %5d", "방어력", "스피드");
                if(scanf("%d %d", &rec.bs.defense_up_value, &rec.bs.speed_up_value) != 2) break;
            }
            else if(rec.bs.buff_type == ATTACK + DEFENSE + SPEED){
                printf("버프 수치 입력\n%5d %5d %5d", "공격력", "방어력", "스피드");
                if(scanf("%d %d %d", &rec.bs.attack_up_value, &rec.bs.defense_up_value, &rec.bs.speed_up_value) != 3) break;
            }

            fseek(fp, ((rec.SID) * rec_size), SEEK_SET);
            fwrite(&rec.bs, rec_size, 1, fp);
        }

        else if(rec.skill_type == 2){        //디버프 스킬
            rec_size += sizeof(rec.ds);
            printf("스킬ID  스킬 이름\n");
            if(scanf("%d %s", &rec.SID, &rec.skill_name) != 2) break;

            printf("디버프 타입 (1: 공격력, 2: 방어력, 3: 스피드, 4: 공격력 + 방어력, 5: 공격력 + 스피드, 6: 방어력 + 스피드, 7: 전부)\n");
            if(scanf("%d", &rec.ds.debuff_type) != 1) break;
                
            if(rec.ds.debuff_type == ATTACK){
                printf("공격력 감소량 입력 : ");
                if(scanf("%d", &rec.ds.attack_down_value) != 1) break;
            }
            else if(rec.ds.debuff_type == DEFENSE){
                printf("방어력 감소량 입력 : ");
                if(scanf("%d", &rec.ds.defense_down_value) != 1) break;
            }
            else if(rec.ds.debuff_type == 3){
                rec.ds.debuff_type = SPEED;
                printf("스피드 감소량 입력 : ");
                if(scanf("%d", &rec.ds.speed_down_value) != 1) break;
            }
            else if(rec.ds.debuff_type == 4){
                rec.ds.debuff_type = ATTACK + DEFENSE;
                printf("디버프 수치 입력\n%5d %5d", "공격력", "방어력");
                if(scanf("%d %d", &rec.ds.attack_down_value, &rec.ds.defense_down_value) != 2) break;
            }
            else if(rec.ds.debuff_type == ATTACK + SPEED){
                printf("디버프 수치 입력\n%5d %5d", "공격력", "스피드");
                if(scanf("%d %d", &rec.ds.attack_down_value, &rec.ds.speed_down_value) != 2) break;
            }
            else if(rec.ds.debuff_type == DEFENSE + SPEED){
                printf("디버프 수치 입력\n%5d %5d", "방어력", "스피드");
                if(scanf("%d %d", &rec.ds.defense_down_value, &rec.ds.speed_down_value) != 2) break;
            }
            else if(rec.ds.debuff_type == ATTACK + DEFENSE + SPEED){
                printf("디버프 수치 입력\n%5d %5d %5d", "공격력", "방어력", "스피드");
                if(scanf("%d %d %d", &rec.ds.attack_down_value, &rec.ds.defense_down_value, &rec.ds.speed_down_value) != 3) break;
            }

            fseek(fp, ((rec.SID) * rec_size), SEEK_SET);
            fwrite(&rec.ds, rec_size, 1, fp);
        }

        else if(rec.skill_type == 3){        //힐 스킬
            rec_size += sizeof(rec.hs);
            printf("스킬ID  스킬 이름\n");
            if(scanf("%d %s", &rec.SID, &rec.skill_name) != 2) break;

            printf("힐량 입력 : ");
            if(scanf("%d", &rec.hs.heal_value) != 1) break;
            fseek(fp, ((rec.SID) * rec_size), SEEK_SET);
            fwrite(&rec.hs, rec_size, 1, fp);
        }
        
        else printf("입력 오류");

		printf("계속하겠습니까? (Y/N)");
		scanf(" %c", &c);
	} while (c == 'Y');

	fclose(fp);
	exit(0);
}
