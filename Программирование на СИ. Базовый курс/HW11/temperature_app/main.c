#include <stdio.h>
#include "temp_api.h"

#define MAX_RECORDS 100

int main() {
    printf("Temperature Monitor Application\n");
    
    // Массив структур пока пустой
    TemperatureData data[MAX_RECORDS];
    int data_size = 0;
    
    // Данные
    printf("Array created for %d temperature records\n", MAX_RECORDS);
    
    printf("Monthly average: %.2f\n", get_monthly_average(data, data_size, "01", 2024));
    printf("Yearly average: %.2f\n", get_yearly_average(data, data_size, 2024));
    
    return 0;
}