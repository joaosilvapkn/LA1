CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O2
LDFLAGS = -lncurses


SOURCE = jogo.c


TARGET = game

all: $(TARGET)

$(TARGET): $(SOURCE)
	@$(CC) $(CFLAGS) $(SOURCE) $(LDFLAGS) -o $(TARGET) 2>/dev/null

run: clean $(TARGET)
	@./$(TARGET)


clean:
	@rm -f $(TARGET) >/dev/null 2>&1
