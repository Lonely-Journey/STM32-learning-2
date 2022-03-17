#include "sys.h"

/*****************************************************
*��������  ���Զ�ʱ��6���г�ʼ������
*������    ��TIM6_Init
*��������  ��void
*��������ֵ��u16 arr,u16 psc  
*����      ��
********************************************************/
void TIM6_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;  //TIMx���ýṹ��
  NVIC_InitTypeDef NVIC_InitStruct;                //�ж����ýṹ��
  
  //TIMxʱ��ʹ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	/*TIMx��ʼ������*/
  TIM_TimeBaseInitStruct.TIM_Period        = arr;                 //����ֵ(TIMx_ARR�Ĵ���)
  TIM_TimeBaseInitStruct.TIM_Prescaler     = psc;                 //��Ƶ(TIMx_PSC�Ĵ���)
  //TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;      //TDTS = Tck_tim(TIMx_CR1�Ĵ����ĵ�9-8λ)(������ʱ��û��)
  //TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//���ϼ���(TIMx_CR1�Ĵ����ĵ�4λ)(������ʱ��ֻ����)
  TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);                 //��ʼ��TIM6
	
	/*NVIC����*/
  //�����ж�����
  NVIC_InitStruct.NVIC_IRQChannel = TIM6_IRQn;             //ѡ��TIM6�ж�
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�3
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;		       //�����ȼ�3
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 		         //IRQͨ��ʹ��
  NVIC_Init(&NVIC_InitStruct);                             //TIM6�жϳ�ʼ��
	
	//����TIMx�����ж�
  TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE );
	
	//ʹ����װ��Ӱ�ӼĴ�������
	TIM_ARRPreloadConfig(TIM6, ENABLE);			// ʹ��TIM6���ؼĴ���ARR
	
  //ʹ��TIMx
  TIM_Cmd(TIM6, ENABLE);
}

/*****************************************************
*��������  ���Զ�ʱ��7���г�ʼ������
*������    ��TIM7_Init
*��������  ��void
*��������ֵ��u16 arr,u16 psc  
*����      ��
********************************************************/
void TIM7_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;  //TIMx���ýṹ��
  NVIC_InitTypeDef NVIC_InitStruct;                //�ж����ýṹ��
  
  //TIMxʱ��ʹ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
	/*TIMx��ʼ������*/
  TIM_TimeBaseInitStruct.TIM_Period        = arr;                 //����ֵ(TIMx_ARR�Ĵ���)
  TIM_TimeBaseInitStruct.TIM_Prescaler     = psc;                 //��Ƶ(TIMx_PSC�Ĵ���)
  //TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;      //TDTS = Tck_tim(TIMx_CR1�Ĵ����ĵ�9-8λ)(������ʱ��û��)
  //TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//���ϼ���(TIMx_CR1�Ĵ����ĵ�4λ)(������ʱ��ֻ����)
  TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStruct);                 //��ʼ��TIM7
	
	/*NVIC����*/
  //�����ж�����
  NVIC_InitStruct.NVIC_IRQChannel = TIM7_IRQn;             //ѡ��TIM7�ж�
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�3
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;		       //�����ȼ�3
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 		         //IRQͨ��ʹ��
  NVIC_Init(&NVIC_InitStruct);                             //TIM7�жϳ�ʼ��
	
	//����TIMx�����ж�
  TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE );
	
	//ʹ����װ��Ӱ�ӼĴ�������
	TIM_ARRPreloadConfig(TIM7, ENABLE);			// ʹ��TIM7���ؼĴ���ARR
	
  //ʹ��TIMx
  TIM_Cmd(TIM7, ENABLE);
}








