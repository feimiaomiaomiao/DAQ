#ifndef __DELAY_H__
#define __DELAY_H__

#include <stm32f10x.h>
#include "sys.h"


void SysTick_Init(uint16_t sysclk);/* 滴答定时器初始化 */
void delay_us(uint32_t us);
void delay_ms(uint16_t ms);
void delay_s(uint16_t s);
void delay(uint32_t mss);


#endif
