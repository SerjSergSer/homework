#include <stdio.h>
#include <stdlib.h>
#include "temp_api.h"

int main(int argc, char *argv[]) 
{
    printf("Temperature Monitor Application\n");
    
    // Создаем массив структур
    TemperatureData data[MAX_RECORDS];
    int data_size = 0;
    
    // Обрабатываем аргументы командной строки
    if (argc > 1) 
    {
        process_command_line(argc, argv, data, &data_size);
    } else 
    {
        // Если нет аргументов - пробуем загрузить данные по умолчанию
        printf("No command line arguments provided.\n");
        printf("Trying to load default data file...\n");
        
        // Пробуем загрузить данные из файла по умолчанию
        if (load_from_csv(data, &data_size, "data.csv")) 
        {
            printf("Successfully loaded data from data.csv\n");
            
            // Показываем общую статистику за 2024 год
            printf("\n YEARLY STATISTICS FOR 2024 \n");
            printf("Average temperature: %.2f C\n", get_yearly_average(data, data_size, 2024));
            printf("Min temperature: %d C\n", get_yearly_min(data, data_size, 2024));
            printf("Max temperature: %d C\n", get_yearly_max(data, data_size, 2024));
            
            // Показываем статистику по всем месяцам
            printf("\n MONTHLY STATISTICS FOR 2024 \n");
            for (int month = 1; month <= 12; month++) 
            {
                float avg = get_monthly_average(data, data_size, month, 2024);
                if (avg != 0.0f) 
                {
                    printf("Month %02d: Avg=%.2f C, Min=%d C, Max=%d C\n", 
                           month, avg, 
                           get_monthly_min(data, data_size, month, 2024),
                           get_monthly_max(data, data_size, month, 2024));
                }
            }
        } else 
        {
            // Если файл не найден - показываем справку
            printf("Default data file 'full_2024_data.csv' not found.\n");
            printf("Please provide a CSV file using -f option.\n\n");
            
            print_help();
        }
    }
    
    return 0;
}