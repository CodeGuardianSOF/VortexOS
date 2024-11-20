#ifndef MANAGER_H
#define MANAGER_H

#include <stdint.h>

#define MAX_PROCESSES 256
#define STACK_SIZE 5096

typedef enum {
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_BLOCKED,
    PROCESS_TERMINATED
} ProcessState;

typedef struct Process {
    int pid;
    ProcessState state;
    void (*entry_point)(void);
    uint32_t* stack_pointer;
    int priority;
    struct Process* next;
} Process;

void initialize_process_manager();
Process* create_process(void (*entry_point)(void), int priority);
void terminate_process(int pid);
void block_process(int pid);
void unblock_process(int pid);
void schedule();
void print_process_info();

#endif // MANAGER_H