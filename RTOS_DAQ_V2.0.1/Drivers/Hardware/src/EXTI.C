#include "EXTI.H"
#include "sys.h"
#include "delay.h"

//#include "led.h"
void Exti_Init(void)
{
   EXTI_InitTypeDef   EXTI_InitStructure;
   GPIO_InitTypeDef   GPIO_InitStructure;
   NVIC_InitTypeDef   NVIC_InitStructure;
  
  //使能GPIO时钟PA  使能AFIO时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);

  
  /* Configure PA.00 pin 上拉输入模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化引脚
  
  /* Connect EXTI0 Line to PA.00 pin 
  连接外部中断0到PA0这个引脚*/
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
  //连接外部中断0到PA4这个引脚
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
  
   /* Configure EXTI0 line 
  配置EXTI0*/
  EXTI_InitStructure.EXTI_Line = EXTI_Line0|EXTI_Line1|EXTI_Line4;//用中断0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//模式，中断模式OR事件模式。用中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //下升沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  
  /* Enable and set EXTI0 Interrupt to the lowest priority 
  配置EXTI0优先级及中断向量入口地址*/
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;//抢占优先级 0级最高
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;//响应优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable and set EXTI1 Interrupt to the lowest priority 
  配置EXTI0优先级及中断向量入口地址*/
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;//抢占优先级 0级最高
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;//响应优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable and set EXTI0 Interrupt to the lowest priority 
  配置EXTI4优先级及中断向量入口地址*/
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;//抢占优先级 1级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;//响应优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
}

void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)//==SET
  {
   /* Toggle LED1 */
    PBout(5) = 0;
    delay(9000000);
    delay(9000000);
    delay(9000000);
    PBout(5) = 1;
    //PBout(4) = 0;
    //PBout(3) ^= 1;

   /* Clear the  EXTI line 0 pending bit
    清除中断标志位
    */
   EXTI_ClearITPendingBit(EXTI_Line0);
  }
}

void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)//==SET
  {
   /* Toggle LED1 */
  //  PBout(4) = ~PBout(4);
    PBout(4) = 0;
    delay(9000000);
    delay(9000000);
    PBout(4) = 1;

   /* Clear the  EXTI line 0 pending bit
    清除中断标志位
    */
   EXTI_ClearITPendingBit(EXTI_Line1);
  }
}
void EXTI4_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line4) != RESET)//==SET
  {
   /* Toggle LED1 */
    //PBout(3) = ~PBout(3);
   // PBout(4) = 0;
    //PBout(3) ^= 1;
    PBout(3) = 0;
    delay(5000000);
    delay(1000000);
    PBout(3) = 1;
   /* Clear the  EXTI line 0 pending bit
    清除中断标志位
    */
   EXTI_ClearITPendingBit(EXTI_Line4);
  }
}
