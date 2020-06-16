#include "magnetic.h"

void magnetic_init(){
    GPIO_InitTypeDef  gpio_init;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    gpio_init.GPIO_Pin = GPIO_Pin_14;
    gpio_init.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOD, &gpio_init);
   	
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource14);
    
	
    EXTI_InitTypeDef EXTI_InitStructure;
    
    EXTI_InitStructure.EXTI_Line=EXTI_Line14;
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;
    EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
    EXTI_Init(&EXTI_InitStructure);

	
    BSP_IntVectSet(BSP_INT_ID_EXTI15_10, EXTI15_10_IRQHandler);
    BSP_IntPrioSet(BSP_INT_ID_EXTI15_10, 2);
    BSP_IntEn(BSP_INT_ID_EXTI15_10);
	
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;

    NVIC_Init(&NVIC_InitStructure);
}

void set_magnetic_bit(int x){
	
	//magnet on : 0
	// 	    off : 1
	
	detect = x;
}

int get_magnetic_bit(){
	
	//magnet on : 0
	// 	    off : 1
	
	return detect;
}

void EXTI15_10_IRQHandler(){
	OSIntEnter();
	OS_ERR err;
	
  	if(EXTI_GetITStatus(EXTI_Line14) != RESET) { //magnetic
    	EXTI_ClearITPendingBit(EXTI_Line14);
		set_magnetic_bit(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_14));
		if(detect){
			printf("MAGNETIC NOT DETECTED\n");
			
		}else{
			printf("MAGNETIC DETECTED\n");
		}
   	 	EXTI_ClearITPendingBit(EXTI_Line14);
  	}
	OSIntExit();
}

void magnetic_task(){
	printf("MAGNETIC Task Start\n");
	magnetic_init();
	
	detect = 0;
	
	while(DEF_TRUE){
		if(detect && !running){
			manager_start();
		}
	}
}