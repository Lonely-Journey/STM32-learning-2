#include "stm32f4xx.h"  

/*****************************************************
*��������  ���Զ�ʱ��6���г�ʼ������
*������    ��TIM6_Init
*��������  ��void
*��������ֵ��u16 arr,u16 psc  
*����      ��
********************************************************/
void TIM6_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx���ýṹ��
	NVIC_InitTypeDef   NVIC_InitStruct;                  //NVIC(�ж�)���ýṹ��
	
	//ʱ��ʹ��  TIM6
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);  
	//TIMx����
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //�Զ���װ��ֵ(TIMx->ARR�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //��ʱ����Ƶ(TIMx->PSC�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ(TIMx->CR1�Ĵ����ĵ�4λ)(������ʱ��ֻ�е�������)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ:tDTS=tCK_INT(TIMx->CR1�Ĵ����ĵ�8-9λ)(ע�⣺������ʱ��û�����)	
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);            //��ʼ��TIM6
	//TIMx�ж�ѡ��ʹ��
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //����ʱ��6�����ж�
	
	//TIM6�ж�����-----NVIC ����
  NVIC_InitStruct.NVIC_IRQChannel = TIM6_DAC_IRQn;    //TIM6�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =3;		  //�����ȼ�3
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			  //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStruct);	                      //����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//ʹ����װ��Ӱ�ӼĴ�������
	TIM_ARRPreloadConfig(TIM6, ENABLE);			            // ʹ��TIM6���ؼĴ���ARR
	
	TIM_Cmd(TIM6,ENABLE); //ʹ�ܶ�ʱ��6
}

/******************************************************
*��������  ���Ի�����ʱ��7���г�ʼ������
*������    ��TIM7_Init
*��������  ��void
*��������ֵ��u16 arr,u16 psc  
*����      ��
********************************************************/
void TIM7_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx���ýṹ��
	NVIC_InitTypeDef   NVIC_InitStruct;                  //NVIC(�ж�)���ýṹ��
	
	//ʱ��ʹ��  TIM7
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  
	//TIMx����
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //�Զ���װ��ֵ(TIMx->ARR�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //��ʱ����Ƶ(TIMx->PSC�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ(TIMx->CR1�Ĵ����ĵ�4λ)(������ʱ��ֻ�е�������)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ:tDTS=tCK_INT(TIMx->CR1�Ĵ����ĵ�8-9λ)(ע�⣺������ʱ��û�����)	
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStruct);            //��ʼ��TIM7
	//TIMx�ж�ѡ��ʹ��
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //����ʱ��7�����ж�
	
	//TIM6�ж�����-----NVIC ����
  NVIC_InitStruct.NVIC_IRQChannel = TIM7_IRQn;        //TIM7�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =3;		  //�����ȼ�3
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			  //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStruct);	                      //����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//ʹ����װ��Ӱ�ӼĴ�������
	TIM_ARRPreloadConfig(TIM7, ENABLE);			            // ʹ��TIM7���ؼĴ���ARR
	
	TIM_Cmd(TIM7,ENABLE); //ʹ�ܶ�ʱ��7
}







