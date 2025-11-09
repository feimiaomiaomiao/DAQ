/*
******************************************************************************
程序简介:
ADC+DMA多通道数据采集，显示在OLED上，并通过串口1发送到串口上。

注：程序芯片为STM32F103C8T6，所有注释仅针对此芯片，其他芯片请参考相同内容章节。

人员：FT
日期：2025年10月30日
版本号：V1.0.0
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
//编码格式：UTF-8



int main(void)
{
  uint8_t i = 0;
  float ad_result1,ad_result16,ad_result2,temp;
  float ad_Vref;
  uint16_t a = 1, b = 0,c= 0, num = 0;
  uint8_t flag = 0;
  OLED_Init();
  OLED_Clear();
  
  
  
  led_init();
   /* Configure the NVIC Preemption Priority Bits */  
  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断优先级分组

  adc_init();
  Uart_Init();
  OLED_DrawTriangle(0, 10, 5, 5, 10, 10, OLED_FILLED);
  OLED_ShowString(10,0,"DAQ_4",OLED_8X16);
  OLED_ShowChinese(55,0,"高坤");
  OLED_ShowString(90,8,"V1.0.0",OLED_6X8);
  OLED_Update();
  Send_String("Hello Word! \r\n");
  printf("ADC-DMA!!!!!\r\n");
  
  ad_Vref = 1.2*4096 /ad_result[0];       //读取数组中0数据，存放的是通道17的1.2V内部参照电压，校准当前供电电压，数据手册5.3.4 26页
  printf("ad_Vref = %0.3f\r\n",ad_Vref);                      //打印校准后的供电电压
  ad_result1 = ad_result[1]*ad_Vref/4095; //读取数组中1数据，存放的ADC通道1连接的设备，采用校准后的电压，计算出模拟数值
  printf("ad_result1 = %0.3f\r\n",ad_result1);                //打印ADC1通道1数据
  ad_result16 = ad_result[2]*ad_Vref/4095; //读取数组中2数据，存放的ADC1通道16内部温度计的模拟值
  temp = (1.43-ad_result16)/0.0043 +25;     //根据模拟值，算出温度，参考手册11.10 168页，数据手册5.3.18 51页
  printf("temp = %0.3f\r\n",temp);                            //打印校准后的内部温度（通道16）
  ad_result2 = ad_result[3]*ad_Vref/4095; //读取数组中3数据，存放的ADC通道2连接的设备，采用校准后的电压，计算出模拟数值
  printf("ad_result2 = %0.3f\r\n",ad_result2);                //打印ADC1通道2数据
  //OLED_ReverseArea(108, 40, 20, 24);
  OLED_Printf(0,20,OLED_6X8,"  Prower:%+5.3f",ad_Vref);
  OLED_ShowChar(98,20,'V',OLED_6X8);
  
  OLED_Printf(0,30,OLED_6X8,"ad1_res1:%+5.3f",ad_result1);
  OLED_ShowChar(98,30,'V',OLED_6X8);
  
  OLED_Printf(0,40,OLED_6X8,"    temp:%+5.3f",temp);
  OLED_ShowImage(98,40,6,8,Celsius);
  
  OLED_Printf(0,50,OLED_6X8,"ad1_res2:%+5.3f",ad_result2);
  OLED_ShowChar(98,50,'V',OLED_6X8);
  OLED_Update();
  
  while(1)
  {
      if(a >= 180)
      {
        b = a-360;//-(360 - a);
      }
      else
      {
        b = a;
      }
      if(flag == 0)
      {
        OLED_DrawArc(118,30,10,0,b,OLED_FILLED);
        OLED_DrawArc(118,52,10,0,b,OLED_FILLED);
        a = a + 5;
        num = num + 5;
      }
      else
      {
        OLED_DrawArc(118,30,10,0,b,OLED_UNFILLED);
        OLED_DrawArc(118,52,10,0,b,OLED_UNFILLED);
         a = a + 1;
        num++;
      }
      //OLED_ReverseArea(108, 40, 20, 24);
      OLED_UpdateArea(60,20,68,64);
      //delay_ms(1000);
    if(a>360)
    { 
      a= 1;
      flag ^= 1; 
      OLED_ClearArea(108,20,50,44);
    }
    
    
    
    
    if(num>50)
    {
      num = 0;
      get_adc1_result();
      ad_Vref = 1.2*4096 /ad_result[0];       //读取数组中0数据，存放的是通道17的1.2V内部参照电压，校准当前供电电压，数据手册5.3.4 26页
      printf("ad_Vref = %0.3f\r\n",ad_Vref);                      //打印校准后的供电电压
      ad_result1 = ad_result[1]*ad_Vref/4095; //读取数组中1数据，存放的ADC通道1连接的设备，采用校准后的电压，计算出模拟数值
      printf("ad_result1 = %0.3f\r\n",ad_result1);                //打印ADC1通道1数据
      ad_result16 = ad_result[2]*ad_Vref/4095; //读取数组中2数据，存放的ADC1通道16内部温度计的模拟值
      temp = (1.43-ad_result16)/0.0043 +25;     //根据模拟值，算出温度，参考手册11.10 168页，数据手册5.3.18 51页
      printf("temp = %0.3f\r\n",temp);                            //打印校准后的内部温度（通道16）
      ad_result2 = ad_result[3]*ad_Vref/4095; //读取数组中3数据，存放的ADC通道2连接的设备，采用校准后的电压，计算出模拟数值
      printf("ad_result2 = %0.3f\r\n",ad_result2);                //打印ADC1通道2数据
      
      OLED_Printf(0,20,OLED_6X8,"  Prower:%+5.3f",ad_Vref);
      OLED_Printf(0,30,OLED_6X8,"ad1_res1:%+5.3f",ad_result1);
      OLED_Printf(0,40,OLED_6X8,"    temp:%+5.3f",temp);
      OLED_ShowImage(98,40,6,8,Celsius);
      OLED_Printf(0,50,OLED_6X8,"ad1_res2:%+5.3f",ad_result2);
      OLED_Update();
    }

  }
}

//int main(void)
//{
// 
//  /**********************字符，字符串，数字显示**********************/
////	OLED_ShowChar(0,0,'A',OLED_8X16);
////  OLED_ShowChar(0,16,'B',OLED_6X8);
////  OLED_ShowString(35,0+16,"Hello World!",OLED_8X16);
////  OLED_ShowString(50,20+16,"Hello World!",OLED_6X8);
////  OLED_ShowNum(10,0,1597,4,OLED_8X16);
////  OLED_ShowSignedNum(55,0,-156,3,OLED_6X8);
////  OLED_ShowHexNum(55+28,0,0XA8C6,4,OLED_6X8);
////  OLED_ShowBinNum(40,8,0XA8C6,16,OLED_6X8);
////  OLED_ShowFloatNum(9,16,-23.141,2,3,OLED_6X8);
//  /**********************中文显示，实际也就是图像显示**********************/
//  OLED_ShowChinese(0,0,"哈喽，世界屏幕");
//  OLED_ShowChinese(0,20,"哈喽，大家好");

////  OLED_ShowImage(0,0,45,30,Battery);
//  /**********************Printf显示**********************/
////  OLED_Printf(0,0,OLED_6X8,"Num1:%+05d,Num2:%d",123%100000,-134);
////  OLED_Printf(0,10,OLED_6X8,"N1:0x%05X,N2:0x%x",0xacab,0xabad);
////  OLED_Printf(0,20,OLED_6X8,"N1:%+5.2f,N2:%+05.3f",15.5575,-134.57);
//  /**********************绘图函数**********************/
////  OLED_DrawPoint(15,20);//点亮一个点
////  if( OLED_GetPoint(50,20))//检查某一个像素点是否点亮，如果点亮返回1，否则返回0
////  {
////    OLED_ShowString(20,30,"Point1:YES",OLED_8X16);
////  }
////  else
////  {
////    OLED_ShowString(20,30,"Point1:NO",OLED_8X16);
////  }
////  
////  if( OLED_GetPoint(15,20))
////  {
////    OLED_ShowString(20,48,"Point2:YES",OLED_8X16);
////  }
////  else
////  {
////    OLED_ShowString(20,48,"Point2:NO",OLED_8X16);
////  }
////  OLED_DrawLine(10,2,50,30);
////  OLED_DrawRectangle(0,0,10,40,OLED_FILLED);//起始点x，起始点y，矩形宽x，矩形长y，是否填充
////  OLED_DrawRectangle(10,20,40,44,OLED_UNFILLED);
//  //OLED_DrawArc(30,30,20,90,-20,OLED_UNFILLED);
//  //OLED_DrawCircle(80,30,30,OLED_UNFILLED);
////  OLED_DrawArc(80,30,30,0,0,OLED_UNFILLED);
////  OLED_UpdateArea(50,0,61,60);
//  OLED_Update();
//	while (1)
//	{
////    OLED_DrawRectangle(10,20,40,44,OLED_UNFILLED);
////		for(a = 1;a<360;a = a+2)
////    {
////      if(a >= 180)
////      {
////        b = -(360-a);
////      }
////      else
////      {
////        b = a;
////      }
////      if(flag == 0)
////        OLED_DrawArc(80,30,30,0,b,OLED_FILLED);
////      else
////        OLED_DrawArc(80,30,30,0,b,OLED_UNFILLED);
////      OLED_UpdateArea(50,0,61,61);
////      OLED_Update();
////      //delay_ms(1);
////    }
////    flag ^= 1; 
////    OLED_ClearArea(50,0,61,61);
//	}
//}

