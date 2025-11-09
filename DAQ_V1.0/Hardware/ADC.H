#ifndef __ADC_H__
#define __ADC_H__

#include <stm32f10x.h>
#define ADC1_DR_Address  ((uint32_t)0x4001244C)
extern uint16_t ad_result[4];
extern void adc_init(void);
extern void get_adc1_result(void);
extern uint16_t get_adc2_result(void);

  
#endif
