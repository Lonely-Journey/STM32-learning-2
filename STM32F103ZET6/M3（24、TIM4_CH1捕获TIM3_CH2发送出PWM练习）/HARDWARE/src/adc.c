#include "sys.h"

/************************************
*   F407��ADCʱ��Ƶ�ʹٷ�����: 36Mhz
*   F103��ADCʱ��Ƶ�ʹٷ�����: 14Mhz
*************************************/

/******************************************
*������    ��ADC3_Init_In6
*��������  ��ADC1�ĵ����ͨ����ʼ������
*��������  ��void
*��������ֵ��void 
*��������  ��
*          PF8----------ADC3_IN6   ģ��ģʽ
*********************************************/
void ADC3_Init_In6(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;     //GPIOx���ýṹ��
	ADC_InitTypeDef   ADC_InitStruct;      //ADCxһ�����ýṹ��
	
	//ʱ��ʹ��  GPIOF  ADC3
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
	
	/*GPIOx��ʼ������*/
  //GPIOx�˿�����                         
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;          //PF8�˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;		  //ģ����������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOF, &GPIO_InitStruct);					    //�����趨������ʼ��PF8
	
	ADC_DeInit(ADC3);                    //ADC3�ӿڸ�λ,������ADC3��ȫ���Ĵ�������Ϊȱʡֵ(RCC_APB2RSTR�Ĵ����ĵ�15λ)
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);    //�����߽���6��Ƶ12MHZ

	/*ADCx��ʼ������*/
  ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;                    //����ģʽ(ADC_CR1�Ĵ����ĵ�16-19λ)
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;                         //��ɨ��(ADC_CR1�Ĵ����ĵ�8λ)
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                   //����ת��ģʽ(ADC_CR2�Ĵ����ĵ�1λ)
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;   //������ʽ���������(ADC_CR2�Ĵ����ĵ�17-19λ)
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                //���ݶ��룺�Ҷ���(ADC_CR2�Ĵ����ĵ�11λ)
  ADC_InitStruct.ADC_NbrOfChannel = 1;                               //�м���ת��(ADC_SQR1�Ĵ����ĵ�20-23λ)
  ADC_Init(ADC3, &ADC_InitStruct);                                   //ADC1��ʼ��
	
	//ADC����ͨ������ƫ�򼰲���ʱ�����ú���
  //ADC3�ĵ�һ��ת������ͨ��6������ʱ��71.5����    (������ADCx->SMPRx)
  ADC_RegularChannelConfig(ADC3, ADC_Channel_6, 1, ADC_SampleTime_71Cycles5); 
	
	//ADC3ʹ�ܺ���
  ADC_Cmd(ADC3, ENABLE);
	
	/*У׼����(���Ǳ����)*/ 
  //��λ��������ò���Ӳ���������У׼�Ĵ�������ʼ�����λ���������
  ADC_ResetCalibration(ADC3);                 //ʹ�ܸ�λУ׼
  while(ADC_GetResetCalibrationStatus(ADC3)); //�ȴ�У׼����
  //��λ����������Կ�ʼУ׼������У׼����ʱ��Ӳ�����
  ADC_StartCalibration(ADC3);                 //����ADCУ׼
  while(ADC_GetCalibrationStatus(ADC3));      //�ȴ�У׼����
}

/******************************************
*������    ��ADC3_In6_Data
*��������  ����ȡADC3ͨ��6��ת������
*��������  ��void
*��������ֵ��u16
*��������  ������ת��������������
*********************************************/
u16 ADC3_In6_Data(void)
{
	u16 data;
	
	//��������ͨ��ת������(ADC_CR2�Ĵ�����22λ)
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);		
	//�ȴ�ת�����(ADC_SR�Ĵ����ĵ�1λ)
	while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));
	//��ȡת���������(ADC_DR�Ĵ���)
	data = ADC_GetConversionValue(ADC3);
	
	return data;
}

/******************************************
*��������  ��ADC1�ĵ�ʮ����ͨ����ʼ������
*������    ��ADC1_Init_In16
*��������  ��void
*��������ֵ��void 
*��������  ���������ţ����ڲ�оƬ�¶�
*********************************************/
void ADC1_Init_In16(void)
{
	ADC_InitTypeDef   ADC_InitStruct;      //ADCxһ�����ýṹ��
	
	//ʱ��ʹ��  ADC1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	ADC_DeInit(ADC1);                    //ADC1�ӿڸ�λ,������ADC1��ȫ���Ĵ�������Ϊȱʡֵ(RCC_APB2RSTR�Ĵ����ĵ�15λ)
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);    //�����߽���6��Ƶ12MHZ

	/*ADCx��ʼ������*/
  ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;                    //����ģʽ(ADC_CR1�Ĵ����ĵ�16-19λ)
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;                         //��ɨ��(ADC_CR1�Ĵ����ĵ�8λ)
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                   //����ת��ģʽ(ADC_CR2�Ĵ����ĵ�1λ)
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;   //������ʽ���������(ADC_CR2�Ĵ����ĵ�17-19λ)
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                //���ݶ��룺�Ҷ���(ADC_CR2�Ĵ����ĵ�11λ)
  ADC_InitStruct.ADC_NbrOfChannel = 1;                               //�м���ת��(ADC_SQR1�Ĵ����ĵ�20-23λ)
  ADC_Init(ADC1, &ADC_InitStruct);                                   //ADC1��ʼ��
	
	//�����ڲ��¶ȴ�����(ADC_CR2�Ĵ����ĵ�23λ)
	ADC_TempSensorVrefintCmd(ENABLE); 
	
	//ADC����ͨ������ƫ�򼰲���ʱ�����ú���
  //ADC1�ĵ�һ��ת������ͨ��16������ʱ��71.5����    (������ADCx->SMPRx)
  ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_71Cycles5); 
	
	//ADC1ʹ�ܺ���
  ADC_Cmd(ADC1, ENABLE);
	
	/*У׼����(���Ǳ����)*/ 
  //��λ��������ò���Ӳ���������У׼�Ĵ�������ʼ�����λ���������
  ADC_ResetCalibration(ADC1);                 //ʹ�ܸ�λУ׼
  while(ADC_GetResetCalibrationStatus(ADC1)); //�ȴ�У׼����
  //��λ����������Կ�ʼУ׼������У׼����ʱ��Ӳ�����
  ADC_StartCalibration(ADC1);                 //����ADCУ׼
  while(ADC_GetCalibrationStatus(ADC1));      //�ȴ�У׼����
}

/******************************************
*��������  ����ȡADC1ͨ��16��ת������
*������    ��ADC1_In16_Data
*��������  ��void
*��������ֵ��u16
*��������  �����оƬ�¶�
*********************************************/
u16 ADC1_In16_Data(void)
{
	u16 data;
	
	//��������ͨ��ת������(ADC_CR2�Ĵ�����22λ)
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		
	//�ȴ�ת�����(ADC_SR�Ĵ����ĵ�1λ)
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
	//��ȡת���������(ADC_DR�Ĵ���)
	data = ADC_GetConversionValue(ADC1);
	
	return data;
}

/******************************************
*��������  ����ADC1ͨ��16ת�������ݽ��д���
*������    ��ADC1_Chip_Temperature
*��������  ��void
*��������ֵ��float
*��������  �����ڼ��оƬ�¶�
*********************************************/
double ADC1_Chip_Temperature(void)
{
  u8 i;
  u32 data;
  float sum = 0;
  double stm_t[6];
  
  for(i=0;i<5;i++)
  {
    data = ADC1_In16_Data();
    stm_t[i]=(float)data*(3.3/4096);		//��ѹֵ 
	  stm_t[i]=(1.43-stm_t[i])/0.0043+25;	//ת��Ϊ�¶�ֵ 	
    sum += stm_t[i];
  }
  
  stm_t[5] = sum/5;
  
  return stm_t[5];
}

/******************************************
*������    ��ADC3_Init2_In6
*��������  ��ADC1�ĵ����ͨ����ʼ������(���õ��ж�)
*��������  ��void
*��������ֵ��void 
*��������  ��
*          PF8----------ADC3_IN6   ģ��ģʽ
*********************************************/
void ADC3_Init2_In6(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;     //GPIOx���ýṹ��
	ADC_InitTypeDef   ADC_InitStruct;      //ADCxһ�����ýṹ��
	NVIC_InitTypeDef  NVIC_InitStruct;     //NVIC(�ж�)���ýṹ��
	
	//ʱ��ʹ��  GPIOF  ADC3
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
	
	/*GPIOx��ʼ������*/
  //GPIOx�˿�����                         
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;          //PF8�˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;		  //ģ����������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOF, &GPIO_InitStruct);					    //�����趨������ʼ��PF8
	
	ADC_DeInit(ADC3);                    //ADC3�ӿڸ�λ,������ADC3��ȫ���Ĵ�������Ϊȱʡֵ(RCC_APB2RSTR�Ĵ����ĵ�15λ)
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);    //�����߽���6��Ƶ12MHZ

	/*ADCx��ʼ������*/
  ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;                    //����ģʽ(ADC_CR1�Ĵ����ĵ�16-19λ)
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;                         //��ɨ��(ADC_CR1�Ĵ����ĵ�8λ)
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                   //����ת��ģʽ(ADC_CR2�Ĵ����ĵ�1λ)
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;   //������ʽ���������(ADC_CR2�Ĵ����ĵ�17-19λ)
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                //���ݶ��룺�Ҷ���(ADC_CR2�Ĵ����ĵ�11λ)
  ADC_InitStruct.ADC_NbrOfChannel = 1;                               //�м���ת��(ADC_SQR1�Ĵ����ĵ�20-23λ)
  ADC_Init(ADC3, &ADC_InitStruct);                                   //ADC1��ʼ��
	
	//ADC����ͨ������ƫ�򼰲���ʱ�����ú���
  //ADC3�ĵ�һ��ת������ͨ��6������ʱ��71.5����    (������ADCx->SMPRx)
  ADC_RegularChannelConfig(ADC3, ADC_Channel_6, 1, ADC_SampleTime_71Cycles5); 
	
	/*NVIC����*/
	//ѡ���жϷ�ʽ��ʹ�ܸ÷�ʽ�ж�
	ADC_ITConfig(ADC3,ADC_IT_EOC, ENABLE);
	
	//ADC�ж�����
  NVIC_InitStruct.NVIC_IRQChannel = ADC3_IRQn;             //ѡ�񴮿�1�ж�
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�3
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;		       //�����ȼ�3
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 		         //IRQͨ��ʹ��
  NVIC_Init(&NVIC_InitStruct);                             //ADC3�жϳ�ʼ��
	
	//ADC3ʹ�ܺ���
  ADC_Cmd(ADC3, ENABLE);
	
	/*У׼����(���Ǳ����)*/ 
  //��λ��������ò���Ӳ���������У׼�Ĵ�������ʼ�����λ���������
  ADC_ResetCalibration(ADC3);                 //ʹ�ܸ�λУ׼
  while(ADC_GetResetCalibrationStatus(ADC3)); //�ȴ�У׼����
  //��λ����������Կ�ʼУ׼������У׼����ʱ��Ӳ�����
  ADC_StartCalibration(ADC3);                 //����ADCУ׼
  while(ADC_GetCalibrationStatus(ADC3));      //�ȴ�У׼����
	
	//��������ͨ��ת������(ADC_CR2�Ĵ�����22λ)
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);	
}








