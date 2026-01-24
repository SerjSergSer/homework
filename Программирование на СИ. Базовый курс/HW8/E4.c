#include <stdio.h>

int main() {
    int arr[10];
    int max1, max2;
    
    // Ввод массива из 10 элементов
    for(int i = 0; i < 10; i++) {
        scanf("%d", &arr[i]);
    }
    
    // Инициализируем два максимальных элемента
    if(arr[0] > arr[1]) {
        max1 = arr[0];
        max2 = arr[1];
    } else {
        max1 = arr[1];
        max2 = arr[0];
    }
    
    // Поиск двух максимальных элементов
    for(int i = 2; i < 10; i++) {
        if(arr[i] > max1) {
            max2 = max1;
            max1 = arr[i];
        } else if(arr[i] > max2) {
            max2 = arr[i];
        }
    }
    
    // Вывод суммы двух максимальных элементов
    printf("%d\n", max1 + max2);
    
    return 0;
}
