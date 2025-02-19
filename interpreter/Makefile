CC=gcc
CFLAGS=-Wall -g

.PHONY: all
all: clean build test_object test_array test_table test_string test_ez_log

.PHONY: build
build:
	mkdir -p build

.PHONY: test_object
test_object: build
	$(CC) $(CFLAGS) boolean.c nothing.c number.c string.c object.c test_object.c -o build/test_object

.PHONY: test_array
test_array: build
	$(CC) $(CFLAGS) test_array.c array.c -o build/test_array

.PHONY: test_table
test_table: build
	$(CC) $(CFLAGS) test_table.c table.c -o build/test_table

.PHONY: test_string
test_string: build
	$(CC) $(CFLAGS) test_string.c -o build/test_string

.PHONY: test_ez_log
test_ez_log: build
	$(CC) $(CFLAGS) test_ez_log.c -o build/test_ez_log

.PHONY: clean
clean:
	rm -rf build

.PHONY: test
test: all
	./build/test_object
	./build/test_array
	./build/test_table
	./build/test_string
	./build/test_ez_log

