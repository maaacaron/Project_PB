#include <stdio.h>
#include <stdlib.h>
#include "skill.h"


 int main(int argc, char *argv[])
 {
    struct attackSkill rec_AS;
    struct buffSkill rec_BS;
    struct debuffSkill rec_DS;
    struct healSkill rec_HS;

    char c;
    int sid;

    FILE *fp;
    if (argc != 2) {
        fprintf(stderr, "사용법: %s 파일이름\n", argv[0]);
        exit(1);
    }

    if ((fp = fopen(argv[1], "rb")) == NULL ) {
        fprintf(stderr, "파일 열기 오류\n");
        exit(2);
    }

    
    do {
        printf("검색할 스킬의 SID 입력: ");
        if (scanf("%d", &sid) == 1) {
            if(sid % 100 == 0)
            {
                fseek(fp, sid * sizeof(rec_AS), SEEK_SET);
                if (fread(&rec_AS,  sizeof(rec_AS),  1,  fp) > 0) 
                {
                    printf("스킬 ID : %d  스킬이름 : %s  타입 : %s\n", rec_AS.sid, rec_AS.skill_name, "공격");
                    printf("데미지 : %d  속성 : %s\n", rec_AS.skill_damage, rec_AS.property);
                }
            }
            fseek(fp, START_ATTACKSKILL_ID * sizeof(rec_AS), SEEK_SET);

            if(sid % 100 == 1)
            {
                fseek(fp, (sid - START_BUFFSKILL_ID) * sizeof(rec_BS), SEEK_CUR);
                if (fread(&rec_BS,  sizeof(rec_BS),  1,  fp) > 0) 
                {
                    printf("스킬 ID : %d 이름 : %s 종류 : %s\n", rec_BS.sid, rec_BS.skill_name, "버프");

                    if(rec_BS.buff_type == 1) 
                    {
                        printf("버프 종류 : 공격력\n");
                        printf("공격력 증가량 : %d", rec_BS.attack_up_value);
                    }
                    if(rec_BS.buff_type == 2)
                    {
                        printf("버프 종류 : 방어력\n");
                        printf("방어력 증가량 : %d", rec_BS.defense_up_value);
                    }
                    if(rec_BS.buff_type == 3)
                    {
                        printf("버프 종류 : 공격력 + 방어력\n");
                        printf("공격력 증가량 : %d  방어력 증가량 : %d", rec_BS.attack_up_value, rec_BS.defense_up_value);
                    }
                    if(rec_BS.buff_type == 4)
                    {
                        printf("버프 종류 : 속도\n");
                        printf("속도 증가량 : %d", rec_BS.speed_up_value);
                    }
                    if(rec_BS.buff_type == 5)
                    {
                        printf("버프 종류 : 공격력 + 속도\n");
                        printf("공격력 증가량 : %d  속도 증가량 : %d", rec_BS.attack_up_value, rec_BS.defense_up_value);
                    }
                    if(rec_BS.buff_type == 6)
                    {
                        printf("버프 종류 : 방어력 + 속도\n");
                        printf("방어력 증가량 : %d  속도 증가량 : %d", rec_BS.defense_up_value, rec_BS.speed_up_value);
                    }
                    if(rec_BS.buff_type == 7)
                    {
                        printf("버프 종류 : 공격력 + 방어력 + 속도\n");
                        printf("공격력 증가량 : %d  방어력 증가량 : %d  속도 증가량 : %d", rec_BS.attack_up_value, rec_BS.defense_up_value, rec_BS.speed_up_value);
                    }
                }
            }
            fseek(fp, (START_BUFFSKILL_ID - START_ATTACKSKILL_ID) * sizeof(rec_BS), SEEK_CUR);


            if(sid % 100 == 2)  
            {
                fseek(fp, (sid - START_BUFFSKILL_ID) * sizeof(rec_DS), SEEK_CUR);
                if (fread(&rec_DS,  sizeof(rec_DS),  1,  fp) > 0) 
                {
                    printf("스킬 ID : %d 이름 : %s 종류 : %s\n", rec_DS.sid, rec_DS.skill_name, "디버프");

                    if(rec_DS.debuff_type == 1) 
                    {
                        printf("디버프 종류 : 공격력\n");
                        printf("공격력 감소량 : %d", rec_DS.attack_down_value);
                    }
                    if(rec_DS.debuff_type == 2)
                    {
                        printf("디버프 종류 : 방어력\n");
                        printf("방어력 감소량 : %d", rec_DS.defense_down_value);
                    }
                    if(rec_DS.debuff_type == 3)
                    {
                        printf("디버프 종류 : 공격력 + 방어력\n");
                        printf("공격력 감소량 : %d  방어력 감소량 : %d", rec_DS.attack_down_value, rec_DS.defense_down_value);
                    }
                    if(rec_DS.debuff_type == 4)
                    {
                        printf("디버프 종류 : 속도\n");
                        printf("속도 감소량 : %d", rec_DS.speed_down_value);
                    }
                    if(rec_DS.debuff_type == 5)
                    {
                        printf("디버프 종류 : 공격력 + 속도\n");
                        printf("공격력 감소량 : %d  속도 감소량 : %d", rec_DS.attack_down_value, rec_DS.defense_down_value);
                    }
                    if(rec_DS.debuff_type == 6)
                    {
                        printf("디버프 종류 : 방어력 + 속도\n");
                        printf("방어력 감소량 : %d  속도 감소량 : %d", rec_DS.defense_down_value, rec_DS.speed_down_value);
                    }
                    if(rec_DS.debuff_type == 7)
                    {
                        printf("디버프 종류 : 공격력 + 방어력 + 속도\n");
                        printf("공격력 감소량 : %d  방어력 감소량 : %d  속도 감소량 : %d", rec_DS.attack_down_value, rec_DS.defense_down_value, rec_DS.speed_down_value);
                    }
                }
            }
            fseek(fp, (START_DEBUFFSKILL_ID - START_BUFFSKILL_ID) * sizeof(rec_DS), SEEK_CUR);

            
            if(sid % 100 == 3)
            {
                fseek(fp, (sid - START_DEBUFFSKILL_ID) * sizeof(rec_HS), SEEK_CUR);
                if (fread(&rec_HS,  sizeof(rec_HS),  1,  fp) > 0) 
                {
                    printf("스킬 ID : %d  스킬이름 : %s  타입 : %s  힐량 : %d\n", rec_HS.sid, rec_HS.skill_name, "힐", rec_HS.heal_value);
                }
            }

        }
        else printf("입력 오류");

        printf("계속하겠습니까?(Y/N)");
        scanf(" %c", &c);
    } while (c == 'Y');
    fclose(fp);
    exit(0);
 }