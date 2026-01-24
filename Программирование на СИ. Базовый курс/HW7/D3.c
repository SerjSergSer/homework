#include <stdio.h>

// Рекурсивная функция для вывода цифр числа в обратном порядке
void print_digits_reverse(int n) {
    if (n < 10) {
        // Если число однозначное, просто выводим его
        printf("%d", n);
    } else {
        // Выводим последнюю цифру
        printf("%d ", n % 10);
        // Рекурсивно обрабатываем оставшуюся часть числа
        print_digits_reverse(n / 10);
    }
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
        print_digits_reverse(N);
    }
    
    return 0;
}
