#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

long long fibo(int n)
{
	if (n == 0) return 1;
	if (n == 1) return 1;

	return fibo(n - 1) + fibo(n - 2);
}

int main()
{
	int n; 
	scanf("%d", &n);
	printf("%lld\n", fibo(n) * 2 + fibo(n - 1) * 2);
	return 0;
}
