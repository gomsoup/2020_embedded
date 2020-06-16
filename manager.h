#ifndef MANAGER
#define MANAGER

#include "includes.h"
#include "motor.h"
#include "magnetic.h"

#define MANAGER_TASK_STK_SIZE 256
#define MANAGER_TASK_PRIO 2
#define MANAGER_CLOCK_TICK 3

static OS_TCB   manager_tcb;
static CPU_STK  manager_stack[MANAGER_TASK_STK_SIZE];

static int running;

void manager_start();
void manager_stop();
void manager_task();
#endif