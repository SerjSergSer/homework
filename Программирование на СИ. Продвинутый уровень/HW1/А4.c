#include <stdio.h>

int main() 
{
    unsigned int N;
    int K;
    
    scanf("%u %d", &N, &K);
    
    int max = 0;
    
    // Перебираем все возможные позиции для K битов
    // В 32-битном числе можно разместить K битов в (32 - K + 1) позициях
    for (int i = 0; i <= 32 - K; i++) 
    {
        // Создаем маску для K битов, начиная с позиции i
        unsigned int mask = 0;
        
        // Заполняем маску единицами в нужных позициях
        for (int j = 0; j < K; j++) 
        {
            mask = mask | (1u << (i + j));
        }
        
        // Применяем маску к числу N
        unsigned int current = N & mask;
        
        // Сдвигаем выделенные биты вправо, чтобы получить число
        current = current >> i;
        
        // Обновляем максимум
        if (current > max) 
        {
            max = current;
        }
    }
    
    printf("%d\n", max);
    
    return 0;
}
