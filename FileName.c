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
			printf("��� �Ͻðڽ��ϱ�?(Y:0,N:1)");
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
	printf("�� ���α׷��� Ȧ¦ �����Դϴ�.\n");
	printf("����ڴ� 100���� ������ �����ϰ� �˴ϴ�.\n");
	printf("��ǻ�ʹ� Ȧ¦ �� �Ѱ����� �����ϰ� �Ǹ�.\n");
	printf("����ڴ� �� ������ �����ϱ� ���� ������ �ִ� ������ �����ϰ� �˴ϴ�.\n");
	printf("���� ����ڰ� ��ǻ�Ͱ� ������ Ȧ¦�� ���߰� �Ǹ�.\n");
	printf("������ ������ 2�踦 �ް� �Ǹ�.\n");
	printf("�׷��� ���� ��쿡�� ������ ������ �Ұ� �˴ϴ�.\n");
	printf("\n");
	printf("��ſ��� ����� ���ϴ�.\n");
}

int get_user_bet(int score) {
	int bet;
	while (1) {
		printf("���� ���� : %d\n", score);
		printf("���� :");
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
		printf("¦(0), Ȧ(1) �Է� :");
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


	

