#include <stdio.h>

int main() 
{
    FILE *input, *output;
    int N;
    int i;
    char result[27]; 
    
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");

    fscanf(input, "%d", &N);
  
    fclose(input);
    
    // Формируем строку
    for(i = 0; i < N; i++) 
    {
        if(i % 2 == 0) 
        {
            // Нечетные позиции 
            result[i] = 'A' + i/2;
        } else 
        {
            // Четные позиции 
            result[i] = '0' + (2 * ((i/2) % 4 + 1));
        }
    }
    
    result[N] = '\0';

    fprintf(output, "%s", result);

    fclose(output);
    
    return 0;
}
