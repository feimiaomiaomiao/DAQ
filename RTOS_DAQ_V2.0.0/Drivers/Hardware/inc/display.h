#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <stm32f10x.h>
#include "OLED.h"
#include "ADC.H"
#include <stdio.h>

extern float ad_result1,ad_result16,ad_result2,temp;
extern float ad_Vref;
extern int16_t a, b,bn;
extern uint8_t flag,num;

void DynamicImage_Shows(void);//动态图显示
void Data_Calculate(void);//数据计算
void Print_Data(void);//数据打印
void Data_Collection(void);//数据收集



#endif
