#include "sys.h"  
#include "stdio.h"
#include "nvic.h"
#include "delay.h"
#include "led.h"
#include "w25q128.h"

/******************************************�����ж�******************************************/
/******************************************************************
*��������  ������1���жϷ�����
*������    ��USART1_IRQHandler
*��������  ��void
*��������ֵ��void
*����      ��
*******************************************************************/
void USART1_IRQHandler(void)
{
  u8 data;
  static u32 addr = 0x0b0000;
  
  //�ж�����ʲô��ʽ�����ж�
	//��������ж�    SET=1   RESET=0
  if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)
  {
    //�ж�Ҫִ�е�����
    data = USART1->DR;
    W25Q128_Page_Write(addr,1,&data);
    addr++;
    LED1_ON;
  }
	//�����ж�        SET=1   RESET=0
  if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)   
  {
		//�����־
    USART1->SR;
    USART1->DR;
    
    LED1_OFF;
  }
}

/******************************************�ⲿ�ж�******************************************/
/******************************************************************
*��������  ���ⲿ�ж�4������
*������    ��EXTI0_IRQHandler
*��������  ��void
*��������ֵ��void
*����      ��
*******************************************************************/
void EXTI4_IRQHandler(void)
{
	//�ж��ⲿ�ж���0�Ƿ���Ĵ���   SET=1   RESET=0
  if(EXTI_GetFlagStatus(EXTI_Line4) == SET)
  {
    delay_ms(10);
		//�˲�
    if(!(GPIOE->IDR & (1<<4)))
    {
      EXTI->PR |= (1<<4);
      GPIOB->ODR ^= (1<<5);
    }
  }
}

/******************************************************************
*��������  ������ⲿ�жϷ���ʼ��
*������    ��EXTI9_5_IRQHandler
*��������  ��void
*��������ֵ��void
*����      ��
             ����˿�
*******************************************************************/
void EXTI9_5_IRQHandler(void)
{
	//�ж��ⲿ�ж���5�Ƿ���Ĵ���   SET=1   RESET=0
  if(EXTI_GetFlagStatus(EXTI_Line5) == SET)
  {
    EXTI->PR |= (1<<5);
		GPIOB->ODR ^= (1<<5);
  }
}




