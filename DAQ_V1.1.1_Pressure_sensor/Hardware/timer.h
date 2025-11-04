#ifndef __TIMER_H__
#define __TIMER_H__

#include <stm32f10x.h>
#include "sys.h"

void time1_init(void);
void time3_init(void);
void time2_init(void);
void time4_init(void);
extern uint8_t Refresh_Data_Flag;

#endif