#include <stdio.h>
#include <stdlib.h>
#include "skill.h"

int main(int argc, char *argv[])
{
	struct skill rec;
	int SID;
	char c;
	FILE *fp;
    int fread_return;
    size_t rec_size;

	if (argc != 2)
	{
		fprintf(stderr, "사용법 : %s 파일이름\n", argv[0]);
		exit(1);
	}
	
	if ((fp = fopen(argv[1], "rb+")) == NULL)
	{
		fprintf(stderr, "파일 열기 오류\n");
		exit(2);
	}
	
	do {
		printf("수정할 스킬의 ID : ");
		if (scanf("%d", &SID) == 1)
		{
            rec_size = sizeof(rec.skill_name) + sizeof(rec.skill_type);
            if(SID % 100 == 0)          //공격형
            {
                rec_size += sizeof(rec.as);
			    fseek(fp, (SID) * rec_size, SEEK_SET);
                fread_return = fread(&rec, rec_size, 1, fp);
                fseek(fp, -rec_size, SEEK_CUR);
            }

            else if(SID % 100 == 1)     //버프
            {
                rec_size += sizeof(rec.bs);
			    fseek(fp, (SID) * rec_size, SEEK_SET);
                fread_return = fread(&rec, rec_size, 1, fp);
                fseek(fp, -rec_size, SEEK_CUR);
            }

            else if(SID % 100 == 2)     //디버프
            {
                rec_size += sizeof(rec.ds);
			    fseek(fp, (SID) * rec_size, SEEK_SET);
                fread_return = fread(&rec, rec_size, 1, fp);
                fseek(fp, -rec_size, SEEK_CUR);
            }
            
            else if(SID % 100 == 3)     //힐
            {
                rec_size += sizeof(rec.hs);
			    fseek(fp, (SID) * rec_size, SEEK_SET);
                fread_return = fread(&rec, rec_size, 1, fp);
                fseek(fp, -rec_size, SEEK_CUR);
            }
            else printf("부적절한 ID입니다.");
            
            
            if (fread_return > 0)
            {
                if(rec.skill_type == 0){
                    printf("스킬 ID : %6d 이름 : %4s 종류 : %4s\n", rec.SID, rec.skill_name, "공격");
                }
                else if(rec.skill_type == 1){
                    printf("스킬 ID : %6d 이름 : %4s 종류 : %4s\n", rec.SID, rec.skill_name, "버프");
                }
                else if(rec.skill_type == 2){
                    printf("스킬 ID : %6d 이름 : %4s 종류 : %4s\n", rec.SID, rec.skill_name, "디버프");
                }
                else if(rec.skill_type == 3){
                    printf("스킬 ID : %6d 이름 : %4s 종류 : %4s\n", rec.SID, rec.skill_name, "힐");
                }
                printf("새로운 이름 입력 : ");
                scanf("%s", &rec.skill_name);
                printf("새로운 스킬 종류 입력 (0: 공격, 1: 버프, 2: 디버프, 3: 힐) : ");
                scanf("%d", &rec.skill_type);

                if(rec.skill_type == 0)
                {
                    printf("\n공격형 스킬\n");
                    printf("데미지 : %3d 스킬 속성 : %3s\n", rec.as.skill_damage, rec.as.property);
                    printf("새로운 공격 스킬 입력\n데미지  스킬속성\n");
                    if(scanf("%d %s",&rec.as.skill_damage, &rec.as.property) != 2) break;
                    fwrite(&rec, rec_size, 1, fp);
                }

                if(rec.skill_type == 1)
                {
                    printf("\n버프 스킬\n");
                    if(rec.bs.buff_type == 1) printf("현재 버프 종류 : 공격력\n");
                    if(rec.bs.buff_type == 2) printf("현재 버프 종류 : 방어력\n");
                    if(rec.bs.buff_type == 3) printf("현재 버프 종류 : 공격력 + 방어력\n");
                    if(rec.bs.buff_type == 4) printf("현재 버프 종류 : 스피드\n");
                    if(rec.bs.buff_type == 5) printf("현재 버프 종류 : 공격력 + 스피드\n");
                    if(rec.bs.buff_type == 6) printf("현재 버프 종류 : 방어력 + 스피드\n");
                    if(rec.bs.buff_type == 7) printf("현재 버프 종류 : 공격력 + 방어력 + 스피드\n");

                    printf("새로운 버프 종류 입력 : ");
                    scanf("%d",&rec.bs.buff_type);

                    if(rec.bs.buff_type == 1){
                        printf("공격력 증가량 입력 : ");
                        scanf("%d", &rec.bs.attack_up_value);
                    }
                    if(rec.bs.buff_type == 2){
                        printf("방어력 증가량 입력 : ");
                        scanf("%d", &rec.bs.defense_up_value);
                    }
                    if(rec.bs.buff_type == 3){
                        printf("증가량 입력\n공격력 방어력");
                        if(scanf("%d %d", &rec.bs.attack_up_value, &rec.bs.defense_up_value) != 2) break;
                    }
                    if(rec.bs.buff_type == 4){
                        printf("스피드 증가량 입력 : ");
                        scanf("%d", &rec.bs.speed_up_value);
                    }
                    if(rec.bs.buff_type == 5){
                        printf("증가량 입력\n공격력 스피드");
                        if(scanf("%d %d", &rec.bs.attack_up_value, &rec.bs.speed_up_value) != 2) break;
                    }
                    if(rec.bs.buff_type == 6){
                        printf("증가량 입력\n방어력 스피드");
                        if(scanf("%d %d", &rec.bs.defense_up_value, &rec.bs.speed_up_value) != 2) break;
                    }
                    if(rec.bs.buff_type == 7){
                        printf("증가량 입력\n공격력 방어력 스피드");
                        if(scanf("%d %d", &rec.bs.attack_up_value, &rec.bs.defense_up_value, &rec.bs.speed_up_value) != 3) break;
                    }
                    fwrite(&rec, rec_size, 1, fp);
                }

                if(rec.skill_type == 2)
                {
                    printf("\n디버프 스킬\n");
                    if(rec.ds.debuff_type == 1) printf("현재 디버프 종류 : 공격력\n");
                    if(rec.ds.debuff_type == 2) printf("현재 디버프 종류 : 방어력\n");
                    if(rec.ds.debuff_type == 3) printf("현재 디버프 종류 : 공격력 + 방어력\n");
                    if(rec.ds.debuff_type == 4) printf("현재 디버프 종류 : 스피드\n");
                    if(rec.ds.debuff_type == 5) printf("현재 디버프 종류 : 공격력 + 스피드\n");
                    if(rec.ds.debuff_type == 6) printf("현재 디버프 종류 : 방어력 + 스피드\n");
                    if(rec.ds.debuff_type == 7) printf("현재 디버프 종류 : 공격력 + 방어력 + 스피드\n");

                    printf("새로운 디버프 종류 입력 : ");
                    scanf("%d",&rec.ds.debuff_type);

                    if(rec.ds.debuff_type == 1){
                        printf("공격력 감소량 입력 : ");
                        scanf("%d", &rec.ds.attack_down_value);
                    }
                    if(rec.ds.debuff_type == 2){
                        printf("방어력 감소량 입력 : ");
                        scanf("%d", &rec.ds.defense_down_value);
                    }
                    if(rec.ds.debuff_type == 3){
                        printf("감소량 입력\n공격력 방어력");
                        if(scanf("%d %d", &rec.ds.attack_down_value, &rec.ds.defense_down_value) != 2) break;
                    }
                    if(rec.ds.debuff_type == 4){
                        printf("스피드 감소량 입력 : ");
                        scanf("%d", &rec.ds.speed_down_value);
                    }
                    if(rec.ds.debuff_type == 5){
                        printf("감소량 입력\n공격력 스피드");
                        if(scanf("%d %d", &rec.ds.attack_down_value, &rec.ds.speed_down_value) != 2) break;
                    }
                    if(rec.ds.debuff_type == 6){
                        printf("감소량 입력\n방어력 스피드");
                        if(scanf("%d %d", &rec.ds.defense_down_value, &rec.ds.speed_down_value) != 2) break;
                    }
                    if(rec.ds.debuff_type == 7){
                        printf("감소량 입력\n공격력 방어력 스피드");
                        if(scanf("%d %d", &rec.ds.attack_down_value, &rec.ds.defense_down_value, &rec.ds.speed_down_value) != 3) break;
                    }
                    fwrite(&rec, rec_size, 1, fp);
                }

                if(rec.skill_type == 3)
                {
                    printf("\n힐 스킬\n");
                    printf("현재 힐량 : %3s\n", rec.hs.heal_value);
                    printf("새로운 힐량 입력 : ");
                    scanf("%d %s",&rec.hs.heal_value);
                    fwrite(&rec, rec_size, 1, fp);
                }

            }
            
            else printf("레코드 %d 없음\n", SID);

			
		}
		else printf("입력오류\n");

		printf("계속하겠습니까? (Y/N)");
		scanf(" %c", &c);
	} while (c == 'Y');
	fclose(fp);
	exit(0);
}
