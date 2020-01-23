CC = gcc
CFLAGS = -Wall -DUSESSE2
LDFLAGS = -lm -msse2

SOURCE = $(wildcard *.c)
OBJS = $(SOURCE:.c=.o)

rgayb: $(OBJS)

clean:
	rm -f *.o rgayb
