#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

void HowtoUse();
int get_user_bet();
int get_user_ans();
void count_3sec();
int game_result();


int main()
{
	int com_ans;
	int user_ans;
	int check;
	int score = 100;
	int bet;
	int result;
	srand((unsigned int)time(NULL));

	HowtoUse();
	Sleep(3000);
	system("cls");

	while (score > 0)
	{
		com_ans = rand() % 2;
		bet = get_user_bet(score);
		score -= bet;
		user_ans = get_user_ans();
		system("cls");
		count_3sec();
		score = game_result(com_ans, user_ans, score, bet);
		if (score > 0)
		{
			printf("계속 하시겠습니까?(Y:0,N:1)");
			scanf("%d", &check);
			system("cls");
		}
		if (check == 1) {
			break;
		}

	}
	printf("Game Over!!\n");

	return 0;

}

void HowtoUse()
{
	printf("이 프로그램은 홀짝 게임입니다.\n");
	printf("사용자는 100점을 가지고 시작하게 됩니다.\n");
	printf("컴퓨터는 홀짝 중 한가지를 결정하게 되며.\n");
	printf("사용자는 이 게임을 도전하기 전에 가지고 있던 점수를 배팅하게 됩니다.\n");
	printf("만약 사용자가 컴퓨터가 선택한 홀짝을 맞추게 되면.\n");
	printf("배팅한 점수의 2배를 받게 되며.\n");
	printf("그렇지 않을 경우에는 배팅한 점수를 잃게 됩니다.\n");
	printf("\n");
	printf("당신에게 행운을 빕니다.\n");
}

int get_user_bet(int score) {
	int bet;
	while (1) {
		printf("현재 점수 : %d\n", score);
		printf("몇점 :");
		scanf("%d", &bet);
		if (bet <= score) {
			break;
		}
		system("cls");
	}
	return bet;
}

int get_user_ans() {
	int user_ans;
	while (1) {
		printf("짝(0), 홀(1) 입력 :");
		scanf("%d", &user_ans);
		if (user_ans == 0 || user_ans == 1) {
			break;
		}
	}
	return user_ans;

}

void count_3sec() {
	for (int i = 3; i > 0; i--) {
		printf("%d", i);
		Sleep(1000);
		system("cls");
	}
}

int game_result(int com_ans, int user_ans, int score, int bet) {
	if (com_ans == user_ans)
	{
		printf("User Win!!\n");
		score = score + bet * 2;
		return score;
	}
	else
	{
		printf("Com Win!!\n");
		return score;
	}
}


	

