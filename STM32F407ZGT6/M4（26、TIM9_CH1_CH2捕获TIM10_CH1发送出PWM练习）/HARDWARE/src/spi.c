#include "stm32f4xx.h"  
#include "stdio.h"

/******************************************************************
*��������  ����SPI1ͨ��Ҫ�õ�GPIO�ڽ��г�ʼ���Ͷ�SPI1�ļĴ�����������
*������    ��SPI1_Init
*��������  ��void
*��������ֵ��void
*����      ��
*           PB3   SPI1_SCK   ʱ����  ���� 
*           PB4   SPI1_MISO  ������  ��������
*           PB5   SPI1_MOSI  �����  ����
*******************************************************************/
void SPI1_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;    //GPIOx���ýṹ��
	SPI_InitTypeDef   SPI_InitStruct;     //SPIx���ýṹ��
	
	//ʱ��ʹ��  GPIOB  SPI1
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	/*GPIOx���ò���*/
	GPIO_InitStruct.GPIO_Pin = (GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);//ѡ��PB3/4/5	
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;                     //���ù���
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;                     //����
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;                   //�������
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;               //�ٶ�Ϊ100MHz
  GPIO_Init(GPIOB, &GPIO_InitStruct);                           //�����趨������ʼ��PB3/4/5

	//SPI1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3����Ϊ SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4����Ϊ SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5����Ϊ SPI1
	
	/*����SPI�Ĵ���*/
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��(SPI->CR1�ĵ�15��10λ)
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;		                   //����SPI����ģʽ:����Ϊ��SPI(SPI->CR1�ĵ�2λ)
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;		               //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ(SPI->CR1�ĵ�11λ)
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;		                       //����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ(SPI->CR1�ĵ�1λ)
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;	                       //�ӵ�һ��ʱ�ӱ��ؿ�ʼ��������(SPI->CR1�ĵ�0λ)
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;		                       //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����(SPI->CR1�ĵ�9λ)
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	 //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ4(SPI->CR1�ĵ�3-5λ)
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;	                 //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ(SPI->CR1�ĵ�7λ)
	SPI_Init(SPI1, &SPI_InitStruct);                                 //��ʼ��SPI1
 
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI1
}



/******************************************************************
*��������  ��SPI1����/����һ���ֽ����ݣ�8λ��
*������    ��SPI1_RS_Byte
*��������  ��u8 data
*��������ֵ��u8
*����      ��
*           PB3   SPI1_SCK   ʱ����
*           PB4   SPI1_MISO  ������
*           PB5   SPI1_MOSI  �����
*******************************************************************/
u8 SPI1_RS_Byte(u8 data)
{
  //�жϷ��ͻ������Ƿ�Ϊ��   SET=1   RESET=0
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){};
	SPI_I2S_SendData(SPI1, data);
  //�жϽ��ջ������Ƿ�Ϊ��   SET=1   RESET=0
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){};
  data = SPI_I2S_ReceiveData(SPI1);
  
  return data;
}



