/*
******************************************************************************
程序简介:
ADC+DMA多通道数据采集，显示在OLED上，并通过串口1发送到串口上。

***改动：将滑动变阻器1变成压力传感器，
         压力传感器变化范围为40K-无穷大，
         采取传统分压电路读取模拟值，串联固定电阻100K

***可优化：因为本程序偏向于实现功能，加上手边没有元件问题，没有在ADC引脚上加滤波电容
           可在ADC引脚和地之间加0.1UF滤波电容
           
存在BUG：
***BUG：在画圆圈时，圆圈快满的时候会造成数据刷显示停顿
***暂时处理-表面修复：降低显示刷新频率，待深究修复底层问题

人员：FT
日期：2025年11月3日
版本号：V1.2.0
******************************************************************************

******************************************************************************
连接引脚：
C8T6               Hardware

PA1   --------    滑动变阻器1
PA2   --------    滑动变阻器2
PA9   --------    UART1_TX
PA10  --------    UART1_RX

PB8   --------    OLED_SCL
PB9   --------    OLED_SDA

******************************************************************************
其他详情请参照Readme

*/
#include  <stm32f10x.h>
#include  <led.h>
#include  "delay.h"
#include  "Key.h"
#include "EXTI.H"
#include "uart.h"
#include "ADC.H"
#include "OLED.h"
#include "delay.h"
#include "timer.h"
#include "display.h"
//编码格式：UTF-8

int main(void)
{
  delay_ms(500);
  OLED_Init();
  OLED_Clear();
  time2_init();
  led_init();
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断优先级分组
  adc_init();
  Uart_Init();
  OLED_DrawTriangle(0, 10, 5, 5, 10, 10, OLED_FILLED);
  OLED_ShowString(10,0,"DAQ_4",OLED_8X16);
  OLED_ShowChinese(55,0,"嘀嘀");
  OLED_ShowString(90,8,"V1.1.1",OLED_6X8);
  OLED_Update();
  Send_String("Hello Word! \r\n");
  printf("ADC-DMA!!!!!\r\n");
  
  ad_Vref = 1.2*4096 /ad_result[0];       //读取数组中0数据，存放的是通道17的1.2V内部参照电压，校准当前供电电压，数据手册5.3.4 26页
  printf("ad_Vref = %0.3f\r\n",ad_Vref);                      //打印校准后的供电电压
  ad_result1 = ad_result[1]*ad_Vref/4095*140/100; //读取数组中1数据，存放的ADC通道1连接的设备，采用校准后的电压，计算出模拟数值
  if(ad_result1>ad_Vref)
    ad_result1=ad_Vref;
  printf("ad_result1 = %0.3f\r\n",ad_result1);                //打印ADC1通道1数据
  ad_result16 = ad_result[2]*ad_Vref/4095; //读取数组中2数据，存放的ADC1通道16内部温度计的模拟值
  temp = (1.43-ad_result16)/0.0043 +25;     //根据模拟值，算出温度，参考手册11.10 168页，数据手册5.3.18 51页
  printf("temp = %0.3f\r\n",temp);                            //打印校准后的内部温度（通道16）
  ad_result2 = ad_result[3]*ad_Vref/4095; //读取数组中3数据，存放的ADC通道2连接的设备，采用校准后的电压，计算出模拟数值
  printf("ad_result2 = %0.3f\r\n",ad_result2);                //打印ADC1通道2数据
  //OLED_ReverseArea(108, 40, 20, 24);
  
  OLED_Printf(0,20,OLED_6X8,"Prower  :%+5.3f",ad_Vref);
  OLED_ShowChar(98,20,'V',OLED_6X8);
  
  OLED_Printf(0,30,OLED_6X8,"ad1_res1:%+5.3f",ad_result1);
  OLED_ShowChar(98,30,'V',OLED_6X8);
  
  OLED_Printf(0,40,OLED_6X8,"temp    :%+5.3f",temp);
  OLED_ShowImage(98,40,6,8,Celsius);
  
  OLED_Printf(0,50,OLED_6X8,"ad1_res2:%+5.3f",ad_result2);
  OLED_ShowChar(98,50,'V',OLED_6X8);
  OLED_Update();
  
  while(1)
  {
    if(Refresh_Data_Flag)
    {
      Data_Shows();
    }
    else
    {
      Data_Collection();
    }
  }
}
