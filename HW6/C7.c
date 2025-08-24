#include <stdio.h>

void decimal_to_p_recursive(int n, int p) {
    if (n == 0) return;
    
    // Рекурсивный вызов для старших разрядов
    decimal_to_p_recursive(n / p, p);
    
    // Вывод текущей цифры
    printf("%d", n % p);
}

int main() {
    int N, P;
    scanf("%d %d", &N, &P);
    
    // Особый случай: ноль
    if (N == 0) {
        printf("0\n");
        return 0;
    }
    
    decimal_to_p_recursive(N, P);
    printf("\n");
    
    return 0;
}
