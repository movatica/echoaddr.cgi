TARGET = echoaddr.cgi
SRC = echoaddr.c

CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -Werror -s -O2 -fpie -flto

prefix ?= /var/www/cgi-bin


all: $(TARGET)


$(TARGET): $(SRC)


%.cgi: %.c
	$(CC) $(CFLAGS) -o $@ $^


install:
	mkdir -p $(DESTDIR)$(prefix)
	cp $(TARGET) $(DESTDIR)$(prefix)


clean:
	rm $(TARGET)


.PHONY: clean install
