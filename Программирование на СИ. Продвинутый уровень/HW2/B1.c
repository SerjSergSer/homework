/*
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct list {
    void *address;
    size_t size;
    char comment[64];
    struct list *next;
} list;
*/
size_t totalMemoryUsage(list *head) {
    size_t total = 0;
    list *current = head;
    
    while (current != NULL) {
        total += current->size;
        current = current->next;
    }
    
    return total;
}
/*
int main() {
    // Создаем тестовый список из примера
    list *head = NULL;
    
    // Первый узел
    list *node1 = (list *)malloc(sizeof(list));
    node1->address = (void *)140525067852320ULL;
    node1->size = 10;
    node1->next = NULL;
    head = node1;
    
    // Второй узел
    list *node2 = (list *)malloc(sizeof(list));
    node2->address = (void *)140525067852350ULL;
    node2->size = 30;
    node2->next = NULL;
    node1->next = node2;
    
    // Третий узел
    list *node3 = (list *)malloc(sizeof(list));
    node3->address = (void *)140525067852900ULL;
    node3->size = 100;
    node3->next = NULL;
    node2->next = node3;
    
    // Вычисляем общее использование памяти
    size_t result = totalMemoryUsage(head);
    
    printf("Total memory usage: %zu byte\n", result);
    
    // Освобождаем память
    free(node1);
    free(node2);
    free(node3);
    
    return 0;
}
*/
