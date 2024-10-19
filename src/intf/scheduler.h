#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

typedef void (*task_func_t)();

void add_task(task_func_t func, uint32_t interval);
void task_scheduler();

#endif 