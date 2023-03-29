#include "LowPower.h"
#include "stm32f10x.h"
#include "delay.h"
#include "key.h"
#include "stdio.h"

/******************************************************************
*��������  ���͹��������������
*������    ��LowPower_Init
*��������  ��void
*��������ֵ��void
*����      ��
*             PA0��Ϊ�ⲿ�ж�0����
*******************************************************************/
void LowPower_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;  	//��������	  
	NVIC_InitTypeDef NVIC_InitStructure;    //�ж����ȼ�
	EXTI_InitTypeDef EXTI_InitStructure;    //�ж�
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);//ʹ��GPIOA�͸��ù���ʱ��
	
	//��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;	   //PA.0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	       //��ʼ��IO
	//ʹ���ⲿ�жϷ�ʽ
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	//�ж���0����GPIOA.0
	
	//�ж�����
	EXTI_InitStructure.EXTI_Line    = EXTI_Line0;	         //���ð������е��ⲿ��·
	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt; //�����ⲿ�ж�ģʽ:EXTI��·Ϊ�ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش���
 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	                       // ��ʼ���ⲿ�ж�
	
	//�ж����ȼ�����
	NVIC_InitStructure.NVIC_IRQChannel                   = EXTI0_IRQn; //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;          //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;          //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;     //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}


void EXTI0_IRQHandler(void)
{ 		    		    				     		    	
	EXTI->IMR &= ~(1<<0);   //�ر��ⲿ�ж�0�������ظ�����
	if(Check_WKUP())//�ػ�?
	{	
		Sys_Standby();  
	}
	EXTI->IMR |= (1<<0);
	EXTI_ClearITPendingBit(EXTI_Line0); // ���LINE10�ϵ��жϱ�־λ
} 

u8 Check_WKUP(void) 
{
	u8 t=0;	//��¼���µ�ʱ��
	while(1)
	{
		if(KEY_UP)
		{
			t++;			//�Ѿ������� 
			delay_ms(30);
			if(t>=100)		//���³���3����
			{
				return 1; 	//����3s������
			}
		}else 
		{ 
			SystemInit();
			return 0; //���²���3��
		}
	}
} 

/******************************************************************
*��������  ���������ģʽ
*������    ��Sys_Standby
*��������  ��void
*��������ֵ��void
*����      ��
*             PA0��Ϊ�ⲿ�ж�0����
*******************************************************************/
void Sys_Standby(void)
{  
	RCC_APB2PeriphResetCmd(0X01FC,DISABLE);	//��λ����IO��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR����ʱ��
	PWR_WakeUpPinCmd(ENABLE);  //ʹ�ܻ��ѹܽŹ���
	PWR_EnterSTANDBYMode();	  //���������STANDBY��ģʽ 		 
}

void Sys_Stop(void)
{  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR����ʱ��
	PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI);	  //����ֹͣģʽ 		 
}

