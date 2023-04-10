#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

long long fibo(int n) // 비트수가 모잘라서 long long 사용하고 함수 선언 하여 정의 
{
	if (n == 0) return 1; // 처음 시작이 1, 1 이므로 0일때 1로 반환 
	if (n == 1) return 1; // 1일때 1로 반환 

	return fibo(n - 1) + fibo(n - 2); /* 재귀함수를 사용하여 값을 계속 반환시켜 구하고자 하는 값을 구한다
	예를 들어 fibo(5)는 fibo(4) 와 fibo(3)을 호출하고 fibo(4)는 3과 2를 호출하고 이때 (0)과 (1)을 더하면 (2)=2 인데 
	(3)=(2)+(1)로 3이고 (4)=(3)+(2)로 5이고 이런식으로 반복 된다.*/
}

int main()
{
	int n; 
	scanf("%d", &n);
	printf("%lld\n", fibo(n) * 2 + fibo(n - 1) * 2); // long long 이기 때문에 서식문자 lld 를 사용하고 둘레 구하는 공식 사용하여 출력.
	return 0;
}