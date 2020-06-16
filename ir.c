#include "ir.h"

void ir_init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
    GPIO_InitTypeDef  gpio_init;
    gpio_init.GPIO_Pin = GPIO_Pin_5;
    gpio_init.GPIO_Mode = GPIO_Mode_IPD;
    
    GPIO_Init(GPIOD, &gpio_init);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource5);
    
    EXTI_InitTypeDef EXTI_InitStructure;
    
    EXTI_InitStructure.EXTI_Line=EXTI_Line5;
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;
    EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
    EXTI_Init(&EXTI_InitStructure);
	
	BSP_IntVectSet(BSP_INT_ID_EXTI9_5, EXTI9_5_IRQHandler);
    BSP_IntPrioSet(BSP_INT_ID_EXTI9_5, 1);
    BSP_IntEn(BSP_INT_ID_EXTI9_5);
	
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

    NVIC_Init(&NVIC_InitStructure);
}

void set_ir_bit(int x){
	detect = x;
}

int get_ir_bit(){
	//magnet on : 0
	// 	    off : 1
	
	return detect;
}

void EXTI9_5_IRQHandler(){
	OSIntEnter();
	OS_ERR err;
	
  	if(EXTI_GetITStatus(EXTI_Line5) != RESET) { //magnetic
    	EXTI_ClearITPendingBit(EXTI_Line5);
		set_ir_bit(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5));
   	 	EXTI_ClearITPendingBit(EXTI_Line5);
  	}
	OSIntExit();
}


void ir_task(){
	printf("IR Task Start\n");
	ir_init();
	
	detect = 1;
	
	while(DEF_TRUE){
		if(!detect && !running){
			manager_start();
		}
	}
}
