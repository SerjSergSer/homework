#ifndef TEMP_FUNCTIONS_H
#define TEMP_FUNCTIONS_H

#define MAX_RECORDS 1000
#define MAX_LINE_LENGTH 100

typedef struct TemperatureData
{
    int year;        
    int month;       
    int day;         
    int hour;       
    int minute;      
    int temperature; 
    struct TemperatureData *next;
} TemperatureData;

typedef struct 
{
    TemperatureData *head;    // первый элемент
    TemperatureData *tail;    // последний элемент  
    int size;                // количество элементов
} TemperatureList;      

// Инициализация и очистка
void init_list(TemperatureList *list);
void free_list(TemperatureList *list);

// Функции проверки данных
int is_valid_date(int year, int month, int day);
int is_valid_time(int hour, int minute);
int is_valid_temperature(int temp);

// Функции статистики
float get_monthly_average(TemperatureList *list, int month, int year);
int get_monthly_min(TemperatureList *list, int month, int year);
int get_monthly_max(TemperatureList *list, int month, int year);
float get_yearly_average(TemperatureList *list, int year);
int get_yearly_min(TemperatureList *list, int year);
int get_yearly_max(TemperatureList *list, int year);

// Функции работы с данными
int add_record(TemperatureList *list, TemperatureData new_record);
int delete_record(TemperatureList *list, int index);
void sort_by_date(TemperatureList *list);
void sort_by_temperature(TemperatureList *list);
void print_all_records(TemperatureList *list);
int load_from_csv(TemperatureList *list, const char *filename);

// Функции командной строки
void print_help();
void process_command_line(int argc, char *argv[], TemperatureList *list);

int get_available_years(TemperatureList *list, int *years, int max_years);
int get_most_common_year(TemperatureList *list);

#endif