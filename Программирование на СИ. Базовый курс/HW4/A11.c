#include <stdio.h>

int main()
{
	int a, b, c, d, e, x, y, sum;
	scanf("%d %d %d %d %d", &a, &b, &c, &d, &e);
	x = a < b ? a : b;
	x = x < c ? x : c;
	x = x < d ? x : d;
	x = x < e ? x : e;
	y = a > b ? a : b;
	y = y > c ? y : c;
	y = y > d ? y : d;
	y = y > e ? y : e;
	sum = x + y;
	printf("%d", sum);
	return 0;
}
