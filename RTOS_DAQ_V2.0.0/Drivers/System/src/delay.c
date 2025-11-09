#include "delay.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

//static uint16_t  g_fac_us = 0;      /* us延时倍乘数 */



/**********************************************************
* @funcName ：SysTick_Init
* @brief    ：滴答定时器初始化
* @param    ：uint16_t sysclk (时钟频率Mhz)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void SysTick_Init(uint16_t sysclk)
{
    uint32_t reload;

    SysTick->CTRL = 0;                          /* 清Systick状态，以便下一步重设，如果这里开了中断会关闭其中断 */
    SysTick->CTRL &= ~(1<<2);                   /* 选择时钟源   写0是外部时钟源   写1是内部时钟源 */

   // g_fac_us = sysclk / 8;                    /* 不论是否使用OS,g_fac_us都需要使用,作为1us的基础时基 */

    reload = sysclk / 8;                        /* 每秒钟的计数次数 单位为M */
    reload *= 1000000 / configTICK_RATE_HZ;     /* 根据delay_ostickspersec设定溢出时间
                                                * reload为24位寄存器,最大值:16777216,在9M下,约合1.86s左右
                                                */
    SysTick->CTRL |= 1 << 1;                    /* 开启SYSTICK中断 */
    SysTick->LOAD = reload;                     /* 每1/delay_ostickspersec秒中断一次 */
    SysTick->CTRL |= 1 << 0;                    /* 开启SYSTICK */
}

//1秒72 000 000  1us就是72次计数
void delay_us(uint16_t us)
{
  SysTick->CTRL = 0;                        // 禁止SysTick
  SysTick->LOAD = 72*us-1;                  // Count from 255 to 0 (256 cycles)
  SysTick->VAL = 0;                         // Clear current value as well as count flag
  SysTick->CTRL = 5;                        // Enable SysTick timer with processor clock
  while ((SysTick->CTRL & 0x00010000)==0);  // Wait until count flag is set
  SysTick->CTRL = 0;                        // Disable SysTick
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

/**********************************************************
* @funcName ：SysTick_Handler
* @brief    ：systick中断服务函数
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
extern void xPortSysTickHandler(void);

void SysTick_Handler(void)
{
    if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        xPortSysTickHandler();
    }
}

