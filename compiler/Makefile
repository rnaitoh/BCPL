#
#
#


CFLAGS=

all: bcplc

bcplc: main.o lexical.o parser.tab.o makecode.o misc.o debug.o
	cc -o bcplc main.o lexical.o parser.tab.o makecode.o misc.o debug.o -lfl


main.o: main.c bcpl.h
	cc $(CFLAGS) -c main.c

misc.o: misc.c parser.tab.c bcpl.h
	cc $(CFLAGS) -c misc.c

debug.o: debug.c parser.tab.c bcpl.h
	cc $(CFLAGS) -c debug.c

lexical.o: lexical.c parser.tab.c bcpl.h
	cc $(CFLAGS) -c lexical.c


lexical.c: lexical.l bcpl.h
	flex -olexical.c lexical.l

parser.tab.o: parser.tab.c bcpl.h
	cc $(CFLAGS) -c parser.tab.c

parser.tab.c: parser.y bcpl.h
	bison -v -t -d parser.y



clean:
	rm -f bcplc *.o parser.tab.c parser.tab.h *~ core lexical.c parser.output

