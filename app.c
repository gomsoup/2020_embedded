#include <includes.h>

#include "motor.h"
#include "joystick.h"

//constant define
#define LED_TASK_STK_SIZE 256
#define LED_TASK_PRIO 1

#define LED_THRESHOLD 1000000
#define LED1_CLOCK_TICK 2 // middle tick
#define LED2_CLOCK_TICK 1 // highest tick
#define LED3_CLOCK_TICK 1 // lowest tick

// task variables
static  OS_TCB          led1_tcb;
static  OS_TCB          led2_tcb;
static  OS_TCB          led3_tcb;
static  OS_TCB          led_init_tcb;

static  CPU_STK         led1_stack[LED_TASK_STK_SIZE];
static  CPU_STK         led2_stack[LED_TASK_STK_SIZE];
static  CPU_STK         led3_stack[LED_TASK_STK_SIZE];
static  CPU_STK         led_init_stack[LED_TASK_STK_SIZE];

// task functions

static  void  led1_task(void);
static  void  led2_task(void);
static  void  led3_task(void);
static  void  led_init_task(void);
static  void  led_task_create(void);

static  void  led_toggle(CPU_INT32U led);

int  main (void)
{
    OS_ERR  err;

    BSP_IntDisAll();                                            
    OSInit(&err);                                              

    OSSchedRoundRobinCfg((CPU_BOOLEAN)DEF_TRUE, 
                         (OS_TICK    )10,
                         (OS_ERR    *)&err);
    
    OSTaskCreate((OS_TCB     *)&led_init_tcb,                
             (CPU_CHAR   *)"Led init task start",
             (OS_TASK_PTR )led_init_task, 
             (void       *)0,
             (OS_PRIO     )LED_TASK_PRIO,
             (CPU_STK    *)&led_init_stack[0],
             (CPU_STK_SIZE)LED_TASK_STK_SIZE / 10,
             (CPU_STK_SIZE)LED_TASK_STK_SIZE,
             (OS_MSG_QTY  )0,
             (OS_TICK     )0,
             (void       *)0,
             (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
             (OS_ERR     *)&err);    
    

    OSStart(&err);                                             
}
static void led_init_task(void){
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;
    

    BSP_Init();                                                 

    CPU_Init();

    cpu_clk_freq = BSP_CPU_ClkFreq();
    cnts         = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;
    OS_CPU_SysTickInit(cnts);                                   

    led_task_create();
}

static void led_task_create(void){
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
             (OS_TICK     )LED1_CLOCK_TICK,
             (void       *)0,
             (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
             (OS_ERR     *)&err);

    
    OSTaskCreate((OS_TCB     *)&led3_tcb,                
             (CPU_CHAR   *)"Led3 task start",
             (OS_TASK_PTR )joystick_task, 
             (void       *)0,
             (OS_PRIO     )JOYSTICK_TASK_PRIO,
             (CPU_STK    *)&led3_stack[0],
             (CPU_STK_SIZE)LED_TASK_STK_SIZE / 10,
             (CPU_STK_SIZE)LED_TASK_STK_SIZE,
             (OS_MSG_QTY  )0,
             (OS_TICK     )LED1_CLOCK_TICK,
             (void       *)0,
             (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
             (OS_ERR     *)&err);
        
    /*
    
    OSTaskCreate((OS_TCB     *)&led2_tcb,                
             (CPU_CHAR   *)"Led2 task start",
             (OS_TASK_PTR )led2_task, 
             (void       *)0,
             (OS_PRIO     )LED_TASK_PRIO,
             (CPU_STK    *)&led2_stack[0],
             (CPU_STK_SIZE)LED_TASK_STK_SIZE / 10,
             (CPU_STK_SIZE)LED_TASK_STK_SIZE,
             (OS_MSG_QTY  )0,
             (OS_TICK     )LED2_CLOCK_TICK,
             (void       *)0,
             (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
             (OS_ERR     *)&err);   

  */
}    

static void led_toggle (CPU_INT32U led){
    CPU_INT32U pins;

    pins = GPIO_ReadOutputData(GPIOD);
    pins ^= led;
    GPIO_SetBits(GPIOD, pins & led);
    GPIO_ResetBits(GPIOD, (~pins) & led);
}

static void led1_task(void){
    /*
    GPIO_InitTypeDef gpio;
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN, ENABLE);
    
    gpio.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);
    
    motor m1, m2;
    m1.GPIO = GPIOA; m1.EN = GPIO_Pin_0; m1.DIR = GPIO_Pin_2; m1.PWM = GPIO_Pin_1;
    m2.GPIO = GPIOA; m2.EN = GPIO_Pin_3; m2.DIR = GPIO_Pin_5; m2.PWM = GPIO_Pin_4;
    
    GPIO_SetBits(m1.GPIO, m1.EN);  // EN0
    GPIO_SetBits(m1.GPIO, m1.DIR); // DIR0
    GPIO_ResetBits(m1.GPIO, m1.PWM); // PWM0
    
    GPIO_SetBits(m2.GPIO, m2.EN);
    GPIO_SetBits(m2.GPIO, m2.DIR);
    GPIO_ResetBits(m2.GPIO, m2.PWM);
*/    

    while(DEF_TRUE){
    }
}

static void led2_task(void){
    volatile unsigned int cnt = 0;
    
    while(DEF_TRUE){
        if(cnt < LED_THRESHOLD) cnt++;
        else{
            cnt = 0;
            led_toggle(BSP_GPIOD_LED2);
        }
    }
}

static void led3_task(void){
    volatile unsigned int cnt = 0;
    
    while(DEF_TRUE){
        if(cnt < LED_THRESHOLD) cnt++;
        else{
            cnt = 0;
            led_toggle(BSP_GPIOD_LED3);
        }
    }
}