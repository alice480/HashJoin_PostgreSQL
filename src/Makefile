CC = gcc
FLAGS = -Wall -Wextra -std=c11
LIB_FLAG = -lm
SOURCE = DynamicArray/*.c HashTable/*.c Table/*.c *.c
HEADERS = DynamicArray/*.h HashTable/*.h Table/*.h *.h


all: clean build launch

clean:
	rm -rf hash_join.out

build:
	$(CC) $(FLAGS) $(SOURCE) $(LIB_FLAG) -o hash_join.out

launch:
	./hash_join.out

clang_format:
	clang-format -style=google -i $(SOURCE) $(HEADERS)

leaks: build
	valgrind -s ./hash_join.out --leak-check=full