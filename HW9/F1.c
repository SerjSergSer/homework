/* #include <stdio.h>
#include <locale.h> */

void sort_array(int size, int a[])
{
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (a[j] > a[j + 1]) {
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
}
/*
int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8"); 
    
    int n;
    printf("Введите размер массива: ");
    scanf("%d", &n);
    
    int arr[n];
    printf("Введите %d элементов массива: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    
    printf("Массив до сортировки: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    sort_array(n, arr);
    
    printf("Массив после сортировки: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    return 0;
}
*/
