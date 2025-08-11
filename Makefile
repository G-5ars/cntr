# completely unnecessary makefile go brrrr
SRC=main.c
TARGET=cntr
CC=cc
CFLAGS=

ifeq ($(PREFIX),)
    PREFIX := /usr
endif

.PHONY: install

$(TARGET): $(SRC)
	cc $(SRC) $(CFLAGS) -o $(TARGET)

install: $(TARGET)
	install $(TARGET) $(PREFIX)/bin/
