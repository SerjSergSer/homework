#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Структура односвязного списка
struct list {
    char word[20];     
    struct list *next;  // Указатель на следующий элемент
};

// Прототипы функций 
struct list* add_to_list(struct list *head, const char *word);
void swap_elements(struct list *a, struct list *b);
void print_list(const struct list *head);
void delete_list(struct list *head);

int main() {
    char input[1000];  // Буфер для всей входной строки
    struct list *head = NULL;  // Начало списка


    // Ввод строки с учетом пробелов
    fgets(input, sizeof(input), stdin);

    // Удаляем символ перевода строки '\n', который fgets добавляет в конец
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    // Разбиваем строку на слова
    char *token = strtok(input, " .");  // Разделители: пробел и точка
    while (token != NULL) {
        // Добавляем слово в список
        head = add_to_list(head, token);
        token = strtok(NULL, " .");
    }

    // Сортировка пузырьком
    if (head != NULL) {
        int swapped;
        struct list *ptr;
        struct list *last = NULL;  // Конец отсортированной части

        do {
            swapped = 0;
            ptr = head;

            while (ptr->next != last) {
                // Сравниваем текущее слово со следующим
                if (strcmp(ptr->word, ptr->next->word) > 0) {
                    // Меняем местами содержимое узлов (сами узлы не переставляем)
                    swap_elements(ptr, ptr->next);
                    swapped = 1;
                }
                ptr = ptr->next;
            }
            last = ptr;  // Сдвигаем границу отсортированной части
        } while (swapped);
    }

    print_list(head);

    delete_list(head);

    getchar();

    return 0;
}

struct list* add_to_list(struct list *head, const char *word) {
    // Создаем новый узел
    struct list *new_node = (struct list*)malloc(sizeof(struct list));
    if (new_node == NULL) {
        fprintf(stderr, "Memory error\n");
        exit(1);
    }

    // Копируем слово (не больше 19 символов, оставляем место под '\0')
    strncpy(new_node->word, word, 19);
    new_node->word[19] = '\0';  // Гарантированно завершаем строку
    new_node->next = NULL;

    // Если список пуст, новый узел становится головой
    if (head == NULL) {
        return new_node;
    }

    // Иначе идем в конец списка
    struct list *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
    return head;
}

void swap_elements(struct list *a, struct list *b) {
    char temp[20];
    strcpy(temp, a->word);
    strcpy(a->word, b->word);
    strcpy(b->word, temp);
}

void print_list(const struct list *head) {
    const struct list *current = head;
    while (current != NULL) {
        printf("%s", current->word);
        if (current->next != NULL) {
            printf(" ");  
        }
        current = current->next;
    }
    printf("\n");
}

void delete_list(struct list *head) {
    struct list *current = head;
    while (current != NULL) {
        struct list *temp = current;
        current = current->next;
        free(temp);
    }
}
