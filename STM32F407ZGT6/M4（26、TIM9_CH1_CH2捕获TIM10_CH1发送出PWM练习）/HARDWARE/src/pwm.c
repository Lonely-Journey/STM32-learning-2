#include "stm32f4xx.h"   

/*****************************************************
*��������  ����ͨ�ö�ʱ��12���г�ʼ�����ò����PWM
*������    ��TIM12_PWM_Init
*��������  ��void
*��������ֵ��u16 arr,u16 psc,u16 ccr2
*����      ��ͨ��PWM������LCD������
*            PWM����� �� PB15   TIM12_CH2  ���ͨ��2
********************************************************/
void TIM12_PWM_Init(u16 arr,u16 psc,u16 ccr2)
{
	GPIO_InitTypeDef   GPIO_InitStruc;                   //GPIOx���ýṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx���ýṹ��
	TIM_OCInitTypeDef  TIM_OCInitStruct;                 //ͨ�����ýṹ��
	
  //ʱ��ʹ��   GPIOB   TIM12
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE); //ʹ��TIM12ʱ��
	
	/*GPIO������*/
	//PB15
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_15;              //ѡ��PB15
	GPIO_InitStruc.GPIO_Mode = GPIO_Mode_AF;            //���ù���
	GPIO_InitStruc.GPIO_PuPd = GPIO_PuPd_NOPULL;        //����������
	GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;          //�������
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_100MHz;     	//�ٶ�100MHz
	GPIO_Init(GPIOB,&GPIO_InitStruc);                   //�����趨������ʼ��PB15
	
	//TIM12��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_TIM12);  //PB15 ����ΪTIM12
	
	//TIMx����
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //�Զ���װ��ֵ(TIMx->ARR�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //��ʱ����Ƶ(TIMx->PSC�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ(TIMx->CR1�Ĵ����ĵ�4λ)(ͨ�ö�ʱ��12ֻ�е�������)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ:tDTS=tCK_INT(TIMx->CR1�Ĵ����ĵ�8-9λ)
	TIM_TimeBaseInit(TIM12,&TIM_TimeBaseInitStruct);           //��ʼ��TIM12
	
	//PWMģʽ����
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //����ΪPWMģʽ1(TIMx_CCMR1�Ĵ��������ģʽ�ĵ�4-6λ)
 	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //ͨ��ѡ�����ģʽ(TIMx->CCER�Ĵ����ĵ�0λ)
	TIM_OCInitStruct.TIM_Pulse = ccr2;	                       //���ñȽϼĴ���(TIMx->CCR1)
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;      //������Ч��ƽΪ�͵�ƽ(TIMx->CCER�Ĵ����ĵ�1λ)
	TIM_OC2Init(TIM12, &TIM_OCInitStruct);                     //ͨ��2��ʼ������
	TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable);         //ͨ��2�ȽϼĴ���Ӱ�ӼĴ���ʹ��
 
	//ʹ����װ��Ӱ�ӼĴ�������
	TIM_ARRPreloadConfig(TIM12, ENABLE);			                  // ʹ��TIM12���ؼĴ���ARR
	
	/*ʹ�ܶ�ʱ��12���� */
	TIM_Cmd(TIM12, ENABLE);   //ʹ�ܶ�ʱ��12
}

/*****************************************************
*��������  ����ͨ�ö�ʱ��10���г�ʼ�����ò����PWM
*������    ��TIM10_PWM_Init
*��������  ��void
*��������ֵ��u16 arr,u16 psc,u16 ccr2
*����      ��ͨ��PWM������LED����
*            PWM����� �� PF6   TIM10_CH1  ���ͨ��1
********************************************************/
void TIM10_PWM_Init(u16 arr,u16 psc,u16 ccr1)
{
	GPIO_InitTypeDef   GPIO_InitStruc;                   //GPIOx���ýṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx���ýṹ��
	TIM_OCInitTypeDef  TIM_OCInitStruct;                 //ͨ�����ýṹ��
	
  //ʱ��ʹ��   GPIOF   TIM10
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE); //ʹ��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE); //ʹ��TIM12ʱ��
	
	/*GPIO������*/
	//PF6
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_6;               //ѡ��PF6
	GPIO_InitStruc.GPIO_Mode = GPIO_Mode_AF;            //���ù���
	GPIO_InitStruc.GPIO_PuPd = GPIO_PuPd_NOPULL;        //����������
	GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;          //�������
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_100MHz;     	//�ٶ�100MHz
	GPIO_Init(GPIOF,&GPIO_InitStruc);                   //�����趨������ʼ��PF6
	
	//TIM12��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource6,GPIO_AF_TIM10);  //PF6 ����ΪTIM10
	
	//TIMx����
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //�Զ���װ��ֵ(TIMx->ARR�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //��ʱ����Ƶ(TIMx->PSC�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ(TIMx->CR1�Ĵ����ĵ�4λ)(ͨ�ö�ʱ��10ֻ�е�������)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ:tDTS=tCK_INT(TIMx->CR1�Ĵ����ĵ�8-9λ)
	TIM_TimeBaseInit(TIM10,&TIM_TimeBaseInitStruct);           //��ʼ��TIM10
	
	//PWMģʽ����
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //����ΪPWMģʽ1(TIMx_CCMR1�Ĵ��������ģʽ�ĵ�4-6λ)
 	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //ͨ��ѡ�����ģʽ(TIMx->CCER�Ĵ����ĵ�0λ)
	TIM_OCInitStruct.TIM_Pulse = ccr1;	                       //���ñȽϼĴ���(TIMx->CCR1)
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;      //������Ч��ƽΪ�͵�ƽ(TIMx->CCER�Ĵ����ĵ�1λ)
	TIM_OC1Init(TIM10, &TIM_OCInitStruct);                     //ͨ��1��ʼ������
	TIM_OC1PreloadConfig(TIM10, TIM_OCPreload_Enable);         //ͨ��1�ȽϼĴ���Ӱ�ӼĴ���ʹ��
 
	//ʹ����װ��Ӱ�ӼĴ�������
	TIM_ARRPreloadConfig(TIM10, ENABLE);			                  // ʹ��TIM10���ؼĴ���ARR
	
	/*ʹ�ܶ�ʱ��12���� */
	TIM_Cmd(TIM10, ENABLE);   //ʹ�ܶ�ʱ��10
}







