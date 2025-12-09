# completely unnecessary makefile go brrrr
SRC=main.c
TARGET=cntr
CFLAGS=

ifeq ($(PREFIX),)
    PREFIX := /usr
endif

.PHONY: install

$(TARGET): $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET)

install: $(TARGET)
	install $(TARGET) $(PREFIX)/bin/
