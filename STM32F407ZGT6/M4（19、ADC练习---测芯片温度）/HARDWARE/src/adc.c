#include "stm32f4xx.h"   
#include "stdio.h"

/************************************
*   F407��ADCʱ��Ƶ�ʹٷ�����: 36Mhz
*   F103��ADCʱ��Ƶ�ʹٷ�����: 14Mhz
*************************************/

//��ʱ��������
static void delay_ms(u32 ms)
{
	u32 i = 168 / 4 * 1000 * ms;   //��ϵͳʱ��Ƶ�ʾ�����
	while(i)
	{
		i--;
	}
}

/******************************************
*������    ��ADC1_Init_In5
*��������  ��ADC1�ĵ����ͨ����ʼ������
*��������  ��void
*��������ֵ��void 
*��������  ��
*          PA5----------ADC12_IN5   ģ��ģʽ
*********************************************/
void ADC1_Init_In5(void)
{
	GPIO_InitTypeDef        GPIO_InitStruc;         //GPIOx�ṹ������
	ADC_CommonInitTypeDef   ADC_CommonInitStruct;   //ADCxһ�����ýṹ��
	ADC_InitTypeDef         ADC_InitStruct;         //ADCx��ʼ�����ýṹ��
  
  //ʱ��ʹ��  GPIOA  ADC1
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	
  /*GPIOx�˿�����*/
  //PA5
  GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_5;         //ѡ��PA5
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_AN;       //ģ������
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;   //��������
  GPIO_Init(GPIOA,&GPIO_InitStruc);               //�����趨������ʼ��PA5
	
	/*ADCx�Ĵ�������*/
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;                    //����ģʽ(ADC->CCR�Ĵ����ĵ�0-4λ)
  ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��(ADC->CCR�Ĵ����ĵ�8-11λ)
  ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;     //DMAʧ��(ADC->CCR�Ĵ����ĵ�14-15λ)
  ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;                 //Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz(ADC->CCR�Ĵ����ĵ�16-17λ)
  ADC_CommonInit(&ADC_CommonInitStruct);                                   //ADCxһ�����ó�ʼ��(ADC->CCR)
	
  ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;                      //12λģʽ(ADC->CR1�Ĵ����ĵ�24��25λ)
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;                               //��ɨ��ģʽ(ADC->CR1�Ĵ����ĵ�8λ)
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                         //�ر�����ת��(ADC->CR2�Ĵ����ĵ�1λ)
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //��ֹ������⣬ʹ���������(ADC->CR2�Ĵ����ĵ�20-21λ/��28-29λ)
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                      //�Ҷ���	(ADC->CR2�Ĵ����ĵ�11λ)
  ADC_InitStruct.ADC_NbrOfConversion = 1;                                  //1��ת���ڹ��������� Ҳ����ֻת����������1(ADC->SQR1�Ĵ����ĵ�20-23λ)
  ADC_Init(ADC1, &ADC_InitStruct);                                         //ADCx���ó�ʼ��
	
	//ADC����ͨ������ƫ�򼰲���ʱ�����ú���
  //ADC1�ĵ�һ��ת��ʱͨ��5������ʱ��480������        (������ADCx->SMPRx)
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1,ADC_SampleTime_480Cycles);  
	
	ADC_Cmd(ADC1, ENABLE);//ʹ��ADCx��������ADת����	
}

/******************************************
*������    ��ADC1_In5_Data
*��������  ����ȡADC1ͨ��5��ת������
*��������  ��void
*��������ֵ��u16
*��������  ������ת���ɵ���������
*********************************************/
u16 ADC1_In5_Data(void)
{
  u16 data;
  
  //��������ͨ��ת������(ADC1->CR2�Ĵ����ĵ�30λ)
	ADC_SoftwareStartConv(ADC1);    
  //�ȴ�ת�����
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC )){};//�ȴ�ת������(ADC1->SR�Ĵ����ĵ�1λ)
  //��ȡת���������(ADC1->DR)
  data = ADC_GetConversionValue(ADC1);   
  
  return data;
}

/******************************************
*������    ��ADC2_Init_In4
*��������  ��ADC2�ĵ�4��ͨ����ʼ������
*��������  ��void
*��������ֵ��void 
*��������  ��
*          PA4----------ADC2_IN4   ģ��ģʽ
*********************************************/
void ADC2_Init_In4(void)
{
	GPIO_InitTypeDef        GPIO_InitStruc;         //GPIOx�ṹ������
	ADC_CommonInitTypeDef   ADC_CommonInitStruct;   //ADCxһ�����ýṹ��
	ADC_InitTypeDef         ADC_InitStruct;         //ADCx��ʼ�����ýṹ��
  
  //ʱ��ʹ��  GPIOA  ADC2
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); 
	
  /*GPIOx�˿�����*/
  //PA4
  GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_4;         //ѡ��PA4
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_AN;       //ģ������
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;   //��������
  GPIO_Init(GPIOA,&GPIO_InitStruc);               //�����趨������ʼ��PA4
	
	/*ADCx�Ĵ�������*/
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;                    //����ģʽ(ADC->CCR�Ĵ����ĵ�0-4λ)
  ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��(ADC->CCR�Ĵ����ĵ�8-11λ)
  ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;     //DMAʧ��(ADC->CCR�Ĵ����ĵ�14-15λ)
  ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;                 //Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz(ADC->CCR�Ĵ����ĵ�16-17λ)
  ADC_CommonInit(&ADC_CommonInitStruct);                                   //ADCxһ�����ó�ʼ��(ADC->CCR)
	
  ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;                      //12λģʽ(ADC->CR1�Ĵ����ĵ�24��25λ)
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;                               //��ɨ��ģʽ(ADC->CR1�Ĵ����ĵ�8λ)
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                         //�ر�����ת��(ADC->CR2�Ĵ����ĵ�1λ)
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //��ֹ������⣬ʹ���������(ADC->CR2�Ĵ����ĵ�20-21λ/��28-29λ)
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                      //�Ҷ���	(ADC->CR2�Ĵ����ĵ�11λ)
  ADC_InitStruct.ADC_NbrOfConversion = 1;                                  //1��ת���ڹ��������� Ҳ����ֻת����������1(ADC->SQR1�Ĵ����ĵ�20-23λ)
  ADC_Init(ADC2, &ADC_InitStruct);                                         //ADCx���ó�ʼ��
	
	//ADC����ͨ������ƫ�򼰲���ʱ�����ú���
  //ADC2�ĵ�һ��ת��ʱͨ��4������ʱ��480������        (������ADCx->SMPRx)
  ADC_RegularChannelConfig(ADC2, ADC_Channel_4, 1,ADC_SampleTime_480Cycles);  
	
	ADC_Cmd(ADC2, ENABLE);//ʹ��ADCx��������ADת����	
}

/******************************************
*������    ��ADC2_In4_Data
*��������  ����ȡADC2ͨ��4��ת������
*��������  ��void
*��������ֵ��u16
*��������  ������ת��������������
*********************************************/
u16 ADC2_In4_Data(void)
{
	u16 data;
  
  //��������ͨ��ת������(ADC2->CR2�Ĵ����ĵ�30λ)
	ADC_SoftwareStartConv(ADC2);    
  //�ȴ�ת�����
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC )){};//�ȴ�ת������(ADC2->SR�Ĵ����ĵ�1λ)
  //��ȡת���������(ADC1->DR)
  data = ADC_GetConversionValue(ADC2);   
  
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
	ADC_CommonInitTypeDef   ADC_CommonInitStruct;   //ADCxһ�����ýṹ��
	ADC_InitTypeDef         ADC_InitStruct;         //ADCx��ʼ�����ýṹ��
  
  //ʱ��ʹ��   ADC1
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	
	ADC_TempSensorVrefintCmd(ENABLE);//ʹ���ڲ��¶ȴ�����(ADC->CCR�Ĵ����ĵ�23λ)
	
	/*ADCx�Ĵ�������*/
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;                    //����ģʽ(ADC->CCR�Ĵ����ĵ�0-4λ)
  ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��(ADC->CCR�Ĵ����ĵ�8-11λ)
  ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;     //DMAʧ��(ADC->CCR�Ĵ����ĵ�14-15λ)
  ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;                 //Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz(ADC->CCR�Ĵ����ĵ�16-17λ)
  ADC_CommonInit(&ADC_CommonInitStruct);                                   //ADCxһ�����ó�ʼ��(ADC->CCR)
	
  ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;                      //12λģʽ(ADC->CR1�Ĵ����ĵ�24��25λ)
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;                               //��ɨ��ģʽ(ADC->CR1�Ĵ����ĵ�8λ)
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                         //�ر�����ת��(ADC->CR2�Ĵ����ĵ�1λ)
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //��ֹ������⣬ʹ���������(ADC->CR2�Ĵ����ĵ�20-21λ/��28-29λ)
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                      //�Ҷ���	(ADC->CR2�Ĵ����ĵ�11λ)
  ADC_InitStruct.ADC_NbrOfConversion = 1;                                  //1��ת���ڹ��������� Ҳ����ֻת����������1(ADC->SQR1�Ĵ����ĵ�20-23λ)
  ADC_Init(ADC1, &ADC_InitStruct);                                         //ADCx���ó�ʼ��
	
	//ADC����ͨ������ƫ�򼰲���ʱ�����ú���
  //ADC1�ĵ�һ��ת��ʱͨ��16������ʱ��480������        (������ADCx->SMPRx)
  ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1,ADC_SampleTime_480Cycles);  
	
	ADC_Cmd(ADC1, ENABLE);//ʹ��ADCx��������ADת����	

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
  
  //��������ͨ��ת������(ADC1->CR2�Ĵ����ĵ�30λ)
	ADC_SoftwareStartConv(ADC1);    
  //�ȴ�ת�����
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){};//�ȴ�ת������(ADC1->SR�Ĵ����ĵ�1λ)
  //��ȡת���������(ADC1->DR)
  data = ADC_GetConversionValue(ADC1);   
  
  return data;
}

/******************************************
*��������  ����ADC1ͨ��16ת�������ݽ����˲�
*������    ��Get_Adc_Average
*��������  ��u8 ch,u8 times
*��������ֵ��u16
*��������  ��
*********************************************/
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
    temp_val+=ADC1_In16_Data();
		delay_ms(5);
	}
	return temp_val/times;
} 

/******************************************
*��������  ����ADC1ͨ��16ת�������ݽ��д���
*������    ��ADC1_Chip_Temperature
*��������  ��void
*��������ֵ��float
*��������  �����ڼ��оƬ�¶�
*********************************************/
float ADC1_Chip_Temperature(void)
{
  u16 data;
  double stm_t;

  data=Get_Adc_Average(16,5);	//��ȡͨ��16,20��ȡƽ��
	stm_t = (float)data*(3.3/4096);		//��ѹֵ
	stm_t = (stm_t-0.76)/0.0025+25; 	//ת��Ϊ�¶�ֵ 
  
  return stm_t;
}
					   





