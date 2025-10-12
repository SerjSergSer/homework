CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = temperature_app
SOURCES = main.c temp_functions.c

$(TARGET).exe: $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)
	@echo.
	@echo "=== Build successful! ==="
	@echo "To show help, run: .\$(TARGET).exe -h"
	@echo "To process data, run: .\$(TARGET).exe -f filename.csv"
	@echo.

clean:
	del $(TARGET).exe 
	@echo "Cleaned up!"

.PHONY: clean