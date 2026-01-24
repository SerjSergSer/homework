#include <stdio.h>
#include <stdlib.h>

int main() {
    int number;
    scanf("%d", &number);
    number = abs(number);
    
    int prev_digit = -1; // Предыдущая цифра (изначально не существует)
    int has_duplicates = 0; // Флаг наличия повторяющихся цифр
    
    while (number > 0) {
        int current_digit = number % 10;
        
        if (current_digit == prev_digit) {
            has_duplicates = 1;
            break;
        }
        
        prev_digit = current_digit;
        number /= 10;
    }
    
    printf(has_duplicates ? "YES\n" : "NO\n");
    return 0;
}
