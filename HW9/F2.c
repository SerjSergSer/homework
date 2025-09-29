/* #include <stdio.h>
#include <locale.h> */

void sort_even_odd(int n, int a[])
{
    int result[n];
    int even_count = 0;  // счетчик четных чисел
    int odd_count = 0;   // счетчик нечетных чисел
    
    // Сначала записываем все четные числа
    for (int i = 0; i < n; i++) {
        if (a[i] % 2 == 0) {
            result[even_count] = a[i];
            even_count++;
        }
    }
    
    // Затем записываем все нечетные числа
    for (int i = 0; i < n; i++) {
        if (a[i] % 2 != 0) {
            result[even_count + odd_count] = a[i];
            odd_count++;
        }
    }
    
    // Копируем результат обратно в исходный массив
    for (int i = 0; i < n; i++) {
        a[i] = result[i];
    }
}
/*
int main()
{
     setlocale(LC_ALL, "ru_RU.UTF-8");
    
    int size;
    
    printf("Введите размер массива: ");
    scanf("%d", &size);
   
    int arr[size];
    
    printf("Введите %d чисел: ", size);
    for (int i = 0; i < size; i++) {
        scanf("%d", &arr[i]);
    }

    printf("\nИсходный массив: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    sort_even_odd(size, arr);
 
    printf("Отсортированный массив (четные сначала): ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    return 0;
}
*/
