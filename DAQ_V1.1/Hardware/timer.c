#include "timer.h"


void time1_init(void)
{
   //配置结构体
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  /* System Clocks Configuration */
  /* TIM3 clock enable TIM1时钟使能
  定时计数器1挂载在APB2总线上，和其他普通定时计数器的APB1总线上有所区别*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  
  /* Time base configuration TIM4配置，定时50ms */
  TIM_TimeBaseStructure.TIM_Period = 10000/1 - 1;//设置计数值，计数到这个值时，会产生一个更新事件
  TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;//设置分频值（0-65535） 72MHZ/7200=10KHZ=10000HZ 1秒钟一万次
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;//时钟分频率，14.4.1，CKD分频因子.虽然是外部的，但是不配置的话，不打开定时计数器，定时器也会工作
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数器模式，向上计数。14.3.2章
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);//如果上上行不设置ClockDivision，则此函数会把CR1->CEN，中断使能打开
  /*↑此数初始化完成，立刻产生一个更新事件，立刻进入中断。是因为更新事件发生，才会让预分频缓冲器设置生效
  //TIMx->EGR = TIM_PSCReloadMode_Immediate;  
  //参考，中文参考手册14.3.1 255页
  
  如果想要消除影响
  1：可以把LED或者你想要设置的任意设备驱动口初始化，放在中断初始化后面。
  2：也可以加清除  更新中断标志位指令
  下方两条指令任意一条都可以实现，两条函数内容是一样的
  */
    //TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    //↑TIMx->SR = (uint16_t)~TIM_FLAG;
    
  TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    //↑函数内容一致，也是：TIMx->SR = (uint16_t)~TIM_IT;
  
  
  /* Enable the TIM4 global Interrupt TIM3中断配置*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
  
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM1, ENABLE);
}

void time2_init(void)
{
  //配置结构体
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  
  /* System Clocks Configuration */
  /* TIM3 clock enable TIM2+4时钟使能*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  
  
  
  /* Time base configuration TIM2配置，定时100ms */
  TIM_TimeBaseStructure.TIM_Period = 10000/100 - 1;//设置计数值，计数到这个值时，会产生一个更新事件
  TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;//设置分频值（0-65535） 72MHZ/7200=10KHZ=10000HZ 1秒钟一万次
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;//时钟分频率，14.4.1，CKD分频因子.虽然是外部的，但是不配置的话，不打开定时计数器，定时器也会工作
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数器模式，向上计数。14.3.2章
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//如果上上行不设置ClockDivision，则此函数会把CR1->CEN，中断使能打开
/*↑此数初始化完成，立刻产生一个更新事件，立刻进入中断。是因为更新事件发生，才会让预分频缓冲器设置生效
  //TIMx->EGR = TIM_PSCReloadMode_Immediate;  
  //参考，中文参考手册14.3.1 255页
  
  如果想要消除影响
  1：可以把LED或者你想要设置的任意设备驱动口初始化，放在中断初始化后面。
  2：也可以加清除  更新中断标志位指令
  下方两条指令任意一条都可以实现，两条函数内容是一样的
  */
    //TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    //↑TIMx->SR = (uint16_t)~TIM_FLAG;
    
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    //↑函数内容一致，也是：TIMx->SR = (uint16_t)~TIM_IT;
  
  
  
  /* Enable the TIM3 global Interrupt TIM3中断配置*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);


  
  
  
  
  /* TIM IT enable  TIM3使能（允许）更新中断 固件库255页，19.2.9   数据手册14.4.4设置TIMX->DIER*/
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

  
  /* TIM2 enable counter使能计数 实际就是在数据手册14.4.1设置CEN，使能计数器*/
  TIM_Cmd(TIM2, ENABLE);

}


void time3_init(void)
{
  //配置结构体
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  
  /* System Clocks Configuration */
  /* TIM3 clock enable TIM3时钟使能*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  
  /* Time base configuration TIM3配置，定时500ms */
  TIM_TimeBaseStructure.TIM_Period = 10000/2 - 1;//设置计数值，计数到这个值时，会产生一个更新事件
  TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;//设置分频值（0-65535） 72MHZ/7200=10KHZ=10000HZ 1秒钟一万次
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;//时钟分频率，14.4.1，CKD分频因子.虽然是外部的，但是不配置的话，不打开定时计数器，定时器也会工作
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数器模式，向上计数。14.3.2章

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);//如果上上行不设置ClockDivision，则此函数会把CR1->CEN，中断使能打开
/*↑此数初始化完成，立刻产生一个更新事件，立刻进入中断。是因为更新事件发生，才会让预分频缓冲器设置生效
  //TIMx->EGR = TIM_PSCReloadMode_Immediate;  
  //参考，中文参考手册14.3.1 255页
  
  如果想要消除影响
  1：可以把LED或者你想要设置的任意设备驱动口初始化，放在中断初始化后面。
  2：也可以加清除  更新中断标志位指令
  下方两条指令任意一条都可以实现，两条函数内容是一样的
  */
    //TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    //↑TIMx->SR = (uint16_t)~TIM_FLAG;
    
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    //↑函数内容一致，也是：TIMx->SR = (uint16_t)~TIM_IT;
  
  /* Enable the TIM3 global Interrupt TIM3中断配置*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

  
  
  
  /* TIM IT enable  TIM3使能（允许）更新中断 固件库255页，19.2.9   数据手册14.4.4设置TIMX->DIER*/
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

  /* TIM2 enable counter使能计数 实际就是在数据手册14.4.1设置CEN，使能计数器*/
  TIM_Cmd(TIM3, ENABLE);
}

void time4_init(void)
{
  //配置结构体
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  
  /* System Clocks Configuration */
  /* TIM3 clock enable TIM3时钟使能*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  
  /* Time base configuration TIM3配置，定时500ms */
  TIM_TimeBaseStructure.TIM_Period = 10000/16 - 1;//设置计数值，计数到这个值时，会产生一个更新事件
  TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;//设置分频值（0-65535） 72MHZ/7200=10KHZ=10000HZ 1秒钟一万次
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;//时钟分频率，14.4.1，CKD分频因子.虽然是外部的，但是不配置的话，不打开定时计数器，定时器也会工作
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数器模式，向上计数。14.3.2章

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);//如果上上行不设置ClockDivision，则此函数会把CR1->CEN，中断使能打开
  /*↑此数初始化完成，立刻产生一个更新事件，立刻进入中断。是因为更新事件发生，才会让预分频缓冲器设置生效
  //TIMx->EGR = TIM_PSCReloadMode_Immediate;  
  //参考，中文参考手册14.3.1 255页
  
  如果想要消除影响
  1：可以把LED或者你想要设置的任意设备驱动口初始化，放在中断初始化后面。
  2：也可以加清除  更新中断标志位指令
  下方两条指令任意一条都可以实现，两条函数内容是一样的
  */
    //TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    //↑TIMx->SR = (uint16_t)~TIM_FLAG;
    
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    //↑函数内容一致，也是：TIMx->SR = (uint16_t)~TIM_IT;
  
  /* Enable the TIM3 global Interrupt TIM3中断配置*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);



  
  /* TIM IT enable  TIM3使能（允许）更新中断 固件库255页，19.2.9   数据手册14.4.4设置TIMX->DIER*/
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

  /* TIM2 enable counter使能计数 实际就是在数据手册14.4.1设置CEN，使能计数器*/
  TIM_Cmd(TIM4, ENABLE);
}

void TIM1_UP_IRQHandler(void)
{
   if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
   {
     
      PAout(15) ^= 1;
      //PAout(15) = 0;
      TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
   }
}

uint8_t Refresh_Data_Flag = 0;
void TIM2_IRQHandler(void)
{
   if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
   {
      Refresh_Data_Flag ^= 1; 
      TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
   }
}

void TIM3_IRQHandler(void)
{
   if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
   {
     
  
      PBout(3) ^= 1;
    
      TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
   }
}

void TIM4_IRQHandler(void)
{
   if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
   {
     
      PBout(4) ^= 1;
    
      TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
   }
}