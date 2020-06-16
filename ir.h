#ifndef IR
#define IR

#include "includes.h"
#include "motor.h"
#include "manager.h"

#define IR_TASK_STK_SIZE 128
#define IR_TASK_PRIO 3
#define IR_CLOCK_TICK 3

static OS_TCB   ir_tcb;
static CPU_STK  ir_stack[IR_TASK_STK_SIZE];

static volatile unsigned int detect;
void set_ir_bit(int x);
int get_ir_bit();
void EXTI9_5_IRQHandler();

void ir_init();
void ir_task();

#endif