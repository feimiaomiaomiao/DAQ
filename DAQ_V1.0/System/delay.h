#ifndef __DELAY_H__
#define __DELAY_H__

#include <stm32f10x.h>

void delay_us(uint16_t us);
void delay_ms(uint16_t ms);
void delay_s(uint16_t s);
void delay(uint32_t mss);
#endif