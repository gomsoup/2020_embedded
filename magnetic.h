#ifndef MAGNETIC
#define MAGNETIC

#include "includes.h"

#ifndef MOTOR
#include "motor.h"
#endif

#ifndef MANAGER
#include "manager.h"
#endif

#define MAGNETIC_TASK_STK_SIZE 128
#define MAGNETIC_TASK_PRIO 3
#define MAGNETIC_CLOCK_TICK 3

static OS_TCB   magnetic_tcb;
static CPU_STK  magnetic_stack[MAGNETIC_TASK_STK_SIZE];

void EXTI15_10_IRQHandler();
static volatile unsigned int detect;
void set_magnetic_bit(int x);
int get_magnetic_bit();

void magnetic_init();
void magnetic_task();

#endif