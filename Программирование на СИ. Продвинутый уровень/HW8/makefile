# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = -lm

# Исходные файлы
SRCS = main.c
OBJS = $(SRCS:.c=.o)
TARGET = area_calc

# Правило по умолчанию
all: $(TARGET)

# Сборка исполняемого файла
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Компиляция объектных файлов
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Зависимости
main.o: main.c

# Очистка
clean:
	del /Q $(OBJS) $(TARGET).exe 2>nul || rm -f $(OBJS) $(TARGET)

# Псевдоцели
.PHONY: all clean