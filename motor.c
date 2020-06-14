#include "motor.h"

int motor_pulse[2] = {0, 255};
motor m1, m2;

void set_motor(int _motor_bit){
    motor_bit = _motor_bit;
}

void stop(){
    OS_ERR err;
    CPU_TS ts;
 
    OSMutexPend((OS_MUTEX *)&motor_mutex, (OS_TICK   )0, (OS_OPT    )OS_OPT_PEND_BLOCKING, (CPU_TS   *)&ts,(OS_ERR   *)&err);
    
    GPIO_ResetBits(m1.GPIO, m1.EN);
    GPIO_SetBits(m1.GPIO, m1.DIR); // DIR0
    GPIO_SetBits(m1.GPIO, m1.PWM); // PWM0
    
    GPIO_ResetBits(m2.GPIO, m2.EN);
    GPIO_SetBits(m2.GPIO, m2.DIR);
    GPIO_SetBits(m2.GPIO, m2.PWM);
    
    OSMutexPost((OS_MUTEX *)&motor_mutex, (OS_OPT    )OS_OPT_POST_NONE, (OS_ERR    *)&err);
}

void right_turn(){
    OS_ERR err;
    CPU_TS ts;
    
    OSMutexPend((OS_MUTEX *)&motor_mutex, (OS_TICK   )0, (OS_OPT    )OS_OPT_PEND_BLOCKING, (CPU_TS   *)&ts,(OS_ERR   *)&err);
    
    GPIO_SetBits(m1.GPIO, m1.EN);  // EN0
    GPIO_SetBits(m1.GPIO, m1.DIR); // DIR0
    GPIO_ResetBits(m1.GPIO , m1.PWM); // PWM0
    
    GPIO_ResetBits(m2.GPIO, m2.EN);
    
    OSMutexPost((OS_MUTEX *)&motor_mutex, (OS_OPT    )OS_OPT_POST_NONE, (OS_ERR    *)&err);
}

void left_turn(){
    OS_ERR err;
    CPU_TS ts;
    
    OSMutexPend((OS_MUTEX *)&motor_mutex, (OS_TICK   )0, (OS_OPT    )OS_OPT_PEND_BLOCKING, (CPU_TS   *)&ts,(OS_ERR   *)&err);
    
    GPIO_ResetBits(m1.GPIO, m1.EN);  // EN0
    
    GPIO_SetBits(m2.GPIO, m2.EN);
    GPIO_SetBits(m2.GPIO, m2.DIR);
    GPIO_ResetBits(m2.GPIO, m2.PWM);
    
    OSMutexPost((OS_MUTEX *)&motor_mutex, (OS_OPT    )OS_OPT_POST_NONE, (OS_ERR    *)&err);
}

void go_straight(){
    OS_ERR err;
    CPU_TS ts;
 
    OSMutexPend((OS_MUTEX *)&motor_mutex, (OS_TICK   )0, (OS_OPT    )OS_OPT_PEND_BLOCKING, (CPU_TS   *)&ts,(OS_ERR   *)&err);
    
    GPIO_SetBits(m1.GPIO, m1.EN);  // EN0
    GPIO_SetBits(m1.GPIO, m1.DIR); // DIR0
    GPIO_ResetBits(m1.GPIO, m1.PWM); // PWM0
    
    GPIO_SetBits(m2.GPIO, m2.EN);
    GPIO_SetBits(m2.GPIO, m2.DIR);
    GPIO_ResetBits(m2.GPIO, m2.PWM);
        
    OSMutexPost((OS_MUTEX *)&motor_mutex, (OS_OPT    )OS_OPT_POST_NONE, (OS_ERR    *)&err);
}

void go_backward(){
    OS_ERR err;
    CPU_TS ts;
 
    OSMutexPend((OS_MUTEX *)&motor_mutex, (OS_TICK   )0, (OS_OPT    )OS_OPT_PEND_BLOCKING, (CPU_TS   *)&ts,(OS_ERR   *)&err);
    
    GPIO_SetBits(m1.GPIO, m1.EN);  // EN0
    GPIO_ResetBits(m1.GPIO, m1.DIR); // DIR0
    GPIO_ResetBits(m1.GPIO, m1.PWM); // PWM0
    
    GPIO_SetBits(m2.GPIO, m2.EN);
    GPIO_ResetBits(m2.GPIO, m2.DIR);
    GPIO_ResetBits(m2.GPIO, m2.PWM);
            
    OSMutexPost((OS_MUTEX *)&motor_mutex, (OS_OPT    )OS_OPT_POST_NONE, (OS_ERR    *)&err);
}


void test(){
    OS_ERR err;
    TIM_OCInitTypeDef ocinit;
    
    ocinit.TIM_OCMode = TIM_OCMode_PWM1;
    ocinit.TIM_OCPolarity = TIM_OCPolarity_High;
    ocinit.TIM_OutputState = TIM_OutputState_Enable;
    
    
    GPIO_SetBits(m1.GPIO, m1.EN);  // EN0
    GPIO_SetBits(m1.GPIO, m1.DIR); // DIR0
    
    ocinit.TIM_Pulse = 700;
    TIM_OC3Init(TIM3, &ocinit);
    OSTimeDlyHMSM(0,0,1,0, OS_OPT_TIME_HMSM_STRICT, &err);
    
    ocinit.TIM_Pulse = 5000;
    TIM_OC3Init(TIM3, &ocinit);
    
    OSTimeDlyHMSM(0,0,1,0, OS_OPT_TIME_HMSM_STRICT, &err);
    GPIO_ResetBits(m1.GPIO, m1.EN);  // EN0
    /*
    for(int i = 0; i <2300; i ++){
      ocinit.TIM_Pulse = i;
      OSTimeDlyHMSM(0,0,0,10, OS_OPT_TIME_HMSM_STRICT, &err);
      TIM_OC4Init(TIM3, &ocinit);
    }
    for(int i = 2300; i >= 0; i--){
      ocinit.TIM_Pulse = i;
      OSTimeDlyHMSM(0,0,0,10, OS_OPT_TIME_HMSM_STRICT, &err);
      TIM_OC4Init(TIM3, &ocinit);
    }
    */
}

void motor_init(){
    GPIO_InitTypeDef gpio;
    TIM_TimeBaseInitTypeDef timer;
    TIM_OCInitTypeDef ocinit;
    
    //clock set
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN, ENABLE);
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    //gpio map
    gpio.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);
    
    /*
    gpio.GPIO_Pin = GPIO_Pin_6;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
    
    //Timer set

    timer.TIM_Period = 1000 - 1;
    timer.TIM_Prescaler = 0; //(CPU_INT16U)(72000000 / 1000000) - 1;
    timer.TIM_ClockDivision = 0;
    timer.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM3, &timer);
    
    //PWM set
    ocinit.TIM_OCMode = TIM_OCMode_PWM1;
    ocinit.TIM_OCPolarity = TIM_OCPolarity_High;
    ocinit.TIM_OutputState = TIM_OutputState_Enable;
    ocinit.TIM_Pulse = 500;
    TIM_OC3Init(TIM3, &ocinit);
    
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
    /*
    
    /*
    m1.GPIO = GPIOA; m1.EN = GPIO_Pin_0; m1.DIR = GPIO_Pin_2; m1.PWM = GPIO_Pin_9;
    m2.GPIO = GPIOA; m2.EN = GPIO_Pin_3; m2.DIR = GPIO_Pin_5; m2.PWM = GPIO_Pin_9;
    */
    
    
    m1.GPIO = GPIOA; m1.EN = GPIO_Pin_0; m1.DIR = GPIO_Pin_2; m1.PWM = GPIO_Pin_1;
    m2.GPIO = GPIOA; m2.EN = GPIO_Pin_3; m2.DIR = GPIO_Pin_5; m2.PWM = GPIO_Pin_4;
    
}

void send_motor_msg(CPU_INT32U type){
    OS_ERR err;
    OSQPost((OS_Q      *)&motor_q,
            (void      *)type,
            (OS_MSG_SIZE)sizeof(void *),
            (OS_OPT     )OS_OPT_POST_FIFO,
            (OS_ERR    *)&err);
}

void motor_task(){
    OS_ERR      err;
    CPU_INT32U  q_msg;
    OS_MSG_SIZE size;
    CPU_TS       ts;
    
    motor_init();
    OSMutexCreate((OS_MUTEX    *)&motor_mutex, (CPU_CHAR    *)"App Mutex", (OS_ERR      *)&err);
    OSQCreate((OS_Q *)&motor_q, (CPU_CHAR *)"Motor queue", (OS_MSG_QTY)20, (OS_ERR *)&err);
    
    while(DEF_TRUE){
        q_msg = (CPU_INT32U)OSQPend((OS_Q        *)&motor_q,
                                    (OS_TICK      )10000 * 10,
                                    (OS_OPT       )OS_OPT_PEND_BLOCKING,
                                    (OS_MSG_SIZE *)&size,
                                    (CPU_TS      *)&ts,
                                    (OS_ERR      *)&err);
        if(err == OS_ERR_TIMEOUT) stop();
        
        printf("MOTOR MESSAGE QUEUE: %d\n", q_msg);
        switch(q_msg){
        case MOTOR_STOP:
            stop();
            break;        
        case MOTOR_STRAIGHT:
            go_straight();
            break;
        case MOTOR_BACK:
            go_backward();
            break;
        case MOTOR_LEFT:
            left_turn();
            break;
        case MOTOR_RIGHT:
            right_turn();
            break;
        default:
            stop();
            break;
        }
    }     
}