CC=gcc
FLAGS=-Wall -Wextra -Werror -Wshadow -std=c89 -pedantic

HASH = hash
LLIST = llist
INCLUDES = -Iinclude/
SRC = src
TEST = tests

$(HASH):
	$(CC) $(CFLAGS) -o $(HASH) $(INCLUDES) $(SRC)/$(HASH).c $(TEST)/$(HASH)_test.c

$(LLIST):
	$(CC) $(CFLAGS) -o $(LLIST) $(INCLUDES) $(SRC)/$(LLIST).c $(TEST)/$(LLIST)_test.c
