#include "stm32f4xx.h"  
#include "stdio.h"
#include "nvic.h"
#include "w25q64.h"
#include "led.h"

static void delay_ms(u32 ms)
{
	u32 i = 168 / 4 * 1000 * ms;
	while(i)
	{
		i--;
	}
}

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
    //ִ���ж�����
    data = USART_ReceiveData(USART1); //��ȡһ���ֽڵ�����(USART1->DR)
		W25Q64_Page_Write(addr,1,&data);
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
*��������  ���ⲿ�ж�1������
*������    ��EXTI0_IRQHandler
*��������  ��void
*��������ֵ��void
*����      ��
*******************************************************************/
void EXTI0_IRQHandler(void)
{
	//�ж��ⲿ�ж���0�Ƿ���Ĵ���   SET=1   RESET=0
  if(EXTI_GetFlagStatus(EXTI_Line0) == SET)
  {
    delay_ms(10);
		//�˲�
    if((GPIOA->IDR & (1<<0)))
    {
      EXTI->PR |= (1<<0);
      GPIOF->ODR ^= (1<<6);
      GPIOF->ODR ^= (1<<9);
      GPIOF->ODR ^= (1<<10);
      GPIOC->ODR ^= (1<<0);
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
    GPIOF->ODR ^= (1<<6);
    GPIOF->ODR ^= (1<<9);
    GPIOF->ODR ^= (1<<10);
    GPIOC->ODR ^= (1<<0);
  }
}







