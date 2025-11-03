#include "delay.h"
#include "sys.h"
//1秒72 000 000  1us就是72次计数
void delay_us(uint16_t us)
{
  SysTick->CTRL = 0; // Disable SysTick
  SysTick->LOAD = 72*us-1; // Count from 255 to 0 (256 cycles)
  SysTick->VAL = 0; // Clear current value as well as count flag
  SysTick->CTRL = 5; // Enable SysTick timer with processor clock
  while ((SysTick->CTRL & 0x00010000)==0);// Wait until count flag is set
  SysTick->CTRL = 0; // Disable SysTick
}

//1秒72 000 000  1ms就是72000次计数
void delay_ms(uint16_t ms)
{
  while(ms--)
  {
    SysTick->CTRL = 0; // Disable SysTick
    SysTick->LOAD = 72000-1; // Count from 255 to 0 (256 cycles)
    SysTick->VAL = 0; // Clear current value as well as count flag
    SysTick->CTRL = 5; // Enable SysTick timer with processor clock
    while ((SysTick->CTRL & 0x00010000)==0);// Wait until count flag is set
    SysTick->CTRL = 0; // Disable SysTick
  }
}

//1秒72 000 000  
void delay_s(uint16_t s)
{
  while(s--)
  {
    delay_ms(1000);
  }
}
void delay(uint32_t mss)
{
  while(mss--);
}