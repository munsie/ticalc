PROGS = $(basename $(wildcard *.c)) 

CC = clang
LD = ld

FLAGS = -g -I/usr/local/include -O2

CFLAGS = $(FLAGS) -std=c99
CPPFLAGS = -D_BSD_SOURCE -D_GNU_SOURCE

LIBS = 
LDFLAGS = -L/usr/local/lib $(LIBS)

.PHONY: all
all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $(filter %.c,$^) $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf core *.o $(PROGS) *~ *.dSYM 
