#include "timer.h"
#include "isr.h" // Include ISR to register the interrupt handler
#include "io.h" // Include port for outb and inb functions
#include "idt.h"

#define IRQ0 32

static uint32_t tick = 0;
static uint32_t frequency = 0;
static uint8_t timer_running = 0;

static void timer_callback(interrupt_frame *regs) {
    tick++;
}

void init_timer(uint32_t freq) {
    frequency = freq;
    // Register the timer callback
    register_interrupt_handler(IRQ0, timer_callback);
    start_timer();
}

void start_timer() {
    if (timer_running) return;

    // The value we send to the PIT is the value to divide its input clock (1193180 Hz) by
    // to get our required frequency.
    uint32_t divisor = 1193180 / frequency;

    // Send the command byte.
    outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split it into upper/lower bytes.
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    // Send the frequency divisor.
    outb(0x40, low);
    outb(0x40, high);

    timer_running = 1;
}

void stop_timer() {
    if (!timer_running) return;

    // Send the command byte to stop the timer.
    outb(0x43, 0x00);
    timer_running = 0;
}

uint32_t get_uptime() {
    return tick / frequency;
}

void set_timer_frequency(uint32_t freq) {
    frequency = freq;
    if (timer_running) {
        start_timer(); // Restart the timer with the new frequency
    }
}

void reset_timer() {
    tick = 0;
}

uint32_t get_tick_count() {
    return tick;
}