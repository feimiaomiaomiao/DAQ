#ifndef __KEY_H__
#define __KEY_H__

#include <stm32f10x.h>
#include "delay.h"

void key_init(void);
uint8_t key_scan(void);


#endif
