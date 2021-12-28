SRC = $(wildcard src/*.c)
OBJ = $(subst .c,.o,$(subst src,work,$(SRC)))
LIBS = $(shell pkg-config --libs libpulse-simple) -lm
CC := gcc
CFLAGS := -O2 -Wall -Wpedantic -Werror
CFLAGS += $(shell pkg-config --cflags libpulse-simple) -Isrc/include
INSTALLDIR := /usr/bin
OUT = decompose

build/$(OUT): $(OBJ)
	$(CC) $(OBJ) -o build/$(OUT) $(LIBS)

work/%.o: src/%.c
	$(CC) $(CFLAGS) $< -c -o $@

install: build/$(OUT)
	cp build/$(OUT) $(INSTALLDIR)/$(OUT)

uninstall: $(INSTALLDIR)/$(OUT)
	rm $(INSTALLDIR)/$(OUT)
