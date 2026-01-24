#include <stdio.h>

int main() {
    int arr[10];
    
    // Ввод массива из 10 элементов
    for(int i = 0; i < 10; i++) {
        scanf("%d", &arr[i]);
    }
    
    // Инверсия первой половины (элементы 0-4)
    for(int i = 0; i < 2; i++) {
        int temp = arr[i];
        arr[i] = arr[4 - i];
        arr[4 - i] = temp;
    }
    
    // Инверсия второй половины (элементы 5-9)
    for(int i = 5; i < 7; i++) {
        int temp = arr[i];
        arr[i] = arr[14 - i]; // 14 - i: для i=5 -> 9, i=6 -> 8
        arr[14 - i] = temp;
    }
    
    // Вывод измененного массива
    for(int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    
    return 0;
}
