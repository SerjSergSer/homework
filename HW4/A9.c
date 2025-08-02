#include <stdio.h>

int main()
{
	int a, b, c, d, e, x;
	scanf("%d %d %d %d %d", &a, &b, &c, &d, &e);
	x = a > b ? a : b;
	x = x > c ? x : c;
	x = x > d ? x : d;
	x = x > e ? x : e;
	printf("%d", x);
	return 0;
}

