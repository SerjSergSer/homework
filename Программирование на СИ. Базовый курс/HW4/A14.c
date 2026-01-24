#include <stdio.h>

int main()
{
	int a, b, c, x, max;
	scanf("%d", &x);
	a = x/100;
	b = x/100%10;
	c = x%10;
	max = a > b ? a : b;
	max = max > c ? max : c;
	printf("%d", max);
	return 0;
}
