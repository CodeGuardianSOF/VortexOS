#include "manager.h"
#include "memory.h"
#include "snprintf.h"
#include "vga.h"
#include <stddef.h>
#include <stdint.h>

static Process process_table[MAX_PROCESSES];
static Process* ready_queue = NULL;
static Process* blocked_queue = NULL;
static Process* current_process = NULL;
static int next_pid = 1;

void initialize_process_manager() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].pid = 0;
        process_table[i].state = PROCESS_TERMINATED;
        process_table[i].entry_point = NULL;
        process_table[i].stack_pointer = NULL;
        process_table[i].priority = 0;
        process_table[i].next = NULL;
    }
}

Process* create_process(void (*entry_point)(void), int priority) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].state == PROCESS_TERMINATED) {
            process_table[i].pid = next_pid++;
            process_table[i].state = PROCESS_READY;
            process_table[i].entry_point = entry_point;
            process_table[i].stack_pointer = (uint32_t*)kmalloc(STACK_SIZE, 0);
            if (!process_table[i].stack_pointer) {
                // Handle memory allocation failure
                return NULL;
            }
            process_table[i].priority = priority;
            process_table[i].next = NULL;

            // Add to ready queue
            if (!ready_queue) {
                ready_queue = &process_table[i];
            } else {
                Process* temp = ready_queue;
                while (temp->next) {
                    temp = temp->next;
                }
                temp->next = &process_table[i];
            }

            // Debug print
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "Created process PID: %d, State: %d, Priority: %d\n", process_table[i].pid, process_table[i].state, process_table[i].priority);
            print_str(buffer);

            return &process_table[i];
        }
    }
    return NULL;
}

void terminate_process(int pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid == pid) {
            // Remove from ready or blocked queue
            Process** queue = (process_table[i].state == PROCESS_BLOCKED) ? &blocked_queue : &ready_queue;
            if (*queue == &process_table[i]) {
                *queue = (*queue)->next;
            } else {
                Process* temp = *queue;
                while (temp && temp->next != &process_table[i]) {
                    temp = temp->next;
                }
                if (temp) {
                    temp->next = process_table[i].next;
                }
            }

            process_table[i].state = PROCESS_TERMINATED;
            process_table[i].pid = 0;
            process_table[i].entry_point = NULL;
            kfree(process_table[i].stack_pointer);
            process_table[i].stack_pointer = NULL;
            return;
        }
    }
}

void block_process(int pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid == pid && process_table[i].state == PROCESS_RUNNING) {
            process_table[i].state = PROCESS_BLOCKED;

            // Remove from ready queue
            if (ready_queue == &process_table[i]) {
                ready_queue = ready_queue->next;
            } else {
                Process* temp = ready_queue;
                while (temp && temp->next != &process_table[i]) {
                    temp = temp->next;
                }
                if (temp) {
                    temp->next = process_table[i].next;
                }
            }

            // Add to blocked queue
            process_table[i].next = blocked_queue;
            blocked_queue = &process_table[i];
            return;
        }
    }
}

void unblock_process(int pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid == pid && process_table[i].state == PROCESS_BLOCKED) {
            process_table[i].state = PROCESS_READY;

            // Remove from blocked queue
            if (blocked_queue == &process_table[i]) {
                blocked_queue = blocked_queue->next;
            } else {
                Process* temp = blocked_queue;
                while (temp && temp->next != &process_table[i]) {
                    temp = temp->next;
                }
                if (temp) {
                    temp->next = process_table[i].next;
                }
            }

            // Add to ready queue
            process_table[i].next = ready_queue;
            ready_queue = &process_table[i];
            return;
        }
    }
}

void context_switch(Process* next_process) {
    // Save the current process state
    if (current_process) {
        // Save the stack pointer and other necessary state
        asm volatile("mov %%esp, %0" : "=m"(current_process->stack_pointer));
    }

    // Load the next process state
    current_process = next_process;
    asm volatile("mov %0, %%esp" : : "m"(current_process->stack_pointer));

    // Perform the actual context switch
    // Ensure the entry point is called only once when the process starts
    if (current_process->state == PROCESS_RUNNING) {
        current_process->entry_point();
    }
}

void schedule() {
    if (!ready_queue) return;

    Process* highest_priority_process = ready_queue;
    Process* prev = NULL;
    Process* temp = ready_queue;
    Process* temp_prev = NULL;

    // Find the highest priority process in the ready queue
    while (temp) {
        if (temp->priority > highest_priority_process->priority) {
            highest_priority_process = temp;
            prev = temp_prev;
        }
        temp_prev = temp;
        temp = temp->next;
    }

    // Remove the highest priority process from the ready queue
    if (prev) {
        prev->next = highest_priority_process->next;
    } else {
        ready_queue = highest_priority_process->next;
    }

    if (current_process) {
        current_process->state = PROCESS_READY;
    }

    current_process = highest_priority_process;
    current_process->state = PROCESS_RUNNING;

    // Debug print
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Scheduled process PID: %d, State: %d, Priority: %d\n", current_process->pid, current_process->state, current_process->priority);
    print_str(buffer);

    // Context switch to the new process
    context_switch(current_process);
}

void print_process_info() {
    char buffer[256];
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].state != PROCESS_TERMINATED) {
            snprintf(buffer, sizeof(buffer), "Process PID: %d, State: %d, Priority: %d\n", process_table[i].pid, process_table[i].state, process_table[i].priority);
            print_str(buffer);
        }
    }
}

__attribute__((constructor))
static void setup_process_manager() {
    initialize_process_manager();
}

__attribute__((destructor))
static void cleanup_process_manager() {
    print_process_info();
}