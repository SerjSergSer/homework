#include <stdio.h>
#include <math.h>

// Функция вычисления количества зёрен на N-й клетке
unsigned long long grains_on_cell(int n) {
    // Количество зёрен = 2^(n-1)
    return (unsigned long long)pow(2, n - 1);
}

int main() {
    int N;
    
    // Ввод номера клетки (от 1 до 64)
    scanf("%d", &N);
    
    // Вызов функции и вывод результата
    unsigned long long result = grains_on_cell(N);
    printf("%llu\n", result);
    
    return 0;
}
