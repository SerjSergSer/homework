#include <stdio.h>
#include <ctype.h>  // для функции toupper()

int main() {
    char c;
    
    while ((c = getchar()) != '.') {
        putchar(toupper(c));
    }
    
    return 0;
}
