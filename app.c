#include <includes.h>

#include "motor.h"
#include "joystick.h"

//constant define
#define MAIN_TASK_STK_SIZE 256
#define MAIN_TASK_PRIO 1

// task variables
static  OS_TCB          main_init_tcb;
static  CPU_STK         main_init_stack[MAIN_TASK_STK_SIZE];

// task functions
static  void  main_init_task(void);
static  void  main_task_create(void);

int  main (void)
{
    OS_ERR  err;

    BSP_IntDisAll();                                            
    OSInit(&err);                                              

    OSSchedRoundRobinCfg((CPU_BOOLEAN)DEF_TRUE, 
                         (OS_TICK    )10,
                         (OS_ERR    *)&err);
    
    OSTaskCreate((OS_TCB     *)&main_init_tcb,                
             (CPU_CHAR   *)"Main init task start",
             (OS_TASK_PTR )main_init_task, 
             (void       *)0,
             (OS_PRIO     )MAIN_TASK_PRIO,
             (CPU_STK    *)&main_init_stack[0],
             (CPU_STK_SIZE)MAIN_TASK_STK_SIZE / 10,
             (CPU_STK_SIZE)MAIN_TASK_STK_SIZE,
             (OS_MSG_QTY  )0,
             (OS_TICK     )0,
             (void       *)0,
             (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
             (OS_ERR     *)&err);    
    

    OSStart(&err);                                             
}
static void main_init_task(void){
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;
    
    BSP_Init();          
    CPU_Init();

    cpu_clk_freq = BSP_CPU_ClkFreq();
    cnts         = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;
    OS_CPU_SysTickInit(cnts);                                   

    main_task_create();
}

static void main_task_create(void){
    OS_ERR  err;
    
    OSTaskCreate((OS_TCB     *)&motor_tcb,                
             (CPU_CHAR   *)"Motor Task Start",
             (OS_TASK_PTR )motor_task, 
             (void       *)0,
             (OS_PRIO     )MOTOR_TASK_PRIO,
             (CPU_STK    *)&motor_stack[0],
             (CPU_STK_SIZE)MOTOR_STACK_SIZE / 10,
             (CPU_STK_SIZE)MOTOR_STACK_SIZE,
             (OS_MSG_QTY  )0,
             (OS_TICK     )MOTOR_CLOCK_TICK,
             (void       *)0,
             (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
             (OS_ERR     *)&err);

    
    OSTaskCreate((OS_TCB     *)&joystick_tcb,                
             (CPU_CHAR   *)"Joystick task start",
             (OS_TASK_PTR )joystick_task, 
             (void       *)0,
             (OS_PRIO     )JOYSTICK_TASK_PRIO,
             (CPU_STK    *)&joystick_stack[0],
             (CPU_STK_SIZE)JOYSTICK_TASK_STK_SIZE / 10,
             (CPU_STK_SIZE)JOYSTICK_TASK_STK_SIZE,
             (OS_MSG_QTY  )0,
             (OS_TICK     )JOYSTICK_CLOCK_TICK,
             (void       *)0,
             (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
             (OS_ERR     *)&err);
}    