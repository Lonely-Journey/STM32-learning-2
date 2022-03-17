#include "stm32f4xx.h"     
#include "spi.h"
#include "w25q64.h"
#include "stdio.h"

/*********************************************
*W25Q64�洢оƬ
*������С             ��8Mbyte  ��  64Mbit
*���п���             ��128��
*ÿ������������       ��16����
*ÿ��������ҳ         ��16ҳ
*ÿҳ�ֽ�             ��256�ֽ�
*��ַ��д���ʮ����������0x7fffff
*        7f:����   f������    f��ҳ    ff���ֽ�
**********************************************/

/*******************************************
*��������  ��W25Q64��ʼ��
*������    ��W25Q64_Init
*��������  ��void
*��������ֵ��void
*��������  ��
*           SPI_CS     PB14
********************************************/
void W25Q64_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruc;  //GPIOx���ýṹ��
	
	/*SPI��ʼ��*/
	SPI1_Init();
	
	/*Ƭѡ����*/
  //ʱ��ʹ��  GPIOF  GPIOB
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
  /*GPIOx�˿�����*/
  //PB14
  GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_14;       //ѡ��PB14
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_OUT;     //ͨ�����
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;  //��������
  GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;     //�������
  GPIO_InitStruc.GPIO_Speed = GPIO_Speed_100MHz; //�ٶ�Ϊ100MHz
  GPIO_Init(GPIOB,&GPIO_InitStruc);              //�����趨������ʼ��PB14
	//Ƭѡ����
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
}
	
/*******************************************
*��������  ����W25Q64����дʹ��
*������    ��W25Q64_Write_Enable
*��������  ��void
*��������ֵ��void
*��������  ��дʹ��ָ��    0x06
*            �˺�������д������
********************************************/
void W25Q64_Write_Enable(void)
{
  SPI_CS_L;
  SPI1_RS_Byte(0x06);
  SPI_CS_H;
}

/*******************************************
*��������  ����W25Q64��״̬�Ĵ�����ֵ
*������    ��W25Q64_Read_Status
*��������  ��void
*��������ֵ��u8
*��������  ����״ָ̬��    0x05
*            �˺�������д������
*            �÷���ֵ�ж��Ƿ�д�����
********************************************/
u8 W25Q64_Read_Status(void)
{
  u8 status;
  
  SPI_CS_L;
  SPI1_RS_Byte(0x05);
  status = SPI1_RS_Byte(0xff);
  SPI_CS_H;
  
  return status;
}

/*****************************************************
*��������  ������оƬ��W25Q64��ĳ����ַ��n���ֽ�����
*������    ��W25Q64_Read_Bytes
*��������  ��void
*��������ֵ��u32 address  u32 num_byte  u8 *str
*��������  ��дָ��    0x03
*******************************************************/
void W25Q64_Read_Bytes(u32 address,u32 num_byte,u8 *str)
{
  SPI_CS_L;
  SPI1_RS_Byte(0x03);
  SPI1_RS_Byte(address>>16);
  SPI1_RS_Byte(address>>8);
  SPI1_RS_Byte(address);
  
  while(num_byte)
  {
    *str = SPI1_RS_Byte(0xff);
    str++;
    num_byte--;
  }
  SPI_CS_H;
}

/***************************************************************
*��������  ������оƬ��W25Q64��ĳ����ַ��дn���ֽ�����(���ɿ�ҳ)
*������    ��W25Q64_Page_Write
*��������  ��void
*��������ֵ��u32 address  u16 num_byte  u8 *str
*��������  ��ҳ��ȡָ��    0x02
****************************************************************/
void W25Q64_Page_Write(u32 address,u16 num_byte,u8 *str)
{
  //дʹ��
  W25Q64_Write_Enable();
  //д���ݹ���
  SPI_CS_L;
  SPI1_RS_Byte(0x02);
  SPI1_RS_Byte(address>>16);
  SPI1_RS_Byte(address>>8);
  SPI1_RS_Byte(address);
  
  while(num_byte)
  {
    SPI1_RS_Byte(*str);
    str++;
    num_byte--;
  }
  SPI_CS_H;
  //�ȴ�д�����
  while(W25Q64_Read_Status() & (1<<0));
}

/***************************************************************
*��������  ��W25Q64��������
*������    ��W25Q64_Sector_Erase
*��������  ��void
*��������ֵ��u32 address
*��������  ����������ָ��    0x20
****************************************************************/
void W25Q64_Sector_Erase(u32 address)
{
  //дʹ��
  W25Q64_Write_Enable();
  //����
  SPI_CS_L;
  SPI1_RS_Byte(0x20);
  SPI1_RS_Byte(address>>16);
  SPI1_RS_Byte(address>>8);
  SPI1_RS_Byte(address);
  SPI_CS_H;
  //�ȴ��������
  while(W25Q64_Read_Status() & (1<<0));
}

/***************************************************************
*��������  ��W25Q64��������
*������    ��W25Q64_Block_Erase
*��������  ��void
*��������ֵ��u32 address
*��������  �������ָ��    0xd8
****************************************************************/
void W25Q64_Block_Erase(u32 address)
{
  //дʹ��
  W25Q64_Write_Enable();
  //����
  SPI_CS_L;
  SPI1_RS_Byte(0xd8);
  SPI1_RS_Byte(address>>16);
  SPI1_RS_Byte(address>>8);
  SPI1_RS_Byte(address);
  SPI_CS_H;
  //�ȴ��������
  while(W25Q64_Read_Status() & (1<<0));
}

/***************************************************************
*��������  ��W25Q64оƬ����
*������    ��W25Q64_Chip_Erase
*��������  ��void
*��������ֵ��void
*��������  ��оƬ����ָ��    0xc7
****************************************************************/
void W25Q64_Chip_Erase(void)
{
  //дʹ��
  W25Q64_Write_Enable();
  //����
  SPI_CS_L;
  SPI1_RS_Byte(0xc7);
  SPI_CS_H;
  //�ȴ��������
  while(W25Q64_Read_Status() & (1<<0));
}

/***************************************************************
*��������  ��W25Q64������������
*������    ��W25Q64_Blocks_Erase
*��������  ��void
*��������ֵ��void
*��������  ��
****************************************************************/
void W25Q64_Blocks_Erase(u32 address,u8 num_blcok)
{
  while(num_blcok)
	{
		W25Q64_Block_Erase(address);
		address += 65536;
		num_blcok--;
	}
}



















