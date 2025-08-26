#include <stdio.h>

int main() {
    int arr[10];
    int max, min;
    int max_index = 0, min_index = 0;  // Исправлено: индексы начинаются с 0
    
    // Ввод массива из 10 элементов
    for(int i = 0; i < 10; i++) {
        scanf("%d", &arr[i]);
    }
    
    // Инициализируем max и min первым элементом массива
    max = arr[0];
    min = arr[0];
    
    // Поиск максимального и минимального элементов и их индексов
    for(int i = 1; i < 10; i++) 
    {  
        if(arr[i] > max) {
            max = arr[i];
            max_index = i;
        }
        if(arr[i] < min) {
            min = arr[i];
            min_index = i;
        }
    }
    
    // Вывод результата (если нужно выводить номера с 1, а не индексы с 0)
    printf("%d %d %d %d\n", max_index + 1, max, min_index + 1, min);
    
    return 0;
}
