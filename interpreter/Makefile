CC=gcc
CFLAGS=-Wall -g

.PHONY: all
all: clean build object test_array test_table test_string

.PHONY: build
build:
	mkdir -p build

.PHONY: object
object: build
	$(CC) $(CFLAGS) object.c -o build/object

.PHONY: test_array
test_array: build
	$(CC) $(CFLAGS) test_array.c array.c -o build/test_array

.PHONY: test_table
test_table: build
	$(CC) $(CFLAGS) test_table.c table.c -o build/test_table

.PHONY: test_string
test_string: build
	$(CC) $(CFLAGS) test_string.c -o build/test_string

.PHONY: clean
clean:
	rm -rf build

.PHONY: test
test: all
	./build/object
	./build/test_array
	./build/test_table
	./build/test_string

