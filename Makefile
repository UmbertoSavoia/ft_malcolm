TARGET = ft_malcolm
CC = gcc
CFLAGS = -g #-Wall -Wextra -Werror
RM = rm -rf

SRC = $(wildcard ./src/*.c)
OBJ = $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(TARGET)

re: fclean all

.PHONY: all clean fclean re

