#include "delay.h"

// Function to get the CPU clock frequency using the rdtsc instruction
unsigned long long get_cpu_frequency() {
    unsigned long long start, end;
    unsigned int low, high;

    // Read the time-stamp counter
    asm volatile ("rdtsc" : "=a" (low), "=d" (high));
    start = ((unsigned long long)high << 32) | low;

    // Wait for a second
    for (volatile int i = 0; i < 100000000; i++);

    // Read the time-stamp counter again
    asm volatile ("rdtsc" : "=a" (low), "=d" (high));
    end = ((unsigned long long)high << 32) | low;

    return end - start;
}

void delay_sec(int seconds) {
    unsigned long long frequency = get_cpu_frequency();
    volatile unsigned long long count = 0;
    unsigned long long target = (unsigned long long)seconds * frequency;

    while (count < target) {
        count++;
    }
}

void delay_min(int minutes) {
    delay_sec(minutes * 60);
}

void delay_hr(int hours) {
    delay_sec(hours * 3600);
}