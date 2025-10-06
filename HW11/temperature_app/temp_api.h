#ifndef TEMP_API_H
#define TEMP_API_H

// Структура для хранения данных о температуре
typedef struct 
{
    int year;       
    char month[3];   
    char day[3];    
    char hour[3];   
    char minute[3];  
    int temperature; 
} TemperatureData;


// Функции для работы с месяцами
float get_monthly_average(TemperatureData *data, int size, char *month, int year);
int get_monthly_min(TemperatureData *data, int size, char *month, int year);
int get_monthly_max(TemperatureData *data, int size, char *month, int year);

// Функции для работы с годом
float get_yearly_average(TemperatureData *data, int size, int year);
int get_yearly_min(TemperatureData *data, int size, int year);
int get_yearly_max(TemperatureData *data, int size, int year);

#endif