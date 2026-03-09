#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ncurses/ncurses.h>
#include <time.h>

#define MIN_Y 2
#define START_TAIL_SIZE 3
#define MAX_TAIL_SIZE 100
#define CONTROLS 3
#define MAX_FOOD_SIZE 20
#define FOOD_EXPIRE_SECONDS 10

enum {
    LEFT = 1,
    UP,
    RIGHT,
    DOWN,
    STOP_GAME = KEY_F(10)
};

struct control_buttons
{
    int down;
    int up;
    int left;
    int right;
};

// Массив с кодами клавиш для разных схем управления
struct control_buttons default_controls[CONTROLS] = {
    {KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT},  
    {'S', 'W', 'A', 'D'},                       
    {'s', 'w', 'a', 'd'}                       
};

typedef struct snake_t
{
    int x;
    int y;
    int direction;
    size_t tsize;
    struct tail_t* tail;
    struct control_buttons* controls;
} snake_t;

typedef struct tail_t
{
    int x;
    int y;
} tail_t;

typedef struct food {
    int x;
    int y;
    time_t put_time;
    char point;
    uint8_t enable;
} food_t;

food_t food[MAX_FOOD_SIZE];

void initTail(tail_t t[], size_t size)
{
    tail_t init_t = {0, 0};
    for (size_t i = 0; i < size; i++)
        t[i] = init_t;
}

void initHead(snake_t* head, int x, int y)
{
    head->x = x;
    head->y = y;
    head->direction = RIGHT;
}

void initSnake(snake_t* head, size_t size, int x, int y)
{
    tail_t* tail = (tail_t*)malloc(MAX_TAIL_SIZE * sizeof(tail_t));
    initTail(tail, MAX_TAIL_SIZE);
    initHead(head, x, y);
    head->tail = tail;
    head->tsize = size;
    head->controls = default_controls;
    
    // Инициализируем начальное положение хвоста (змейка горизонтально вправо)
    for (int i = 0; i < head->tsize; i++)
    {
        head->tail[i].x = x - i - 1;
        head->tail[i].y = y;
    }
}

void addTail(snake_t* head)
{
    head->tsize++;
}

int haveEat(snake_t* head, food_t f[])
{
    for (int i = 0; i < MAX_FOOD_SIZE; i++)
    {
        if (f[i].enable && head->x == f[i].x && head->y == f[i].y)
        {
            f[i].enable = 0;
            addTail(head);
            return 1;
        }
    }
    return 0;
}

void go(snake_t* head)
{
    char ch = '@';
    mvprintw(head->y, head->x, " ");
    
    switch (head->direction)
    {
        case LEFT:
            head->x--;
            break;
        case RIGHT:
            head->x++;
            break;
        case UP:
            head->y--;
            break;
        case DOWN:
            head->y++;
            break;
    }
    
    mvprintw(head->y, head->x, "%c", ch);
    refresh();
}

void goTail(snake_t* head)
{
    char ch = '*';
    // Очищаем последний элемент хвоста
    mvprintw(head->tail[head->tsize - 1].y, head->tail[head->tsize - 1].x, " ");
    
    // Сдвигаем хвост
    for (size_t i = head->tsize - 1; i > 0; i--)
    {
        head->tail[i] = head->tail[i - 1];
    }
    
    // Новый первый элемент хвоста - бывшая позиция головы
    head->tail[0].x = head->x;
    head->tail[0].y = head->y;
    
    // Отрисовываем весь хвост
    for (size_t i = 0; i < head->tsize; i++)
    {
        mvprintw(head->tail[i].y, head->tail[i].x, "%c", ch);
    }
}

int checkDirection(snake_t* snake, int32_t key)
{
    // Проверяем для всех схем управления
    for (int i = 0; i < CONTROLS; i++)
    {
        if ((key == snake->controls[i].down && snake->direction == UP) ||
            (key == snake->controls[i].up && snake->direction == DOWN) ||
            (key == snake->controls[i].left && snake->direction == RIGHT) ||
            (key == snake->controls[i].right && snake->direction == LEFT))
            return 0;  // Нельзя повернуть назад
    }
    return 1;  
}

//Тестирование WASD
void changeDirection(snake_t* snake, const int32_t key)
{
    // Сначала проверяем, можно ли изменить направление
    if (!checkDirection(snake, key))
        return;
    
    // Прямая проверка клавиш
    if (key == 'W' || key == 'w')
    {
        mvprintw(4, 1, "UP pressed!    "); 
        snake->direction = UP;
    }
    else if (key == 'S' || key == 's')
    {
        mvprintw(4, 1, "DOWN pressed!  "); 
        snake->direction = DOWN;
    }
    else if (key == 'A' || key == 'a')
    {
        mvprintw(4, 1, "LEFT pressed!  "); 
        snake->direction = LEFT;
    }
    else if (key == 'D' || key == 'd')
    {
        mvprintw(4, 1, "RIGHT pressed! "); 
        snake->direction = RIGHT;
    }
    else if (key == KEY_UP)
    {
        mvprintw(4, 1, "UP ARROW pressed!    ");
        snake->direction = UP;
    }
    else if (key == KEY_DOWN)
    {
        mvprintw(4, 1, "DOWN ARROW pressed!  ");
        snake->direction = DOWN;
    }
    else if (key == KEY_LEFT)
    {
        mvprintw(4, 1, "LEFT ARROW pressed!  ");
        snake->direction = LEFT;
    }
    else if (key == KEY_RIGHT)
    {
        mvprintw(4, 1, "RIGHT ARROW pressed! ");
        snake->direction = RIGHT;
    }
}

_Bool isCrush(snake_t* snake) // Проверка, не врезалась ли змейка сама в себя
{
    for (size_t i = 1; i < snake->tsize; i++)
    {
        if (snake->x == snake->tail[i].x && snake->y == snake->tail[i].y)
            return 1;
    }
    return 0;
}

// Проверка выхода за границы (игра заканчивается)
_Bool isOutOfBounds(snake_t* snake)
{
    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x);
    
    if (snake->x < 0 || snake->x >= max_x || 
        snake->y < MIN_Y || snake->y >= max_y)
        return 1;
    return 0;
}

void printGameOver()
{
    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x);
    mvprintw(max_y / 2 - 1, max_x / 2 - 5, "GAME OVER!");
    mvprintw(max_y / 2, max_x / 2 - 8, "Press any key...");
    refresh();
}

void initFood(food_t f[], size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        f[i].x = 0;
        f[i].y = 0;
        f[i].put_time = 0;
        f[i].point = 0;
        f[i].enable = 0;
    }
}

void putFoodSeed(food_t* fp)
{
    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x);
    
    // Очищаем старую позицию, если была
    if (fp->enable)
        mvprintw(fp->y, fp->x, " ");
    
    // Генерируем новую позицию
    fp->x = 1 + rand() % (max_x - 2);
    fp->y = MIN_Y + rand() % (max_y - MIN_Y - 1);
    fp->put_time = time(NULL);
    fp->point = '$';
    fp->enable = 1;
    
    mvprintw(fp->y, fp->x, "%c", fp->point);
}

void putFood(food_t f[], size_t number_seeds)
{
    for (size_t i = 0; i < number_seeds; i++)
        putFoodSeed(&f[i]);
}

void refreshFood(food_t f[], int nfood)
{
    for (int i = 0; i < nfood; i++)
    {
        if (!f[i].enable || (time(NULL) - f[i].put_time) > FOOD_EXPIRE_SECONDS)
        {
            putFoodSeed(&f[i]);
        }
    }
}

// Функция для задержки через clock()
void delay(int milliseconds)
{
    clock_t start_time = clock();
    while (clock() < start_time + milliseconds * CLOCKS_PER_SEC / 1000);
}

int main(void)
{
    snake_t* snake = (snake_t*)malloc(sizeof(snake_t));
    initSnake(snake, START_TAIL_SIZE, 10, 10);
    
    initscr();
    keypad(stdscr, TRUE);
    raw();
    noecho();
    curs_set(FALSE);
    nodelay(stdscr, TRUE);  // Делаем getch() неблокирующим
    
    mvprintw(0, 1, "Use arrows or WASD keys for control. Press 'F10' for EXIT.");
    mvprintw(1, 1, "Score: %lld", (long long)(snake->tsize - START_TAIL_SIZE));
    mvprintw(2, 1, "WASD Test - Press keys to see codes:");
    
    initFood(food, MAX_FOOD_SIZE);
    putFood(food, MAX_FOOD_SIZE);
    
    int key_pressed = 0;
    int game_running = 1;
    
    while (key_pressed != STOP_GAME && game_running)
    {
        key_pressed = getch();
        
        if (key_pressed != ERR)
        {
            // Отображаем код нажатой клавиши
            mvprintw(3, 1, "Key code: %d (%c)    ", key_pressed, 
                    (key_pressed >= 32 && key_pressed <= 126) ? key_pressed : '?');
            
            // Изменяем направление
            changeDirection(snake, key_pressed);
        }
       
        go(snake);
        goTail(snake);
        
        // Проверка выхода за границы
        if (isOutOfBounds(snake))
        {
            game_running = 0;
            printGameOver();
            break;
        }
        
        // Проверка на столкновение с собой
        if (isCrush(snake))
        {
            game_running = 0;
            printGameOver();
            break;
        }
        
        // Проверка съедения еды
        if (haveEat(snake, food))
        {
            mvprintw(1, 8, "%lld", (long long)(snake->tsize - START_TAIL_SIZE));
        }
        
        // Обновляем еду
        refreshFood(food, MAX_FOOD_SIZE);
        
        // Задержка для контроля скорости
        delay(150);
    }
    
    if (!game_running)
    {
        nodelay(stdscr, FALSE);
        timeout(-1);
        getch();
    }
    
    free(snake->tail);
    free(snake);
    endwin();
    return 0;
}
