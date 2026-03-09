#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NUM 999  

int main() 
{
    char N[100001]; 
    bool found[MAX_NUM + 1] = { false };
    int count = 0;
    int len;

    scanf("%s", N);

    len = strlen(N);

    // Перебираем все тройки цифр с сохранением порядка
    for (int i = 0; i < len - 2; i++) 
    {
        // Первая цифра не может быть нулем для трехзначного числа
        if (N[i] == '0') continue;

        for (int j = i + 1; j < len - 1; j++) 
        {
            for (int k = j + 1; k < len; k++) 
            {
                // Составляем трехзначное число
                int number = (N[i] - '0') * 100 + 
                            (N[j] - '0') * 10 + 
                            (N[k] - '0');

                // Если такое число еще не встречалось
                if (!found[number]) 
                {
                    found[number] = true;
                    count++;
                }
            }
        }
    }

    printf("%d\n", count); 
    return 0;
}
