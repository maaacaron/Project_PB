#include <stdio.h>
#include <stdlib.h>
#include "monster.h"

int main(int argc, char *argv[])
{
	struct monster rec;
	int MID;
	char c;
	FILE *fp;

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
		printf("수정할 몬스터의 ID : ");
		if (scanf("%d", &MID) == 1)
		{
			fseek(fp, (MID) * sizeof(rec), SEEK_SET);
			if (fread(&rec, sizeof(rec), 1, fp) > 0)
			{
				printf("몬스터 ID : %3d   이름 : %4s   속성 : %4s\n", rec.MID, rec.monster_name, rec.property);
				printf("새로운 이름 입력 : ");
				scanf("%s", &rec.monster_name);
				printf("새로운 속성 입력 : ");
				scanf("%s", &rec.property);

                printf("\n현재 스탯\n");
                printf("HP : %3d   공격력 : %3d   방어력 : %3d   스피드 : %3d\n", rec.stats.HP, rec.stats.attackPower, rec.stats.defensePower, rec.stats.speed);
                printf("새로운 스탯 입력\n%3s %10s %10s %7s\n", "HP", "공격력", "방어력", "속도");
                if(scanf("%d %d %d %d",&rec.stats.HP, &rec.stats.attackPower, &rec.stats.defensePower, &rec.stats.speed) == 4){

                    fseek(fp, -sizeof(rec), SEEK_CUR);
                    fwrite(&rec, sizeof(rec), 1, fp);
                }
            }
            else printf("레코드 %d 없음\n", MID);

			
		}
		else printf("입력오류\n");

		printf("계속하겠습니까? (Y/N)");
		scanf(" %c", &c);
	} while (c == 'Y');

	fclose(fp);
	exit(0);
}
