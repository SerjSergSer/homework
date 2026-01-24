#include <stdio.h>
int main() 
{
    int n;
    // Считываем количество чисел
    scanf("%d", &n);
    if (n <= 0) 
    {
        return 0;
    } 
    int number;          // Текущее число
    int max;             // Максимальное значение
    int count = 0;       // Количество максимумов
    
    // Считываем первое число и инициализируем максимум
    scanf("%d", &number);
    max = number;
    count = 1;
    
    // Обрабатываем оставшиеся n-1 чисел
    for (int i = 1; i < n; i++) 
    {
        scanf("%d", &number);
        
        if (number > max) 
        {
            // Нашли новый максимум
            max = number;
            count = 1;
        } else if (number == max) 
        {
            // Нашли такое же значение как максимум
            count++;
        }
    }
    
    printf("%d\n", count);
    return 0;
}
