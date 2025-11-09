#include "usart.h"

uint8_t Txd_buf[7] = {1,2,3,4,'a','$','B'};
uint8_t Rxd_buf[10]; //接受数据缓冲区
uint8_t Rxd_flag = 0 ; //接受标志
uint8_t Rxd_index = 0;//接受字节索引


void Uart_Init(uint32_t baud)
{
   USART_InitTypeDef USART_InitStructure;//声明结构体变量
   GPIO_InitTypeDef GPIO_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure;
  /*********************************************************************************/
   /* Enable GPIO clock 
  打开A口时钟，打开AFIO时钟，因为A9,A10要作为串口来用，不是普通IO口
  */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);

  /* Enable USARTy Clock 
  打开串口时钟*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
  
  /*********************************************************************************/
  
  /* Configure USARTy Rx as input floating 
  配置RXD PA10 浮空输入模式*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Configure USARTy Tx as alternate function push-pull 
  配置RXD PA9 复用推挽输出模式*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  
/*********************************************************************************/
  /* USARTy and USARTz configuration ------------------------------------------------------*/
  /* USARTy and USARTz configured as follow:
        - BaudRate = 230400 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - Even parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = baud;//配置波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//选择8位数据位，
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//1位停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无需硬件流控
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//选择全双工，还是半双工。此选全双工，既可以发送又可以接受
  
  /* Configure USART1 
  初始化USART1*/
  USART_Init(USART1, &USART_InitStructure);


/*********************************************************************************/
  /* Enable the USARTy Interrupt 使能串口中断
  配置中断向量入口，配置中断优先级*/
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//响应优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能中断通道
  NVIC_Init(&NVIC_InitStructure);



  /* Enable USARTy Receive and Transmit interrupts 
  使能串口1接受中断
  固件库552页，表722*/
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);


  /* Enable the USARTy 
  使能串口工作*/
  USART_Cmd(USART1, ENABLE);
}


void send_byte(uint8_t byte)
{
  /* Send one byte from USARTy to USARTz */
  USART_SendData(USART1, byte);
  /* Loop until USARTy DR register is empty 
  等待发送DR寄存器为空（完成）*/ 
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  
}


void Send_String(char *str)
{
  while(*str != '\0' )
  {
    send_byte(*str++);
  }
}


void Send_Buf(uint8_t *buf,uint16_t len)
{
  uint16_t i = 0;
  for(i = 0;i<len;i++)
  {
    send_byte(buf[i]);
  }
}

int fputc(int ch, FILE *f)
{
  (void)f;
  send_byte(ch);
  return (ch);
}

//发送数据包
void Send_pack(void)
{
  send_byte(0xfe);          //帧头
  Send_Buf(Txd_buf,8);
  send_byte(0xef);          //帧尾
}

//串口1中断服务函数
void USART1_IRQHandler()
{
  uint16_t recv_dat = 0;
  
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//接受中断
  {
    recv_dat = USART_ReceiveData(USART1);//从UASRT->DR寄存器中读取
    switch(recv_dat)
    {
      case 0x01:  
        PBout(5) ^=1;
        break;
      case 'B'://实际要接受文本的B也就是ASCII码的B。或者直接HEX模式发生0x42也是对应ASCII的B字符
        PBout(4) ^=1;
        break;
      case 0x03:
        PBout(3) ^=1;
        break;
      default :
        break;
    }
    USART_ClearITPendingBit(USART1, USART_IT_RXNE );
  }
}
