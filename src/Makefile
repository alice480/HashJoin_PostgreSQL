CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c11
LIB_FLAG = -lm
SOURCE = DynamicArray/*.c HashTable/*.c Table/*.c *.c


all: clean build launch

clean:
	rm -rf hash_join.out

build:
	$(CC) $(FLAGS) $(SOURCE) $(LIB_FLAG) -o hash_join.out

launch:
	./hash_join.out

clang_format:
	clang-format -style=google -i $(SOURCE)