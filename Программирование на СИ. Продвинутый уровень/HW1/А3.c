#include <stdio.h>
#include <math.h> 

int main() 
{
    unsigned int N;
    int K;
    
    scanf("%u %d", &N, &K);
    
    // Остаток от деления на 2^K
    unsigned int result = N % (unsigned int)pow(2, K);
    
    printf("%u\n", result);
    
    return 0;
}
