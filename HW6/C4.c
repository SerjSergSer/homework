#include <stdio.h>
#include <limits.h> // Добавляем для INT_MIN

// Функция вычисления f(x) по заданной формуле
int f(int x) {
    if (x >= 2) {
        // x ≥ 2: f(x) = x*x + 4*x + 5
        return x * x + 4 * x + 5;
    } else if (x >= -2) {
        // -2 ≤ x < 2: f(x) = x*x
        return x * x;
    } else {
        // x < -2: f(x) = 4
        return 4;
    }
}

int main() {
    int x;
    int max_value = INT_MIN; // Используем минимальное возможное значение int
    
    // Читаем последовательность чисел до 0
    while (1) {
        scanf("%d", &x);
        if (x == 0) {
            break; // Завершаем при вводе 0
        }
        
        // Вычисляем f(x) для текущего числа
        int current = f(x);
        
        // Обновляем максимум
        if (current > max_value) {
            max_value = current;
        }
    }
    
    // Выводим наибольшее значение
    printf("%d\n", max_value);
    
    return 0;
}
