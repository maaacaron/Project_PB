#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "player.h"
#include "monster.h"
#include "skill.h"

#define MAXLINE 100

// 배틀 끝나고 배틀이 시작되었을 때 스텟값으로 되돌려주기 위해 만들어놓은 변수
int start_hp;
int start_attack;
int start_defense;
int start_speed;

// 스킬 읽어올때 사용할 전역변수
char c;
FILE* fp;

void Devide_Team(int playerID);
void player_turn_attack(struct player* shmp, int processID, int opponentID); // 플레이어 공격 함수
void waiting_opponent(struct player* shmp, int processID, int opponentID); // 상대 플레이어 대기 함수
void Reset_Shm(struct player* shmp, int processID, int opponentID);
void Print_Battle_Begine(struct player* shmp, int processID, int opponentID);

void Print_Skill_name(int sid); // 스킬 아이디를 입력받아 적절하게 보유 스킬 텍스트를 출력
void Print_attackSkill(struct attackSkill rec_AS);
void Print_buffSkill(struct buffSkill rec_BS);
void Print_debuffSkill(struct debuffSkill rec_DS);
void Print_healSkill(struct healSkill rec_HS);

void Ready_skill(struct player* shmp, int processID, int opponentID, int sid); // 사용자로부터 스킬 아이디를 입력받아 스킬 타입에 맞는 스킬 실행
void Run_attackSkill(FILE* fp, struct attackSkill rec_AS, struct player* shmp, int processID, int opponentID, int sid);
void Run_buffSkill(FILE* fp, struct buffSkill rec_BS, struct player* shmp, int processID, int opponentID, int sid);
void Run_debuffSkill(FILE* fp, struct debuffSkill rec_DS, struct player* shmp, int processID, int opponentID, int sid);
void Run_healSkill(FILE* fp, struct healSkill rec_HS, struct player* shmp, int processID, int opponentID, int sid);

int main(int argc, char* argv[]) // 플레이어가 맨 처음에 입력받은 아이디를 들고올것. 0,1,2,3
{
	int status, child;
	int playerID = 0;

	// 예외처리
	if (argc < 2)
	{
		printf("함수 인자값 및 함수 실행 실패");
		return 0;
	}

	// 전달받은 인자값 (process_ID)를 다시 정수화 하여 해당 코드 변수에 저장
	playerID = atoi(argv[1]);

	// process_ID으로 팀을 나눠주고, 배틀을 시작함
	Devide_Team(playerID);

	// 다시 BattleManager로 돌아가기
	exit(0);

	// 입력 권한을 BattleManager에게 넘겨주는 용도
	child = wait(&status);
}

void player_turn_attack(struct player* shmp, int playerID, int opponentID)
{
	int answer = 0;
	char buffer[MAXLINE];

	printf("\n[Battle Manager]: || 당신의 턴! ||\n");

	// 스킬과 포켓몬은 사용자가 입력받은 인덱스에 저장되어 있음
	printf("스킬 1: ");
	Print_Skill_name(shmp[playerID].selectedMonster.skills.skill_1_ID);
	printf("\n스킬 2: ");
	Print_Skill_name(shmp[playerID].selectedMonster.skills.skill_2_ID);
	printf("\n스킬 3: ");
	Print_Skill_name(shmp[playerID].selectedMonster.skills.skill_3_ID);
	printf("\n스킬 4: ");
	Print_Skill_name(shmp[playerID].selectedMonster.skills.skill_4_ID);

	printf("\n[Battle Manager]: 스킬을 선택하세요(1 ~ 4)\n");

	// 사용자 입력 (gpt 수정부분)
	while (1)
	{
		printf("[Battle Manager]: 입력: ");

		// 사용자 입력 받기
		if (fgets(buffer, sizeof(buffer), stdin) == NULL)
		{
			printf("\n[Battle Manager]: 입력 오류. 다시 입력해주세요.\n");
			printf("stdin 상태: EOF=%d\n", feof(stdin));
			exit(0);
		}
		sscanf(buffer, "%d", &answer);

		break;
	}

	if (answer == 1)
	{
		Ready_skill(shmp, playerID, opponentID, shmp[playerID].selectedMonster.skills.skill_1_ID);
	}

	else if (answer == 2)
	{
		Ready_skill(shmp, playerID, opponentID, shmp[playerID].selectedMonster.skills.skill_2_ID);
	}

	else if (answer == 3)
	{
		Ready_skill(shmp, playerID, opponentID, shmp[playerID].selectedMonster.skills.skill_3_ID);
	}

	else if (answer == 4)
	{
		Ready_skill(shmp, playerID, opponentID, shmp[playerID].selectedMonster.skills.skill_4_ID);
	}

	// 공격 후에는 상대 플레이어의 공격 함수 차례
	shmp[playerID].isMyTurn = 0;
	shmp[opponentID].isMyTurn = 1;

	// 해당 공격으로 상대 플레이어의 체력이 0이 되는지 체크
	if (shmp[opponentID].selectedMonster.stats.HP <= 0)
	{
		shmp[opponentID].is_dead = 1; // isdead = 1;
		shmp[playerID].is_battle_end = 1; // is_battle_End = 1;
		shmp[opponentID].is_battle_end = 1; // is_battle_End = 1;

		printf("[Battle Manager]: 당신은 승리하였습니다. 메인 화면으로 돌아갑니까?\n");
		printf("[Battle Manager]: 예: 1, 아니요: 0\n");

		Reset_Shm(shmp, playerID, opponentID); // is_wined가 1일때 battle_end가 0으로 초기화되니 주의

		shmp[playerID].is_wined = 1;

		scanf("%d", &answer);
		printf("[Battle Manager]: 메인 화면으로 돌아갑니다..\n");
		shmdt(shmp);
		return;
	}

	printf("\n[Battle Manager]: || 상대 턴! ||\n");

	waiting_opponent(shmp, playerID, opponentID);
}

void waiting_opponent(struct player* shmp, int playerID, int opponentID) // 플레이어가 입력받은 아이디 받아올것 0,1,2,3,4...
//void waiting_opponent(int who_am_i, int* shmp, int* shmp2)
{
	// 상대 플레이어의 선택 기다리기
	// 내 턴이 아닐때는 상대 기다리기
	struct monster before_selectedMonster = shmp[playerID].selectedMonster;
	struct monster After_selectedMonster;

	struct monster before_opponentMonster = shmp[opponentID].selectedMonster;
	struct monster After_opponentMonster;

	if (shmp[playerID].isMyTurn == 0) // 만약 내 턴이 아니라면
	{
		printf("[Battle Manager]: 상대의 결정을 기다리는중..\n");

		while (shmp[opponentID].isMyTurn == 1) // 여기까지 수정
		{
			sleep(1);
		}

		After_selectedMonster = shmp[playerID].selectedMonster;
		After_opponentMonster = shmp[opponentID].selectedMonster;

		if (before_selectedMonster.stats.HP > After_selectedMonster.stats.HP) 
			// 기다리기 전 내 포켓몬 HP와 기다린 후 HP가 다르면 상대방이 공격스킬을 썼다는 뜻
		{
			printf("[Battle Manager]: 상대는 당신의 포켓몬에게 %d의 피해를 입혔다.\n", before_selectedMonster.stats.HP - After_selectedMonster.stats.HP);
			printf("[Battle Manager]: 당신의 포켓몬의 체력은 %d로 줄어들었다.\n\n", shmp[playerID].selectedMonster.stats.HP);
		}

		if (before_opponentMonster.stats.attackPower < After_opponentMonster.stats.attackPower) // 기다리기 전 상대 포켓몬 ATK과 기다린 후 ATK가 다르면 상대방이 공격버프스킬을 썼다는 뜻
		{
			printf("[Battle Manager]: 상대는 버프스킬을 사용하여 포켓몬의 공격력이 %d 올랐다.\n", After_opponentMonster.stats.attackPower - before_opponentMonster.stats.attackPower);
		}

		if (before_opponentMonster.stats.defensePower < After_opponentMonster.stats.defensePower) // 기다리기 전 상대 포켓몬 DF과 기다린 후 DF가 다르면 상대방이 방어버프스킬을 썼다는 뜻
		{
			printf("[Battle Manager]: 상대는 버프스킬을 사용하여 포켓몬의 방어력이 %d 올랐다.\n", After_opponentMonster.stats.defensePower - before_opponentMonster.stats.defensePower);
		}

		if (before_selectedMonster.stats.attackPower > After_selectedMonster.stats.attackPower) // 기다리기 전 내 포켓몬 ATK과 기다린 후 ATK가 다르면 상대방이 공격디버프스킬을 썼다는 뜻
		{
			printf("[Battle Manager]: 상대는 디버프스킬을 사용하여 당신의 포켓몬의 공격력이 %d 내려갔다.\n", before_selectedMonster.stats.attackPower - After_selectedMonster.stats.attackPower);
		}

		if (before_selectedMonster.stats.defensePower > After_selectedMonster.stats.defensePower) // 기다리기 전 내 포켓몬 DF과 기다린 후 DF가 다르면 상대방이 방어디버프스킬을 썼다는 뜻
		{
			printf("[Battle Manager]: 상대는 디버프스킬을 사용하여 당신의 포켓몬의 방어력이 %d 내려갔다.\n", before_selectedMonster.stats.defensePower - After_selectedMonster.stats.defensePower);
		}

		if ((before_opponentMonster.stats.HP < After_opponentMonster.stats.HP) && (After_selectedMonster.stats.HP != start_hp)) // 기다리기 전 내 포켓몬 체력과 기다린 후 체력이 다르면 상대방이 힐스킬을씀
		{
			printf("[Battle Manager]: 상대는 회복스킬을 사용하여 체력을 %d 회복했다.\n", After_opponentMonster.stats.HP - before_opponentMonster.stats.HP);
		}

		// 해당 공격으로 플레이어의 체력이 0이 되는지 체크
		if (shmp[playerID].selectedMonster.stats.HP <= 0)
		{
			shmp[playerID].is_dead = 1; // isdead = 1;
			shmp[playerID].is_battle_end = 1; // is_battle_End = 1;
			shmp[opponentID].is_battle_end = 1; // is_battle_End = 1;

			shmdt(shmp);
			return;
		}

		shmp[playerID].isMyTurn = 1; // is_myturn = 1으로 만들어 주기
		shmp[opponentID].isMyTurn = 0;

		player_turn_attack(shmp, playerID, opponentID);
	}
}

void Devide_Team(int playerID) // 플레이어가 맨 처음에 입력받은 값을 들고올것. 0,1,2,3
{
	int shmid;
	key_t key;
	struct player* shmp; // p1 공유 메모리 저장 공간

	int opponentID = 0;
	int fd1, fd2;
	int opponentSpeed;
	int firstTurn;

	// 키값(키 정보) 설정
	key = ftok("main", 1);

	//공유 메모리 접근
	shmid = shmget(key, sizeof(struct player) * 8, 0); // 플레이어

	// 접근 예외처리
	if (shmid == -1)
	{
		perror("공유 메모리 접근 실패.");
		exit(1);
	}

	// 해당 프로세스에 플레이어 공유 메모리 부착
	shmp = (struct player*)shmat(shmid, NULL, 0);

	// 2명 생존했을 때 상대를 정하는 블럭////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (shmp[playerID].is_wined == 1) // 만약에 내가 이긴적이 있다 -> 이전에 토너먼트가 진행된 적이 있다 => 2명만 생존하고 있다
	{
		for (int i = 0; i < 4; i++)
		{
			// 내가 아닌 4명중 is_wined가 찍혀있는사람이 내 싸움 상대임
			if (shmp[i].is_wined == 1 && playerID != i)
			{
				opponentID = i;
			}
			if (playerID < opponentID) // 플레이어 아이디 0,1,2,3
			{
				fd1 = open("./battlefifo1", O_WRONLY);
				fd2 = open("./battlefifl2", O_RDONLY);
			}

			else
			{
				fd1 = open("./battlefifo2", O_WRONLY);
				fd2 = open("./battlefifl1", O_RDONLY);
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 4명 생존했을 때 상대를 정하는 블럭////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if (shmp[playerID].is_wined != 1) // 4명 이상 생존하였을 때 임의로 정해준다
	{
		if (playerID == 0) // 플레이어 아이디 0,1,2,3
		{
			// p1 vs p2
			opponentID = 1;
			fd1 = open("./battlefifo1", O_WRONLY);
			fd2 = open("./battlefifl2", O_RDONLY);
			write(fd1, &shmp[playerID].selectedMonster.stats.speed, sizeof(shmp[playerID].selectedMonster.stats.speed));
			read(fd2, &opponentSpeed, sizeof(opponentSpeed));
		}

		if (playerID == 1)
		{
			// p2 vs p1
			opponentID = 0;
			fd1 = open("./battlefifo2", O_WRONLY);
			fd2 = open("./battlefifl1", O_RDONLY);
			write(fd1, &shmp[playerID].selectedMonster.stats.speed, sizeof(shmp[playerID].selectedMonster.stats.speed));
			read(fd2, &opponentSpeed, sizeof(opponentSpeed));
		}

		if (playerID == 2)
		{
			// p3 vs p4
			opponentID = 3;
			fd1 = open("./battlefifo3", O_WRONLY);
			fd2 = open("./battlefifl4", O_RDONLY);
			write(fd1, &shmp[playerID].selectedMonster.stats.speed, sizeof(shmp[playerID].selectedMonster.stats.speed));
			read(fd2, &opponentSpeed, sizeof(opponentSpeed));
		}

		if (playerID == 3)
		{
			// p4 vs p3
			opponentID = 2;
			fd1 = open("./battlefifo4", O_WRONLY);
			fd2 = open("./battlefifl3", O_RDONLY);
			write(fd1, &shmp[playerID].selectedMonster.stats.speed, sizeof(shmp[playerID].selectedMonster.stats.speed));
			read(fd2, &opponentSpeed, sizeof(opponentSpeed));
		}
		close(fd1);
		close(fd2);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 배틀 시작 전 원래 포켓몬의 스텟값을 저장해두기 (배틀 끝나고 회복할 때 사용)
	start_attack = shmp[playerID].selectedMonster.stats.attackPower;
	start_defense = shmp[playerID].selectedMonster.stats.defensePower;
	start_hp = shmp[playerID].selectedMonster.stats.HP;
	start_speed = shmp[playerID].selectedMonster.stats.speed;

	shmp[playerID].equal_flag = 0;

	Reset_Shm(shmp, playerID, opponentID); // 공유 메모리 값들 중 포켓몬 관련 함수 초기화
	Print_Battle_Begine(shmp, playerID, opponentID); // 배틀 문구 프린트

	// 각 포켓몬 스피드값 비교하여 선제턴 주기
	if (shmp[playerID].selectedMonster.stats.speed == opponentSpeed) // 해당 경우 처리 해줘야함.
	{
		if (playerID < opponentID) // 배틀씬 먼저 들어온 사람한테 스피드+1해줌.
		{
            srand((unsigned int) time(NULL));
            firstTurn = rand() % 2;
			if(firstTurn == 0)
			{
				shmp[playerID].selectedMonster.stats.speed++;
			}
			if(firstTurn == 1)
			{
				shmp[opponentID].selectedMonster.stats.speed++;
				opponentSpeed++;
			}
		}
		sleep(1);
	}

	if (shmp[playerID].selectedMonster.stats.speed > opponentSpeed) // 내 포켓몬이 상대 포켓몬보다 빠르면
	{
		shmp[playerID].isMyTurn = 1;
		shmp[opponentID].isMyTurn = 0;
	}

	if (opponentSpeed > shmp[playerID].selectedMonster.stats.speed)
	{
		shmp[playerID].isMyTurn = 0;
		shmp[opponentID].isMyTurn = 1;
	}


	// 만약 플레이어의 턴이면 공격 기회 얻기
	if (shmp[playerID].isMyTurn == 1)
	{
		printf("\nDebug| p1 Speed: %d, p2 Speed: %d\n", shmp[playerID].selectedMonster.stats.speed, opponentSpeed);
		player_turn_attack(shmp, playerID, opponentID);
	}

	//만약 플레이어의 턴이 아니면 기다리기
	else if (shmp[playerID].isMyTurn == 0)
	{
		waiting_opponent(shmp, playerID, opponentID);
	}

	printf("\n결과: player is win: %d, opponent is win: %d\n", shmp[playerID].is_wined, shmp[opponentID].is_wined);
	return;
}

void Reset_Shm(struct player* shmp, int playerID, int opponentID) // 플레이어 아이디는 0,1,2,3 등 직접 입력받은 값..
{
	if (shmp[playerID].is_wined == 1 && shmp[opponentID].selectedMonster.stats.HP > 0) // 2번째 경기 시작 전에 is_battle_end = 0으로 만들어 주기
	{
		shmp[playerID].is_battle_end = 0;
	}
	shmp[playerID].isMyTurn = 0;

	// 몬스터의 스텟값을 원래대로 되돌림
	shmp[playerID].selectedMonster.stats.attackPower = start_attack;
	shmp[playerID].selectedMonster.stats.defensePower = start_defense;
	shmp[playerID].selectedMonster.stats.HP = start_hp;
	shmp[playerID].selectedMonster.stats.speed = start_speed;

	return;
}

void Print_Battle_Begine(struct player* shmp, int playerID, int opponentID) // 처음에 입력받은값 그대로 0,1,2,3..
{
	printf("\n[Battle Manager]: || 포켓몬 배틀 시작! ||\n");

	printf("\n[Battle Manager]: P1 shm 정보: [0]| hp: %d, [1]| speed: %d, [2]| attack: %d, [3]| is_dead: %d, [4]| is_my_turn: %d, [5]| is_battle_End: %d, [6]| ID: %d, [7]| win: %d\n",
		shmp[playerID].selectedMonster.stats.HP, shmp[playerID].selectedMonster.stats.speed, shmp[playerID].selectedMonster.stats.attackPower,
		shmp[playerID].is_dead, shmp[playerID].isMyTurn, shmp[playerID].is_battle_end, playerID, shmp[playerID].is_wined);

	printf("\n[Battle Manager]: P2 shm 정보: [0]| hp: %d, [1]| speed: %d, [2]| attack: %d, [3]| is_dead: %d, [4]| is_my_turn: %d, [5]| is_battle_End: %d, [6]| ID: %d, [7]| win: %d\n",
		shmp[opponentID].selectedMonster.stats.HP, shmp[opponentID].selectedMonster.stats.speed, shmp[opponentID].selectedMonster.stats.attackPower,
		shmp[opponentID].is_dead, shmp[opponentID].isMyTurn, shmp[opponentID].is_battle_end, opponentID, shmp[opponentID].is_wined);

	return;
}

void Print_Skill_name(int sid) // 스킬 아이디를 입력받아 적절하게 보유 스킬 텍스트를 출력
{	
	// 예외처리
	if ((fp = fopen("skillDex", "rb")) == NULL) {
		fprintf(stderr, "스킬 파일 열기 오류\n");
		exit(2);
	}

	// 스킬 sid를 입력받아 해당 스킬 확인시켜주기
	struct attackSkill rec_AS;
	struct buffSkill rec_BS;
	struct debuffSkill rec_DS;
	struct healSkill rec_HS;

	// sid 값에 따라 해당 스킬 영역에 접근
	if (sid >= 0 && sid < 100)
	{
		// AS 영역
		// sid가 0~99 범위에 있을 때, AS 영역의 데이터에 접근
		if (sid < 8) {  // 실제로 저장된 AS 항목이 8개이므로, sid가 0~7인 경우만 유효
			fseek(fp, sid * sizeof(rec_AS), SEEK_SET);
			Print_attackSkill(rec_AS);
		}
		else
		{
			printf("없음");
		}
	}

	if (sid >= 100 && sid < 200)
	{
		// BS 영역
		// sid가 100~199 범위에 있을 때, BS 영역으로 이동
		if (sid - 100 < 3)
		{  // 실제로 저장된 BS 항목이 3개이므로, sid가 100~102인 경우만 유효
			fseek(fp, 8 * sizeof(rec_AS), SEEK_SET);  // AS 영역을 넘기기
			fseek(fp, (sid - 100) * sizeof(rec_BS), SEEK_CUR);  // BS 영역으로 이동
			Print_buffSkill(rec_BS);
		}
		else
		{
			printf("없음");
		}
	}

	if (sid >= 200 && sid < 300)
	{  // DS 영역
		if (sid - 200 < 2)
		{  // 실제로 저장된 DS 항목이 2개이므로, sid가 200~201인 경우만 유효
			fseek(fp, 8 * sizeof(rec_AS), SEEK_SET);  // AS 영역을 넘기기
			fseek(fp, 3 * sizeof(rec_BS), SEEK_CUR);  // BS 영역을 넘기기
			fseek(fp, (sid - 200) * sizeof(rec_DS), SEEK_CUR);  // DS 영역으로 이동
			Print_debuffSkill(rec_DS);
		}
		else
		{
			printf("없음");
		}
	}

	if (sid >= 300 && sid < 400)
	{  // HS 영역
		if (sid - 300 < 1)
		{  // 실제로 저장된 HS 항목이 1개이므로, sid가 300~300인 경우만 유효
			fseek(fp, 8 * sizeof(rec_AS), SEEK_SET);  // AS 영역을 넘기기
			fseek(fp, 3 * sizeof(rec_BS), SEEK_CUR);  // BS 영역을 넘기기
			fseek(fp, 2 * sizeof(rec_DS), SEEK_CUR);  // DS 영역을 넘기기
			fseek(fp, (sid - 300) * sizeof(rec_HS), SEEK_CUR);  // HS 영역으로 이동
			Print_healSkill(rec_HS);
		}
		else
		{
			printf("없음");
		}
	}

	fclose(fp);
	return;
}

void Print_attackSkill(struct attackSkill rec_AS)
{
	// 공격 스킬의 정보 출력
	if (fread(&rec_AS, sizeof(rec_AS), 1, fp) > 0)
	{
		printf("공격스킬| %s: 데미지 %d, 속성: %s ", rec_AS.skill_name, rec_AS.skill_damage, rec_AS.property);
		return;
	}
	else perror("공격 스킬 읽기 오류");
}

void Print_buffSkill(struct buffSkill rec_BS)
{
	// 버프 스킬의 정보 출력
	if (fread(&rec_BS, sizeof(rec_BS), 1, fp) > 0)
	{
		// 버프 종류| 1: 공격력 2: 방어력 3: 공격력 + 방어력 4: 속도 5: 공격력 + 속도 6: 방어력 + 속도
		// 버프 종류| 7: 공격력 + 방어력 + 속도

		if (rec_BS.buff_type == 1)
		{
			printf("공격력 버프 스킬| %s: 스텟 증가량 %d ", rec_BS.skill_name, rec_BS.attack_up_value);
		}
		if (rec_BS.buff_type == 2)
		{
			printf("방어력 버프 스킬| %s: 스텟 증가량 %d ", rec_BS.skill_name, rec_BS.defense_up_value);
		}
		return;
	}
	else perror("버프 스킬 읽기 오류");
}

void Print_debuffSkill(struct debuffSkill rec_DS)
{
	// 버프 스킬의 정보 출력
	if (fread(&rec_DS, sizeof(rec_DS), 1, fp) > 0)
	{
		// 디버프 종류| 1: 공격력 2: 방어력 3: 공격력 + 방어력 4: 속도 5: 공격력 + 속도 6: 방어력 + 속도
		// 디버프 종류| 7: 공격력 + 방어력 + 속도

		if (rec_DS.debuff_type == 1)
		{
			printf("공격력 디버프 스킬| %s: 스텟 증가량 %d ", rec_DS.skill_name, rec_DS.attack_down_value);
		}
		if (rec_DS.debuff_type == 2)
		{
			printf("방어력 디버프 스킬| %s: 스텟 증가량 %d ", rec_DS.skill_name, rec_DS.defense_down_value);
		}
		return;
	}
	else perror("디버프 스킬 읽기 오류");
}

void Print_healSkill(struct healSkill rec_HS)
{
	// 힐 스킬의 정보 출력
	if (fread(&rec_HS, sizeof(rec_HS), 1, fp) > 0)
	{
		printf("체력 회복 스킬| %s: 회복량 %d ", rec_HS.skill_name, rec_HS.heal_value);
		return;
	}
	else perror("힐 스킬 읽기 오류");
}

void Ready_skill(struct player* shmp, int playerID, int opponentID, int sid) // 사용자로부터 스킬 아이디를 입력받아 스킬 타입에 맞는 스킬 실행
{
	// 예외처리
	if ((fp = fopen("skillDex", "rb")) == NULL) {
		fprintf(stderr, "스킬 파일 열기 오류\n");
		exit(2);
	}

	// 스킬 sid를 입력받아 해당 스킬 확인시켜주기
	struct attackSkill rec_AS;
	struct buffSkill rec_BS;
	struct debuffSkill rec_DS;
	struct healSkill rec_HS;

	// sid 값에 따라 해당 스킬 영역에 접근
	if (sid >= 0 && sid < 100) 
	{  
		// AS 영역
		// sid가 0~99 범위에 있을 때, AS 영역의 데이터에 접근
		if (sid < 8) {  // 실제로 저장된 AS 항목이 8개이므로, sid가 0~7인 경우만 유효
			fseek(fp, sid * sizeof(rec_AS), SEEK_SET);
			Run_attackSkill(fp, rec_AS, shmp, playerID, opponentID, sid);
		}
	}

	if (sid >= 100 && sid < 200) 
	{  
		// BS 영역
		// sid가 100~199 범위에 있을 때, BS 영역으로 이동
		if (sid - 100 < 3) 
		{  // 실제로 저장된 BS 항목이 3개이므로, sid가 100~102인 경우만 유효
			fseek(fp, 8 * sizeof(rec_AS), SEEK_SET);  // AS 영역을 넘기기
			fseek(fp, (sid - 100) * sizeof(rec_BS), SEEK_CUR);  // BS 영역으로 이동
			Run_buffSkill(fp, rec_BS, shmp, playerID, opponentID, sid);
		}
	}

	if (sid >= 200 && sid < 300) 
	{  // DS 영역
		if (sid - 200 < 2) 
		{  // 실제로 저장된 DS 항목이 2개이므로, sid가 200~201인 경우만 유효
			fseek(fp, 8 * sizeof(rec_AS), SEEK_SET);  // AS 영역을 넘기기
			fseek(fp, 3 * sizeof(rec_BS), SEEK_CUR);  // BS 영역을 넘기기
			fseek(fp, (sid - 200) * sizeof(rec_DS), SEEK_CUR);  // DS 영역으로 이동
			Run_debuffSkill(fp, rec_DS, shmp, playerID, opponentID, sid);
		}
	}

	if (sid >= 300 && sid < 400) 
	{  // HS 영역
		if (sid - 300 < 1) 
		{  // 실제로 저장된 HS 항목이 1개이므로, sid가 300~300인 경우만 유효
			fseek(fp, 8 * sizeof(rec_AS), SEEK_SET);  // AS 영역을 넘기기
			fseek(fp, 3 * sizeof(rec_BS), SEEK_CUR);  // BS 영역을 넘기기
			fseek(fp, 2 * sizeof(rec_DS), SEEK_CUR);  // DS 영역을 넘기기
			fseek(fp, (sid - 300) * sizeof(rec_HS), SEEK_CUR);  // HS 영역으로 이동
			Run_healSkill(fp, rec_HS, shmp, playerID, opponentID, sid);
		}
	}

	fclose(fp);
	return;
}

void Run_attackSkill(FILE* fp, struct attackSkill rec_AS, struct player* shmp, int playerID, int opponentID, int sid)
{
	int resultDamage = 0;

	if (fread(&rec_AS, sizeof(rec_AS), 1, fp) > 0)
	{
		printf("\nDEBUG| 스킬 ID : %d  스킬이름 : %s  타입 : %s\n", rec_AS.sid, rec_AS.skill_name, "공격");
		printf("\nDEBUG| 데미지 : %d  속성 : %s\n", rec_AS.skill_damage, rec_AS.property);

		// 상대 포켓몬의 HP -= (내 포켓몬 공격력 + 해당 스킬 공격력 - 상대 포켓몬의 DF)
		resultDamage = shmp[playerID].selectedMonster.stats.attackPower + rec_AS.skill_damage - shmp[opponentID].selectedMonster.stats.defensePower;
		shmp[opponentID].selectedMonster.stats.HP -= resultDamage;

		printf("\n[Battle Manager]: || 공격스킬결과 ||");
		printf("\n[Battle Manager]: 당신은 상대 포켓몬에게 %d의 피해를 입혔다.\n", resultDamage);
		printf("[Battle Manager]: 상대 포켓몬의 체력은 %d로 줄어들었다.\n", shmp[opponentID].selectedMonster.stats.HP);

		return;
	}
	else perror("공격력 스킬 읽기 실패");
}

void Run_buffSkill(FILE* fp, struct buffSkill rec_BS, struct player* shmp, int playerID, int opponentID, int sid)
{
	if (fread(&rec_BS, sizeof(rec_BS), 1, fp) > 0)
	{
		printf("\nDEBUG| 스킬 ID : %d 이름 : %s 종류 : %s\n", rec_BS.sid, rec_BS.skill_name, "버프");

		// 버프 종류| 1: 공격력 2: 방어력 3: 공격력 + 방어력 4: 속도 5: 공격력 + 속도 6: 방어력 + 속도
		// 버프 종류| 7: 공격력 + 방어력 + 속도
		if (rec_BS.buff_type == 1)
		{
			printf("DEBUG| 버프 종류 : 공격력\n");
			printf("DEBUG| 공격력 증가량 : %d\n", rec_BS.attack_up_value);
			printf("DEBUG| 버프 전 포켓몬의 공격력 : %d\n", shmp[playerID].selectedMonster.stats.attackPower);

			shmp[playerID].selectedMonster.stats.attackPower += rec_BS.attack_up_value;

			printf("DEBUG| 버프 후 포켓몬의 공격력 : %d\n", shmp[playerID].selectedMonster.stats.attackPower);
		}

		if (rec_BS.buff_type == 2)
		{
			printf("DEBUG| 버프 종류 : 방어력\n");
			printf("DEBUG| 방어력 증가량 : %d", rec_BS.defense_up_value);
			printf("DEBUG| 버프 전 포켓몬의 방어력 : %d\n", shmp[playerID].selectedMonster.stats.defensePower);

			shmp[playerID].selectedMonster.stats.defensePower += rec_BS.defense_up_value;

			printf("DEBUG| 버프 후 포켓몬의 방어력 : %d\n", shmp[playerID].selectedMonster.stats.defensePower);
		}
		return;
	}
	else perror("버프 스킬 읽기 실패");
}

void Run_debuffSkill(FILE* fp, struct debuffSkill rec_DS, struct player* shmp, int playerID, int opponentID, int sid)
{
	if (fread(&rec_DS, sizeof(rec_DS), 1, fp) > 0)
	{
		printf("스킬 ID : %d 이름 : %s 종류 : %s\n", rec_DS.sid, rec_DS.skill_name, "디버프");

		// 디버프 종류| 1: 공격력 2: 방어력 3: 공격력 + 방어력 4: 속도 5: 공격력 + 속도 6: 방어력 + 속도
		// 디버프 종류| 7: 공격력 + 방어력 + 속도
		if (rec_DS.debuff_type == 1)
		{
			printf("디버프 종류 : 공격력\n");
			printf("공격력 감소량 : %d", rec_DS.attack_down_value);
			printf("DEBUG| 디버프 전 상대 포켓몬의 공격력 : %d\n", shmp[opponentID].selectedMonster.stats.attackPower);

			shmp[opponentID].selectedMonster.stats.attackPower -= rec_DS.attack_down_value;

			printf("DEBUG| 디버프 후 상대 포켓몬의 공격력 : %d\n", shmp[opponentID].selectedMonster.stats.attackPower);
		}

		if (rec_DS.debuff_type == 2)
		{
			printf("디버프 종류 : 방어력\n");
			printf("방어력 감소량 : %d", rec_DS.defense_down_value);
			printf("DEBUG| 디버프 전 상대 포켓몬의 방어력 : %d\n", shmp[opponentID].selectedMonster.stats.defensePower);

			shmp[opponentID].selectedMonster.stats.defensePower -= rec_DS.defense_down_value;

			printf("DEBUG| 디버프 후 상대 포켓몬의 방어력 : %d\n", shmp[opponentID].selectedMonster.stats.defensePower);
		}

		return;
	}
	else perror("공격력 스킬 읽기 실패");
}

void Run_healSkill(FILE* fp, struct healSkill rec_HS, struct player* shmp, int playerID, int opponentID, int sid)
{
	if (fread(&rec_HS, sizeof(rec_HS), 1, fp) > 0)
	{
		printf("스킬 ID : %d  스킬이름 : %s  타입 : %s  힐량 : %d\n", rec_HS.sid, rec_HS.skill_name, "힐", rec_HS.heal_value);
		printf("DEBUG| 회복 전 플레이어 포켓몬의 체력 : %d\n", shmp[playerID].selectedMonster.stats.HP);

		shmp[playerID].selectedMonster.stats.HP += rec_HS.heal_value;

		printf("DEBUG| 회복 후 플레이어 포켓몬의 체력 : %d\n", shmp[playerID].selectedMonster.stats.HP);
	}
}