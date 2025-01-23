CC=gcc
CFLAGS=-Wall -g

.PHONY: all
all: test_array test_table

.PHONY: test_array
test_array:
	$(CC) $(CFLAGS) test_array.c array.c -o test_array

.PHONY: test_table
test_table:
	$(CC) $(CFLAGS) test_table.c table.c -o test_table

.PHONY: clean
clean:
	rm -rf test_array test_table

