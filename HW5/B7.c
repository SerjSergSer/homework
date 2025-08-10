#include <stdio.h>
#include <stdlib.h>

int main() {
    int number;
    scanf("%d", &number);
    
    number = abs(number); // Обрабатываем отрицательные числа
    
    int digit_counts[10] = {0}; // Массив для подсчёта цифр (0..9)
    int has_duplicates = 0;
    
    while (number > 0) {
        int digit = number % 10;
        digit_counts[digit]++;
        
        if (digit_counts[digit] >= 2) {
            has_duplicates = 1;
            break;
        }
        
        number /= 10;
    }
    
    printf(has_duplicates ? "YES\n" : "NO\n");
    return 0;
}
