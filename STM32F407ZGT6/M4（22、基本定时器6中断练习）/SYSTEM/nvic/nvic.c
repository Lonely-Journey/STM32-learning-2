#include "stm32f4xx.h"  
#include "stdio.h"
#include "nvic.h"

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
  static u8 i = 0;
  static u8 str[200];
  
  //�ж�����ʲô��ʽ�����ж�
	//��������ж�    SET=1   RESET=0
  if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)
  {
    //ִ���ж�����
    str[i] = USART_ReceiveData(USART1); //��ȡһ���ֽڵ�����(USART1->DR)
    i++;
  }
	//�����ж�        SET=1   RESET=0
  if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)   
  {
		//�����־
    USART1->SR;
    USART1->DR;
    
    str[i] = '\0';
    printf("�㷢�͵������ǣ�%s\r\n",str);
    i = 0;
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


/******************************************ADC******************************************/
/******************************************************************
*��������  ��ADC�жϷ�����
*������    ��ADC_IRQHandler
*��������  ��void
*��������ֵ��void
*����      ��
*******************************************************************/
void ADC_IRQHandler(void)
{
  u16 data;
  if(ADC_GetITStatus(ADC1,ADC_IT_EOC) == SET)
  { 
    //��ȡת���������(ADC1->DR)
		data = ADC_GetConversionValue(ADC1);
    printf("data:%d\r\n",data);
  }
	//�����־
	ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
  //��������ͨ��ת������(ADC1->CR2�Ĵ����ĵ�30λ)
	ADC_SoftwareStartConv(ADC1);
}

/******************************************TIMx��ʱ��******************************************/
/******************************************
*��������  ����ʱ��6���жϷ�����
*������    ��TIM6_DAC_IRQHandler ��
*��������  ����
*��������ֵ����
*��������  ��
*********************************************/
void TIM6_DAC_IRQHandler(void)
{
	//�ж��Ƿ�ʱ��6�ж�
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET)
	{
		//����жϱ�־λ
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update); 
		//�����¼�
		GPIOF->ODR ^= ((1<<6)|(1<<9)|(1<<10));
	}
}

