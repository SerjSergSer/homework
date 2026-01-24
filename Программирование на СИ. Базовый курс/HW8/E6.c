#include <stdio.h>

int main() {
    int arr[12];
    int sum = 0;
    
    // Ввод массива из 12 элементов
    for(int i = 0; i < 12; i++) {
        scanf("%d", &arr[i]);
        sum += arr[i];
    }
    
    // Вычисление среднего арифметического
    double average = (double)sum / 12;
    
    // Вывод результата с точностью до 2 знаков после запятой
    printf("%.2f\n", average);
    
    return 0;
}
