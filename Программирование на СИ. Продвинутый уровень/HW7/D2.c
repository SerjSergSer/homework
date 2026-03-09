#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура дерева
typedef struct tree {
    int key;
    struct tree *left, *right;
    struct tree *parent;
} tree;

// Прототипы функций
tree* findBrother(tree *root, int key);
tree* findNode(tree *root, int key);
void insert(tree **root, int key, tree *parent);
void freeTree(tree *root);

int main() {
    tree *root = NULL;
    int searchKey;
    char line[1000];
    
    // Читаем всю строку
    fgets(line, sizeof(line), stdin);
    
    // Разбиваем строку на числа
    char *token = strtok(line, " \n");
    int numbers[100];
    int count = 0;
    
    while (token != NULL && count < 100) {
        numbers[count] = atoi(token);
        count++;
        token = strtok(NULL, " \n");
    }
    
    // Последнее число - ключ для поиска
    if (count > 0) {
        searchKey = numbers[count - 1];
        
        // Вставляем все числа кроме последнего
        for (int i = 0; i < count - 1; i++) {
            insert(&root, numbers[i], NULL);
        }
    }
    
    // Ищем брата
    tree *brother = findBrother(root, searchKey);
    
    // Выводим только результат
    if (brother != NULL) {
        printf("%d\n", brother->key);
    } else {
        printf("0\n");
    }
    
    // Освобождаем память
    freeTree(root);
    
    return 0;
}

// Вставка узла в бинарное дерево поиска
void insert(tree **root, int key, tree *parent) {
    if (*root == NULL) {
        *root = (tree*)malloc(sizeof(tree));
        (*root)->key = key;
        (*root)->left = (*root)->right = NULL;
        (*root)->parent = parent;
        return;
    }
    
    if (key < (*root)->key) {
        insert(&((*root)->left), key, *root);
    } else if (key > (*root)->key) {
        insert(&((*root)->right), key, *root);
    }
}

// Рекурсивный поиск узла по ключу
tree* findNode(tree *root, int key) {
    if (root == NULL) return NULL;
    
    if (root->key == key) return root;
    
    tree *leftResult = findNode(root->left, key);
    if (leftResult != NULL) return leftResult;
    
    return findNode(root->right, key);
}

// Поиск брата узла с заданным ключом
tree* findBrother(tree *root, int key) {
    if (root == NULL) return NULL;
    
    // Находим узел с заданным ключом
    tree *node = findNode(root, key);
    
    // Если узел не найден
    if (node == NULL) return NULL;
    
    // Если у узла нет родителя (это корень)
    if (node->parent == NULL) return NULL;
    
    // Определяем, является ли узел левым или правым ребенком
    if (node->parent->left == node) {
        // Узел - левый ребенок, брат - правый ребенок
        return node->parent->right;
    } else {
        // Узел - правый ребенок, брат - левый ребенок
        return node->parent->left;
    }
}

// Освобождение памяти
void freeTree(tree *root) {
    if (root == NULL) return;
    
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}
