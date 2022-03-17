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

/******************************************ADC�ж�******************************************/
/******************************************************************
*��������  ��ADC3�жϷ�����
*������    ��ADC3_IRQHandler
*��������  ��void
*��������ֵ��void
*����      ��
*******************************************************************/
void ADC3_IRQHandler(void)
{
	u16 data;
	
	//�Ƿ�ת�����
	if(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == SET)
	{
		//��ȡת���������(ADC_DR�Ĵ���)
		data = ADC_GetConversionValue(ADC3);
		printf("data:%d\r\n",data);
	}
	//�����־
	ADC_ClearITPendingBit(ADC3,ADC_IT_EOC);
	//��������ͨ��ת������(ADC_CR2�Ĵ�����22λ)
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);	
}



/******************************************��ʱ���ж�******************************************/
/******************************************
*��������  ����ʱ��6���жϷ�����
*������    ��TIM6_DAC_IRQHandler ��
*��������  ����
*��������ֵ����
*��������  ��
*********************************************/
void TIM6_IRQHandler(void)
{
	//���TIM6�����жϷ������
  if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
  { 
    //�ж��¼�
    GPIOE->ODR ^= (1<<5);
  }
	//���TIMx�����жϱ�־ 
  TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
}
