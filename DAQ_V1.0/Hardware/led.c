#include "led.h"

//PB5做位带编辑区
//PB端口的ODR寄存器的地址：PB5->ODR：0x40010c00+0x0c  位带区
//通过位带别名区地址访问：0x42000000+(0x40010c00-0X40000000)*32+5*4=0x42218194
//参考M3权威指南第89页
//#define PB5 *(volatile uint32_t *)(0x42218194)
//#define PB5 *(volatile uint32_t *)(0x42000000 + (0x40010c0C - 0X40000000)*32 + 5*4)
//#define PB4 *(volatile uint32_t *)(0x42000000 + (0x40010c0C - 0X40000000)*32 + 4*4)
//#define PB3 *(volatile uint32_t *)(0x42000000 + (0x40010c0C - 0X40000000)*32 + 3*4)


void led_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  //开启GPIO硬件时钟 PB5
  /* GPIOB Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  //配置GPIO模式
  /* Configure PB5 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  //控制端口寄存器输出电平
  GPIO_SetBits(GPIOC, GPIO_Pin_13);
  
  
  //GPIO_InitTypeDef GPIO_InitStructure;
  //开启GPIO硬件时钟 PB5
  /* GPIOB Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
  //配置GPIO模式
  /* Configure PB5 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //控制端口寄存器输出电平
  //GPIO_SetBits(GPIOB, GPIO_Pin_5);
//  PB5 = 1;
//  PB4 = 1;
//  PB3 = 1;
  PBout(5) = 1;
  PBout(4) = 1;
  PBout(3) = 1;
}

void Led_on()
{
//  GPIO_ResetBits(GPIOC, GPIO_Pin_13);
//  GPIO_ResetBits(GPIOB, GPIO_Pin_5);
//  PB5 = 0;
//  PB4 = 0;
//  PB3 = 0;
  PBout(5) =0;
  PBout(4) = 0;
  PBout(3) = 0;
}
void Led_off()
{
//  GPIO_SetBits(GPIOC, GPIO_Pin_13);
//  GPIO_SetBits(GPIOB, GPIO_Pin_5);
//  PB5 = 1;
//  PB4 = 1;
//  PB3 = 1;
   PBout(5) = 1;
  PBout(4) = 1;
  PBout(3) = 1;
}