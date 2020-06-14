#ifndef JOYSTICK
#define JOYSTICK

#include "includes.h"
#include "motor.h"

#define JOYSTICK_TASK_STK_SIZE 64
#define JOYSTICK_TASK_PRIO 3
#define JOYSTICK_CLOCK_TICK 3
#define JOYSTICK_STACK_SIZE 64

static OS_TCB   joystick_tcb;
static CPU_STK  joystick_stack[JOYSTICK_TASK_STK_SIZE];

static volatile unsigned int coord[2];

int read_x();
int read_y();

void joystick_task();

#endif