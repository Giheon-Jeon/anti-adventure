CC = gcc
CFLAGS = -Iinclude -Wall
SRC = src/main.c src/character.c src/combat.c src/event.c src/shop.c src/utils.c src/village.c src/crafting.c
OBJ = $(SRC:.c=.o)
TARGET = trpg_game

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)
