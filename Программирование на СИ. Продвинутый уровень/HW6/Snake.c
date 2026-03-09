#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses/ncurses.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>

#define MIN_Y  2
double DELAY = 0.1;
#define PLAYERS  2

enum {LEFT=1, UP, RIGHT, DOWN, STOP_GAME=KEY_F(10)};

enum {MAX_TAIL_SIZE=100, START_TAIL_SIZE=3, MAX_FOOD_SIZE=20, FOOD_EXPIRE_SECONDS=10, SEED_NUMBER=3, CONTROLS=2};

enum {SNAKE1_COLOR=1, SNAKE2_COLOR=2, FOOD_COLOR=3, WALL_COLOR=4};

enum {CLASSIC_MODE=1, WALLS_MODE=2, NO_SELF_COLLISION_MODE=3};

enum {HUMAN_OPPONENT=1, BOT_OPPONENT=2};

enum {ARROWS=1, WASD=2};

// Структура для кнопок управления
struct control_buttons
{
    int down;
    int up;
    int left;
    int right;
} control_buttons;

struct control_buttons default_controls[CONTROLS] = {{KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT},
                                                    {'s', 'w', 'a', 'd'}};

// Структура змейки
typedef struct snake_t
{
    int x;                          // координата x головы
    int y;                          // координата y головы
    int direction;                  // направление движения
    size_t tsize;                    // размер хвоста
    struct tail_t *tail;            // указатель на хвост
    struct control_buttons controls; // кнопки управления для этой змейки
    int color;                       // цвет змейки
    int score;                       // счет игрока
    int is_bot;                       // флаг: 1 если бот, 0 если человек
    int just_ate;                     // флаг: только что съел еду
    int control_scheme;               // схема управления (ARROWS или WASD)
} snake_t;

// Структура элемента хвоста
typedef struct tail_t
{
    int x;
    int y;
} tail_t;

// Структура еды
struct food
{
    int x;
    int y;
    time_t put_time;    // время появления
    char point;         // символ еды
    uint8_t enable;      // активна ли еда
} food[MAX_FOOD_SIZE];

// Глобальные переменные для настроек игры
int game_mode = CLASSIC_MODE;           // режим игры
int opponent_type = HUMAN_OPPONENT;     // тип противника
int snake1_color = SNAKE1_COLOR;        // цвет первой змейки
int snake2_color = SNAKE2_COLOR;        // цвет второй змейки
int player1_control = ARROWS;           // управление первого игрока
char control_str[100];                   // строка с инструкцией по управлению
time_t last_bonus_time = 0; // Время последнего появления бонуса

// Функция установки цвета
void setColor(int objectType)
{
    attroff(COLOR_PAIR(1));
    attroff(COLOR_PAIR(2));
    attroff(COLOR_PAIR(3));
    attroff(COLOR_PAIR(4));
    switch (objectType)
    {
        case 1: // ЗМЕЙКА1
            attron(COLOR_PAIR(1));
            break;
        case 2: // ЗМЕЙКА2
            attron(COLOR_PAIR(2));
            break;
        case 3: // ЕДА
            attron(COLOR_PAIR(3));
            break;
        case 4: // СТЕНА
            attron(COLOR_PAIR(4));
            break;
    }
}

// Функция отрисовки границ игрового поля
void drawBorders()
{
    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x);
    
    setColor(WALL_COLOR);
    
    // Рисуем верхнюю границу
    for (int i = 45; i < max_x; i++)
    {
        if (i % 2 == 0)
            mvprintw(MIN_Y - 1, i, ".");
    }
    
    // Рисуем нижнюю границу
    for (int i = 0; i < max_x; i++)
    {
        if (i % 2 == 0)
            mvprintw(max_y - 1, i, ".");
    }
    
    // Рисуем левую границу, очищаем угол где будет счет
    mvprintw(MIN_Y - 1, 0, " ");
    for (int i = MIN_Y; i < max_y - 1; i++)
    {
        if (i % 2 == 0)
            mvprintw(i, 0, ".");
    }
    
    // Рисуем правую границу
    for (int i = MIN_Y; i < max_y - 1; i++)
    {
        if (i % 2 == 0)
            mvprintw(i, max_x - 1, ".");
    }
}

// Функция вычисления расстояния до еды (для бота)
int distance(const snake_t snake, const struct food food)
{
    return (abs(snake.x - food.x) + abs(snake.y - food.y));
}

// Проверка безопасности позиции для бота
int is_position_safe(snake_t *snake, int new_x, int new_y)
{
    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x);
    
    // Проверка столкновения со стеной в классическом режиме
    if (game_mode == CLASSIC_MODE)
    {
        if (new_x <= 0 || new_x >= max_x - 1 || 
            new_y <= MIN_Y - 1 || new_y >= max_y - 1)
        {
            return 0; 
        }
    }
    
    // Проверка столкновения с самим собой
    for (size_t i = 0; i < snake->tsize; i++)
    {
        if (snake->tail[i].x == new_x && snake->tail[i].y == new_y)
        {
            return 0; 
        }
    }
    
    return 1; // Безопасно
}

// Функция ИИ бота с избеганием препятствий
void autoChangeDirection(snake_t *snake, struct food food[], int foodSize)
{
    if (!snake->is_bot) return; // Если не бот, выходим
    
    int pointer = -1;
    int min_dist = 1000;
    
    // Поиск ближайшей активной еды
    for (int i = 0; i < foodSize; i++)
    {
        if (food[i].enable)
        {
            int dist = distance(*snake, food[i]);
            if (pointer == -1 || dist < min_dist)
            {
                pointer = i;
                min_dist = dist;
            }
        }
    }
    
    if (pointer == -1) return; // Если нет активной еды, выходим
    
    int dx = food[pointer].x - snake->x;
    int dy = food[pointer].y - snake->y;
    
    // Пробуем направления в порядке приоритета
    int possible_directions[4] = {0};
    int direction_count = 0;
    
    // Добавляем направления на основе положения еды
    if (abs(dx) > abs(dy))
    {
        // Приоритет горизонтального движения
        if (dx > 0) {
            possible_directions[direction_count++] = RIGHT;
            possible_directions[direction_count++] = DOWN;
            possible_directions[direction_count++] = UP;
            possible_directions[direction_count++] = LEFT;
        } else {
            possible_directions[direction_count++] = LEFT;
            possible_directions[direction_count++] = DOWN;
            possible_directions[direction_count++] = UP;
            possible_directions[direction_count++] = RIGHT;
        }
    }
    else
    {
        // Приоритет вертикального движения
        if (dy > 0) {
            possible_directions[direction_count++] = DOWN;
            possible_directions[direction_count++] = RIGHT;
            possible_directions[direction_count++] = LEFT;
            possible_directions[direction_count++] = UP;
        } else {
            possible_directions[direction_count++] = UP;
            possible_directions[direction_count++] = RIGHT;
            possible_directions[direction_count++] = LEFT;
            possible_directions[direction_count++] = DOWN;
        }
    }
    
    // Пробуем каждое направление по порядку
    for (int i = 0; i < direction_count; i++)
    {
        int new_dir = possible_directions[i];
        
        // Запрещаем разворот на 180 градусов
        if ((snake->direction == RIGHT && new_dir == LEFT) ||
            (snake->direction == LEFT && new_dir == RIGHT) ||
            (snake->direction == UP && new_dir == DOWN) ||
            (snake->direction == DOWN && new_dir == UP))
        {
            continue;
        }
        
        // Вычисляем новую позицию
        int new_x = snake->x;
        int new_y = snake->y;
        
        switch (new_dir)
        {
            case LEFT:  new_x--; break;
            case RIGHT: new_x++; break;
            case UP:    new_y--; break;
            case DOWN:  new_y++; break;
        }
        
        // Проверяем безопасность направления
        if (is_position_safe(snake, new_x, new_y))
        {
            snake->direction = new_dir;
            return;
        }
    }
    
    // Если безопасное направление не найдено, пробуем продолжить текущее
    int new_x = snake->x;
    int new_y = snake->y;
    switch (snake->direction)
    {
        case LEFT:  new_x--; break;
        case RIGHT: new_x++; break;
        case UP:    new_y--; break;
        case DOWN:  new_y++; break;
    }
    
    if (is_position_safe(snake, new_x, new_y))
    {
        return; // Продолжаем текущее направление
    }
    
    // Пробуем любое безопасное направление
    int all_directions[4] = {RIGHT, DOWN, LEFT, UP};
    for (int i = 0; i < 4; i++)
    {
        int new_dir = all_directions[i];
        
        // Запрещаем разворот
        if ((snake->direction == RIGHT && new_dir == LEFT) ||
            (snake->direction == LEFT && new_dir == RIGHT) ||
            (snake->direction == UP && new_dir == DOWN) ||
            (snake->direction == DOWN && new_dir == UP))
        {
            continue;
        }
        
        new_x = snake->x;
        new_y = snake->y;
        switch (new_dir)
        {
            case LEFT:  new_x--; break;
            case RIGHT: new_x++; break;
            case UP:    new_y--; break;
            case DOWN:  new_y++; break;
        }
        
        if (is_position_safe(snake, new_x, new_y))
        {
            snake->direction = new_dir;
            return;
        }
    }
}

// Инициализация массива еды
void initFood(struct food f[], size_t size)
{
    struct food init = {0,0,0,0,0};
    for(size_t i=0; i<size; i++)
    {
        f[i] = init;
    }
}

// Размещение одного зерна еды на поле
void putFoodSeed(struct food *fp)
{
    int max_x=0, max_y=0;
    char spoint[2] = {0};
    getmaxyx(stdscr, max_y, max_x);
    mvprintw(fp->y, fp->x, " "); // Очищаем старую позицию
    fp->x = rand() % (max_x - 2) + 1; // Не занимаем границы
    fp->y = rand() % (max_y - MIN_Y - 1) + MIN_Y; // Не занимаем границы
    fp->put_time = time(NULL);
    fp->point = '$';
    fp->enable = 1;
    spoint[0] = fp->point;
    
    setColor(FOOD_COLOR);
    mvprintw(fp->y, fp->x, "%s", spoint);
}

// Размещение всей еды на поле
void putFood(struct food f[], size_t number_seeds)
{
    for(size_t i=0; i<number_seeds; i++)
    {
        putFoodSeed(&f[i]);
    }
}

// Обновление еды (перемещение старой)
void refreshFood(struct food f[], int nfood)
{
    for(size_t i=0; i<nfood; i++)
    {
        if( f[i].put_time )
        {
            if( !f[i].enable || (time(NULL) - f[i].put_time) > FOOD_EXPIRE_SECONDS )
            {
                putFoodSeed(&f[i]);
            }
        }
    }
}

// Инициализация хвоста
void initTail(struct tail_t t[], size_t size)
{
    struct tail_t init_t= {0,0};
    for(size_t i=0; i<size; i++)
    {
        t[i]=init_t;
    }
}

// Инициализация головы змейки
void initHead(struct snake_t *head, int x, int y)
{
    head->x = x;
    head->y = y;
    head->direction = RIGHT;
    head->score = 0;
    head->just_ate = 0;
}

// Инициализация всей змейки
void initSnake(snake_t *head[], size_t size, int x, int y, int i)
{
    head[i] = (snake_t*)malloc(sizeof(snake_t));
    tail_t* tail = (tail_t*) malloc(MAX_TAIL_SIZE*sizeof(tail_t));
    initTail(tail, MAX_TAIL_SIZE);
    initHead(head[i], x, y);
    head[i]->tail = tail;
    head[i]->tsize = size+1;
    
    if (i == 0) // Первая змейка (игрок)
    {
        head[i]->control_scheme = player1_control;
        if (player1_control == ARROWS)
        {
            head[i]->controls = default_controls[0]; // Стрелки
            head[i]->control_scheme = ARROWS;
        }
        else
        {
            head[i]->controls = default_controls[1]; // WASD
            head[i]->control_scheme = WASD;
        }
        head[i]->is_bot = 0;
    }
    else // Вторая змейка (противник)
    {
        if (opponent_type == HUMAN_OPPONENT)
        {
            // Для человека - противника используем противоположную схему управления
            if (player1_control == ARROWS)
            {
                head[i]->controls = default_controls[1]; // WASD
                head[i]->control_scheme = WASD;
            }
            else
            {
                head[i]->controls = default_controls[0]; // Стрелки
                head[i]->control_scheme = ARROWS;
            }
            head[i]->is_bot = 0;
        }
        else
        {
            // Для бота всегда WASD (не важно, бот не использует клавиши)
            head[i]->controls = default_controls[1];
            head[i]->is_bot = 1;
            head[i]->control_scheme = WASD;
        }
    }
    
    if (i == 0)
        head[i]->color = snake1_color;
    else
        head[i]->color = snake2_color;
}

// Движение головы
void go(struct snake_t *head)
{
    char ch = '@';
    int max_x=0, max_y=0;
    getmaxyx(stdscr, max_y, max_x);
    
    setColor(head->color);
    mvprintw(head->y, head->x, " "); // Очищаем старую позицию
    
    // Изменяем координаты в зависимости от направления
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
    
    // Телепортация при выходе за границы в режиме WALLS_MODE или NO_SELF_COLLISION_MODE
    if (game_mode == WALLS_MODE || game_mode == NO_SELF_COLLISION_MODE)
    {
        if (head->x <= 0) head->x = max_x - 2;
        if (head->x >= max_x - 1) head->x = 1;
        if (head->y <= MIN_Y - 1) head->y = max_y - 2;
        if (head->y >= max_y - 1) head->y = MIN_Y;
    }
    
    mvprintw(head->y, head->x, "%c", ch); // Рисуем голову на новой позиции
}

// Изменение направления движения
void changeDirection(struct snake_t* snake, const int32_t key)
{
    if (key == snake->controls.down)
        snake->direction = DOWN;
    else if (key == snake->controls.up)
        snake->direction = UP;
    else if (key == snake->controls.right)
        snake->direction = RIGHT;
    else if (key == snake->controls.left)
        snake->direction = LEFT;
}

// Движение хвоста
void goTail(struct snake_t *head)
{
    char ch = '*';
    setColor(head->color);
    mvprintw(head->tail[head->tsize-1].y, head->tail[head->tsize-1].x, " "); // Очищаем последний элемент хвоста
    
    // Сдвигаем все элементы хвоста
    for(size_t i = head->tsize-1; i>0; i--)
    {
        head->tail[i] = head->tail[i-1];
        if( head->tail[i].y || head->tail[i].x)
            mvprintw(head->tail[i].y, head->tail[i].x, "%c", ch);
    }
    // Новый первый элемент хвоста становится на место головы
    head->tail[0].x = head->x;
    head->tail[0].y = head->y;
}

// Проверка, съела ли змейка еду
_Bool haveEat(struct snake_t *head, struct food f[])
{
    for (size_t i = 0; i < MAX_FOOD_SIZE; i++)
    {
        if (f[i].enable && head->x == f[i].x && head->y == f[i].y)
        {
            f[i].enable = 0; // Деактивируем съеденную еду
            head->score += 10; // Увеличиваем счет
            head->just_ate = 1; // Устанавливаем флаг, что только что съели
            return 1;
        }
    }
    return 0;
}

// Увеличение хвоста (после съедания еды)
void addTail(struct snake_t *head)
{
    if (head->tsize < MAX_TAIL_SIZE)
    {
        // Сдвигаем все элементы хвоста
        for (size_t i = head->tsize; i > 0; i--)
        {
            head->tail[i] = head->tail[i-1];
        }
        
        // Добавляем новый элемент в начало хвоста
        head->tail[0].x = head->x;
        head->tail[0].y = head->y;
        head->tsize++;
    }
}

// Проверка корректности направления (запрет разворота)
int checkDirection(snake_t* snake, int32_t key)
{
    // Проверяем, принадлежит ли клавиша этой змейке
    int is_this_snakes_key = (key == snake->controls.down || 
                              key == snake->controls.up || 
                              key == snake->controls.left || 
                              key == snake->controls.right);
    
    if (!is_this_snakes_key)
        return 0;
    
    // Запрещаем разворот на 180 градусов
    if ((key == snake->controls.up && snake->direction == DOWN) ||
        (key == snake->controls.down && snake->direction == UP) ||
        (key == snake->controls.left && snake->direction == RIGHT) ||
        (key == snake->controls.right && snake->direction == LEFT))
    {
        return 0;
    }
    return 1;
}

// Функция обновления игры для одной змейки
void update(struct snake_t *head, struct food f[], const int32_t key)
{
    clock_t begin = clock();
    
    head->just_ate = 0; // Сбрасываем флаг
    
    if (head->is_bot)
    {
        autoChangeDirection(head, f, SEED_NUMBER); // Управление бота
    }
    
    setColor(head->color);
    
    // Очищаем конкретные области для каждой змейки с отступами
    if (!head->is_bot && head->control_scheme == player1_control) // Змейка 1 (игрок)
    {
        mvprintw(1, 2, "                "); // Очищаем позицию Snake1 с отступом 2
    }
    else // Змейка 2 или Бот
    {
        mvprintw(1, 30, "                "); // Очищаем позицию Snake2/Bot с отступом 30
    }
    
    go(head);
    goTail(head);
    
    // Изменяем направление только для человека
    if (!head->is_bot && checkDirection(head, key))
    {
        changeDirection(head, key);
    }
    
    refreshFood(food, SEED_NUMBER); // Обновляем еду
    
    if (haveEat(head, food))
    {
        addTail(head); // Увеличиваем хвост, ускоряем игру при наборе очков
        if (head->score % 50 == 0 && DELAY > 0.03)
            DELAY -= 0.01;
    }
    
    // Отображаем счет после всех изменений с отступами
    setColor(head->color);
    if (!head->is_bot && head->control_scheme == player1_control) // Змейка 1 (игрок)
    {
        mvprintw(1, 2, "Snake1 Score: %d", head->score); // Отступ 2 от левого края
    }
    else // Змейка 2 или Бот
    {
        if (head->is_bot)
            mvprintw(1, 30, "Bot Score: %d", head->score); // Отступ 30 от левого края
        else
            mvprintw(1, 30, "Snake2 Score: %d", head->score); // Отступ 30 от левого края
    }
    
    // Восстанавливаем строку с управлением (с отступом 2)
    mvprintw(0, 2, "%s", control_str);
    drawBorders(); // Перерисовываем границы
    
    refresh(); // Обновление экрана
    
    // Задержка для управления скоростью игры
    while ((double)(clock() - begin)/CLOCKS_PER_SEC < DELAY)
    {}
}   

// Проверка столкновения змейки
_Bool isCrush(snake_t * snake)
{
    if (game_mode == NO_SELF_COLLISION_MODE)
        return 0; // В режиме без самопересечения всегда возвращаем 0
        
    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x);
    
    // Проверка столкновения со стеной только в CLASSIC_MODE
    if (game_mode == CLASSIC_MODE)
    {
        if (snake->x <= 0 || snake->x >= max_x - 1 || 
            snake->y <= MIN_Y - 1 || snake->y >= max_y - 1)
        {
            return 1; // Столкновение со стеной
        }
    }
    
    // Проверка столкновения с собственным хвостом (для всех режимов, кроме NO_SELF_COLLISION_MODE)
    for (size_t i = 1; i < snake->tsize; i++)
    {
        // Пропускаем первый сегмент, если змейка только что съела
        if (snake->just_ate && i == 1)
            continue;
            
        if (snake->tail[i].x == snake->x && snake->tail[i].y == snake->y)
        {
            return 1; // Столкновение с хвостом
        }
    }
    
    return 0; // Столкновений нет
}

// Исправление позиций еды (чтобы не появлялась на змейке)
void repairSeed(struct food f[], size_t nfood, struct snake_t *head)
{
    // Проверка совпадения еды с хвостом
    for (size_t i = 0; i < head->tsize; i++)
    {
        for (size_t j = 0; j < nfood; j++)
        {
            if (f[j].enable && 
                head->tail[i].x == f[j].x && 
                head->tail[i].y == f[j].y)
            {
                putFoodSeed(&f[j]); // Перемещаем зерно
            }
        }
    }
    
    // Проверка совпадения еды с головой
    for (size_t j = 0; j < nfood; j++)
    {
        if (f[j].enable && 
            head->x == f[j].x && 
            head->y == f[j].y)
        {
            putFoodSeed(&f[j]); // Перемещаем зерно
        }
    }
    
    // Проверка совпадения зерен между собой
    for (size_t i = 0; i < nfood; i++)
    {
        for (size_t j = i + 1; j < nfood; j++)
        {
            if (f[i].enable && f[j].enable && 
                f[i].x == f[j].x && 
                f[i].y == f[j].y)
            {
                putFoodSeed(&f[j]); // Перемещаем второе зерно
            }
        }
    }
}

// Стартовое меню
void startMenu()
{
    int choice = 0;
    int color_choice = 0;
    
    clear();
    mvprintw(5, 10, "=== SNAKE GAME ===");
    mvprintw(7, 10, "Select opponent type:");
    mvprintw(9, 12, "1. Human opponent (2 players)");
    mvprintw(10, 12, "2. Bot opponent (play against AI)");
    mvprintw(11, 10, "Your choice (1-2): ");
    
    choice = getch();
    switch(choice)
    {
        case '1':
            opponent_type = HUMAN_OPPONENT;
            break;
        case '2':
            opponent_type = BOT_OPPONENT;
            break;
        default:
            opponent_type = HUMAN_OPPONENT;
    }
    
    clear();
    mvprintw(5, 10, "=== SNAKE GAME ===");
    mvprintw(7, 10, "Select control scheme for Player 1:");
    mvprintw(9, 12, "1. Arrow keys");
    mvprintw(10, 12, "2. WASD keys");
    mvprintw(11, 10, "Your choice (1-2): ");
    
    choice = getch();
    switch(choice)
    {
        case '1':
            player1_control = ARROWS;
            break;
        case '2':
            player1_control = WASD;
            break;
        default:
            player1_control = ARROWS;
    }
    
    clear();
    mvprintw(5, 10, "=== SNAKE GAME ===");
    mvprintw(7, 10, "Select game mode:");
    mvprintw(9, 12, "1. Classic (walls kill)");
    mvprintw(10, 12, "2. Walls mode (teleport through walls)");
    mvprintw(11, 12, "3. No self-collision mode (teleport through walls)");
    mvprintw(12, 10, "Your choice (1-3): ");
    
    choice = getch();
    switch(choice)
    {
        case '1':
            game_mode = CLASSIC_MODE;
            break;
        case '2':
            game_mode = WALLS_MODE;
            break;
        case '3':
            game_mode = NO_SELF_COLLISION_MODE;
            break;
        default:
            game_mode = CLASSIC_MODE;
    }
    
    clear();
    mvprintw(5, 10, "=== SNAKE GAME ===");
    mvprintw(7, 10, "Select Snake1 color:");
    mvprintw(9, 12, "1. Red (default)");
    mvprintw(10, 12, "2. Blue");
    mvprintw(11, 12, "3. Green");
    mvprintw(13, 10, "Your choice (1-3): ");
    
    color_choice = getch();
    switch(color_choice)
    {
        case '1':
            snake1_color = SNAKE1_COLOR;
            break;
        case '2':
            snake1_color = SNAKE2_COLOR;
            break;
        case '3':
            snake1_color = FOOD_COLOR;
            break;
        default:
            snake1_color = SNAKE1_COLOR;
    }
    
    clear();
    mvprintw(5, 10, "=== SNAKE GAME ===");
    mvprintw(7, 10, "Select Snake2 color:");
    mvprintw(9, 12, "1. Red");
    mvprintw(10, 12, "2. Blue (default)");
    mvprintw(11, 12, "3. Green");
    mvprintw(13, 10, "Your choice (1-3): ");
    
    color_choice = getch();
    switch(color_choice)
    {
        case '1':
            snake2_color = SNAKE1_COLOR;
            break;
        case '2':
            snake2_color = SNAKE2_COLOR;
            break;
        case '3':
            snake2_color = FOOD_COLOR;
            break;
        default:
            snake2_color = SNAKE2_COLOR;
    }
    
    clear();
    mvprintw(10, 10, "Press any key to start game...");
    refresh();
    getch();
    clear();
}

// Добавление бонусной еды, бот не будет видеть бонусную еду
void putBonusFood(struct food f[], size_t nfood)
{
    time_t current_time = time(NULL);
    
    // Проверяем, прошло ли достаточно времени с последнего бонуса (минимум 3 секунды)
    if (current_time - last_bonus_time < 3)
        return;
    
    // Считаем текущую бонусную еду
    int bonus_count = 0;
    for (size_t i = SEED_NUMBER; i < nfood; i++)
    {
        if (f[i].enable && f[i].point == 'B')
            bonus_count++;
    }
    
    // Не больше 2 бонусов одновременно
    if (bonus_count >= 2)
        return;
    
    // 10% шанс появления бонуса (при выполнении условий)
    if (rand() % 100 < 10)
    {
        for (size_t i = SEED_NUMBER; i < nfood; i++)
        {
            if (!f[i].enable)
            {
                int max_x, max_y;
                getmaxyx(stdscr, max_y, max_x);
                f[i].x = rand() % (max_x - 2) + 1;
                f[i].y = rand() % (max_y - MIN_Y - 1) + MIN_Y;
                f[i].put_time = current_time;
                f[i].point = 'B';
                f[i].enable = 1;
                setColor(FOOD_COLOR);
                mvprintw(f[i].y, f[i].x, "B");
                
                last_bonus_time = current_time;
                break;
            }
        }
    }
}

int main()
{
    srand(time(NULL)); // Инициализация генератора случайных чисел
    
    // Инициализация ncurses
    initscr();
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_BLACK); // Цвет для стен
    
    keypad(stdscr, TRUE); // Включаем функциональные клавиши
    raw();                // Отключаем буферизацию строк
    noecho();             // Отключаем отображение вводимых символов
    curs_set(FALSE);      // Отключаем курсор
    
    startMenu(); // Показываем меню
    
    // Создание змеек
    snake_t* snakes[PLAYERS];
    for (int i = 0; i < PLAYERS; i++)
        initSnake(snakes, START_TAIL_SIZE, 10+i*10, 10+i*10, i);
    
    // Формирование строки с управлением
    if (opponent_type == BOT_OPPONENT)
    {
        if (player1_control == ARROWS)
            sprintf(control_str, "Player: arrows, Opponent: BOT. Press 'F10' for EXIT");
        else
            sprintf(control_str, "Player: WASD, Opponent: BOT. Press 'F10' for EXIT");
    }
    else
    {
        if (player1_control == ARROWS)
            sprintf(control_str, "P1: arrows, P2: WASD. Press 'F10' for EXIT");
        else
            sprintf(control_str, "P1: WASD, P2: arrows. Press 'F10' for EXIT");
    }
    
    // Отображаем строку с управлением с отступом
    mvprintw(0, 2, "%s", control_str);
    drawBorders(); // Рисуем границы
    
    timeout(0); // Неблокирующий ввод
    
    initFood(food, MAX_FOOD_SIZE);
    putFood(food, SEED_NUMBER); // Размещаем еду
    
    int key_pressed = 0;
    
    // Основной игровой цикл
    while(key_pressed != STOP_GAME)
    {
        key_pressed = getch(); // Считываем клавишу
        
        putBonusFood(food, MAX_FOOD_SIZE); // Добавляем бонусную еду
        
        for (int i = 0; i < PLAYERS; i++)
        {
            update(snakes[i], food, key_pressed); // Обновляем состояние змейки
            
            // Проверка на столкновение
            if(isCrush(snakes[i]))
            {
                clear();
                mvprintw(5, 10, "=== GAME OVER ===");
                if (i == 0)
                    mvprintw(7, 10, "Snake 1 CRASHED!");
                else if (snakes[i]->is_bot)
                    mvprintw(7, 10, "Bot CRASHED! You win!");
                else
                    mvprintw(7, 10, "Snake 2 CRASHED!");
                
                mvprintw(9, 10, "Final Scores:");
                mvprintw(10, 10, "Snake1: %d", snakes[0]->score);
                if (snakes[1]->is_bot)
                    mvprintw(11, 10, "Bot: %d", snakes[1]->score);
                else
                    mvprintw(11, 10, "Snake2: %d", snakes[1]->score);
                
                refresh();
                sleep(3);
                key_pressed = STOP_GAME;
                break;
            }
            
            repairSeed(food, SEED_NUMBER, snakes[i]); // Исправляем позиции еды
        }
    }
    
    // Показываем финальный счет
    clear();
    mvprintw(5, 10, "=== GAME OVER ===");
    mvprintw(7, 10, "Snake1 Score: %d", snakes[0]->score);
    
    if (snakes[1]->is_bot)
        mvprintw(8, 10, "Bot Score: %d", snakes[1]->score);
    else
        mvprintw(8, 10, "Snake2 Score: %d", snakes[1]->score);
    
    if (snakes[0]->score > snakes[1]->score)
        mvprintw(10, 10, "Snake 1 wins!");
    else if (snakes[0]->score < snakes[1]->score)
    {
        if (snakes[1]->is_bot)
            mvprintw(10, 10, "Bot wins!");
        else
            mvprintw(10, 10, "Snake 2 wins!");
    }
    else
        mvprintw(10, 10, "It's a tie!");
    
    mvprintw(12, 10, "Press any key to exit...");
    refresh();
    getch();
    
    // Освобождение памяти
    for (int i = 0; i < PLAYERS; i++)
    {
        free(snakes[i]->tail);
        free(snakes[i]);
    }
    
    endwin(); // Завершение работы с ncurses
    return 0;
}
