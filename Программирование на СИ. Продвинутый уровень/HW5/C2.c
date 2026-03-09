#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_STACK 10000

int stack[MAX_STACK];
int top = -1;

void push(int x) 
{
    stack[++top] = x;
}

int pop() 
{
    return stack[top--];
}

int main() 
{
    char token[100];
    
    while (scanf("%s", token) == 1) 
    {
        // Проверяем, содержит ли токен точку
        int len = strlen(token);
        
        if (token[len-1] == '.') 
        {
            // Убираем точку из токена
            token[len-1] = '\0';
            
            // Обрабатываем токен без точки
            if (isdigit(token[0])) 
            {
                push(atoi(token));
            } 
            else 
            {
                int b = pop();
                int a = pop();
                
                if (token[0] == '+') push(a + b);
                else if (token[0] == '-') push(a - b);
                else if (token[0] == '*') push(a * b);
                else if (token[0] == '/') push(a / b);
            }
            
            break;
        }
        
        if (isdigit(token[0])) 
        {
            push(atoi(token));
        } 
        else 
        {
            int b = pop();
            int a = pop();
            
            if (token[0] == '+') 
            {
                push(a + b);
            }
            else if (token[0] == '-') 
            {
                push(a - b);
            }
            else if (token[0] == '*') 
            { 
                push(a * b);
            }
            else if (token[0] == '/') 
            {
                push(a / b);
            }
        }
    }
    
    printf("%d\n", pop());
    
    return 0;
}
