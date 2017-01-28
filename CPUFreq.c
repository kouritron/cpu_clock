// CPUFreq.c 
//
// small program to Calculate cpu clock from user space. 
//
// License: 
// Public Domain, software "as is", no guarantees of functionality or safety made.



#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>



// ----------------------------------------------------------------------------------------
// -------------------------------------------------------------------------- cycle counter


// initialize the cycle counter
uint64_t start = 0;


// cycle counter register is 64 bits, read it into hi and lo using the rdtsc instruction.
inline static void read_cycle_counter(uint32_t *hi, uint32_t *lo)
{
	asm volatile("rdtsc; movl %%edx, %0; movl %%eax, %1" /* Read cycle counter */
	: "=r" (*hi), "=r" (*lo)                /* and move results to */
	: /* No input */                        /* the two outputs */
	: "%edx", "%eax"
	);
}


// Reset counter
void reset_counter()
{
	uint32_t hi, lo;
	
	// read the total cycle count now and save it somewhere
	read_cycle_counter(&hi, &lo);
	start = ((uint64_t)hi << 32) | lo;
}

// get the number of cycles that have passed since the last time counter was reset.
uint64_t get_counter()
{
	uint32_t hi, lo;
	
	// read the total cycle count now, and Calculate the difference from when it was reset.
	read_cycle_counter(&hi, &lo);
	return (((uint64_t)hi << 32) | lo) - start;
	
}


// ----------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------   main

int main(int argc, char ** argv)
{
	// we get this from commandline, program should be called like this
	// $ ./CPUFreq 2
	// $ ./CPUFreq 1
	// 1 and 2 are the sleep times. default is 1 second.
	
	int sleep_time = 1;
	
	if (2 == argc)
	{
		sleep_time = atoi(argv[1]);
		if (sleep_time <= 0)
		{
			printf("Invalid sleep time, will default to 1 second.\n");
			sleep_time = 1;
		}
	}
	else
	{
		printf("Sleep time was not supplied, will default to 1 second.\n");
		sleep_time = 1;
	}
	
	
	// Calculate CPU freq.
	double frqInMHz = 0;
	
	printf("Calculating CPU frequency. Plz stand by ...\n");
	
	// burn the CPU a little bit. Assuming the CPU scaling algorithms (in hardware usually), 
	// will scale up the CPU when it is being utilized, Dont forget to compile with no optimizations
	// otherwise gcc may take this out realizing it has no side effects for the rest of the code.
	volatile int ignore=0;
	for (ignore=0; ignore < 100000000; ignore++)
	{
		ignore++;
		ignore--;
	}
	
	reset_counter();
	sleep(sleep_time);
	frqInMHz = get_counter() / (sleep_time * 1000000.0);
	
	printf("CPU frequency appears to be:\n%f Mhz.\n", frqInMHz);
	
	return EXIT_SUCCESS;
}


