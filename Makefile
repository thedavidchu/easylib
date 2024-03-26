CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c99 -g

SRCS=$(filter-out src/deprecated/%.c, $(shell find src -name "*.c"))
HDRS=$(shell find src -name "*.h")

.PHONY: all clean

main: $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(SRCS) -I src -o $@

# Remove all objects (*.o) and executables within the top-level directory.
clean:
	rm main
	find . -type f -executable -delete
