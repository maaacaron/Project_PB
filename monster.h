struct stat{
	int level;
	int experience;
	int HP;
	int attackPower;
	int defensePower;
	int speed;
};	

struct monsterSkill{
	int skill_1_ID;
	int skill_2_ID;
	int skill_3_ID;
	int skill_4_ID;
};

struct monster{
	int MID;
	char monster_name[50];
	char property[50];
	struct stat stats;
	struct monsterSkill skills;
};	
