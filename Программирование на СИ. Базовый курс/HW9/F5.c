/* #include <stdio.h>
#include <locale.h> */

int find_max_array(int size, int a[]) {
    if (size <= 0) {
        return 0;
    }
    
    int max = a[0];
    
    for (int i = 1; i < size; i++) {
        if (a[i] > max) {
            max = a[i];
        }
    }
    
    return max;
}
/*
int main() 
{
	setlocale(LC_ALL, "ru_RU.UTF-8");
    int size;
   
    printf("Введите размер массива: ");
    scanf("%d", &size);
  
    if (size <= 0) {
        printf("Размер массива должен быть положительным числом\n");
        return 1;
    }
    
    int arr[size];
    
    printf("Введите %d элементов массива:\n", size);
    for (int i = 0; i < size; i++) {
        scanf("%d", &arr[i]);
    }
  
    int max = find_max_array(size, arr);
    
    printf("Максимальный элемент: %d\n", max);
    
    return 0;
}
*/
