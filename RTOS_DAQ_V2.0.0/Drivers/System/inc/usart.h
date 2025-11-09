#ifndef __USART_H__
#define __USART_H__

#include <stm32f10x.h>
#include <stdio.h>
#include "led.h"



void Uart_Init(uint32_t baud);
void send_byte(uint8_t byte);
void Send_String(char *str);
void Send_Buf(uint8_t *buf,uint16_t len);
void Send_pack(void);


#endif
