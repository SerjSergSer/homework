#include <stdio.h>

// Рекурсивная функция печати чисел от 1 до N
void print_numbers(int n) {
    if (n > 1) {
        // Сначала печатаем числа от 1 до n-1
        print_numbers(n - 1);
    }
    // Затем печатаем текущее число n
    printf("%d ", n);
}

int main() {
    int N;
    
    // Ввод числа
    scanf("%d", &N);
    
    // Вызов рекурсивной функции
    print_numbers(N);
    
    return 0;
}
