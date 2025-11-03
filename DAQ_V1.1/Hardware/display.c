#include "display.h"

uint8_t i = 0;
float ad_result1,ad_result16,ad_result2,temp;
float ad_Vref;
int16_t a = 1, b = 0,bn= 0;
uint8_t flag = 0,num = 0;



void Data_Shows(void)
{
    if(a >= 180)
    {
      b = a - 360;  //-(360 - a);
      bn = 360 - a;
    }
    else
    {
      b = a;
      bn = -a;
    }
    if(flag == 0)
    {
      OLED_DrawArc(117,28,10,0,b,OLED_FILLED);
      OLED_DrawArc(117,53,10,bn,0,OLED_UNFILLED);
      a = a + 5;
    }
    else
    {
      OLED_DrawArc(117,28,10,0,b,OLED_UNFILLED);
      OLED_DrawArc(117,53,10,bn,0,OLED_FILLED);
      a = a + 5;
    }

    //OLED_UpdateArea(60,18,68,46);
   num++;
    if(a>=360)
    { 
      a= 1;
      
      flag ^= 1; 
      OLED_ClearArea(107,18,50,46);
    }
    if(num>5)
    {
      num = 0;
      ad_Vref = 1.2*4096 /ad_result[0];       //读取数组中0数据，存放的是通道17的1.2V内部参照电压，校准当前供电电压，数据手册5.3.4 26页
      printf("ad_Vref = %0.3f\r\n",ad_Vref);                      //打印校准后的供电电压
      ad_result1 = ad_result[1]*ad_Vref/4095; //读取数组中1数据，存放的ADC通道1连接的设备，采用校准后的电压，计算出模拟数值
      printf("ad_result1 = %0.3f\r\n",ad_result1);                //打印ADC1通道1数据
      ad_result16 = ad_result[2]*ad_Vref/4095; //读取数组中2数据，存放的ADC1通道16内部温度计的模拟值
      temp = (1.43-ad_result16)/0.0043 +25;     //根据模拟值，算出温度，参考手册11.10 168页，数据手册5.3.18 51页
      printf("temp = %0.3f\r\n",temp);                            //打印校准后的内部温度（通道16）
      ad_result2 = ad_result[3]*ad_Vref/4095; //读取数组中3数据，存放的ADC通道2连接的设备，采用校准后的电压，计算出模拟数值
      printf("ad_result2 = %0.3f\r\n",ad_result2);                //打印ADC1通道2数据
      
      OLED_Printf(0,20,OLED_6X8,"Prower  :%+5.3f",ad_Vref);
      OLED_Printf(0,30,OLED_6X8,"ad1_res1:%+5.3f",ad_result1);
      OLED_Printf(0,40,OLED_6X8,"temp    :%+5.3f",temp);
      OLED_Printf(0,50,OLED_6X8,"ad1_res2:%+5.3f",ad_result2);
    }
    OLED_Update();
    
}
void Data_Collection(void)
{
  get_adc1_result();
}