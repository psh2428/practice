#include <stdio.h>


void print_array(int a[5][5]);

void snail_sort(int b[5][5]);


int main() {
	int arr[5][5];
	snail_sort(arr);
	print_array(arr);

}

void print_array(int a[5][5]) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			printf("%3d ", a[i][j]);

		}
		printf("\n");
	}
}

void snail_sort(int b[5][5]) {
	int x = 0;
	int y = -1;
	int num = 0;
	int flag = -1;
	int i, j, n = 5;
	for (i = 0; i < n; i++) {
		num++;
		y++;
		b[x][y] = num;
	}

	for (i = n - 1; i > 0; i--) {
		for (j = 0; j < i; j++) {
			if (flag == -1) {
				x++;
			}
			else {
				x--;
			}
			num++;
			b[x][y] = num;
		}

		for (j = 0; j < i; j++) {
			if (flag == -1) {
				y--;
			}
			else {
				y++;
			}
			num++;
			b[x][y] = num;
		}
		flag *= -1;
	}

}






