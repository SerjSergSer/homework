#ifndef TEMP_API_H
#define TEMP_API_H

#define MAX_RECORDS 1000
#define MAX_LINE_LENGTH 100

typedef struct 
{
    int year;        
    int month;       
    int day;         
    int hour;       
    int minute;      
    int temperature; 
} TemperatureData;

// Основные функции работы с данными
int add_record(TemperatureData *data, int *size, TemperatureData new_record);
int delete_record(TemperatureData *data, int *size, int index);
void sort_by_date(TemperatureData *data, int size);
void sort_by_temperature(TemperatureData *data, int size);
void print_all_records(TemperatureData *data, int size);
int load_from_csv(TemperatureData *data, int *size, const char *filename);

// Функции статистики
float get_monthly_average(TemperatureData *data, int size, int month, int year);
int get_monthly_min(TemperatureData *data, int size, int month, int year);
int get_monthly_max(TemperatureData *data, int size, int month, int year);
float get_yearly_average(TemperatureData *data, int size, int year);
int get_yearly_min(TemperatureData *data, int size, int year);
int get_yearly_max(TemperatureData *data, int size, int year);

// Функции работы с данными
int add_record(TemperatureData *data, int *size, TemperatureData new_record);
int delete_record(TemperatureData *data, int *size, int index);
void sort_by_date(TemperatureData *data, int size);
void sort_by_temperature(TemperatureData *data, int size);
void print_all_records(TemperatureData *data, int size);
int load_from_csv(TemperatureData *data, int *size, const char *filename);

void print_help();
void process_command_line(int argc, char *argv[], TemperatureData *data, int *data_size);

#endif