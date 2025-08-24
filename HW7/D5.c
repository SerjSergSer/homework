#include <stdio.h>

// Рекурсивная функция для перевода числа в двоичную систему
void decimal_to_binary(int n) {
    if (n > 1) {
        // Сначала обрабатываем старшие разряды
        decimal_to_binary(n / 2);
    }
    // Затем выводим текущий разряд
    printf("%d", n % 2);
}

int main() {
    int N;
    
    // Ввод числа
    scanf("%d", &N);
    
    // Особый случай: если число 0
    if (N == 0) {
        printf("0");
    } else {
        // Вызов рекурсивной функции
        decimal_to_binary(N);
    }
    
    return 0;
}
