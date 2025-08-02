#include <stdio.h>

int main()
{
	int a, b, c, x;
	scanf("%d %d %d", &a, &b, &c);
	x = a > b ? a : b;
	x = x > c ? x : c;
	printf("%d", x);
	return 0;
}

