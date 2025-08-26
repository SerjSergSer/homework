#include <stdio.h>

int main() {
    int arr[5];
    int sum = 0;
    
    // Ввод массива из 5 элементов
    for(int i = 0; i < 5; i++) {
        scanf("%d", &arr[i]);
        sum += arr[i];
    }
    
    // Вычисление среднего арифметического
    double average = (double)sum / 5;
    
    // Вывод результата с точностью до 3 знаков после запятой
    printf("%.3f\n", average);
    
    return 0;
}
