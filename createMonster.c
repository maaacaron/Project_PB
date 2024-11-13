#include <stdio.h>
#include <stdlib.h>
#include "monster.h"
#define START_MID 0

int main(int argc, char* argv[])
{
	struct monster rec;
	char c;
	FILE *fp;

	if(argc != 2)
	{
		fprintf(stderr, "사용법: %s 파일이름\n", argv[0]);
		exit(1);
	}

	if((fp = fopen(argv[1], "wb")) == NULL)
	{
    	fprintf(stderr, "파일 열기 오류\n");
    	exit(2);
	}

	do{
		printf("%3s %3s %3s\n", "몬스터ID", "이름", "속성");
		if(scanf("%d %s %s", &rec.mid, &rec.monster_name, &rec.property) != 3) break;

		rec.stats.level = 1;			//경험치 초기화
		rec.stats.exp = 0;
		printf("%3s %3s %3s %3s\n", "HP", "공격력", "방어력", "속도");
		if(scanf("%d %d %d %d",&rec.stats.HP, &rec.stats.attackPower, &rec.stats.defensePower, &rec.stats.speed) != 4) break;	//스탯 입력
		
		printf("첫 스킬 ID\n");
		if(scanf("%d", &rec.skills.skill_1_ID) != 1) break;		//첫 스킬 입력
		rec.skills.skill_2_ID = -1;								//초기화
		rec.skills.skill_3_ID = -1;								//초기화
		rec.skills.skill_4_ID = -1;								//초기화
		
		fseek(fp, (rec.mid * sizeof(rec)), SEEK_SET);
		fwrite(&rec, sizeof(rec), 1, fp);
		
		printf("계속하겠습니까? (Y/N)");
		scanf(" %c", &c);
	} while (c == 'Y');

	fclose(fp);
	exit(0);
}
