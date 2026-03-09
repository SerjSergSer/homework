#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

// Структура дерева
typedef struct tree {
    int key;
    struct tree *left, *right;
} tree;

// Прототипы функций
void btUpView(tree *root);
void insert(tree **root, int key, tree *pt);

int main() {
    tree *root = NULL;
    char line[1000];
    char *token;
    
    // Читаем всю строку целиком
    fgets(line, sizeof(line), stdin);
    
    // Разбиваем строку на числа
    token = strtok(line, " \n");
    while (token != NULL) {
        int value = atoi(token);
        insert(&root, value, NULL);
        token = strtok(NULL, " \n");
    }
    
    // Вызываем функцию 
    btUpView(root);
    
    return 0;
}

// Вставка элемента в дерево
void insert(tree **root, int key, tree *pt) {
    if (*root == NULL) {
        *root = (tree*)malloc(sizeof(tree));
        (*root)->key = key;
        (*root)->left = (*root)->right = NULL;
        return;
    }
    
    if (key < (*root)->key) {
        insert(&((*root)->left), key, NULL);
    } else if (key > (*root)->key) {
        insert(&((*root)->right), key, NULL);
    }
}

// Печать вида сверху
void btUpView(tree *root) {
    if (root == NULL) return;
    
    // Структура для очереди
    typedef struct queue_node {
        tree *node;
        int hd;
        struct queue_node *next;
    } queue_node;
    
    // Массивы для хранения результатов
    int min_hd = 0, max_hd = 0;
    int top_view[200] = {0};
    int is_set[200] = {0};
    
    // Создаем очередь
    queue_node *front = NULL, *rear = NULL;
    
    // Добавляем корень
    queue_node *first = (queue_node*)malloc(sizeof(queue_node));
    first->node = root;
    first->hd = 0;
    first->next = NULL;
    front = rear = first;
    
    // Обход в ширину
    while (front != NULL) {
        tree *node = front->node;
        int hd = front->hd;
        
        // Если этот horizontal distance еще не видели
        if (!is_set[hd + 100]) {
            is_set[hd + 100] = 1;
            top_view[hd + 100] = node->key;
            
            // Обновляем границы
            if (hd < min_hd) min_hd = hd;
            if (hd > max_hd) max_hd = hd;
        }
        
        // Добавляем левого потомка
        if (node->left != NULL) {
            queue_node *new_node = (queue_node*)malloc(sizeof(queue_node));
            new_node->node = node->left;
            new_node->hd = hd - 1;
            new_node->next = NULL;
            if (rear != NULL) {
                rear->next = new_node;
                rear = new_node;
            }
        }
        
        // Добавляем правого потомка
        if (node->right != NULL) {
            queue_node *new_node = (queue_node*)malloc(sizeof(queue_node));
            new_node->node = node->right;
            new_node->hd = hd + 1;
            new_node->next = NULL;
            if (rear != NULL) {
                rear->next = new_node;
                rear = new_node;
            }
        }
        
        // Удаляем обработанный узел
        queue_node *temp = front;
        front = front->next;
        free(temp);
    }
    
    // Выводим результат
    for (int i = min_hd; i <= max_hd; i++) {
        if (is_set[i + 100]) {
            printf("%d ", top_view[i + 100]);
        }
    }
    printf("\n");
}
