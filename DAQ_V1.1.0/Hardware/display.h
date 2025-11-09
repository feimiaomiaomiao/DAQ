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
void Data_Shows(void);
void Data_Collection(void);



#endif
