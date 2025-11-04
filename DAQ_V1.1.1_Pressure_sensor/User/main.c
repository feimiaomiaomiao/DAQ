/*
ADC+DMA多通道数据采集，显示在OLED上，并发送到串口上。

***暂时处理-表面修复：在画圆圈时，圆圈快满的时候会造成数据刷显示停顿降低显示刷新频率，待深究修复底层问题
***BUG修复：修复了偶尔，通电重启时，OLED无法正常显示问题

******压力传感器特化程序，压力传感器变化范围为40K-无穷，取值范围0-2925重新映射为0-4095


日期：2025年11月4日
版本号：V1.1.1

ADC1多通道（通道1，2，16，17）采集数据，通过DMA1转换为数组ad_result[4]中
其中0存放通道17数据，1存放通道1数据，2存放通道16数据，3存放通道2数据

对于本芯片STM32F103C8T6(LQFP48)
DMA1通道1配置，能且只能通道1 参考手册10.3.7  147页 
(DMA1的通道和ADC1的通道不是一个概念，不要混淆)

1，打开ADC1和DMA1和对应GPIO引脚的时钟
2，配置对应GPIO引脚为模拟输入（ADC1用的通道1，通道2，
      内部通道16（温度传感器）内部通道17（Vrefint）所以打开PA1,PA2即可）
3，配置DMA1，目的是实现ADC1->DR寄存器里面的数据搬到RAM【ad1_result】数组中
4，打开DMA1使能
5，配置ADC1多通道单次模式（必须为单次模式，因为要刷新OLED，连续采集会破坏SPI，IIC时序，导致显示错误）

6，配置ADC1采样周期
   其中采样列表，通道17在第一位，获取了基准电压后，修正后续的其他通道数据）
   采样列表：1-ADC1通道17，2-ADC1通道1，3-ADC1通道16，4-ADC1通道2
7，使能ADC1的DMA功能
8，********使能ADC1内部通道************
9，使能ADC1
10，ADC1校准
11，启动ADC1转换

之后CPU不用做任何操作，因为数据会自动存在ad1_result数组中对应的位置
显示部分：
【初始化OLED
在最前面显示项目名称，制作人名称于版本号
在尾部显示动态画圆图；
采集的数据显示在OLED上的同时，发送到串口上】


扫描模式时，会持续转换把列表内的通道数据存在ADC->DR寄存器里面，但是只有列表最后才会产生EOC转换结束标志位。
这样会导致ADC->DR寄存器里面的数据会被覆盖
但是每次转换一个通道结束后，会产生一个DMA请求，这样可以调用DMA去转存数据


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
  OLED_ShowString(90,8,"V1.1",OLED_6X8);
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
//  OLED_Printf(0,0,OLED_6X8,"Num1:%+05d,Num2:%d",123%100000,-134);
//  OLED_Printf(0,10,OLED_6X8,"N1:0x%05X,N2:0x%x",0xacab,0xabad);
//  OLED_Printf(0,20,OLED_6X8,"N1:%+5.2f,N2:%+05.3f",15.5575,-134.57);
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

