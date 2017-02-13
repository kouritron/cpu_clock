
CC=gcc

## -O0 is for no optimizations and is necessary
CFLAGS=-g -Wall -Wextra -O0 


cpu_clock: cpu_clock.c
	$(CC) $(CFLAGS) -o cpu_clock cpu_clock.c


clean:
	rm -f cpu_clock
