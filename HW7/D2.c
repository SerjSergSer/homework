#include <stdio.h>

// Рекурсивная функция вычисления суммы чисел от 1 до N
int sum_to_n(int n) {
    if (n == 1) {
        return 1;  // Базовый случай: сумма чисел от 1 до 1 = 1
    } else {
        return n + sum_to_n(n - 1);  // Рекурсивный случай
    }
}

int main() {
    int N;
    
    // Ввод числа
    scanf("%d", &N);
    
    // Вызов рекурсивной функции и вывод результата
    printf("%d\n", sum_to_n(N));
    
    return 0;
}
