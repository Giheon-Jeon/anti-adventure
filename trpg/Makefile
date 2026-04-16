CC = gcc
CFLAGS = -Iinclude -Wall
SRC = src/main.c src/character.c src/combat.c src/event.c src/shop.c src/utils.c src/village.c src/crafting.c src/ability.c src/skill.c src/encyclopedia.c src/saveload.c
OBJ = $(SRC:.c=.o)
TARGET = trpg_game

PYTHON = $(shell where py >nul 2>nul && echo py || echo python3)

all: generate $(TARGET)

generate:
	$(PYTHON) scripts/generate_data.py

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c include/generated_data.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /f /s src\*.o $(TARGET).exe
