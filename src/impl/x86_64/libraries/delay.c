#include "delay.h"

// Function to read the current value of the TSC
static inline unsigned long long read_tsc() {
    unsigned int low, high;
    asm volatile ("rdtsc" : "=a" (low), "=d" (high));
    return ((unsigned long long)high << 32) | low;
}

// Function to delay for a specified number of seconds
void delay_sec(int seconds) {
    unsigned long long start_tsc = read_tsc();
    unsigned long long frequency = 2500000000ULL; // Assume a 2.5 GHz CPU for example
    unsigned long long target_tsc = start_tsc + (unsigned long long)seconds * frequency;

    while (read_tsc() < target_tsc) {
        // Busy-wait loop
    }
}

// Function to delay for a specified number of minutes
void delay_min(int minutes) {
    delay_sec(minutes * 60);
}

// Function to delay for a specified number of hours
void delay_hr(int hours) {
    delay_sec(hours * 3600);
}