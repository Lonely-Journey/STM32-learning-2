#include "stm32f4xx.h" 

/******************************************************************
*��������  ���ⲿ�ж�EXIT0������ʼ��
*������    ��EXTI0_Init
*��������  ��void
*��������ֵ��void
*����      ��
             PA0����       �����ش���
*******************************************************************/
void EXTI0_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruc;    //GPIOx���ýṹ��
	EXTI_InitTypeDef EXTI_InitStruct;   //�ⲿ�ж����ýṹ��
	NVIC_InitTypeDef NVIC_InitStruct;   //NVIC(�ж�)���ýṹ��
	
	//ʱ��ʹ��     SYSCFG  GPIOA 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	/*GPIOx����*/
	GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_0;       //PA0
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_IN;     //ͨ������
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL; //��������
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_50MHz;  //�ٶ�Ϊ50MHz
  GPIO_Init(GPIOA,&GPIO_InitStruc);             //�����趨������ʼ��PA0
	//ӳ��ܽ�
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0); //PA0 ���ӵ��ж���0(SYSCFG->EXTICR1�Ĵ���)

	/*�ж�����*/
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;            //�ⲿ�ж�0
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x02;       //�����ȼ�2
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStruct);                             //����
	
	/*�ⲿ�ж��߳�ʼ��*/
  EXTI_InitStruct.EXTI_Line = EXTI_Line0;             //ѡ���ⲿ�ж���0        
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;    //�ж��¼�
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش���    (EXTI_RTSR�Ĵ����ĵ�0λ)
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;              //ʹ��0�ߵ��ж� (EXTI->IMR�Ĵ����ĵ�0λ)
  EXTI_Init(&EXTI_InitStruct);                        //��ʼ���ⲿ�ж�0
}












