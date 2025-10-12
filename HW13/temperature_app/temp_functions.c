#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "temp_functions.h"

// Функции работы с динамическим списком

void init_list(TemperatureList *list) 
{
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void free_list(TemperatureList *list) 
{
    TemperatureData *current = list->head;
    TemperatureData *next;
    
    while (current != NULL) 
    {
        next = current->next;
        free(current);
        current = next;
    }
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

int add_record(TemperatureList *list, TemperatureData new_record) 
{
    // Выделяем память для нового элемента
    TemperatureData *new_node = (TemperatureData*)malloc(sizeof(TemperatureData));
    if (!new_node) 
    {
        printf("Error: Memory allocation failed!\n");
        return 0;
    }
    
    // Копируем данные
    *new_node = new_record;
    new_node->next = NULL;
    
    // Добавляем в конец списка
    if (list->head == NULL) 
    {
        // Первый элемент
        list->head = new_node;
        list->tail = new_node;
    } else 
    {
        // Добавляем в конец
        list->tail->next = new_node;
        list->tail = new_node;
    }
    
    list->size++;
    return 1;
}

int delete_record(TemperatureList *list, int index) 
{
    if (index < 0 || index >= list->size) 
    {
        printf("Error: Invalid index!\n");
        return 0;
    }
    
    TemperatureData *current = list->head;
    TemperatureData *prev = NULL;
    
    // Находим нужный элемент
    for (int i = 0; i < index; i++) 
    {
        prev = current;
        current = current->next;
    }
    
    // Удаляем элемент
    if (prev == NULL) 
    {
        // Удаляем первый элемент
        list->head = current->next;
    } else 
    {
        prev->next = current->next;
    }
    
    // Если удаляем последний элемент
    if (current == list->tail) 
    {
        list->tail = prev;
    }
    
    free(current);
    list->size--;
    return 1;
}

// Функции проверки данных
int is_valid_date(int year, int month, int day) 
{
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Проверка високосного года
    int is_leap = 0;
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) 
    {
        is_leap = 1;
    }
    
    if (month == 2) 
    {
        if (is_leap) 
        {
            if (day > 29) return 0;
        } else 
        {
            if (day > 28) return 0;
        }
    } else 
    {
        if (day < 1 || day > days_in_month[month - 1]) return 0;
    }
    
    return 1;
}

int is_valid_time(int hour, int minute) 
{
    return (hour >= 0 && hour <= 23) && (minute >= 0 && minute <= 59);
}

int is_valid_temperature(int temp) 
{
    return temp >= -99 && temp <= 99;
}

// Статистические функции

float get_monthly_average(TemperatureList *list, int month, int year) 
{
    if (list->size == 0) return 0.0f;
    
    int sum = 0, count = 0;
    TemperatureData *current = list->head;
    
    while (current != NULL) {
        if (current->month == month && current->year == year) 
        {
            sum += current->temperature;
            count++;
        }
        current = current->next;
    }
    
    return count > 0 ? (float)sum / count : -999.0f; // Изменили на -999
}

int get_monthly_min(TemperatureList *list, int month, int year) 
{
    if (list->size == 0) return -999; // Изменили на -999
    
    int min_temp = 100;
    TemperatureData *current = list->head;
    int found = 0;
    
    while (current != NULL) 
    {
        if (current->month == month && current->year == year) 
        {
            if (current->temperature < min_temp) 
            {
                min_temp = current->temperature;
            }
            found = 1;
        }
        current = current->next;
    }
    
    return found ? min_temp : -999; // Изменили на -999
}

int get_monthly_max(TemperatureList *list, int month, int year) 
{
    if (list->size == 0) return -999; // Изменили на -999
    
    int max_temp = -100;
    TemperatureData *current = list->head;
    int found = 0;
    
    while (current != NULL) 
    {
        if (current->month == month && current->year == year) 
        {
            if (current->temperature > max_temp) 
            {
                max_temp = current->temperature;
            }
            found = 1;
        }
        current = current->next;
    }
    
    return found ? max_temp : -999; // Изменили на -999
}

float get_yearly_average(TemperatureList *list, int year) 
{
    if (list->size == 0) return 0.0f;
    
    int sum = 0, count = 0;
    TemperatureData *current = list->head;
    
    while (current != NULL) 
    {
        if (current->year == year) 
        {
            sum += current->temperature;
            count++;
        }
        current = current->next;
    }
    
    return count > 0 ? (float)sum / count : 0.0f;
}

int get_yearly_min(TemperatureList *list, int year) 
{
    if (list->size == 0) return 0;
    
    int min_temp = 100;
    TemperatureData *current = list->head;
    
    while (current != NULL) 
    {
        if (current->year == year && current->temperature < min_temp) 
        {
            min_temp = current->temperature;
        }
        current = current->next;
    }
    
    return min_temp == 100 ? 0 : min_temp;
}

int get_yearly_max(TemperatureList *list, int year) 
{
    if (list->size == 0) return 0;
    
    int max_temp = -100;
    TemperatureData *current = list->head;
    
    while (current != NULL) 
    {
        if (current->year == year && current->temperature > max_temp) 
        {
            max_temp = current->temperature;
        }
        current = current->next;
    }
    
    return max_temp == -100 ? 0 : max_temp;
}

// Вспомогательные функции

void print_all_records(TemperatureList *list) 
{
    printf("\n ALL TEMPERATURE RECORDS (%d) \n", list->size);
    printf("No.  Date       Time  Temp\n");
    printf("                          \n");
    
    TemperatureData *current = list->head;
    int index = 1;
    
    while (current != NULL) 
    {
        printf("%-4d %04d-%02d-%02d %02d:%02d %3d C\n", 
               index,
               current->year, current->month, current->day,
               current->hour, current->minute,
               current->temperature);
        current = current->next;
        index++;
    }
}

int load_from_csv(TemperatureList *list, const char *filename) 
{
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    int records_loaded = 0;
    int line_number = 0;
    int error_count = 0;
    
    while (fgets(line, sizeof(line), file)) 
    {
        line_number++;
        
        // Пропускаем заголовок если есть
        if (line_number == 1 && (strstr(line, "YEAR") || strstr(line, "year"))) 
        {
            continue;
        }
        
        // Убираем символы новой строки
        line[strcspn(line, "\r\n")] = 0;
        
        // Пропускаем пустые строки
        if (strlen(line) == 0) 
        {
            continue;
        }
        
        TemperatureData record;
        char original_line[MAX_LINE_LENGTH];
        strcpy(original_line, line); // Сохраняем оригинальную строку для вывода ошибок
        
        // Парсим строку
        if (sscanf(line, "%d;%d;%d;%d;%d;%d",
                   &record.year, &record.month, &record.day,
                   &record.hour, &record.minute, &record.temperature) == 6) 
        {
            // Проверяем корректность данных
            if (!is_valid_date(record.year, record.month, record.day)) 
            {
                printf("Error in line %d: Invalid date %04d-%02d-%02d\n", line_number, record.year, record.month, record.day);
                printf("  Line: %s\n", original_line);
                error_count++;
                continue;
            }
            
            if (!is_valid_time(record.hour, record.minute)) 
            {
                printf("Error in line %d: Invalid time %02d:%02d\n", line_number, record.hour, record.minute);
                printf("  Line: %s\n", original_line);
                error_count++;
                continue;
            }
            
            if (!is_valid_temperature(record.temperature)) 
            {
                printf("Error in line %d: Temperature out of range (-99 to 99): %d\n", line_number, record.temperature);
                printf("  Line: %s\n", original_line);
                error_count++;
                continue;
            }
            
            if (add_record(list, record)) 
            {
                records_loaded++;
            }
        }
        else 
        {
            printf("Error in line %d: Invalid format or missing fields\n", line_number);
            printf("  Line: %s\n", original_line);
            printf("  Expected format: YEAR;MONTH;DAY;HOUR;MINUTE;TEMPERATURE\n");
            error_count++;
        }
    }
    
    fclose(file);
    
    printf("\nFile processing summary for %s:\n", filename);
    printf("  Total lines processed: %d\n", line_number);
    printf("  Valid records loaded: %d\n", records_loaded);
    printf("  Errors found: %d\n", error_count);
    
    return records_loaded;
}

// Функции сортировки 
void sort_by_date(TemperatureList *list) 
{
    // Сортировка пузырьком
    if (list->size < 2) return;
    
    int swapped;
    do {
        swapped = 0;
        TemperatureData *current = list->head;
        TemperatureData *prev = NULL;
        
        while (current->next != NULL) 
        {
            TemperatureData *next = current->next;
            
            // Сравниваем даты
            if (current->year > next->year ||
                (current->year == next->year && current->month > next->month) ||
                (current->year == next->year && current->month == next->month && current->day > next->day) ||
                (current->year == next->year && current->month == next->month && current->day == next->day && current->hour > next->hour) ||
                (current->year == next->year && current->month == next->month && current->day == next->day && current->hour == next->hour && current->minute > next->minute)) 
            {
                
                // Меняем местами
                if (prev != NULL) 
                {
                    prev->next = next;
                } else 
                {
                    list->head = next;
                }
                current->next = next->next;
                next->next = current;
                
                if (current->next == NULL) 
                {
                    list->tail = current;
                }
                
                swapped = 1;
                prev = next;
            } else 
            {
                prev = current;
                current = current->next;
            }
        }
    } while (swapped);
}

void sort_by_temperature(TemperatureList *list) 
{
    if (list->size < 2) return;
    
    int swapped;
    do {
        swapped = 0;
        TemperatureData *current = list->head;
        TemperatureData *prev = NULL;
        
        while (current->next != NULL) 
        {
            TemperatureData *next = current->next;
            
            if (current->temperature > next->temperature) 
            {
                // Меняем местами
                if (prev != NULL) 
                {
                    prev->next = next;
                } else 
                {
                    list->head = next;
                }
                current->next = next->next;
                next->next = current;
                
                if (current->next == NULL) 
                {
                    list->tail = current;
                }
                
                swapped = 1;
                prev = next;
            } else 
            {
                prev = current;
                current = current->next;
            }
        }
    } while (swapped);
}

// Функция для получения списка доступных годов
int get_available_years(TemperatureList *list, int *years, int max_years) 
{
    if (list->size == 0 || max_years <= 0) return 0;
    
    int year_count = 0;
    TemperatureData *current = list->head;
    
    while (current != NULL && year_count < max_years) 
    {
        int found = 0;
        for (int i = 0; i < year_count; i++) 
        {
            if (years[i] == current->year) 
            {
                found = 1;
                break;
            }
        }
        if (!found) 
        {
            years[year_count++] = current->year;
        }
        current = current->next;
    }
    
    return year_count;
}

// Функция для получения самого частого года
int get_most_common_year(TemperatureList *list) 
{
    if (list->size == 0) return 2024;
    
    int years[100] = {0};
    int counts[100] = {0};
    int year_count = 0;
    
    TemperatureData *current = list->head;
    
    while (current != NULL) 
    {
        int found = 0;
        for (int i = 0; i < year_count; i++) 
        {
            if (years[i] == current->year) 
            {
                counts[i]++;
                found = 1;
                break;
            }
        }
        if (!found && year_count < 100) 
        {
            years[year_count] = current->year;
            counts[year_count] = 1;
            year_count++;
        }
        current = current->next;
    }
    
    int max_count = 0;
    int most_common_year = years[0];
    
    for (int i = 0; i < year_count; i++) 
    {
        if (counts[i] > max_count) 
        {
            max_count = counts[i];
            most_common_year = years[i];
        }
    }
    
    return most_common_year;
}

void print_help() 
{
    printf("\n TEMPERATURE MONITOR HELP \n");
    printf("Usage: temperature_app.exe [OPTIONS]\n\n");
    printf("Options:\n");
    printf("  -h              Show this help message\n");
    printf("  -f <filename>   Load data from CSV file (REQUIRED)\n");
    printf("  -m <month>      Show statistics for specific month (1-12)\n");
    printf("  -y <year>       Show statistics for specific year (default: 2024)\n");
    printf("\nExamples:\n");
     printf("Note for PowerShell users: Use 'dot' and 'backslash' before commands\n");
    printf("  temperature_app.exe -f temperature_big.csv           # Show yearly statistics\n");
    printf("  temperature_app.exe -f temperature_big.csv -m 7      # Show July statistics\n");
    printf("  temperature_app.exe -f temperature_big.csv -y 2021   # Show 2021 statistics\n");
    printf("\nFile format: year;month;day;hour;minute;temperature\n");
}

void process_command_line(int argc, char *argv[], TemperatureList *list) 
{
    char *filename = NULL;
    int month = 0;
    int year = 2021;
    
    for (int i = 1; i < argc; i++) 
    {
        if (strcmp(argv[i], "-h") == 0) 
        {
            print_help();
            return;
        }
        else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) 
        {
            filename = argv[++i];
        }
        else if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) 
        {
            month = atoi(argv[++i]);
            if (month < 1 || month > 12) 
            {
                printf("Error: Invalid month %d. Month must be between 1 and 12.\n", month);
                return;
            }
        }
        else if (strcmp(argv[i], "-y") == 0 && i + 1 < argc) 
        {
            year = atoi(argv[++i]);
        }
    }
    
    if (!filename) 
    {
        printf("Error: No input file specified. Use -f <filename>\n");
        print_help();
        return;
    }
    
    if (!load_from_csv(list, filename)) 
    {
        printf("Error: Failed to load data from %s\n", filename);
        return;
    }
    
    // Если указан месяц - показываем статистику только за этот месяц
    if (month > 0 && month <= 12) 
    {
        // Считаем записи только для этого месяца и года
        int records_count = 0;
        TemperatureData *current = list->head;
        while (current != NULL) {
            if (current->month == month && current->year == year) 
            {
                records_count++;
            }
            current = current->next;
        }
        
        printf("\n STATISTICS FOR %d-%02d \n", year, month);
        printf("Records analyzed: %d\n", records_count);
        
        if (records_count > 0) 
        {
            printf("Average temperature: %.2f C\n", get_monthly_average(list, month, year));
            printf("Min temperature: %d C\n", get_monthly_min(list, month, year));
            printf("Max temperature: %d C\n", get_monthly_max(list, month, year));
        } else 
        {
            printf("❌ No data available for %d-%02d\n", year, month);
            printf("💡 Available data periods:\n");
            
            // Показываем какие периоды есть в данных
            int available_years[10];
            int year_count = get_available_years(list, available_years, 10);
            
            for (int y = 0; y < year_count; y++) 
            {
                printf("  Year %d: ", available_years[y]);
                int months_found[12] = {0};
                int month_count = 0;
                
                current = list->head;
                while (current != NULL) 
                {
                    if (current->year == available_years[y]) 
                    {
                        if (!months_found[current->month - 1]) 
                        {
                            months_found[current->month - 1] = 1;
                            month_count++;
                        }
                    }
                    current = current->next;
                }
                
                if (month_count > 0) 
                {
                    for (int m = 0; m < 12; m++) 
                    {
                        if (months_found[m]) 
                        {
                            printf("%02d ", m + 1);
                        }
                    }
                } else {
                    printf("No monthly data");
                }
                printf("\n");
            }
            
            printf("\n");
            print_help();
        }
    } 
    // Если месяц не указан - показываем полную статистику
    else 
    {
        // Считаем записи только для этого года
        int records_count = 0;
        TemperatureData *current = list->head;
        while (current != NULL) {
            if (current->year == year) 
            {
                records_count++;
            }
            current = current->next;
        }
        
        printf("\n YEARLY STATISTICS FOR %d \n", year);
        printf("Records analyzed: %d\n", records_count);
        
        if (records_count > 0) 
        {
            printf("Average temperature: %.2f C\n", get_yearly_average(list, year));
            printf("Min temperature: %d C\n", get_yearly_min(list, year));
            printf("Max temperature: %d C\n", get_yearly_max(list, year));
            
            // Статистика по всем месяцам
            printf("\n MONTHLY STATISTICS \n");
            for (int m = 1; m <= 12; m++)
            {
                int month_records = 0;
                current = list->head;
                while (current != NULL) 
                {
                    if (current->month == m && current->year == year) 
                    {
                        month_records++;
                    }
                    current = current->next;
                }
                
                if (month_records > 0) 
                {
                    printf("Month %02d: Avg=%.2f C, Min=%d C, Max=%d C\n", 
                           m, 
                           get_monthly_average(list, m, year),
                           get_monthly_min(list, m, year),
                           get_monthly_max(list, m, year));
                }
            }
        } else 
        {
            printf("No data available for year %d\n", year);
            printf("Available years in data: ");
            
            int available_years[10];
            int year_count = get_available_years(list, available_years, 10);
            
            for (int i = 0; i < year_count; i++) 
            {
                printf("%d", available_years[i]);
                if (i < year_count - 1) printf(", ");
            }
            printf("\n\n");
            print_help();
        }
    }
}