#include <stdio.h>
#include <stdlib.h>
int main()
{
	int a, b, i;
	scanf("%d %d", &a, &b);
	if (a > b || abs(a) > 100 || abs(b) > 100) {
        return 1;
	}
	for (i=a; i<=b; i++) {
		printf("%d ", i*i);
	}
	return 0;
}

