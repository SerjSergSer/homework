#include <stdio.h>

// Функция возвращает среднее арифметическое двух чисел
int middle(int a, int b) {
    // Используем формулу: (a + b) / 2
    return (a + b) / 2;
}

int main() {
    int num1, num2;
    
    // Ввод двух целых неотрицательных чисел
    scanf("%d %d", &num1, &num2);
    
    // Вызов функции и вывод результата
    int result = middle(num1, num2);
    printf("%d\n", result);
    
    return 0;
}
