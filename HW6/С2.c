#include <stdio.h>

// Функция возведения числа n в степень p
int power(int n, int p) {
    int result = 1;
    
    // Умножаем result на n p раз 
    for (int i = 0; i < p; i++) {
        result *= n;
    }
    
    return result;
}

int main() {
    int N, P;
    
    // Ввод данных
    scanf("%d %d", &N, &P);
    
    // Вызов функции и вывод результата
    int result = power(N, P);
    printf("%d\n", result);
    
    return 0;
}
