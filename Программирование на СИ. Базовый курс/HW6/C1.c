#include <stdio.h>

int abs(int num) {
    return (num < 0) ? -num : num;
}

int main() {
    int num;
    scanf("%d", &num);
    printf("%d\n", abs(num)); 
    return 0;
}
