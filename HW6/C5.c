#include <stdio.h>

// Функция вычисления суммы чисел от 1 до N
int sum_to_n(int n) {
    int sum = 0;
    for (int i = 1; i <= n; i++) {
        sum += i;
    }
    return sum;
}

int main() {
    int N;
    
    // Ввод числа N
    scanf("%d", &N);
    
    // Вызов функции и вывод результата
    int result = sum_to_n(N);
    printf("%d\n", result);
    
    return 0;
}
