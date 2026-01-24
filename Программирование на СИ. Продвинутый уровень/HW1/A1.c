#include <stdio.h>
int main() 
{
    int N;
    char c;
    
    scanf("%d", &N);
    
    // Читаем пробел после числа
    scanf(" ");
    
    // Обрабатываем символы
    while (scanf("%c", &c) == 1 && c != '.') 
    {
        if (c >= 'A' && c <= 'Z') 
        {
            printf("%c", 'A' + (c - 'A' + N) % 26);
        } 
        else if (c >= 'a' && c <= 'z') 
        {
            printf("%c", 'a' + (c - 'a' + N) % 26);
        } 
        else if (c == ' ') 
        {
            printf(" ");
        }
    }
    
    printf(".\n");
    return 0;
}
