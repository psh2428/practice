#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

// 1 <= N  M <= 1000  1 <= K <= 100000

int N, M, K; //입력받을 수를 전역변수로 선언하기

void procession(); // 행렬 구하는 함수 원형 선언 

int main() { 
	while (1) {
		scanf("%d %d %d", &N, &M, &K); // N M K 입력 받고 범위에서 벗어날 시 다시 입력하세요 문구 출력하여 다시 입력하게 만듦
		if (1 <= N && M <= 1000 && 1 <= K &&  K <= 100000) {
			break;
		}
		else
		{
			system("cls");
			printf("다시 입력하세요 : ");
		}
	}
	procession(); // 행렬 구하는 함수 호출
	return 0;
}

void procession() {
	if ((N + M) - 1 == K) { // N과M을 더하고 1을 뺀 값이 K와 같다면 행렬 출력
		int i, j, o = 2; // i와 j 는 반복문 사용하기 위해 선언하고 o는 j가 시작을 2로 시작해야하기 때문에 2로 선언 
		printf("YES\n"); // 행렬이 가능하기 때문에 YES 출력 

		for (i = 1; i <= N; i++) { // 세로로 오름차순 나열 
			printf("%d ", i);

			for (j = o; j <= M; j++) { // 가로로 오름차순 나열 
				printf("%d ", j);
			}
			o++; // o를 증감연산자를 사용하여 한개씩 늘려주어 세로로 오름차순 나열한 수에 다음수부터 나올 수 있도록 함.
			M++; //M도 증가시켜 자리수 맞추기
			printf("\n");
		}
	}
	else { // if문 조건식이 맞지 않다면 NO를 출력
		printf("NO");
	}
	
}