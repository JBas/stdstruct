CC=gcc
FLAGS=-Wall -Wextra -Werror -Wshadow -std=c89 -pedantic

HASH = hash
LLIST = llist
INCLUDES = -Iinclude/
SRC = src
TEST = tests

$(HASH):
	$(CC) $(CFLAGS) -o $(TEST)/$(HASH).out $(INCLUDES) $(SRC)/$(HASH).c $(TEST)/$(HASH)_test.c

$(LLIST):
	$(CC) $(CFLAGS) -o $(TEST)/$(LLIST).out $(INCLUDES) $(SRC)/$(LLIST).c $(TEST)/$(LLIST)_test.c
