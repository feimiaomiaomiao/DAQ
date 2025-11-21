#include "ADC.H"



//ADC1  通道1，通道2，通道16（内部温度），通道17（电压基准源）
uint16_t ad_result[4];


void adc_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
/**********************************************************************************/
// ADCCLK = PCLK2/6  72/6 = 12MHZ 打开ADC时钟，ADC时钟来自APB2总线 来自参考手册11.1  155页
  RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
// 使能ADC1时钟和GPIOA时钟  使用PA1引脚
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
    
//  使能DMA1时钟
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
/**********************************************************************************/

//  ADC1-通道1  配置成模拟输入模式  （硬件决定）  应用于ADC电压值采样输入
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
/**********************************************************************************/


//-------------------------------------DMA1通道1配置-------------------------------------
// DMA1通道1配置，能且只能通道1参考手册10.3.7  147页 
// 目的是实现ADC1->DR寄存器里面的数据搬到ad1_result
  DMA_DeInit(DMA1_Channel1);
//  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;               //与下面一句是等价的
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);             //外设站点（ADC采集数据）
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)& ad_result;                 //存储器站点地址 存放转化结果变量的地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                            //方向：外设是源头。即外设到存储器
  DMA_InitStructure.DMA_BufferSize = 4;                                         //传输计数器的值是4
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;              //外设地址不自增
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                       //存储器地址自增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;   //宽度，半字因为ADC数据是16位 对于32位单片机16位就是半字
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;           //宽度，半字16位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                 //单次模式，传输完，要手动会把传输计数器的值加载回去
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;           //高优先级
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                  //外设到存储器之间的搬运 不是M到M（存储器到存储器）所以是失能
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
//  使能DMA1
  DMA_Cmd(DMA1_Channel1, ENABLE);

 //打开DMA1中断
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;     //抢占优先级10
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;             //响应优先级0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

//   使能通道1的中断，传输完成中断
  DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);


/**********************************************************************************/

//-------------------------------------ADC1配置-------------------------------------

  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                    //独立模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;                          //多通道扫描模式（ENABLE）  单通道（DISABLE）
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                   //连续转换（ENABLE） 单次转换（DISABLE）。防止连续转换干扰OLED显示，选择单次转换
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;   //软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                //数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel = 4;                               //4通道 有几个通道写几
  ADC_Init(ADC1, &ADC_InitStructure);

//-------------------------------------ADC1规则通道配置-------------------------------------
// （ADCx（1，2，3）,对应ADC通道X（0-16，看数据手册），该ADCX排队转换列表序号，采样周期配置）
//  设置采样周期55.5个时钟周期+固定转换时间12.5个时钟周期   68/12M=5.67us
  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 1, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1,  2, ADC_SampleTime_55Cycles5);    
  ADC_RegularChannelConfig(ADC1, ADC_Channel_2,  4, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 3, ADC_SampleTime_239Cycles5);
  
//  Enable ADC1 DMA 使能ADC1的DMA功能
  ADC_DMACmd(ADC1, ENABLE);


/**********************************************************************************/
 
/***********************打开内部通道*******************************/
//使能内部温度传感器和内部的参考电压基准的通道
//配置ADC1->CR2 |= CR2_TSVREFE_Set;  CR2_TSVREFE_Set→（0x00800000）参考手册11.12.3  CR2第23位
  ADC_TempSensorVrefintCmd(ENABLE);


/***********************ADC校准*******************************/
  ADC_Cmd(ADC1, ENABLE);        //使能ADC1
  ADC_ResetCalibration(ADC1);   //ADC校准（固定步骤）参考手册11.4
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));
  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));


  ADC_SoftwareStartConvCmd(ADC1, ENABLE);//启动ADC1转换
}

void get_adc1_result(void)
{
  
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);//启动单次转换
  
  //while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);//ADC标志位参考手册11.12.1 170页 读取DR寄存器的时候，自动清除标志位。标志位不用判断DMA自动读取了
  //ADC_ClearFlag(ADC1, ADC_FLAG_EOC);可以加，也可以不加这句
  //return (ADC_GetConversionValue(ADC1));//ADCx->DR;一般情况是读取ADC规则数据寄存器(ADC_DR)。这里采用了DMA搬运数据，不用读取返回值
}

void ADC_StartConversion(void)
{
    // 等待DMA传输完成（确保DMA就绪）
   // while(DMA_GetCmdStatus(DMA1_Channel1) == ENABLE);
    DMA_Cmd(DMA1_Channel1, DISABLE);
    // 重新设置DMA传输数据量
    DMA_SetCurrDataCounter(DMA1_Channel1, 4);
    
    // 使能DMA
    DMA_Cmd(DMA1_Channel1, ENABLE);

    // 启动ADC转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


//DMA1中断
void DMA1_Channel1_IRQHandler(void)
{
 static BaseType_t xHigherPriorityTaskWoken = pdFALSE;
 if(DMA_GetITStatus(DMA1_IT_TC1))//如果传输DMA1传输完成
 {
   DMA_ClearITPendingBit(DMA1_IT_GL1);              //清除中断标志位
   if(semphore_handle != NULL)
   {
      xSemaphoreGiveFromISR(semphore_handle, &xHigherPriorityTaskWoken);//传输完成释放信号量
      // 如果需要，立即切换到等待这个信号量的高优先级任务
      portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
   }
 }
}
