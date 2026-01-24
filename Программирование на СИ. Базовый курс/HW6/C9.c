#include <stdio.h>

// Функция вычисления факториала
int factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    return n * factorial(n - 1);  // Рекурсивный вызов
}

int main() {
    int n;
    
    // Ввод числа
    scanf("%d", &n);
    
    // Вызов функции и вывод результата
    int result = factorial(n);
    printf("%d\n", result);
    
    return 0;
}
