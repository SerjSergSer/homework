#include <stdio.h>

int main()
{
	int a, b, c, x, y;
	scanf("%d %d %d", &a, &b, &c);
	x=a+b+c;
	y=a*b*c;
	printf("%d+%d+%d=%d\n", a, b, c, x);
	printf("%d*%d*%d=%d\n", a, b, c, y);
	return 0;
}

