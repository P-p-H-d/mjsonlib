CC=cc -std=c99
RM=rm -f
RMDIR=rm -rf
AR=ar cru
RANLIB=ranlib
MKDIR=mkdir -p
CP=cp
GIT_CLONE=git clone

CFLAGS=-Wall -W -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wswitch-default -Wswitch-enum -Wcast-align -Wpointer-arith -Wbad-function-cast -Wstrict-overflow=5 -Wstrict-prototypes -Winline -Wundef -Wnested-externs -Wcast-qual -Wshadow -Wunreachable-code -Wlogical-op -Wstrict-aliasing=2 -Wredundant-decls -Wold-style-definition -Wno-unused-function -ftrapv -g
CPPFLAGS=-Imlib
LDFLAGS=
MLIB_URL=https://github.com/P-p-H-d/mlib.git

all: mjsonlib.a

mjsonlib.a: mjsonlib.o
	$(AR) mjsonlib.a mjsonlib.o
	$(RANLIB) mjsonlib.a

mjsonlib.o: mjsonlib.c mjsonlib.h mlib

mlib:
	$(GIT_CLONE) $(MLIB_URL)

clean:
	$(RM) *~ *.o *.a test.exe
	$(RMDIR) mlib
	$(RMDIR) afl-out
	$(RMDIR) afl-in

check: test.exe
	./test.exe

test:	check

test.exe: test.c mjsonlib.a
	$(CC) $(CFLAGS) test.c mjsonlib.a -o test.exe

test-afl.exe: test-afl.c mjsonlib.c
	afl-gcc -std=c99 -O2 -g $(CPPFLAGS) test-afl.c mjsonlib.c -o test-afl.exe

afl: test-afl.exe
	$(MKDIR) afl-out
	$(MKDIR) afl-in
	$(CP) test.json afl-in
	afl-fuzz -i afl-in -o afl-out -- ./test-afl.exe

