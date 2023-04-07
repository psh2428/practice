#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
	char str[50];
	gets_s(str);
	for (int i = 0; str[i]; i++) {
		if (str[i] >= 'A' && str[i] <= 'Z') {
			str[i] += 32;
		}
	}
	printf("%s", str);
	return 0;
	
}