#ifndef _USART_H
#define _USART_H

  #include "stm32f4xx.h" 
	
  void USART1_Init(u32 bps);
  void USART1_Send_Byte(u8 data);
  u8 USART1_Receive_Byte(void);

#endif











