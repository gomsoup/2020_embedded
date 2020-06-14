#ifndef MOTOR
#define MOTOR

#include "includes.h"

#define MOTOR_TASK_STK_SIZE 128
#define MOTOR_TASK_PRIO 3
#define MOTOR_CLOCK_TICK 3
#define MOTOR_STACK_SIZE 128

#define MOTOR_STOP      0
#define MOTOR_STRAIGHT  1
#define MOTOR_RIGHT     2
#define MOTOR_LEFT      3
#define MOTOR_BACK      4

static OS_TCB   motor_tcb;
static CPU_STK  motor_stack[MOTOR_TASK_STK_SIZE];
static OS_MUTEX motor_mutex;
static OS_Q     motor_q;

static int motor_bit = MOTOR_STOP;

typedef struct _motor{
    GPIO_TypeDef *GPIO;
    CPU_INT32U EN;
    CPU_INT32U DIR;
    CPU_INT32U PWM;
}motor;

void send_motor_msg(CPU_INT32U type);

void motor_init();
void stop();
void right_turn();
void left_turn();
void go_straight();
void go_backward();
void motor_task();


#endif