/* #include <stdio.h>
#include <locale.h> */

void print_digit(char s[]) {
    int count[10] = {0};
    
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] >= '0' && s[i] <= '9') {
            int digit = s[i] - '0';
            count[digit]++;
        }
    }
    
    for (int i = 0; i < 10; i++) {
        if (count[i] > 0) {
            printf("%d %d\n", i, count[i]);
        }
    }
}
/*
int main() 
{
	setlocale(LC_ALL, "ru_RU.UTF-8");
    char str[1001];
    
    printf("Введите строку: ");
    fgets(str, sizeof(str), stdin);
    
    print_digit(str);
    
    return 0;
}
*/
