#include "stm32f4xx.h"  
#include "stdio.h"

/*****************************************************
*��������  ����ͨ�ö�ʱ��5���г�ʼ�����ò����������ź�
*������    ��TIM5_Capture_Init
*��������  ��void
*��������ֵ��u16 arr,u16 psc,u16 ccr2
*����      ��ͨ�����������������㰴��ʱ��
*            ��������� �� PA0   TIM5_CH1  ����ͨ��1
********************************************************/
void TIM5_Capture_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef   GPIO_InitStruc;                   //GPIOx���ýṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx���ýṹ��
	TIM_ICInitTypeDef  TIM_ICInitStruct;                 //����ͨ�����ýṹ��
	NVIC_InitTypeDef   NVIC_InitStruct;                  //�ж����ýṹ��
	
  //ʱ��ʹ��   GPIOA   TIM5
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  //ʹ��TIM5ʱ��
	
	/*GPIO������*/
	//PA0
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_0;               //ѡ��PA0
	GPIO_InitStruc.GPIO_Mode = GPIO_Mode_AF;            //���ù���
	GPIO_InitStruc.GPIO_PuPd = GPIO_PuPd_NOPULL;        //����������
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_100MHz;     	//�ٶ�100MHz
	GPIO_Init(GPIOA,&GPIO_InitStruc);                   //�����趨������ʼ��PA0
	
	//TIM5��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);  //PA0 ����ΪTIM5
	
	//TIMx����
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //�Զ���װ��ֵ(TIMx->ARR�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //��ʱ����Ƶ(TIMx->PSC�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ(TIMx->CR1�Ĵ����ĵ�4λ)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ:tDTS=tCK_INT(TIMx->CR1�Ĵ����ĵ�8-9λ)
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStruct);            //��ʼ��TIM5
	
	//����ͨ������
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;                //ѡ�������ͨ��
  TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;	   //�����ز���(TIMx->CCER�Ĵ����ĵ�1λ)
  TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��(TIMx->CCMR1�Ĵ���������ģʽ�ĵ�0-1λ)
  TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;	         //���������Ƶ������Ƶ(TIMx->CCMR1�Ĵ���������ģʽ�ĵ�2-3λ)
  TIM_ICInitStruct.TIM_ICFilter = 0x00;                        //���������˲��������˲�(TIMx->CCMR1�Ĵ���������ģʽ�ĵ�4-7λ)
  TIM_ICInit(TIM5, &TIM_ICInitStruct);                         //����ͨ�����ó�ʼ��
	
	//��ʱ���ж���ʽѡ��ʹ��
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);     //��������ж�
	TIM_ITConfig(TIM5,TIM_IT_CC1,ENABLE);        //����CC1IE�����ж�	
	
	//TIM5�ж�����-----NVIC ����
  NVIC_InitStruct.NVIC_IRQChannel = TIM5_IRQn;        //TIM5�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =3;		  //�����ȼ�3
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			  //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStruct);	                      //����ָ���Ĳ�����ʼ��VIC�Ĵ���
 
	//ʹ����װ��Ӱ�ӼĴ�������
	TIM_ARRPreloadConfig(TIM5, ENABLE);			                  // ʹ��TIM5���ؼĴ���ARR
	
	/*ʹ�ܶ�ʱ��5���� */
	TIM_Cmd(TIM5, ENABLE);   //ʹ�ܶ�ʱ��5
}

/*************************************************************************
*��������  ����ͨ�ö�ʱ��9���г�ʼ�����ò����������ź�
*������    ��TIM9_Capture_Init
*��������  ��void
*��������ֵ��u16 arr,u16 psc
*����      ��ͨ�����������������㰴��ʱ��
*            ��������� �� PA2   TIM9_CH1  ����ͨ��1  ӳ��ͨ��IC1��IC2
*****************************************************************************/
void TIM9_Capture_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef   GPIO_InitStruc;                   //GPIOx���ýṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx���ýṹ��
	TIM_ICInitTypeDef  TIM_ICInitStruct;                 //����ͨ�����ýṹ��
	NVIC_InitTypeDef   NVIC_InitStruct;                  //�ж����ýṹ��
	
  //ʱ��ʹ��   GPIOA   TIM9
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);  //ʹ��TIM9ʱ��
	
	/*GPIO������*/
	//PA2
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_2;               //ѡ��PA2
	GPIO_InitStruc.GPIO_Mode = GPIO_Mode_AF;            //���ù���
	GPIO_InitStruc.GPIO_PuPd = GPIO_PuPd_NOPULL;        //����������
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_100MHz;     	//�ٶ�100MHz
	GPIO_Init(GPIOA,&GPIO_InitStruc);                   //�����趨������ʼ��PA2
	
	//TIM9��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM9);  //PA2 ����ΪTIM9
	
	//TIMx����
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //�Զ���װ��ֵ(TIMx->ARR�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //��ʱ����Ƶ(TIMx->PSC�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ(TIMx->CR1�Ĵ����ĵ�4λ)(��ʱ��9ֻ�ܵ�������)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ:tDTS=tCK_INT(TIMx->CR1�Ĵ����ĵ�8-9λ)
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStruct);            //��ʼ��TIM9
	
	//����ͨ������
	//ͨ��1ӳ�䵽T1
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;                //ѡ�������ͨ��  
  TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;	   //�����ز���(TIMx->CCER�Ĵ����ĵ�1λ)
  TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��(TIMx->CCMR1�Ĵ���������ģʽ�ĵ�0-1λ)
  TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;	         //���������Ƶ������Ƶ(TIMx->CCMR1�Ĵ���������ģʽ�ĵ�2-3λ)
  TIM_ICInitStruct.TIM_ICFilter = 0x00;                        //���������˲��������˲�(TIMx->CCMR1�Ĵ���������ģʽ�ĵ�4-7λ)
  TIM_ICInit(TIM9, &TIM_ICInitStruct);                         //����ͨ�����ó�ʼ��
 
	//ͨ��1ӳ�䵽T2
	//��Ϊ����һ��������һЩ��������ı�
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;                  //ѡ�������ͨ��  
  TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;	     //�½��ز���(TIMx->CCER�Ĵ����ĵ�1λ)
  TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_IndirectTI; //ӳ�䵽ͨ��1��(TI1)��(����������ֲ�������)(TIMx->CCMR1�Ĵ���������ģʽ�ĵ�0-1λ)
  TIM_ICInit(TIM9, &TIM_ICInitStruct);                           //����ͨ�����ó�ʼ��
	
	//����ѡ��TI1 ���ؼ����(TIMx->SMCR�Ĵ����ĵ�4-6)
	TIM_SelectInputTrigger(TIM9,TIM_TS_TI1FP1);
	//��ģʽѡ��(TIMx->SMCR�Ĵ����ĵ�0-2)
	TIM_SelectSlaveMode(TIM9,TIM_SlaveMode_Reset);                 //��ģʽѡ�񣺸�λģʽ(TIMx->SMCR�Ĵ����ĵ�0-2λ)
	
	//��ʱ���ж���ʽѡ��ʹ��
	TIM_ITConfig(TIM9,TIM_IT_CC1,ENABLE);        //����CC1IE�����ж�	
	TIM_ITConfig(TIM9,TIM_IT_CC2,ENABLE);        //����CC2IE�����ж�	
	
	//TIM5�ж�����-----NVIC ����
  NVIC_InitStruct.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;        //TIM5�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;         //��ռ���ȼ�3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =3;		           //�����ȼ�3
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			           //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStruct);	                               //����ָ���Ĳ�����ʼ��VIC�Ĵ���
 
	//ʹ����װ��Ӱ�ӼĴ�������
	TIM_ARRPreloadConfig(TIM9, ENABLE);			                  // ʹ��TIM9���ؼĴ���ARR
	
	/*ʹ�ܶ�ʱ��9���� */
	TIM_Cmd(TIM9, ENABLE);   //ʹ�ܶ�ʱ��9
}




