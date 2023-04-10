#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

long long fibo(int n) // ��Ʈ���� ���߶� long long ����ϰ� �Լ� ���� �Ͽ� ���� 
{
	if (n == 0) return 1; // ó�� ������ 1, 1 �̹Ƿ� 0�϶� 1�� ��ȯ 
	if (n == 1) return 1; // 1�϶� 1�� ��ȯ 

	return fibo(n - 1) + fibo(n - 2); /* ����Լ��� ����Ͽ� ���� ��� ��ȯ���� ���ϰ��� �ϴ� ���� ���Ѵ�
	���� ��� fibo(5)�� fibo(4) �� fibo(3)�� ȣ���ϰ� fibo(4)�� 3�� 2�� ȣ���ϰ� �̶� (0)�� (1)�� ���ϸ� (2)=2 �ε� 
	(3)=(2)+(1)�� 3�̰� (4)=(3)+(2)�� 5�̰� �̷������� �ݺ� �ȴ�.*/
}

int main()
{
	int n; 
	scanf("%d", &n);
	printf("%lld\n", fibo(n) * 2 + fibo(n - 1) * 2); // long long �̱� ������ ���Ĺ��� lld �� ����ϰ� �ѷ� ���ϴ� ���� ����Ͽ� ���.
	return 0;
}