#include <stdio.h>
#include <string.h>

int main() {
    FILE *input, *output;
    char str[101];  
    int length;
    
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");
    
    fgets(str, 101, input);
    
    // Убираем символ переноса строки в конце, если он есть
    length = strlen(str);
    if (str[length - 1] == '\n') {
        str[length - 1] = '\0';
        length--;  // Уменьшаем длину на 1
    }

    fprintf(output, "%s, %s, %s %d", str, str, str, length);

    fclose(input);
    fclose(output);
    
    return 0;
}
