#include "scheduler.h"
#include "timer.h"

#define MAX_TASKS 50

typedef struct {
    task_func_t func;
    uint32_t interval;
    uint32_t last_execution;
} Task;

static Task tasks[MAX_TASKS];
static uint32_t task_count = 0;

void add_task(task_func_t func, uint32_t interval) {
    if (task_count < MAX_TASKS) {
        tasks[task_count].func = func;
        tasks[task_count].interval = interval;
        tasks[task_count].last_execution = get_tick_count();
        task_count++;
    }
}

void task_scheduler() {
    uint32_t current_time = get_tick_count();
    for (uint32_t i = 0; i < task_count; i++) {
        if (current_time - tasks[i].last_execution >= tasks[i].interval) {
            tasks[i].func();
            tasks[i].last_execution = current_time;
        }
    }
}