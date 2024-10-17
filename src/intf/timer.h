#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "isr.h" // Include ISR to ensure registers_t is defined

void init_timer(uint32_t frequency);
void start_timer();
void stop_timer();
uint32_t get_uptime();
void set_timer_frequency(uint32_t frequency); // Function to set timer frequency
void reset_timer(); // Function to reset the timer
uint32_t get_tick_count(); // Function to get the current tick count

extern uint32_t frequency; // Declare frequency as extern

#endif // TIMER_H