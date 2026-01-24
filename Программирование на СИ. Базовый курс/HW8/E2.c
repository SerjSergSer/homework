#include <stdio.h>

int main() {
    int arr[5];
    int min;
    
    // Ввод массива из 5 элементов
    for(int i = 0; i < 5; i++) {
        scanf("%d", &arr[i]);
    }
    
    // Инициализируем min первым элементом массива
    min = arr[0];
    
    // Поиск минимального элемента
    for(int i = 1; i < 5; i++) {
        if(arr[i] < min) {
            min = arr[i];
        }
    }
    
    // Вывод результата
    printf("%d\n", min);
    
    return 0;
}
