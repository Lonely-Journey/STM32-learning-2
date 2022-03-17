#include "sys.h"
#include "spi.h"
#include "w25q128.h"
#include "stdio.h"

/*********************************************
*W25Q128�洢оƬ
*������С             ��16Mbyte  ��  128Mbit
*���п���             ��256��
*ÿ������������       ��16����
*ÿ��������ҳ         ��16ҳ
*ÿҳ�ֽ�             ��256�ֽ�
*��ַ��д���ʮ����������0x9fffff
*        9f:����   f������    f��ҳ    ff���ֽ�
**********************************************/


/*******************************************
*��������  ��W25Q128��ʼ��
*������    ��W25Q128_Init
*��������  ��void
*��������ֵ��void
*��������  ��
*           SPI_CS     PB12
********************************************/
void W25Q128_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;     //GPIOx���ýṹ��
	
	//SPI��ʼ��
  SPI2_Init();
	
  /*Ƭѡ����*/
  //ʱ��ʹ��   GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  /*GPIOx��ʼ������*/
  //GPIOx�˿�����
  //PB13��PB14��PB15
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_12;		     //PB12 �˿�����
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP; 	 //�������
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStruct);              //�����趨������ʼ��PB12
  //�˿�������ݼĴ���(Ƭѡ����)
  GPIO_SetBits(GPIOB,GPIO_Pin_12);                 //�˿�����Ĵ��������(��������)
}

/*******************************************
*��������  ����W25Q128����дʹ��
*������    ��W25Q128_Write_Enable
*��������  ��void
*��������ֵ��void
*��������  ��дʹ��ָ��    0x06
*            �˺�������д������
********************************************/
void W25Q128_Write_Enable(void)
{
  SPI_CS_L;
  SPI2_RS_Byte(0x06);
  SPI_CS_H;
}

/*******************************************
*��������  ����W25Q128��״̬�Ĵ�����ֵ
*������    ��W25Q128_Read_Status
*��������  ��void
*��������ֵ��u8
*��������  ����״ָ̬��    0x05
*            �˺�������д������
*            �÷���ֵ�ж��Ƿ�д�����
********************************************/
u8 W25Q128_Read_Status(void)
{
  u8 status;
  
  SPI_CS_L;
  SPI2_RS_Byte(0x05);
  status = SPI2_RS_Byte(0xff);
  SPI_CS_H;
  
  return status;
}

/*****************************************************
*��������  ������оƬ��W25Q128��ĳ����ַ��n���ֽ�����
*������    ��W25Q128_Read_Bytes
*��������  ��void
*��������ֵ��u32 address  u32 num_byte  u8 *str
*��������  ����ָ��    0x03
*******************************************************/
void W25Q128_Read_Bytes(u32 address,u32 num_byte,u8 *str)
{
  SPI_CS_L;
  //��ָ��
  SPI2_RS_Byte(0x03);
  //����ַ
  SPI2_RS_Byte(address>>16);
  SPI2_RS_Byte(address>>8);
  SPI2_RS_Byte(address);
  //������
  while(num_byte)
  {
    *str = SPI2_RS_Byte(0xff);
    str++;
    num_byte--;
  }
  SPI_CS_H;
}

/***************************************************************
*��������  ������оƬ��W25Q128��ĳ����ַ��дn���ֽ�����(���ɿ�ҳ)
*������    ��W25Q128_Page_Write
*��������  ��void
*��������ֵ��u32 address  u32 num_byte  u8 *str
*��������  ��ҳдȡָ��    0x02
****************************************************************/
void W25Q128_Page_Write(u32 address,u32 num_byte,u8 *str)
{
  //дʹ��
  W25Q128_Write_Enable();
  //д���ݹ���
  SPI_CS_L;
  SPI2_RS_Byte(0x02);
  SPI2_RS_Byte(address>>16);
  SPI2_RS_Byte(address>>8);
  SPI2_RS_Byte(address);
  
  while(num_byte)
  {
    SPI2_RS_Byte(*str);
    str++;
    num_byte--;
  }
  SPI_CS_H;
  //�ȴ�д�����
  while(W25Q128_Read_Status() & (1<<0));
}

/***************************************************************
*��������  ��W25Q128��������
*������    ��W25Q64_Sector_Erase
*��������  ��void
*��������ֵ��u32 address
*��������  ����������ָ��    0x20
****************************************************************/
void W25Q128_Sector_Erase(u32 address)
{
  //дʹ��
  W25Q128_Write_Enable();
  //����
  SPI_CS_L;
  SPI2_RS_Byte(0x20);
  SPI2_RS_Byte(address>>16);
  SPI2_RS_Byte(address>>8);
  SPI2_RS_Byte(address);
  SPI_CS_H;
  //�ȴ��������
  while(W25Q128_Read_Status() & (1<<0));
}

/***************************************************************
*��������  ��W25Q128��������
*������    ��W25Q128_Block_Erase
*��������  ��void
*��������ֵ��u32 address
*��������  �������ָ��    0xd8
****************************************************************/
void W25Q128_Block_Erase(u32 address)
{
  //дʹ��
  W25Q128_Write_Enable();
  //����
  SPI_CS_L;
  SPI2_RS_Byte(0xd8);
  SPI2_RS_Byte(address>>16);
  SPI2_RS_Byte(address>>8);
  SPI2_RS_Byte(address);
  SPI_CS_H;
  //�ȴ��������
  while(W25Q128_Read_Status() & (1<<0));
}

/***************************************************************
*��������  ��W25Q128оƬ����
*������    ��W25Q128_Chip_Erase
*��������  ��void
*��������ֵ��void
*��������  ��оƬ����ָ��    0xc7
****************************************************************/
void W25Q128_Chip_Erase(void)
{
  //дʹ��
  W25Q128_Write_Enable();
  //����
  SPI_CS_L;
  SPI2_RS_Byte(0xc7);
  SPI_CS_H;
  //�ȴ��������
  while(W25Q128_Read_Status() & (1<<0));
}

/***************************************************************
*��������  ��W25Q128������������
*������    ��W25Q128_Blocks_Erase
*��������  ��void
*��������ֵ��void
*��������  ��
****************************************************************/
void W25Q128_Blocks_Erase(u32 address,u8 num_blcok)
{
  while(num_blcok)
	{
		W25Q128_Block_Erase(address);
		address += 65536;
		num_blcok--;
	}
}















