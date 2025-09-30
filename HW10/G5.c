#include <stdio.h>
#include <string.h>

int main() 
{
    FILE *input, *output;
    char str[1001];
    int i;
 
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");

    fgets(str, 1001, input);

    fclose(input);
 
    for(i = 0; str[i] != '\0'; i++) 
    {
        if(str[i] == 'a') 
        {
            str[i] = 'b';
        } else if(str[i] == 'b') 
        {
            str[i] = 'a';
        } else if(str[i] == 'A') 
        {
            str[i] = 'B';
        } else if(str[i] == 'B') 
        {
            str[i] = 'A';
        }
    }
 
    fprintf(output, "%s", str);
   
    fclose(output);
    
    return 0;
}
