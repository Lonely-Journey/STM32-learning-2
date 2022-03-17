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

/******************************************
*��������  ����ʱ��6���жϷ�����
*������    ��TIM6_DAC_IRQHandler ��
*��������  ����
*��������ֵ����
*��������  ��
*********************************************/
void TIM7_IRQHandler(void)
{
	static u8 temp = 0;
	static u16 pwm = 500;
	
	//�ж��Ƿ�ʱ��7�ж�
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET)
	{
		//����жϱ�־λ
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update); 
		//�����¼�
		if(temp == 0)
    {
      if(pwm == 0) temp = 1;
      else pwm -=50;
			TIM_SetCompare1(TIM10,pwm);
    }
    if(temp == 1)
    {
      if(pwm == 1000) temp = 0;
      else pwm +=50;
			TIM_SetCompare1(TIM10,pwm);
    }
	}
}

/******************************************
*��������  ����ʱ��5���жϷ�����
*������    ��TIM5_IRQHandler
*��������  ����
*��������ֵ����
*��������  ��
*********************************************/
void TIM5_IRQHandler(void)
{
  static u16 update_num;
  static u8 update_flay = 0;
  static u16 rising_edge; 
  u16 falling_edge;
  u16 cont; 
  u16 t;
	//�����ж�  
	if(TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET)
	{
		//���жϱ�־λ
		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
		//�����¼�
    if(update_flay)
    {
      update_num++;
    }
	}
  //���ش����ж�
  if(TIM_GetITStatus(TIM5,TIM_IT_CC1) != RESET)
  {
		//���жϱ�־λ
		TIM_ClearITPendingBit(TIM5,TIM_IT_CC1);
		
    //�ж��ǲ��������ش���(ֱ��ͨ���Ĵ����ж�)  0Ϊ������  1Ϊ�½���
    if(!(TIM5->CCER & (1<<1)))
    {
      //�������������־λ
      update_flay = 1;
      //��ȡ�����ؼ���(TIMx->CCR1�Ĵ���)
      rising_edge = TIM_GetCapture1(TIM5);
      //�ѱ��ش�����ʽ��Ϊ�½���(TIMx->CCER�Ĵ����ĵ�1-2λ)
      TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);	
    }
    else if(TIM5->CCER & (1<<1))
    {
      //��ȡ�����ؼ���(TIMx->CCR1�Ĵ���)
      falling_edge = TIM_GetCapture1(TIM5);
      //����������
			cont = 65535 * update_num - rising_edge + falling_edge;
      t = cont / 10;
      
      printf("�����ȣ�%d\r\n",t);
      
      update_num = 0;
      update_flay = 0;
      
      //�л���������(TIMx->CCER�Ĵ����ĵ�1-2λ)
			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);
    }
  }
}







