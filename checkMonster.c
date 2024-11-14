#include <stdio.h>
#include <stdlib.h>
#include "monster.h"

int check_monster(FILE* fp)
{
    struct monster rec;
    int mid;

    printf("검색할 몬스터의 MID 입력: ");
    if (scanf("%d", &mid) == 1) {
        fseek(fp, mid *sizeof(rec),  SEEK_SET);
        if (fread(&rec,  sizeof(rec),  1,  fp) > 0) 
        {
            printf("몬스터ID : %d  이름 : %s  속성 : %s  레벨 : %s\n", rec.mid, rec.monster_name, rec.property, rec.stats.exp);
            printf("HP : %d  공격력 : %d  방어력 : %d  속도 : %d\n",rec.stats.HP, rec.stats.attackPower, rec.stats.defensePower, rec.stats.speed);
        }
        else printf("레코드 %d 없음\n", mid);
    }
    else printf("입력 오류");
}


int main(int argc, char *argv[])
{
    char c;
    FILE *fp;

    if (argc != 2) {
        fprintf(stderr, "사용법: %s 파일이름\n", argv[0]);
        exit(1);
    }

    if ((fp = fopen(argv[1], "rb")) == NULL ) {
        fprintf(stderr, "파일 열기 오류\n");
        exit(2);
    }


    do 
    {
        check_monster(fp);

        printf("계속하겠습니까?(Y/N)");
        scanf(" %c", &c);
    } while (c == 'Y');
    fclose(fp);
    exit(0);
}