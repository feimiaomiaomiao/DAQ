/*
******************************************************************************
程序简介:
ADC+DMA多通道数据采集，显示在OLED上，并通过串口1发送到串口上。
V2.0.x->RTOS基础搭建版本

V2.2.0
把采集和显示整合为一个系统，相互关联
1.配置DMA1的中断，当DMA1完成信号转换会打开中断，在DMA1的中断中释放二值信号量
2.在RTOS任务中判断信号量是否被释放，如果信号量被释放，则启动刷新程序
3.0.96英寸屏幕换成1.3英寸屏幕，显示面积增加

V2.0.1
修复BUG：调用延时函数时，意外关闭滴答计时器，导致RTOS系统死机

V2.0.0
***结构调整：标准库的支持，从RTE方式改为手动搭建标准库

***取消定时器切换显示和数据采集，利用RTOS去执行


***BUG：可能会出现读取和写入互相影响。
***可优化：使用队列

***BUG2：图片动态显示单度拉取出去会遇到打乱显示问题
***原因：（猜测）多了一步更新显示的操作，导致显示的数据队列被打断

人员：FT
日期：2025年11月10日
版本号：V2.0.1
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
#include "usart.h"
#include "OLED.h"
#include "OLED_Data.h"
#include "display.h"
#include "ADC.H"
#include "FreeRTOS.h"
#include "freeRTOS_adc.h"

int main(void)
{
  
  SysTick_Init(72);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//中断优先级分组
  delay_ms(500);
  led_init();           //LED初始化
  OLED_Init();          //OLED初始化三步走
  OLED_Clear();         
  OLED_Update();       
  adc_init();           //初始ADC-DMA

  Uart_Init(115200);
    
  Send_String("ADC+DMA+ROTS+OLED+UART \r\n");
  printf("版本V2.2.0\r\n");
    
  OLED_DrawTriangle(0, 10, 5, 5, 10, 10, OLED_FILLED);
  OLED_ShowString(10,0,"DAQ_4",OLED_8X16);
  OLED_ShowChinese(55,0,"嘀嘀");
  OLED_ShowString(90,8,"V2.2.0",OLED_6X8);
  
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

  freeRTOS_adc();        /* freeRTOS实验 */
}

