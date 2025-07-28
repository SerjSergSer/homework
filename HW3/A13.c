#include <stdio.h>

int main()
{
	int a, x;
	scanf("%d", &a);
	x=(a/100)*(a/10%10)*(a%10);
	printf("%d\n", x);
	return 0;
}

