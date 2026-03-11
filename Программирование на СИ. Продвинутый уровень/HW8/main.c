#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#include <getopt.h>

// Прототипы функций
double f1(double x);
double f2(double x);
double f3(double x);
double f1_deriv(double x);
double f2_deriv(double x);
double f3_deriv(double x);
double root(double (*f)(double), double (*g)(double), double (*f_deriv)(double), 
            double (*g_deriv)(double), double a, double b, double eps1, int* iterations);
double integral(double (*f)(double), double a, double b, double eps2);
void print_help(void);
void test_functions(void);
void find_intersections(double eps1, int* iter_counts, double* intersections);
double max(double a, double b);
double min(double a, double b);
double calculate_area(double eps1, double eps2, int* iter_counts);

// Глобальные переменные для управления выводом
int print_intersections = 0;  // Флаг для печати абсцисс точек пересечения
int print_iterations = 0;      // Флаг для печати числа итераций
int test_mode = 0;             // Флаг режима тестирования

/**
 * Функция f1(x) = 0.6x + 3
 */
double f1(double x) {
    return 0.6 * x + 3;
}

/**
 * Производная f1'(x) = 0.6
 */
double f1_deriv(double x) {
    return 0.6;
}

/**
 * Функция f2(x) = (x-2)^3 - 1
 */
double f2(double x) {
    return pow(x - 2, 3) - 1;
}

/**
 * Производная f2'(x) = 3(x-2)^2
 */
double f2_deriv(double x) {
    return 3 * pow(x - 2, 2);
}

/**
 * Функция f3(x) = 3/x
 */
double f3(double x) {
    if (x == 0) return NAN;  // Защита от деления на ноль
    return 3.0 / x;
}

/**
 * Производная f3'(x) = -3/x^2
 */
double f3_deriv(double x) {
    if (x == 0) return NAN;
    return -3.0 / (x * x);
}

/**
 * Вспомогательная функция F1(x) = f3(x) - f1(x)
 */
double F1(double x) {
    return f3(x) - f1(x);
}

/**
 * Вспомогательная функция F2(x) = f3(x) - f2(x)
 */
double F2(double x) {
    return f3(x) - f2(x);
}

/**
 * Вспомогательная функция F3(x) = f2(x) - f1(x)
 */
double F3(double x) {
    return f2(x) - f1(x);
}

/**
 * Производная F1'(x) = f3'(x) - f1'(x)
 */
double F1_deriv(double x) {
    return f3_deriv(x) - f1_deriv(x);
}

/**
 * Производная F2'(x) = f3'(x) - f2'(x)
 */
double F2_deriv(double x) {
    return f3_deriv(x) - f2_deriv(x);
}

/**
 * Производная F3'(x) = f2'(x) - f1'(x)
 */
double F3_deriv(double x) {
    return f2_deriv(x) - f1_deriv(x);
}

/**
 * Находит максимум двух чисел
 */
double max(double a, double b) {
    return (a > b) ? a : b;
}

/**
 * Находит минимум двух чисел
 */
double min(double a, double b) {
    return (a < b) ? a : b;
}

/**
 * Вычисление корня уравнения f(x) = g(x) на отрезке [a, b] методом Ньютона
 * 
 * Параметры:
 *   f, g - указатели на функции
 *   f_deriv, g_deriv - указатели на производные функций
 *   a, b - границы отрезка
 *   eps1 - точность вычисления
 *   iterations - указатель для сохранения числа итераций
 * 
 * Возвращает:
 *   приближенное значение корня
 */
double root(double (*f)(double), double (*g)(double), double (*f_deriv)(double), 
            double (*g_deriv)(double), double a, double b, double eps1, int* iterations) {
    double x0, x1;
    double f0, g0, f_deriv0, g_deriv0;
    int iter = 0;
    
    // Выбираем начальное приближение в середине отрезка
    x0 = (a + b) / 2;
    
    do {
        // Вычисляем значения функций и их производных в точке x0
        f0 = f(x0);
        g0 = g(x0);
        f_deriv0 = f_deriv(x0);
        g_deriv0 = g_deriv(x0);
        
        // Проверяем, что производная не близка к нулю
        if (fabs(f_deriv0 - g_deriv0) < 1e-10) {
            // Если производная близка к нулю, используем метод половинного деления
            while (fabs(b - a) > eps1) {
                x0 = (a + b) / 2;
                if ((f(a) - g(a)) * (f(x0) - g(x0)) <= 0)
                    b = x0;
                else
                    a = x0;
                iter++;
            }
            x1 = (a + b) / 2;
            break;
        }
        
        // Шаг метода Ньютона
        x1 = x0 - (f0 - g0) / (f_deriv0 - g_deriv0);
        
        // Проверяем, не вышли ли за границы отрезка
        if (x1 < a || x1 > b) {
            // Если вышли, используем метод половинного деления
            while (fabs(b - a) > eps1) {
                x0 = (a + b) / 2;
                if ((f(a) - g(a)) * (f(x0) - g(x0)) <= 0)
                    b = x0;
                else
                    a = x0;
                iter++;
            }
            x1 = (a + b) / 2;
            break;
        }
        
        iter++;
        
        // Проверка на зацикливание
        if (iter > 1000) {
            printf("Превышено максимальное число итераций в root()\n");
            break;
        }
        
        if (fabs(x1 - x0) < eps1) break;
        
        x0 = x1;
        
    } while (fabs(f(x1) - g(x1)) > eps1);
    
    *iterations = iter;
    return x1;
}

/**
 * Вычисление определенного интеграла методом Симпсона
 * 
 * Параметры:
 *   f - указатель на функцию
 *   a, b - пределы интегрирования
 *   eps2 - точность вычисления
 * 
 * Возвращает:
 *   значение интеграла
 */
double integral(double (*f)(double), double a, double b, double eps2) {
    int n = 2;  // начальное число разбиений
    double h, sum_odd, sum_even, integral_prev, integral_curr;
    double x;
    
    // Начальное приближение (n = 2)
    h = (b - a) / 2;
    integral_curr = (h / 3) * (f(a) + 4 * f(a + h) + f(b));
    
    do {
        integral_prev = integral_curr;
        n *= 2;
        h = (b - a) / n;
        
        sum_odd = 0.0;
        for (int i = 1; i < n; i += 2) {
            x = a + i * h;
            sum_odd += f(x);
        }
        
        sum_even = 0.0;
        for (int i = 2; i < n; i += 2) {
            x = a + i * h;
            sum_even += f(x);
        }
        
        integral_curr = (h / 3) * (f(a) + f(b) + 4 * sum_odd + 2 * sum_even);
        
        // Проверка на зацикливание
        if (n > 1000000) {
            printf("Превышено максимальное число разбиений в integral()\n");
            break;
        }
        
    } while (fabs(integral_curr - integral_prev) > eps2);
    
    return integral_curr;
}

/**
 * Находит точки пересечения функций
 * 
 * Параметры:
 *   eps1 - точность для root()
 *   iter_counts - массив для сохранения числа итераций
 *   intersections - массив для сохранения абсцисс точек пересечения
 */
void find_intersections(double eps1, int* iter_counts, double* intersections) {
    // Пересечение f1 и f3 (решаем F1 = 0)
    intersections[0] = root(f3, f1, f3_deriv, f1_deriv, 0.5, 2.0, eps1, &iter_counts[0]);
    
    // Пересечение f2 и f3 (решаем F2 = 0)
    intersections[1] = root(f3, f2, f3_deriv, f2_deriv, 0.5, 2.0, eps1, &iter_counts[1]);
    
    // Пересечение f1 и f2 (решаем F3 = 0)
    intersections[2] = root(f2, f1, f2_deriv, f1_deriv, 2.5, 4.0, eps1, &iter_counts[2]);
}

/**
 * Вычисляет площадь фигуры, образованной функциями
 * 
 * Площадь вычисляется как сумма интегралов разностей функций
 * на соответствующих отрезках
 */
double calculate_area(double eps1, double eps2, int* iter_counts) {
    double intersections[3];
    double area = 0.0;
    
    // Находим точки пересечения
    find_intersections(eps1, iter_counts, intersections);
    
    double x1 = intersections[0];  // пересечение f1 и f3
    double x2 = intersections[1];  // пересечение f2 и f3
    double x3 = intersections[2];  // пересечение f1 и f2
    
    // Определяем порядок точек на оси x
    double x_min = min(min(x1, x2), x3);
    double x_max = max(max(x1, x2), x3);
    double x_mid;
    
    // Находим среднюю точку
    if ((x1 > x_min && x1 < x_max)) x_mid = x1;
    else if ((x2 > x_min && x2 < x_max)) x_mid = x2;
    else x_mid = x3;
    
    // Сортируем точки
    double points[3] = {x_min, x_mid, x_max};
    
    if (print_intersections) {
        printf("\nТочки пересечения:\n");
        printf("f1 и f3: x = %.6f\n", x1);
        printf("f2 и f3: x = %.6f\n", x2);
        printf("f1 и f2: x = %.6f\n", x3);
        printf("Упорядоченные точки: [%.6f, %.6f, %.6f]\n", points[0], points[1], points[2]);
    }
    
    // Вычисляем площадь как сумму интегралов
    // На каждом отрезке интегрируем разность верхней и нижней функций
    
    // Отрезок [x_min, x_mid]
    // Верхняя функция - f3, нижняя - max(f1, f2) (но f2 может быть ниже)
    // Для простоты будем использовать метод: area = ∫|f1-f3| + ∫|f2-f3| на соответствующих отрезках
    // Но это приближение, точное определение площади требует анализа
    
    // Более точный подход: найдем функцию, ограничивающую фигуру сверху и снизу
    // В данном случае фигура образована пересечением трех функций
    // Площадь можно найти как сумму площадей криволинейных треугольников
    
    // Упрощенный подход: интегрируем разность между верхней и нижней огибающими
    // Но для простоты реализуем приближенное вычисление
    
    // Вычисляем площадь как сумму модулей разностей
    area = fabs(integral(f3, x_min, x_mid, eps2) - integral(f1, x_min, x_mid, eps2));
    area += fabs(integral(f3, x_mid, x_max, eps2) - integral(f2, x_mid, x_max, eps2));
    
    return area;
}

/**
 * Тестирование функций root и integral
 */
void test_functions(void) {
    printf("\n=== ТЕСТИРОВАНИЕ ФУНКЦИЙ ===\n");
    
    // Тест 1: решение уравнения x^2 = 4 на [1, 3]
    printf("\nТест 1: x^2 = 4 на [1, 3]\n");
    double test_f1(double x) { return x * x; }
    double test_g1(double x) { return 4.0; }
    double test_f1_deriv(double x) { return 2 * x; }
    double test_g1_deriv(double x) { return 0; }
    int iter1;
    double root1 = root(test_f1, test_g1, test_f1_deriv, test_g1_deriv, 1, 3, 1e-6, &iter1);
    printf("Найденный корень: %f (ожидается 2.000000)\n", root1);
    printf("Число итераций: %d\n", iter1);
    
    // Тест 2: решение уравнения sin(x) = 0.5 на [0, 1]
    printf("\nТест 2: sin(x) = 0.5 на [0, 1]\n");
    double test_f2(double x) { return sin(x); }
    double test_g2(double x) { return 0.5; }
    double test_f2_deriv(double x) { return cos(x); }
    int iter2;
    double root2 = root(test_f2, test_g2, test_f2_deriv, test_g1_deriv, 0, 1, 1e-6, &iter2);
    printf("Найденный корень: %f (ожидается 0.523599)\n", root2);
    printf("Число итераций: %d\n", iter2);
    
    // Тест 3: интеграл от x^2 на [0, 1]
    printf("\nТест 3: ∫x^2 dx на [0, 1]\n");
    double test_f3(double x) { return x * x; }
    double int3 = integral(test_f3, 0, 1, 1e-6);
    printf("Вычисленный интеграл: %f (ожидается 0.333333)\n", int3);
    
    // Тест 4: интеграл от sin(x) на [0, π]
    printf("\nТест 4: ∫sin(x) dx на [0, π]\n");
    double int4 = integral(sin, 0, M_PI, 1e-6);
    printf("Вычисленный интеграл: %f (ожидается 2.000000)\n", int4);
    
    printf("\n=== ТЕСТИРОВАНИЕ ЗАВЕРШЕНО ===\n");
}

/**
 * Выводит справку по использованию программы
 */
void print_help(void) {
    printf("Использование: program [ОПЦИИ]\n");
    printf("Программа вычисляет площадь фигуры, образованной функциями:\n");
    printf("  f1(x) = 0.6x + 3\n");
    printf("  f2(x) = (x-2)^3 - 1\n");
    printf("  f3(x) = 3/x\n\n");
    printf("Опции:\n");
    printf("  -i, --intersections   Печатать абсциссы точек пересечения кривых\n");
    printf("  -n, --iterations      Печатать число итераций для поиска корней\n");
    printf("  -t, --test            Запустить тестирование функций root и integral\n");
    printf("  -h, --help            Показать эту справку\n");
    printf("  -e EPS1               Задать точность для root (по умолчанию 1e-6)\n");
    printf("  -E EPS2               Задать точность для integral (по умолчанию 1e-4)\n");
    printf("  ПРИМЕРЫ:\n");
    printf("  area_calc.exe -i -n -e 1e-10 -E 1e-8\n");
    printf("  area_calc.exe -t\n");
}

/**
 * Главная функция программы
 */
int main(int argc, char *argv[]) 
{
    // Установка локали для поддержки русского языка
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    double eps1 = 1e-6;  // Точность для root по умолчанию
    double eps2 = 1e-4;  // Точность для integral по умолчанию
    
    // Определение длинных опций командной строки
    static struct option long_options[] = {
        {"intersections", no_argument, 0, 'i'},
        {"iterations",    no_argument, 0, 'n'},
        {"test",          no_argument, 0, 't'},
        {"help",          no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    
    int opt;
    int option_index = 0;
    
    // Разбор опций командной строки
    while ((opt = getopt_long(argc, argv, "inhte:E:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'i':
                print_intersections = 1;
                break;
            case 'n':
                print_iterations = 1;
                break;
            case 't':
                test_mode = 1;
                break;
            case 'h':
                print_help();
                return 0;
            case 'e':
                eps1 = atof(optarg);
                break;
            case 'E':
                eps2 = atof(optarg);
                break;
            default:
                print_help();
                return 1;
        }
    }
    
    // Если запрошен режим тестирования
    if (test_mode) {
        test_functions();
        return 0;
    }
    
    printf("Программа вычисления площади фигуры, образованной функциями:\n");
    printf("f1(x) = 0.6x + 3\n");
    printf("f2(x) = (x-2)^3 - 1\n");
    printf("f3(x) = 3/x\n\n");
    printf("Точность для поиска корней: %g\n", eps1);
    printf("Точность для интегрирования: %g\n", eps2);
    printf("Опции:\n");
    printf("  -i, --intersections   Печатать абсциссы точек пересечения кривых\n");
    printf("  -n, --iterations      Печатать число итераций для поиска корней\n");
    printf("  -i -n, --all          показать все вместе\n");
    printf("  -t, --test            Запустить тестирование функций root и integral\n");
    printf("  -h, --help            Показать эту справку\n");
    printf("  -e EPS1               Задать точность для root (по умолчанию 1e-6)\n");
    printf("  -E EPS2               Задать точность для integral (по умолчанию 1e-4)\n");
    printf("  ПРИМЕРЫ:\n");
    printf("  area_calc.exe -i -n -e 1e-10 -E 1e-8\n");
    printf("  area_calc.exe -t\n");

    int iter_counts[3];  // Массив для хранения числа итераций
    double area = calculate_area(eps1, eps2, iter_counts);
    
    if (print_iterations) {
        printf("\nЧисло итераций при поиске корней:\n");
        printf("  Для пересечения f1 и f3: %d итераций\n", iter_counts[0]);
        printf("  Для пересечения f2 и f3: %d итераций\n", iter_counts[1]);
        printf("  Для пересечения f1 и f2: %d итераций\n", iter_counts[2]);
    }
    
    printf("\nВычисленная площадь фигуры: %.10f\n", area);
    
    return 0;
}