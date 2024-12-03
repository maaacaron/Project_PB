#include <stdio.h>
#include <stdlib.h>
#include "skill.h" // 스킬 종류 구조체 저장된 헤더파일 

void Run_attackSkill(FILE* fp, struct attackSkill rec_AS)
{
    // 공격 스킬의 정보 출력
    if (fread(&rec_AS,  sizeof(rec_AS),  1,  fp) > 0) 
    {
        printf("스킬 ID : %d  스킬이름 : %s  타입 : %s\n", rec_AS.sid, rec_AS.skill_name, "공격");
        printf("데미지 : %d  속성 : %s\n", rec_AS.skill_damage, rec_AS.property);
    }
}

void Run_buffSkill(FILE* fp, struct buffSkill rec_BS)
{
    printf("현 위치 : %ld\n", ftell(fp));
    // 버프 스킬의 정보 출력
    if (fread(&rec_BS,  sizeof(rec_BS),  1,  fp) > 0) 
    {
        printf("스킬 ID : %d 이름 : %s 종류 : %s\n", rec_BS.sid, rec_BS.skill_name, "버프");

        // 버프 종류| 1: 공격력 2: 방어력 3: 공격력 + 방어력 4: 속도 5: 공격력 + 속도 6: 방어력 + 속도
        // 버프 종류| 7: 공격력 + 방어력 + 속도
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
        printf("\n");
    }
}

void Run_debuffSkill(FILE* fp, struct debuffSkill rec_DS)
{
    // 디버프 스킬의 정보 출력
    if (fread(&rec_DS,  sizeof(rec_DS),  1,  fp) > 0) 
    {
        printf("스킬 ID : %d 이름 : %s 종류 : %s\n", rec_DS.sid, rec_DS.skill_name, "디버프");

        // 디버프 종류| 1: 공격력 2: 방어력 3: 공격력 + 방어력 4: 속도 5: 공격력 + 속도 6: 방어력 + 속도
        // 디버프 종류| 7: 공격력 + 방어력 + 속도
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
        printf("\n");
    }
}

void Run_healSkill(FILE* fp, struct healSkill rec_HS)
{
    // 힐 스킬의 정보 출력
    if (fread(&rec_HS,  sizeof(rec_HS),  1,  fp) > 0) 
    {
        printf("스킬 ID : %d  스킬이름 : %s  타입 : %s  힐량 : %d\n", rec_HS.sid, rec_HS.skill_name, "힐", rec_HS.heal_value);
    }
}


void Ready_skill(FILE* fp, int sid) 
{
    struct attackSkill rec_AS;
    struct buffSkill rec_BS;
    struct debuffSkill rec_DS;
    struct healSkill rec_HS;

    // sid 값에 따라 해당 스킬 영역에 접근
    if (sid >= 0 && sid < 100) {  // AS 영역
        // sid가 0~99 범위에 있을 때, AS 영역의 데이터에 접근
        if (sid < 8) {  // 실제로 저장된 AS 항목이 8개이므로, sid가 0~7인 경우만 유효
            fseek(fp, sid * sizeof(rec_AS), SEEK_SET);
            Run_attackSkill(fp, rec_AS);
        }
        else {
            printf("유효하지 않은 SID입니다. AS 범위 내에 SID가 없습니다.\n");
        }
    }
    else if (sid >= 100 && sid < 200) {  // BS 영역
        // sid가 100~199 범위에 있을 때, BS 영역으로 이동
        if (sid - 100 < 3) {  // 실제로 저장된 BS 항목이 3개이므로, sid가 100~102인 경우만 유효
            fseek(fp, 8 * sizeof(rec_AS), SEEK_SET);  // AS 영역을 넘기기
            fseek(fp, (sid - 100) * sizeof(rec_BS), SEEK_CUR);  // BS 영역으로 이동
            Run_buffSkill(fp, rec_BS);
        }
        else {
            printf("유효하지 않은 SID입니다. BS 범위 내에 SID가 없습니다.\n");
        }
    }
    else if (sid >= 200 && sid < 300) {  // DS 영역
        if (sid - 200 < 2) {  // 실제로 저장된 DS 항목이 2개이므로, sid가 200~201인 경우만 유효
            fseek(fp, 8 * sizeof(rec_AS), SEEK_SET);  // AS 영역을 넘기기
            fseek(fp, 3 * sizeof(rec_BS), SEEK_CUR);  // BS 영역을 넘기기
            fseek(fp, (sid - 200) * sizeof(rec_DS), SEEK_CUR);  // DS 영역으로 이동
            Run_debuffSkill(fp, rec_DS);
        }
        else {
            printf("유효하지 않은 SID입니다. DS 범위 내에 SID가 없습니다.\n");
        }
    }
    else if (sid >= 300 && sid < 400) {  // HS 영역
        if (sid - 300 < 1) {  // 실제로 저장된 HS 항목이 1개이므로, sid가 300~300인 경우만 유효
            fseek(fp, 8 * sizeof(rec_AS), SEEK_SET);  // AS 영역을 넘기기
            fseek(fp, 3 * sizeof(rec_BS), SEEK_CUR);  // BS 영역을 넘기기
            fseek(fp, 2 * sizeof(rec_DS), SEEK_CUR);  // DS 영역을 넘기기
            fseek(fp, (sid - 300) * sizeof(rec_HS), SEEK_CUR);  // HS 영역으로 이동
            Run_healSkill(fp, rec_HS);
        }
        else {
            printf("유효하지 않은 SID입니다. HS 범위 내에 SID가 없습니다.\n");
        }
    }
    else {
        printf("유효하지 않은 SID입니다. 범위를 벗어났습니다.\n");
    }
}



int main(int argc, char *argv[])
{
    char c;
    FILE *fp;
    int sid = atoi(argv[1]);

    // 예외처리
    if ((fp = fopen("skillDex", "rb")) == NULL ) {
        fprintf(stderr, "파일 열기 오류\n");
        exit(2);
    }

    Ready_skill(fp, sid);

    fclose(fp);
    exit(0);
}
