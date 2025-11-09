#include "Key.h"
#include "sys.h"

void key_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  //使能硬件时钟 PA0
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
  //配置GPIO模式,PA0上拉输入
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  //配置GPIO模式,PA0上拉输入
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}


uint8_t key_scan(void)
{
  static uint8_t key_value = 0;
  
  //if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==0)
   if(PAin(0)==0)
  {
    delay_ms(10);
//    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==0)
      if(PAin(0)==0)
    {
        key_value = 1;
    }
    while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==0);
  }
  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==0)
  {
    delay_ms(10);
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==0)
    {
        key_value = 2;
    }
    while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==0);
  }
  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)==0)
  {
    delay_ms(10);
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)==0)
    {
        key_value = 3;
    }
    while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)==0);
  }
  return key_value;
}

