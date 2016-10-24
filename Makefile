CC = gcc
LINK = ${CC}
CFLAGS = -g
VERSION = 0.1.0

.PHONY: all

all: liblogc.so testlogc

liblogc.so: logc.o
	$(CC) -shared -o $@ $^

logc.o: logc.c
	$(CC) -fPIC -c $<

testlogc: test-logc.o liblogc.so
	$(LINK) -o $@ test-logc.o -L. -llogc -lpthread

test-logc.o: test-logc.c
	$(CC) -c $<

tar:
	tar -czf logc-$(VERSION).tar.gz logc.h logc.c test-logc.c Makefile

.PHONY: clean

clean:
	rm *.o testlogc liblogc.so logc*.tar.*

