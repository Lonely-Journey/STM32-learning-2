#include "sys.h"
#include "stdio.h"

/******************************************************************
*��������  ����SPI2ͨ��Ҫ�õ�GPIO�ڽ��г�ʼ���Ͷ�SPI1�ļĴ�����������
*������    ��SPI2_Init
*��������  ��void
*��������ֵ��void
*����      ��
*           PB13   SPI2_SCK   ʱ����  ��������
*           PB14   SPI2_MISO  ������  ��������
*           PB15   SPI2_MOSI  �����  ��������
*******************************************************************/
void SPI2_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;     //GPIOx���ýṹ��
	SPI_InitTypeDef   SPI_InitStruct;      //SPIx���ýṹ��
	
	//ʱ��ʹ��   GPIOB   SPI2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	
	/*GPIOx��ʼ������*/
  //GPIOx�˿�����
  //PB13��PB14��PB15
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;		//PB13��PB14��PB15 �˿�����
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP; 	  	                //����������
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		                  //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStruct);                                 //�����趨������ʼ��PB13��PB14��PB15
	
	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);            //�˿�����Ĵ��������(��������)
	
	/*SPI2�Ĵ�����ʼ������*/
	SPI_InitStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;   //SPI����Ϊ˫��˫��ȫ˫��,��NSS�ϵĵ�ƽΪ�ߵ�ƽ(SPI_CR1�Ĵ����ĵ�10��15λ)
	SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;                   //����Ϊ��SPI(SPI_CR1�Ĵ����ĵ�2��8λ)
	SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;                   //ʹ��8λ����֡��ʽ���з���/����(SPI_CR1�Ĵ����ĵ�11λ)
	SPI_InitStruct.SPI_CPOL              = SPI_CPOL_Low;                      //����״̬ʱ�� SCK���ֵ͵�ƽ(SPI_CR1�Ĵ����ĵ�1λ)
	SPI_InitStruct.SPI_CPHA              = SPI_CPHA_1Edge;                    //���ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ(SPI_CR1�Ĵ����ĵ�0λ)
	SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;                      //����������豸����NSS�����ϵĵ�ƽ��SSIλ��ֵ����(SPI_CR1�Ĵ����ĵ�9λ)
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;           //������Ԥ��ƵֵΪ4  (SPI_CR1�Ĵ����ĵ�3-5λ)
	SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;                  //���ݴ����MSBλ��ʼ(�����׼:�����)(SPI_CR1�Ĵ����ĵ�7λ)
	SPI_InitStruct.SPI_CRCPolynomial     = 7;                                 //CRCֵ����Ķ���ʽ:0x7�Ǹ�λֵ(SPI_CRCPR�Ĵ���)
	SPI_Init(SPI2, &SPI_InitStruct);                                          //��ʼ��SPI2
	
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����(SPI_CR1�Ĵ����ĵ�6λ)
}


/******************************************************************
*��������  ��SPI2����/����һ���ֽ����ݣ�8λ��
*������    ��SPI1_RS_Byte
*��������  ��u8 data
*��������ֵ��u8
*����      ��
*           PB3   SPI1_SCK   ʱ����
*           PB4   SPI1_MISO  ������
*           PB5   SPI1_MOSI  �����
*******************************************************************/
u8 SPI2_RS_Byte(u8 data)
{
  //�жϷ��ͻ������Ƿ�Ϊ��(SPI->SR�Ĵ����ĵ�1λ)
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){};
	//��������
	SPI_I2S_SendData(SPI2,data);
  //�жϽ��ջ������Ƿ�Ϊ��(SPI->SR�Ĵ����ĵ�0λ)
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};
	//��������
  data = SPI_I2S_ReceiveData(SPI2);
  
  return data;
}







