CC=gcc
CFLAGS=-Wall -Wextra -Werror -Wpedantic -g

SRC=$(shell find src -name "*.c")

lecurl:
	clear && $(CC) $(CFLAGS) $(SRC) -o lecurl

clean:
	rm -f lecurl