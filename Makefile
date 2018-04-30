CC=cc -std=c99
CFLAGS=-Wall -W -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wswitch-default -Wswitch-enum -Wcast-align -Wpointer-arith -Wbad-function-cast -Wstrict-overflow=5 -Wstrict-prototypes -Winline -Wundef -Wnested-externs -Wcast-qual -Wshadow -Wunreachable-code -Wlogical-op -Wstrict-aliasing=2 -Wredundant-decls -Wold-style-definition -Wno-unused-function -ftrapv
CPPFLAGS=-Imlib
LDFLAGS=
RM=rm -f
AR=ar cru
RANLIB=ranlib

all: mjsonlib.a

mjsonlib.a: mjsonlib.o
	$(AR) mjsonlib.a mjsonlib.o
	$(RANLIB) mjsonlib.a

mjsonlib.o: mjsonlib.c mjsonlib.h mlib

mlib:
	git clone https://github.com/P-p-H-d/mlib.git

clean:
	$(RM) *~ *.o *.a test.exe

check: test

test: test.c mjsonlib.a
	$(CC) test.c mjsonlib.a -o test.exe
	./test.exe

