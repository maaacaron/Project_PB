// 몬스터 스텟 구조 저장 헤더파일 

struct stat
{
	int level;
	int exp;
	int HP;
	int attackPower;
	int defensePower;
	int speed;
};	

struct monsterSkill
{
	int skill_1_ID;
	int skill_2_ID;
	int skill_3_ID;
	int skill_4_ID;
};

struct monster
{
	int mid;
	char monster_name[50];
	char property[50];
	struct stat stats;
	struct monsterSkill skills;
};	