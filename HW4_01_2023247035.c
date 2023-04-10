#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void main() {
	int a, b, c, d, e, f;// 입력 받을 값 선언
	while (1) { // 범위 밖인 숫자를 입력하면 다시 입력하게 반복
		scanf("%d %d %d %d %d %d", &a, &b, &c, &d, &e, &f);
		if (-999 <= a, b, c, d, e, f && a, b, c, d, e, f <= 999) {
			break; // 범위에 맞게 입력하면 반복문 탈출
		}
	}
	int x1 = a * d; // 연립 방정식을 풀때 x값을 같게 하도록 2번째 식의 x 앞에 있는 숫자와 곱하여 똑같이 만든다.
	int y1 = b * d; // x에 곱한 값을 y에도 곱하여 식이 같아지도록 한다.
	int z1 = c * d;// z도 위와 마찬가지이다.
	int x2 = d * a;// 2번째 식도 첫번째 식과 곱하여 x앞에 숫자가 같도록 만든다.
	int y2 = e * a;// 위와 마찬가지이다.
	int z2 = f * a;// 위와 마찬가지이다.
	if (a > 0 && d > 0) {
		int y3 = y1 - y2; //x는 빼면 사라지므로 y끼리 빼서 y=z 형으로 만든다. 
		int z3 = z1 - z2;// z끼리 빼서 y=z형으로 만든다.
		int y = z3 / y3; //상수y=z는 y=z/상수 이므로 이렇게 표현하였다.
		int y4 = b * d * y;//y값을 x가 있는 식에 대입.
		int x = (z1 - y4) / x1;//x 구하기.
		printf("%d %d", x, y);
	}
	else if ((a > 0 && b < 0) || (a < 0 && b>0)) {
		int y3 = y1 + y2; //x는 더하면 사라지므로 y끼리 더해서 y=z 형으로 만든다. 
		int z3 = z1 + z2;// z끼리 더해서 y=z형으로 만든다.
		int y = z3 / y3; //상수 y = z 는 y = z / 상수 이므로 이렇게 표현하였다.
		int y4 = b * d * y;//y값을 x가 있는 식에 대입.
		int x = (z1 - y4) / x1;//x 구하기.
		printf("%d %d", x, y);
	}
	
}