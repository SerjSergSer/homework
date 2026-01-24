#include <stdio.h>
#include <string.h>

void count_digits(char number[]) {
    int count[10] = {0};
    
    for (int i = 0; number[i] != '\0'; i++) {
        if (number[i] >= '0' && number[i] <= '9') {
            int digit = number[i] - '0';
            count[digit]++;
        }
    }
    
    for (int i = 0; i < 10; i++) {
        if (count[i] > 0) {
            printf("%d %d ", i, count[i]);
        }
    }
}

int main() {
    char number[1002];
    
    scanf("%1001s", number);
    count_digits(number);
    
    return 0;
}

