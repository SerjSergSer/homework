#include <stdio.h>
#include <stdlib.h>
#include "temp_functions.h"

int main(int argc, char *argv[]) 
{
    printf("Temperature Monitor Application\n");
    
    TemperatureList data_list;
    init_list(&data_list);
    
    if (argc > 1) 
    {
        process_command_line(argc, argv, &data_list);
    } else 
    {
        printf("No command line arguments provided.\n");
        printf("Trying to load default data file...\n");
        
        if (load_from_csv(&data_list, "temperature_big.csv")) 
        {
            printf("Successfully loaded data from temperature_big.csv\n");
            
            // Автоматически определяем самый частый год
            int target_year = get_most_common_year(&data_list);
            
            // Получаем все доступные годы для информации
            int available_years[10];
            int year_count = get_available_years(&data_list, available_years, 10);
            
            printf("Data contains records for years: ");
            for (int i = 0; i < year_count; i++) 
            {
                printf("%d", available_years[i]);
                if (i < year_count - 1) printf(", ");
            }
            printf("\n");
            printf("Using year %d (most records)\n", target_year);
            
            // Показываем статистику
            printf("\n=== YEARLY STATISTICS FOR %d ===\n", target_year);
            
            int records_for_year = 0;
            TemperatureData *current = data_list.head;
            while (current != NULL) 
            {
                if (current->year == target_year) records_for_year++;
                current = current->next;
            }
            
            printf("Records analyzed: %d\n", records_for_year);
            
            if (records_for_year > 0) 
            {
                printf("Average temperature: %.2f C\n", get_yearly_average(&data_list, target_year));
                printf("Min temperature: %d C\n", get_yearly_min(&data_list, target_year));
                printf("Max temperature: %d C\n", get_yearly_max(&data_list, target_year));
                
                printf("\n=== MONTHLY BREAKDOWN ===\n");
                for (int month = 1; month <= 12; month++) 
                {
                    int month_records = 0;
                    current = data_list.head;
                    while (current != NULL) 
                    {
                        if (current->year == target_year && current->month == month) 
                        {
                            month_records++;
                        }
                        current = current->next;
                    }
                    
                    if (month_records > 0) 
                    {
                        printf("Month %02d: Records=%3d, Avg=%6.2f C, Min=%3d C, Max=%3d C\n", 
                               month, month_records,
                               get_monthly_average(&data_list, month, target_year),
                               get_monthly_min(&data_list, month, target_year),
                               get_monthly_max(&data_list, month, target_year));
                    }
                }
            } else 
            {
                printf("No data found for year %d\n", target_year);
            }
            
        } else 
        {
            printf("Default data file 'temperature_big.csv' not found or contains errors.\n");
            printf("Please provide a CSV file using -f option.\n\n");
            print_help();
        }
    }
    
    free_list(&data_list);
    return 0;
}