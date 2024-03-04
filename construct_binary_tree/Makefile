CC = gcc
CFLAGS = -O1 -g -Wall


OBJS := main.o tree.o

all: treetest

treetest: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ -lm

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c -MMD -MF .$@.d $<

clean:
	rm -f $(OBJS)
	rm -f *.dSYM
	rm -f treetest
	rm -f .*.o.d