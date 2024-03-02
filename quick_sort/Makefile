CC = gcc
CFLAGS = -O1 -g -Wall


OBJS := main.o

all: main

main: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ -lm

%.o: %.c
	$(CC) -o $@ $(CFLAGS) .$@.d $<

clean:
	rm -f $(OBJS)
	rm -f *.dSYM