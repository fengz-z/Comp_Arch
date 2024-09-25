#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REPEAT 1000000  // Number of repetitions for statistical averaging

// Cache line flush function using GCC inline assembly
void clflush(volatile void *p) {
    asm volatile ("clflush (%0)" : : "r"(p));
}

// Read Time Stamp Counter (RDTSC) function using GCC inline assembly
uint64_t rdtsc() {
    unsigned long a, d;
    asm volatile ("rdtsc" : "=a" (a), "=d" (d));
    return a | ((uint64_t)d << 32);
}

// Memory test function for copying memory and measuring time
void memtest() {
    uint64_t start, end, clock;
    int rep;  // Declare the 'rep' variable
    char *lineBuffer = (char *) malloc(64);
    char *lineBufferCopy = (char *) malloc(64);

    // Initialize the buffer with '1'
    for (int i = 0; i < 64; i++) {
        lineBuffer[i] = '1';
    }

    clock = 0;

    // Perform the memory test REPEAT times
    for (rep = 0; rep < REPEAT; rep++) {
        start = rdtsc();  // Start timing
        memcpy(lineBufferCopy, lineBuffer, 64);  // Copy 64 bytes
        end = rdtsc();  // End timing

        // Add the elapsed time for this iteration
        clock += (end - start);

        // Flush cache for next iteration
        clflush(lineBuffer);
        clflush(lineBufferCopy);

        // Print the time for each copy
        printf("%llu ticks to copy 64B\n", (end - start));
    }

    // Print the total time for all iterations
    printf("Took %llu ticks in total\n", clock);

    // Free allocated memory
    free(lineBuffer);
    free(lineBufferCopy);
}

int main(int argc, char **argv) {
    printf("——————————————\n");
    memtest();
    return 0;
}
