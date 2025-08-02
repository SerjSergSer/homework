#include <stdio.h>

int main()
{
	int a, b, x, y;
	scanf("%d %d", &a, &b);
	x = a < b ? a : b;
	y = x < b ? b : a;
	printf("%d %d", x, y);
	return 0;
}

