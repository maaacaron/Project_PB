#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "player.h"
#include "monster.h"

#define MAXLINE 256

void Check_Loser(int playerID, struct player* shmp);
int findPlayerIndexByID(struct player* shmp, int playerID);
void Check_Winner(int playerID, int processID, struct player* shmp);
void waitingPlayer_All_BattleEnd(struct player* shmp);
void MakePipe(int fd[2]);
void Write_to_Pipe(int fd[2], int winnerPlayerID);
void Read_to_Pipe(int fd[2], int* winnerPlayerID);

// 두 플레이어 간의 상호작용을 도와주는 배틀 메니저. (사실상 서버 역할임)
int main(int argc, char*argv[]) // 플레이어 ID 넘겨 받을것.
{
	struct player* shmp; // 플레이어 공유 메모리 저장 공간

	int pid, pid2, pid3, pid4, status;
	int child = 0;

	int playerID = 0;
	int processID = 0;
	playerID = atoi(argv[1]); // 플레이어 아이디 ex(1, 281, 19, 22)

	char processID_str[MAXLINE];

	int shmid;
	key_t key;

	// 키값(키 정보) 설정
	key = ftok("main", 1);

	//공유 메모리 접근
	shmid = shmget(key, sizeof(struct player) * 4, 0); // 플레이어

	// 접근 예외처리
	if (shmid == -1) 
	{
		perror("공유 메모리 접근 실패.");
		exit(1);
	}

	// 해당 프로세스에 플레이어 공유 메모리 부착
	shmp = (struct player*)shmat(shmid, NULL, 0);
	processID = findPlayerIndexByID(shmp, playerID); // 생성할 때 순차적으로 부여되는 프로세스 아이디 (1, 2, 3, 4..)

	printf("[Battle Manager]: |서버 구동 시작...|\n");

	Check_Loser(processID, shmp);

	if (processID + 1 == 1) // who am i == 1
	{
		sleep(1);
		if ((pid = fork()) == 0)
		{
			printf("\n[Battle Manager]: 플레이어 %d 배틀 프로세스 시작..\n", processID + 1);

			// 플레이어 아이디를 Battle Client에 넘겨주기 위해 문자열로 만들기
			snprintf(processID_str, sizeof(processID_str), "%d", processID);

			// Battle Client 실행
			execl("./Battle_Client_Skill", "./Battle_Client_Skill", processID_str, NULL);
			perror("execl");
			exit(5);
		}
	}

	if (processID + 1 == 2) // who am i == 1
	{
		sleep(1);
		if ((pid2 = fork()) == 0)
		{
			printf("\n[Battle Manager]: 플레이어 %d 배틀 프로세스 시작..\n", processID + 1);

			// 플레이어 아이디를 Battle Client에 넘겨주기 위해 문자열로 만들기
			snprintf(processID_str, sizeof(processID_str), "%d", processID);

			// Battle Client 실행
			execl("./Battle_Client_Skill", "./Battle_Client_Skill", processID_str, NULL);
			perror("execl");
			exit(5);
		}
	}

	if (processID + 1 == 3) // who am i == 1
	{
		sleep(1);
		if ((pid3 = fork()) == 0)
		{
			printf("\n[Battle Manager]: 플레이어 %d 배틀 프로세스 시작..\n", processID + 1);

			// 플레이어 아이디를 Battle Client에 넘겨주기 위해 문자열로 만들기
			snprintf(processID_str, sizeof(processID_str), "%d", processID);

			// Battle Client 실행
			execl("./Battle_Client_Skill", "./Battle_Client_Skill", processID_str, NULL);
			perror("execl");
			exit(5);
		}
	}

	if (processID + 1 == 4) // who am i == 1
	{
		sleep(1);
		if ((pid4 = fork()) == 0)
		{
			printf("\n[Battle Manager]: 플레이어 %d 배틀 프로세스 시작..\n", processID + 1);

			// 플레이어 아이디를 Battle Client에 넘겨주기 위해 문자열로 만들기
			snprintf(processID_str, sizeof(processID_str), "%d", processID);

			// Battle Client 실행
			execl("./Battle_Client_Skill", "./Battle_Client_Skill", processID_str, NULL);
			perror("execl");
			exit(5);
		}
	}

	child = wait(&status);
	Check_Loser(processID, shmp);

	waitingPlayer_All_BattleEnd(shmp);
	Check_Winner(playerID, processID, shmp);
}

void Check_Loser(int processID, struct player* shmp)
{
	// processID: who am i | shmp[~]: is_dead
	if (shmp[processID].is_dead == 1) //shmp[3]: is_dead
	{
		printf("\n당신은 패배하였다. (프로그램 종료)\n");
		shmdt(shmp);
	}
	return;
}

void Check_Winner(int playerID, int processID, struct player* shmp)
{
	int count = 0;
	// processID: who am i | shmp[~]: is_dead
	int fd[2];
	int pipePid;
	int status;
	int receivedWinnerID;

	for (int i = 0; i < 4; i++)
	{
		if (shmp[i].is_dead != 1 && shmp[i].is_wined == 1) //죽지 않았고 1차 대전에서 통과한사람 몇명?
		{
			count++;
		}
	}

	printf("\ncount: %d", count);

	if (count == 2)
	{
		printf("\n당신은 첫 경기에서 승리하였다. (메인 프로그램으로)\n");
		shmdt(shmp);
		MakePipe(fd);

		if ((pipePid = fork()) == 0) // 나오는 지 보려고 4인에 껴논거
		{
			int winnerPlayerID = playerID;
			printf("%d\n", winnerPlayerID);

			Write_to_Pipe(fd, winnerPlayerID);
		}
		else
		{
			wait(&status); // 자식 프로세스에서 전달하는 결과 대기
			Read_to_Pipe(fd, &receivedWinnerID);
		}
		exit(0);
		//execl("./main", "main", playerID, NULL); // 메인프로그램으로 다시 이동
	}

	if (count == 1)
	{
		printf("\n당신은 최종 경기에서 승리하였다. (승리 이벤트씬으로)\n");
		shmdt(shmp);

		// 승리 이벤트씬은 pipe 써서 표준출력으로 할 것

		MakePipe(fd);

		if ((pipePid = fork()) == 0) // 나오는 지 보려고 4인에 껴논거
		{
			int winnerPlayerID = playerID;

			Write_to_Pipe(fd, winnerPlayerID);
		}
		else
		{
			wait(&status); // 자식 프로세스에서 전달하는 결과 대기
			Read_to_Pipe(fd, &receivedWinnerID);
		}
		exit(0);
	}
	return;
}

// 플레이어 ID를 사용해 프로세스 ID를 탐색 (플레이어가 생성된 순서대로 부여되는 번호)
int findPlayerIndexByID(struct player* shmp, int playerID)
{
	for (int i = 0; i < 4; i++)
	{ // 4명 기준
		if (shmp[i].playerID == playerID)
		{
			return i; // 해당 플레이어의 인덱스 반환
		}
	}
	perror("noFoundPID");
	exit(1);
}

void waitingPlayer_All_BattleEnd(struct player* shmp)
{
	int flag1 = 0;
	int flag2 = 0;
	int flag3 = 0;
	int flag4 = 0;

	printf("\n[BattleManager]: 다른 모든 플레이어가 전투가 끝날 때 까지 대기중..\n");

	while (flag1 + flag2 + flag3 + flag4 != 4)
	{
		if (shmp[0].is_battle_end == 1 && flag1 == 0) //플레이어 1 전투 끝났나?
		{
			flag1 = 1;
		}

		if (shmp[1].is_battle_end == 1 && flag2 == 0) // 플레이어 2 전투 끝났나?
		{
			flag2 = 1;
		}

		if (shmp[2].is_battle_end == 1 && flag3 == 0) // 플레이어 2 전투 끝났나?
		{
			flag3 = 1;
		}

		if (shmp[3].is_battle_end == 1 && flag4 == 0) // 플레이어 2 전투 끝났나?
		{
			flag4 = 1;
		}
		
		printf("p1: %d, p2: %d, p3: %d, p4: %d", flag1, flag2, flag3, flag4);
		sleep(1);
	}

	return;
}

void MakePipe(int fd[2])
{
	if (pipe(fd) == -1)
	{
		perror("pipe 생성 실패");
		return;
	}
}

void Write_to_Pipe(int fd[2], int winnerPlayerID) //자식에서 쓸 함수
{
	close(fd[0]);
	write(fd[1], &winnerPlayerID, sizeof(int));
	close(fd[1]);
}

void Read_to_Pipe(int fd[2], int* winnerPlayerID) //부모에서 쓸 함수
{
	close(fd[1]);
	read(fd[0], winnerPlayerID, sizeof(int));
	printf("=================================================================\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("최종 승리한 플레이어는 플레이어 %d입니다. 축하합니다!!\n", *winnerPlayerID);
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("=================================================================\n");

	close(fd[0]);
}