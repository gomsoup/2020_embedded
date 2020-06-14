#include "joystick.h"

void joystick_init(){
  ADC_InitTypeDef adc;
  GPIO_InitTypeDef gpio;
  DMA_InitTypeDef dma;
  
  gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  gpio.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &gpio);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);

  DMA_DeInit(DMA1_Channel1);
  dma.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;
  dma.DMA_MemoryBaseAddr = (u32)coord;   
  dma.DMA_DIR = DMA_DIR_PeripheralSRC;
  dma.DMA_BufferSize = 2;
  dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
  dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  dma.DMA_Mode = DMA_Mode_Circular;
  dma.DMA_Priority = DMA_Priority_High;
  dma.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &dma);
  DMA_Cmd(DMA1_Channel1, ENABLE);
  
  adc.ADC_Mode = ADC_Mode_Independent;
  adc.ADC_ScanConvMode = ENABLE;
  adc.ADC_ContinuousConvMode = ENABLE;
  adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  adc.ADC_DataAlign = ADC_DataAlign_Right;
  adc.ADC_NbrOfChannel = 2;
  ADC_Init(ADC1, &adc);
  ADC_TempSensorVrefintCmd(ENABLE);
  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_41Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_41Cycles5);
  ADC_Cmd(ADC1, ENABLE);
  ADC_DMACmd(ADC1, ENABLE);
  
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
  
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void joystick_task(){
    joystick_init();
    
    while(DEF_TRUE){
      int result = MOTOR_STOP;

      if(coord[1] > 3000)
          result = MOTOR_STRAIGHT;
      else if (coord[2] < 1000)
          result = MOTOR_BACK;
      else if( coord[0] > 3000) 
          result = MOTOR_RIGHT;
      else if (coord[0] > 1500 && coord[0] < 3000) 
          result = MOTOR_STOP;
      else if (coord[0] < 1500) 
          result = MOTOR_LEFT;
    
               
      printf("JOYSTICK -> MOTOR :  %d\n", result);
      send_motor_msg(result);
    }
}