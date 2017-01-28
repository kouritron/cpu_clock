
CC=gcc

## -O0 is for no optimizations and is necessary
CFLAGS=-g -Wall -Wextra -O0 


CPUFreq: CPUFreq.c
	$(CC) $(CFLAGS) -o CPUFreq CPUFreq.c


clean:
	rm -f CPUFreq
