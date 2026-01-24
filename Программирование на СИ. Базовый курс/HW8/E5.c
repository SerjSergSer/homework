#include <stdio.h>

int main() {
    int arr[10];
    int sum = 0;
    
    // Ввод массива из 10 элементов
    for(int i = 0; i < 10; i++) {
        scanf("%d", &arr[i]);
        
        // Если элемент положительный, добавляем его к сумме
        if(arr[i] > 0) {
            sum += arr[i];
        }
    }
    
    // Вывод суммы положительных элементов
    printf("%d\n", sum);
    
    return 0;
}
