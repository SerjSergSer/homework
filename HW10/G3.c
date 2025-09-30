#include <stdio.h>
#include <string.h>

int main() 
{
    FILE *input, *output;
    char str[1001];
    int length;
    int i;
    int first = 1;
    
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");
    
    fgets(str, 1001, input);
    fclose(input);
    
    length = strlen(str);
    if (length > 0 && str[length - 1] == '\n') 
    {
        str[length - 1] = '\0';
        length--;
    }
    
    if (length == 0) 
    {
        fclose(output);
        return 0;
    }
    
    char last_char = str[length - 1];
   
    for (i = 0; i < length - 1; i++) 
    {
        if (str[i] == last_char) 
        {
            if (first) 
            {
                fprintf(output, "%d", i); // Нумерация с 0
                first = 0;
            } else 
            {
                fprintf(output, " %d", i);
            }
        }
    }
    
    fclose(output);
    return 0;
}
