#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "temp_api.h"

// Статистические функции

float get_monthly_average(TemperatureData *data, int size, int month, int year) 
{
    if (data == NULL || size <= 0) return 0.0f;
    
    int sum = 0, count = 0;
    for (int i = 0; i < size; i++) 
    {
        if (data[i].month == month && data[i].year == year) 
        {
            sum += data[i].temperature;
            count++;
        }
    }
    return count > 0 ? (float)sum / count : 0.0f;
}

int get_monthly_min(TemperatureData *data, int size, int month, int year) 
{
    if (data == NULL || size <= 0) return 0;
    
    int min_temp = 100; // Начинаем с максимально возможной
    for (int i = 0; i < size; i++) 
    {
        if (data[i].month == month && data[i].year == year) 
        {
            if (data[i].temperature < min_temp) 
            {
                min_temp = data[i].temperature;
            }
        }
    }
    return min_temp == 100 ? 0 : min_temp;
}

int get_monthly_max(TemperatureData *data, int size, int month, int year) 
{
    if (data == NULL || size <= 0) return 0;
    
    int max_temp = -100; // Начинаем с минимально возможной
    for (int i = 0; i < size; i++) 
    {
        if (data[i].month == month && data[i].year == year) 
        {
            if (data[i].temperature > max_temp) 
            {
                max_temp = data[i].temperature;
            }
        }
    }
    return max_temp == -100 ? 0 : max_temp;
}

float get_yearly_average(TemperatureData *data, int size, int year) 
{
    if (data == NULL || size <= 0) return 0.0f;
    
    int sum = 0, count = 0;
    for (int i = 0; i < size; i++) 
    {
        if (data[i].year == year) 
        {
            sum += data[i].temperature;
            count++;
        }
    }
    return count > 0 ? (float)sum / count : 0.0f;
}

int get_yearly_min(TemperatureData *data, int size, int year) 
{
    if (data == NULL || size <= 0) return 0;
    
    int min_temp = 100;
    for (int i = 0; i < size; i++) 
    {
        if (data[i].year == year && data[i].temperature < min_temp) 
        {
            min_temp = data[i].temperature;
        }
    }
    return min_temp == 100 ? 0 : min_temp;
}

int get_yearly_max(TemperatureData *data, int size, int year) 
{
    if (data == NULL || size <= 0) return 0;
    
    int max_temp = -100;
    for (int i = 0; i < size; i++) 
    {
        if (data[i].year == year && data[i].temperature > max_temp) 
        {
            max_temp = data[i].temperature;
        }
    }
    return max_temp == -100 ? 0 : max_temp;
}

// Функции работы с данными

int add_record(TemperatureData *data, int *size, TemperatureData new_record) 
{
    if (*size >= MAX_RECORDS) 
    {
        printf("Error: Maximum records reached!\n");
        return 0;
    }
    
    data[*size] = new_record;
    (*size)++;
    return 1;
}

int delete_record(TemperatureData *data, int *size, int index) 
{
    if (index < 0 || index >= *size) 
    {
        printf("Error: Invalid index!\n");
        return 0;
    }
    
    for (int i = index; i < *size - 1; i++) 
    {
        data[i] = data[i + 1];
    }
    (*size)--;
    return 1;
}

void sort_by_date(TemperatureData *data, int size) 
{
    for (int i = 0; i < size - 1; i++)
     {
        for (int j = 0; j < size - i - 1; j++) 
        {
            // Сравниваем даты
            if (data[j].year > data[j + 1].year ||
                (data[j].year == data[j + 1].year && data[j].month > data[j + 1].month) ||
                (data[j].year == data[j + 1].year && data[j].month == data[j + 1].month && data[j].day > data[j + 1].day) ||
                (data[j].year == data[j + 1].year && data[j].month == data[j + 1].month && data[j].day == data[j + 1].day && data[j].hour > data[j + 1].hour) ||
                (data[j].year == data[j + 1].year && data[j].month == data[j + 1].month && data[j].day == data[j + 1].day && data[j].hour == data[j + 1].hour && data[j].minute > data[j + 1].minute)) 
            {
                // Меняем местами
                TemperatureData temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}

void sort_by_temperature(TemperatureData *data, int size) 
{
    for (int i = 0; i < size - 1; i++) 
    {
        for (int j = 0; j < size - i - 1; j++) 
        {
            if (data[j].temperature > data[j + 1].temperature) 
            {
                TemperatureData temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}

void print_all_records(TemperatureData *data, int size) 
{
    printf("\n ALL TEMPERATURE RECORDS \n");
    printf("No.  Date       Time  Temp\n");
    printf("----------------------------\n");
    
    for (int i = 0; i < size; i++) 
    {
        printf("%-4d %04d-%02d-%02d %02d:%02d %3d C\n", 
               i + 1,
               data[i].year, data[i].month, data[i].day,
               data[i].hour, data[i].minute,
               data[i].temperature);
    }
    printf("Total records: %d\n", size);
}

int load_from_csv(TemperatureData *data, int *size, const char *filename) 
{
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    int records_loaded = 0;
    
    // Пропускаем заголовок если есть
    fgets(line, sizeof(line), file);
    
    while (fgets(line, sizeof(line), file)) 
    {
        TemperatureData record;
        if (sscanf(line, "%d;%d;%d;%d;%d;%d",
                   &record.year, &record.month, &record.day,
                   &record.hour, &record.minute, &record.temperature) == 6) 
        {
            if (add_record(data, size, record)) 
            {
                records_loaded++;
            }
        }
    }
    
    fclose(file);
    printf("Loaded %d records from %s\n", records_loaded, filename);
    return records_loaded;
}

// Функции командной строки

void print_help() 
{
    printf("\n TEMPERATURE MONITOR HELP \n");
    printf("Usage: temperature_app.exe [OPTIONS]\n\n");
    printf("Options:\n");
    printf("  -h              Show this help message\n");
    printf("  -f <filename>   Load data from CSV file\n");
    printf("  -m <month>      Show statistics for specific month (1-12)\n");
    printf("  -y <year>       Show statistics for specific year (default: 2024)\n");
    printf("\nExample:\n");
    printf("  temperature_app.exe -f data.csv -m 1\n");
    printf("  temperature_app.exe -f data.csv -y 2024\n");
}

void process_command_line(int argc, char *argv[], TemperatureData *data, int *data_size) 
{
    char *filename = NULL;
    int month = 0;
    int year = 2024;
    
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
        }
        else if (strcmp(argv[i], "-y") == 0 && i + 1 < argc) 
        {
            year = atoi(argv[++i]);
        }
    }
    
    if (filename) 
    {
        if (load_from_csv(data, data_size, filename)) 
        {
            if (month > 0 && month <= 12) 
            {
                printf("\n STATISTICS FOR %d-%02d \n", year, month);
                printf("Average temperature: %.2f C\n", get_monthly_average(data, *data_size, month, year));
                printf("Min temperature: %d C\n", get_monthly_min(data, *data_size, month, year));
                printf("Max temperature: %d C\n", get_monthly_max(data, *data_size, month, year));
            } else 
            {
                printf("\n YEARLY STATISTICS FOR %d \n", year);
                printf("Average temperature: %.2f C\n", get_yearly_average(data, *data_size, year));
                printf("Min temperature: %d C\n", get_yearly_min(data, *data_size, year));
                printf("Max temperature: %d C\n", get_yearly_max(data, *data_size, year));
                
                // Статистика по всем месяцам
                printf("\n MONTHLY STATISTICS \n");
                for (int m = 1; m <= 12; m++) 
                {
                    float avg = get_monthly_average(data, *data_size, m, year);
                    if (avg != 0.0f) 
                    {
                        printf("Month %02d: Avg=%.2f C, Min=%d C, Max=%d C\n", 
                               m, avg, 
                               get_monthly_min(data, *data_size, m, year),
                               get_monthly_max(data, *data_size, m, year));
                    }
                }
            }
        }
    } else {
        print_help();
    }
}