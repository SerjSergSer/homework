/*#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

// Определение структуры списка
typedef struct list {
    uint64_t address;
    size_t size;
    char comment[64];
    struct list *next;
} list;

// Функция для поиска адреса блока с максимальным размером
*/
uint64_t findMaxBlock(list *head) {
    // Если список пустой, возвращаем 0
    if (head == NULL) {
        return 0;
    }
    
    // Инициализируем максимальные значения первым элементом
    size_t max_size = head->size;
    uint64_t max_address = head->address;
    
    // Проходим по всем элементам списка
    list *current = head;
    while (current != NULL) {
        // Если находим блок с большим размером, обновляем значения
        if (current->size > max_size) {
            max_size = current->size;
            max_address = current->address;
        }
        current = current->next;
    }
    
    return max_address;
}
/*
// Вспомогательная функция для создания нового узла
list* create_node(uint64_t address, size_t size) {
    list* new_node = (list*)malloc(sizeof(list));
    if (new_node == NULL) {
        return NULL;
    }
    
    new_node->address = address;
    new_node->size = size;
    new_node->next = NULL;
    
    return new_node;
}

// Вспомогательная функция для освобождения памяти
void free_list(list* head) {
    list* current = head;
    while (current != NULL) {
        list* next = current->next;
        free(current);
        current = next;
    }
}

// Основная функция для демонстрации
int main() {
    // Тест 1: Пустой список
    printf("Test 1\n");
    list* test1 = NULL;
    uint64_t result1 = findMaxBlock(test1);
    printf("Result: %llu (Expected: 0)\n\n", result1);
    
    // Тест 2: Список с одним элементом
    printf("Test 2: List\n");
    list* test2 = create_node(1000, 50);
    result1 = findMaxBlock(test2);
    printf("Result: %llu (Expected: 1000)\n\n", result1);
    free_list(test2);
    
    // Тест 3: Список из примера задания
    printf("Test 3: List\n");
    list* test3 = create_node(140525067852320, 10);
    test3->next = create_node(140525067852350, 30);
    test3->next->next = create_node(140525067852900, 100);
    
    // Выводим список для наглядности
    list* current = test3;
    printf("List:\n");
    while (current != NULL) {
        printf("  Adress: %llu, size: %zu\n", current->address, current->size);
        current = current->next;
    }
    
    result1 = findMaxBlock(test3);
    printf("Result: %llu (Expected: 140525067852900)\n\n", result1);
    
    // Тест 4: Несколько блоков с одинаковым максимальным размером
    printf("Test 4: List\n");
    list* test4 = create_node(100, 50);
    test4->next = create_node(200, 30);
    test4->next->next = create_node(300, 50);  // Такой же размер как у первого
    
    current = test4;
    printf("List:\n");
    while (current != NULL) {
        printf("  Adress: %llu, Size: %zu\n", current->address, current->size);
        current = current->next;
    }
    
    result1 = findMaxBlock(test4);
    printf("Result: %llu (100 or 300)\n\n", result1);
    
    // Освобождаем память
    free_list(test3);
    free_list(test4);
    
    // Дополнительный тест: чтение из стандартного ввода
    printf("Test\n");
    printf("Enter the number of items: ");
    int n;
    scanf("%d", &n);
    
    if (n > 0) {
        list* user_list = NULL;
        list* last = NULL;
        
        printf("Enter %d pairs (adress size):\n", n);
        for (int i = 0; i < n; i++) {
            uint64_t addr;
            size_t sz;
            scanf("%llu %zu", &addr, &sz);
            
            list* new_node = create_node(addr, sz);
            
            if (user_list == NULL) {
                user_list = new_node;
                last = new_node;
            } else {
                last->next = new_node;
                last = new_node;
            }
        }
        
        printf("Entered list:\n");
        current = user_list;
        while (current != NULL) {
            printf("  Adress: %llu, Size: %zu\n", current->address, current->size);
            current = current->next;
        }
        
        result1 = findMaxBlock(user_list);
        printf("Adress: %llu\n", result1);
        
        free_list(user_list);
    }
    
    return 0;
}
*/
