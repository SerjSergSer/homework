#include <stdio.h>
#include <stdint.h>

int main() 
{
    uint32_t N;
    int K;
    
    scanf("%u %d", &N, &K);
    
    // Убедимся, что K в допустимом диапазоне
    K = K % 32;  // если K >= 32, берем остаток
    
    // Если K = 0, результат равен N
    if (K == 0) 
    {
        printf("%u\n", N);
        return 0;
    }
    
    // Часть 1: биты, которые сдвигаются просто вправо
    uint32_t right_part = N >> K;
    
    // Часть 2: K младших битов, которые нужно перенести в старшие позиции
    uint32_t left_part = N << (32 - K);
    
    // Объединяем обе части
    uint32_t result = right_part | left_part;
    
    printf("%u\n", result);
    
    return 0;
}
