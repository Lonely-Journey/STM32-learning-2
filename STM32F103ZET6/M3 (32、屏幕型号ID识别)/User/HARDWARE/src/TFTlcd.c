#include "stm32f10x.h"
#include "TFTLCD.h"
#include "delay.h"
#include "fsmc.h"


//����֪��ARGBָ����һ��ɫ��ģʽ������A����Alpha��R��ʾred��G��ʾgreen��B��ʾblue��
typedef struct _HEADCOLOR
{
   unsigned char scan;      //ɨ�跽��
   unsigned char gray;      //�Ҷ�ֵ
   unsigned short w;        //ͼƬ��
   unsigned short h;        //ͼƬ��
   unsigned char is565;     //ÿ������ռ��λ����R=5��G=6��B=5��û��͸���ȣ���ôһ�����ص�ռ5+6+5=16λ
   unsigned char rgb;       //RGB
}HEADCOLOR;

/****************************************************************************
*��������  ����LCD��Ӧ��GPIO�ڽ��г�ʼ������
*������    ��TFTLCD_IO_Init
*��������  ��void
*��������ֵ��void
*����      ��
*     �����ߣ�//ͨ����� ����      �ٶ�50M
*           DO0---PD14	DO1---PD15	DO2---PD0	DO3---PD1
*           DO4---PE7	DO5---PE8	DO6---PE9	DO7---PE10
*           DO8---PE11	DO9---PE12	DO10---PE13	DO11---PE14
*           DO12---PE15	DO13---PD8	DO14---PD9	DO15---PD10
*     �����ߣ�//ͨ�����  ����      �ٶ�50M
*           WR---PD5	RD---PD4	CS---PG12 	
*           RS/DC(����/����)---PG0	 
*           BL�����⣩--PB0   
*     ����
*         PB��0
*         PD��0-1  4-5  8-10  14-15  
*         PE��7-15
*         PG��0  12          
***************************************************************************/
void TFTLCD_IO_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStruct;    //GPIOx���ýṹ��
	
	//ʱ��ʹ��   GPIOB   GPIOD   GPIOE  GPIOG
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PD�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PE�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PG�˿�ʱ��
	
	//PB0
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;				     //PB0 �˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStruct);					       //�����趨������ʼ��PB0
	
	//PD0/1/4/5/8/9/10/14/15
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;  //PD0/1/4/5/8/9/10/14/15 �˿�����
	GPIO_Init(GPIOD, &GPIO_InitStruct);					     //�����趨������ʼ��PD0/1/4/5/8/9/10/14/15
	
	//PE7-15
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;  //PE7-15 �˿�����
	GPIO_Init(GPIOE, &GPIO_InitStruct);	             //�����趨������ʼ��PE7-15
	
	//PG0/12 
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12;  //PG0/12 �˿�����
	GPIO_Init(GPIOG, &GPIO_InitStruct);	              //�����趨������ʼ��PG0/12 
	
	//Ƭѡ����
  TFTLCD_CS_H;
  //дʹ������
  TFTLCD_WR_H;
  //��ʹ������
  TFTLCD_RD_H;
  //��������ѡ��������
  TFTLCD_DC_H;
  //��������(�ر�)
  TFTLCD_BL_L;
}	

/***************************************************************************************
*��������  ������оƬ���������
*������    ��TFTLCD_ILI9341_CMD
*��������  ��u8 cmd    
*��������ֵ��void
*��������  ��  
*         Ƭѡ             TFTLCD_CS_H   TFTLCD_CS_L   
*         дʹ��           TFTLCD_WR_H   TFTLCD_WR_L   
*         ��ʹ��           TFTLCD_RD_H   TFTLCD_RD_L 
*         ��������ѡ����   TFTLCD_DC_H   TFTLCD_DC_L
*         ����             TFTLCD_BL_H   TFTLCD_BL_L  
*****************************************************************************************/
void TFTLCD_ILI9341_CMD(u8 cmd)
{
//********************** 8080ͨ�ŷ�ʽ���� **********************//
	/*
  //��ʹ������
  TFTLCD_RD_H;
  //��������ѡ��������
  TFTLCD_DC_L;
  //Ƭѡ����
  TFTLCD_CS_L;
  //дʹ������
  TFTLCD_WR_L;
  //׼��Ҫ���͵�����
  if(cmd & 0x01) TFTLCD_DO_0(1);    else TFTLCD_DO_0(0);
	if(cmd & 0x02) TFTLCD_DO_1(1);    else TFTLCD_DO_1(0);
	if(cmd & 0x04) TFTLCD_DO_2(1);    else TFTLCD_DO_2(0);
	if(cmd & 0x08) TFTLCD_DO_3(1);    else TFTLCD_DO_3(0);
	if(cmd & 0x10) TFTLCD_DO_4(1);    else TFTLCD_DO_4(0);
	if(cmd & 0x20) TFTLCD_DO_5(1);    else TFTLCD_DO_5(0);
	if(cmd & 0x40) TFTLCD_DO_6(1);    else TFTLCD_DO_6(0);
	if(cmd & 0x80) TFTLCD_DO_7(1);    else TFTLCD_DO_7(0);
  //дʹ������
  TFTLCD_WR_H;
  //Ƭѡ����
  TFTLCD_CS_H;
	*/
	/********************** ͨ��FSMC������ʽ **********************/
  //TFTLCD_CMD1 = cmd;
  TFTLCD->TFTLCD_CMD = cmd;
}

/***************************************************************************************
*��������  ������оƬ�������ݺ���
*������    ��TFTLCD_ILI9341_Parameter
*��������  ��u16  data   
*��������ֵ��void
*��������  ��	
*         Ƭѡ             TFTLCD_CS_H   TFTLCD_CS_L   
*         дʹ��           TFTLCD_WR_H   TFTLCD_WR_L   
*         ��ʹ��           TFTLCD_RD_H   TFTLCD_RD_L 
*         ��������ѡ����   TFTLCD_DC_H   TFTLCD_DC_L
*         ����             TFTLCD_BL_H   TFTLCD_BL_L  
*****************************************************************************************/
void TFTLCD_ILI9341_Parameter(u16 data)
{
//********************** 8080ͨ�ŷ�ʽ���� **********************//
  /*
  //��ʹ������
  TFTLCD_RD_H;
  //��������ѡ��������
  TFTLCD_DC_H;
  //Ƭѡ����
  TFTLCD_CS_L;
  //дʹ������
  TFTLCD_WR_L;
  //׼��Ҫ���͵�����
  if(data & 0x0001)  TFTLCD_DO_0(1);    else TFTLCD_DO_0(0);
	if(data & 0x0002)  TFTLCD_DO_1(1);    else TFTLCD_DO_1(0);
	if(data & 0x0004)  TFTLCD_DO_2(1);    else TFTLCD_DO_2(0);
	if(data & 0x0008)  TFTLCD_DO_3(1);    else TFTLCD_DO_3(0);
	if(data & 0x0010)  TFTLCD_DO_4(1);    else TFTLCD_DO_4(0);
	if(data & 0x0020)  TFTLCD_DO_5(1);    else TFTLCD_DO_5(0);
	if(data & 0x0040)  TFTLCD_DO_6(1);    else TFTLCD_DO_6(0);
	if(data & 0x0080)  TFTLCD_DO_7(1);    else TFTLCD_DO_7(0);
	if(data & 0x0100)  TFTLCD_DO_8(1);    else TFTLCD_DO_8(0);
	if(data & 0x0200)  TFTLCD_DO_9(1);    else TFTLCD_DO_9(0);
	if(data & 0x0400)  TFTLCD_DO_10(1);   else TFTLCD_DO_10(0);
	if(data & 0x0800)  TFTLCD_DO_11(1);   else TFTLCD_DO_11(0);
	if(data & 0x1000)  TFTLCD_DO_12(1);   else TFTLCD_DO_12(0);
	if(data & 0x2000)  TFTLCD_DO_13(1);   else TFTLCD_DO_13(0);
	if(data & 0x4000)  TFTLCD_DO_14(1);   else TFTLCD_DO_14(0);
	if(data & 0x8000)  TFTLCD_DO_15(1);   else TFTLCD_DO_15(0);
  //дʹ������
  TFTLCD_WR_H;
  //Ƭѡ����
  TFTLCD_CS_H;
	*/
/********************** ͨ��FSMC������ʽ **********************/
  //TFTLCD_DAT1 = data;
  TFTLCD->TFTLCD_DAT = data;
}

/**************************************************************************
*��������  ��TFTLCD��Ļ��������
*������    ��TFTLCD_Clear
*��������  ��u16 colour
*��������ֵ��void
*��������  ��
*         Ƭѡ             TFTLCD_CS_H   TFTLCD_CS_L   
*         дʹ��           TFTLCD_WR_H   TFTLCD_WR_L   
*         ��ʹ��           TFTLCD_RD_H   TFTLCD_RD_L 
*         ��������ѡ����   TFTLCD_DC_H   TFTLCD_DC_L
*         ����             TFTLCD_BL_H   TFTLCD_BL_L  
*         ��������������   0x2a
*         ��������������   0x2b
*         ��ɫ��������     0x2c
*****************************************************************************/
void TFTLCD_Clear(u16 colour)
{
  u32 i;
  
  i = TFTLCD_Long * TFTLCD_Wide;
  
  //��������(x����)
  TFTLCD_ILI9341_CMD(0x2a);
  //д��x��������
  //��ʼx����
  TFTLCD_ILI9341_Parameter(0);
  TFTLCD_ILI9341_Parameter(0);
  //ĩβx����
  TFTLCD_ILI9341_Parameter((TFTLCD_Long-1) >> 8);
  TFTLCD_ILI9341_Parameter(TFTLCD_Long-1);
  
  //��������(y����)
  TFTLCD_ILI9341_CMD(0x2b);
  //д��y��������
  //��ʼy����
  TFTLCD_ILI9341_Parameter(0);
  TFTLCD_ILI9341_Parameter(0);
  //ĩβy����
  TFTLCD_ILI9341_Parameter((TFTLCD_Wide-1) >> 8);
  TFTLCD_ILI9341_Parameter(TFTLCD_Wide-1);
  
  //��������(��ɫ)
  TFTLCD_ILI9341_CMD(0x2c);
  //��ʼ��ɫ
  while(i)
  {
    TFTLCD_ILI9341_Parameter(colour);
    i--;
  }
}

/***************************************************************************************
*��������  ��LCD��Ļ��ʼ������
*������    ��TFTLCD_Init
*��������  ��void   
*��������ֵ��void
*��������  ��
*         Ƭѡ             TFTLCD_CS_H   TFTLCD_CS_L   
*         дʹ��           TFTLCD_WR_H   TFTLCD_WR_L   
*         ��ʹ��           TFTLCD_RD_H   TFTLCD_RD_L 
*         ��������ѡ����   TFTLCD_DC_H   TFTLCD_DC_L
*         ����             TFTLCD_BL_H   TFTLCD_BL_L  
*         ��������������   0x2a
*         ��������������   0x2b
*         ��ɫ��������     0x2c
*****************************************************************************************/
void TFTLCD_Init(void)
{
	/******** 8080ͨ��������ʽ ********/
  /*IO�ڳ�ʼ��*/
	//TFTLCD_IO_Init();
	/******** FSMC������ʽ ********/
  /*IO�ڳ�ʼ����FSMC��ʼ��*/
  FSMC_TFTLCD_Init();
  
  /*TFTLCD���Ҵ���*/
	TFTLCD_ILI9341_CMD(0xCF);  
	TFTLCD_ILI9341_Parameter(0x00); 
	TFTLCD_ILI9341_Parameter(0xC1); 
	TFTLCD_ILI9341_Parameter(0X30); 
	TFTLCD_ILI9341_CMD(0xED);  
	TFTLCD_ILI9341_Parameter(0x64); 
	TFTLCD_ILI9341_Parameter(0x03); 
	TFTLCD_ILI9341_Parameter(0X12); 
	TFTLCD_ILI9341_Parameter(0X81); 
	TFTLCD_ILI9341_CMD(0xE8);  
	TFTLCD_ILI9341_Parameter(0x85); 
	TFTLCD_ILI9341_Parameter(0x10); 
	TFTLCD_ILI9341_Parameter(0x7A); 
	TFTLCD_ILI9341_CMD(0xCB);  
	TFTLCD_ILI9341_Parameter(0x39); 
	TFTLCD_ILI9341_Parameter(0x2C); 
	TFTLCD_ILI9341_Parameter(0x00); 
	TFTLCD_ILI9341_Parameter(0x34); 
	TFTLCD_ILI9341_Parameter(0x02); 
	TFTLCD_ILI9341_CMD(0xF7);  
	TFTLCD_ILI9341_Parameter(0x20); 
	TFTLCD_ILI9341_CMD(0xEA);  
	TFTLCD_ILI9341_Parameter(0x00); 
	TFTLCD_ILI9341_Parameter(0x00); 
	TFTLCD_ILI9341_CMD(0xC0);    //Power control 
	TFTLCD_ILI9341_Parameter(0x1B);   //VRH[5:0] 
	TFTLCD_ILI9341_CMD(0xC1);    //Power control 
	TFTLCD_ILI9341_Parameter(0x01);   //SAP[2:0];BT[3:0] 
	TFTLCD_ILI9341_CMD(0xC5);    //VCM control 
	TFTLCD_ILI9341_Parameter(0x30); 	 //3F
	TFTLCD_ILI9341_Parameter(0x30); 	 //3C
	TFTLCD_ILI9341_CMD(0xC7);    //VCM control2 
	TFTLCD_ILI9341_Parameter(0XB7); 
	TFTLCD_ILI9341_CMD(0x36);    // Memory Access Control 
	TFTLCD_ILI9341_Parameter(0x48); 
	TFTLCD_ILI9341_CMD(0x3A);   
	TFTLCD_ILI9341_Parameter(0x55); 
	TFTLCD_ILI9341_CMD(0xB1);   
	TFTLCD_ILI9341_Parameter(0x00);   
	TFTLCD_ILI9341_Parameter(0x1A); 
	TFTLCD_ILI9341_CMD(0xB6);    // Display Function Control 
	TFTLCD_ILI9341_Parameter(0x0A); 
	TFTLCD_ILI9341_Parameter(0xA2); 
	TFTLCD_ILI9341_CMD(0xF2);    // 3Gamma Function Disable 
	TFTLCD_ILI9341_Parameter(0x00); 
	TFTLCD_ILI9341_CMD(0x26);    //Gamma curve selected 
	TFTLCD_ILI9341_Parameter(0x01); 
	TFTLCD_ILI9341_CMD(0xE0);    //Set Gamma 
	TFTLCD_ILI9341_Parameter(0x0F); 
	TFTLCD_ILI9341_Parameter(0x2A); 
	TFTLCD_ILI9341_Parameter(0x28); 
	TFTLCD_ILI9341_Parameter(0x08); 
	TFTLCD_ILI9341_Parameter(0x0E); 
	TFTLCD_ILI9341_Parameter(0x08); 
	TFTLCD_ILI9341_Parameter(0x54); 
	TFTLCD_ILI9341_Parameter(0XA9); 
	TFTLCD_ILI9341_Parameter(0x43); 
	TFTLCD_ILI9341_Parameter(0x0A); 
	TFTLCD_ILI9341_Parameter(0x0F); 
	TFTLCD_ILI9341_Parameter(0x00); 
	TFTLCD_ILI9341_Parameter(0x00); 
	TFTLCD_ILI9341_Parameter(0x00); 
	TFTLCD_ILI9341_Parameter(0x00); 		 
	TFTLCD_ILI9341_CMD(0XE1);    //Set Gamma 
	TFTLCD_ILI9341_Parameter(0x00); 
	TFTLCD_ILI9341_Parameter(0x15); 
	TFTLCD_ILI9341_Parameter(0x17); 
	TFTLCD_ILI9341_Parameter(0x07); 
	TFTLCD_ILI9341_Parameter(0x11); 
	TFTLCD_ILI9341_Parameter(0x06); 
	TFTLCD_ILI9341_Parameter(0x2B); 
	TFTLCD_ILI9341_Parameter(0x56); 
	TFTLCD_ILI9341_Parameter(0x3C); 
	TFTLCD_ILI9341_Parameter(0x05); 
	TFTLCD_ILI9341_Parameter(0x10); 
	TFTLCD_ILI9341_Parameter(0x0F); 
	TFTLCD_ILI9341_Parameter(0x3F); 
	TFTLCD_ILI9341_Parameter(0x3F); 
	TFTLCD_ILI9341_Parameter(0x0F); 
	TFTLCD_ILI9341_CMD(0x2B); 
	TFTLCD_ILI9341_Parameter(0x00);
	TFTLCD_ILI9341_Parameter(0x00);
	TFTLCD_ILI9341_Parameter(0x01);
	TFTLCD_ILI9341_Parameter(0x3f);
	TFTLCD_ILI9341_CMD(0x2A); 
	TFTLCD_ILI9341_Parameter(0x00);
	TFTLCD_ILI9341_Parameter(0x00);
	TFTLCD_ILI9341_Parameter(0x00);
	TFTLCD_ILI9341_Parameter(0xef);	
	
	TFTLCD_ILI9341_CMD(0x36);//���ƺ�������������xy(0,0)���ĵ������ϡ����ϡ����¡�����
	TFTLCD_ILI9341_Parameter(TFTLCD_Lengthways); //������ʾ��(0,0)�����Ͻ�
//  TFTLCD_ILI9341_Parameter(TFTLCD_Crosswise);
	
	TFTLCD_ILI9341_CMD(0x11); //Exit Sleep
	delay_ms(120);
	TFTLCD_ILI9341_CMD(0x29); //display on	

  
  /*�û��Լ���д*/
	//����ƴ�
	TFTLCD_BL_H;
	//��������
	TFTLCD_Clear(WHITE);
}

/**************************************************************************
*��������  ��LCD��Ļ��ĳ����������ĳ����ɫ
*������    ��TFTLCD_Clear_XY
*��������  ��u16 x,u16 y,u16 w,u16 h,u16 colour  
*��������ֵ��void
*��������  ��
*         Ƭѡ             TFTLCD_CS_H   TFTLCD_CS_L   
*         дʹ��           TFTLCD_WR_H   TFTLCD_WR_L   
*         ��ʹ��           TFTLCD_RD_H   TFTLCD_RD_L 
*         ��������ѡ����   TFTLCD_DC_H   TFTLCD_DC_L
*         ����             TFTLCD_BL_H   TFTLCD_BL_L  
*         ��������������   0x2a
*         ��������������   0x2b
*         ��ɫ��������     0x2c
*****************************************************************************/
void TFTLCD_Clear_XY(u16 x,u16 y,u16 Long,u16 Wide,u16 colour)
{
  u32 i;
  i = Long * Wide;
  //��������(x����)
  TFTLCD_ILI9341_CMD(0x2a);
  //д��x��������
  //��ʼx����
  TFTLCD_ILI9341_Parameter(x>>8);
  TFTLCD_ILI9341_Parameter(x);
  //ĩβx����
  TFTLCD_ILI9341_Parameter((x+Long) >> 8);
  TFTLCD_ILI9341_Parameter(x+Long);
  
  //��������(y����)
  TFTLCD_ILI9341_CMD(0x2b);
  //д��y��������
  //��ʼy����
  TFTLCD_ILI9341_Parameter(y>>8);
  TFTLCD_ILI9341_Parameter(y);
  //ĩβy����
  TFTLCD_ILI9341_Parameter((y+Wide) >> 8);
  TFTLCD_ILI9341_Parameter(y+Wide);
  
  //��������(��ɫ)
  TFTLCD_ILI9341_CMD(0x2c);
  //��ʼ��ɫ
  while(i)
  {
    TFTLCD->TFTLCD_DAT = colour;
    i--;
  }
}

/**************************************************************************
*��������  ��LCD��Ļ���㺯��
*������    ��TFTLCD_Point
*��������  ��u16 x,u16 y,u16 colour 
*��������ֵ��void
*��������  ��
*         Ƭѡ             TFTLCD_CS_H   TFTLCD_CS_L   
*         дʹ��           TFTLCD_WR_H   TFTLCD_WR_L   
*         ��ʹ��           TFTLCD_RD_H   TFTLCD_RD_L 
*         ��������ѡ����   TFTLCD_DC_H   TFTLCD_DC_L
*         ����             TFTLCD_BL_H   TFTLCD_BL_L  
*         ��������������   0x2a
*         ��������������   0x2b
*         ��ɫ��������     0x2c
*****************************************************************************/
void TFTLCD_Point(u16 x,u16 y,u16 colour)
{
  //��������(x����)
  TFTLCD_ILI9341_CMD(0x2a);
  //д��x��������
  //��ʼx����
  TFTLCD_ILI9341_Parameter(x >> 8);
  TFTLCD_ILI9341_Parameter(x);
  //ĩβx����
  TFTLCD_ILI9341_Parameter(x >> 8);
  TFTLCD_ILI9341_Parameter(x);
  
  //��������(y����)
  TFTLCD_ILI9341_CMD(0x2b);
  //д��y��������
  //��ʼy����
  TFTLCD_ILI9341_Parameter(y >> 8);
  TFTLCD_ILI9341_Parameter(y);
  //ĩβy����
  TFTLCD_ILI9341_Parameter(y >> 8);
  TFTLCD_ILI9341_Parameter(y);
  
  //��������(��ɫ)
	TFTLCD->TFTLCD_CMD = 0x2c;
  //��ʼ��ɫ
	TFTLCD->TFTLCD_DAT = colour;
}

u16 LCD_RD_DATA(void)
{
	vu16 ram;			//��ֹ���Ż�
	ram=TFTLCD->TFTLCD_DAT;	
	return ram;	 
}	

void opt_delay(u8 i)
{
	while(i--);
}

/**************************************************************************
*��������  ����ȡ��ĳ�����ɫֵ	 
*������    ��TFTLCD_ReadPoint
*��������  ��u16 x,u16 y
*��������ֵ��u16   �˵����ɫ
*��������  ��
*         ��������������   0x2a
*         ��������������   0x2b
*         ��ɫ��������     0x2c
*****************************************************************************/
u16 TFTLCD_ReadPoint(u16 x,u16 y)
{
	u16 r=0,g=0,b=0;
	if(x>=TFTLCD_Long||y>=TFTLCD_Wide)return 0;	//�����˷�Χ,ֱ�ӷ���
	
	//��������(x����)
  TFTLCD_ILI9341_CMD(0x2a);
  //д��x��������
  TFTLCD_ILI9341_Parameter(x >> 8);TFTLCD_ILI9341_Parameter(x);
  TFTLCD_ILI9341_Parameter(x >> 8);TFTLCD_ILI9341_Parameter(x);
  
  //��������(y����)
  TFTLCD_ILI9341_CMD(0x2b);
  //д��y��������
  TFTLCD_ILI9341_Parameter(y >> 8);TFTLCD_ILI9341_Parameter(y);
  TFTLCD_ILI9341_Parameter(y >> 8);TFTLCD_ILI9341_Parameter(y);
	
	TFTLCD->TFTLCD_CMD = 0X2E;
	
	//������
	r=TFTLCD->TFTLCD_DAT;
	opt_delay(2);	
	r=TFTLCD->TFTLCD_DAT;  		  						//ʵ��������ɫ
	opt_delay(2);
	b=TFTLCD->TFTLCD_DAT;  		  						//ʵ��������ɫ
	g=r&0XFF;		//����9341/5310/5510,��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
	g<<=8;
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));//ILI9341/NT35310/NT35510��Ҫ��ʽת��һ��
}

/******************************************************************************
*��������  ��LCD��Ļ��Բ����(����)
*������    ��TFTLCD_Circle
*��������  ��u16 x,u16 y,u16 r,u16 colour 
*��������ֵ��void
*��������  ��			
*         Ƭѡ             TFTLCD_CS_H   TFTLCD_CS_L   
*         дʹ��           TFTLCD_WR_H   TFTLCD_WR_L   
*         ��ʹ��           TFTLCD_RD_H   TFTLCD_RD_L 
*         ��������ѡ����   TFTLCD_DC_H   TFTLCD_DC_L
*         ����             TFTLCD_BL_H   TFTLCD_BL_L  
*         ��������������   0x2a
*         ��������������   0x2b
*         ��ɫ��������     0x2c
********************************************************************************/
void TFTLCD_Circle(u16 x,u16 y,u16 r,u16 colour)
{
  u16 i,j;
	
	for(i=x-r;i<=x+r;i++)
	{
		for(j=y-r;j<=y+r;j++)
		{
			if((i-x)*(i-x) + (j-y)*(j-y) >= (r)*(r-2) && (i-x)*(i-x) + (j-y)*(j-y) <= r*r)
			{
				TFTLCD_Point(i,j,colour);
			}
		}
	}
}

/******************************************************************************
*��������  ��LCD��Ļ��Բ����(ʵ��Բ)
*������    ��TFTLCD_Circle2
*��������  ��u16 x,u16 y,u16 r,u16 colour  ��ɫ
*��������ֵ��void
*��������  ��
*         Ƭѡ             TFTLCD_CS_H   TFTLCD_CS_L   
*         дʹ��           TFTLCD_WR_H   TFTLCD_WR_L   
*         ��ʹ��           TFTLCD_RD_H   TFTLCD_RD_L 
*         ��������ѡ����   TFTLCD_DC_H   TFTLCD_DC_L
*         ����             TFTLCD_BL_H   TFTLCD_BL_L  
*         ��������������   0x2a
*         ��������������   0x2b
*         ��ɫ��������     0x2c
********************************************************************************/
void TFTLCD_Circle2(u16 x,u16 y,u16 r,u16 colour)
{
	u16 i,j;
	
	for(i=x-r;i<=x+r;i++)
	{
		for(j=y-r;j<=y+r;j++)
		{
			if((i-x)*(i-x) + (j-y)*(j-y)<=r*r)
			{
				TFTLCD_Point(i,j,colour);
			}
		}
	}
}


/******************************************************************************
*��������  ��ʶ����ĻID�ͺ�
*������    ��TFTLCD_Id
*��������  ��void
*��������ֵ��u16
*��������  ��
********************************************************************************/
u16 TFTLCD_Id(void)
{
	u16 id = 0;

	//��ID��9320/9325/9328/4531/4535��IC��
	TFTLCD->TFTLCD_CMD = 0x0000;
	delay_us(5);
	id = TFTLCD->TFTLCD_DAT;
	
	//����ID����ȷ,����lcddev.id==0X9300�жϣ���Ϊ9341��δ����λ������»ᱻ����9300
	if(id<0XFF || id==0XFFFF || id==0X9300)
	{
		//����9341 ID�Ķ�ȡ		
		TFTLCD->TFTLCD_CMD = 0XD3;				   
		id = TFTLCD->TFTLCD_DAT;	  //dummy read 	
 		id = TFTLCD->TFTLCD_DAT;	  //����0X00
  	id = TFTLCD->TFTLCD_DAT;   	//��ȡ93								   
 		id <<= 8;
		id |= TFTLCD->TFTLCD_DAT;  	//��ȡ41 
		
		//��9341,�����ǲ���6804
		if(id != 0X9341)
		{
			TFTLCD->TFTLCD_CMD = 0XBF;
			id = TFTLCD->TFTLCD_DAT; 	//dummy read 	 
	 		id = TFTLCD->TFTLCD_DAT;  //����0X01			   
	 		id = TFTLCD->TFTLCD_DAT; 	//����0XD0 			  	
	  	id = TFTLCD->TFTLCD_DAT;	//�������0X68 
			id <<= 8;
	  	id |= TFTLCD->TFTLCD_DAT;	//�������0X04	  
			
			//Ҳ����6804,���Կ����ǲ���NT35310
			if(id != 0X6804)
			{
				TFTLCD->TFTLCD_CMD = 0XD4;				   
				id = TFTLCD->TFTLCD_DAT;//dummy read  
				id = TFTLCD->TFTLCD_DAT;//����0X01	 
				id = TFTLCD->TFTLCD_DAT;//����0X53	
				id <<= 8;	 
				id |= TFTLCD->TFTLCD_DAT;	//�������0X10	 
				
				//Ҳ����NT35310,���Կ����ǲ���NT35510
				if(id != 0X5310)	
				{
					TFTLCD->TFTLCD_CMD = 0XDA00;	
					id = TFTLCD->TFTLCD_DAT;		//����0X00	 
					TFTLCD->TFTLCD_CMD = 0XDB00;	
					id = TFTLCD->TFTLCD_DAT;		//����0X80
					id <<= 8;	
					TFTLCD->TFTLCD_CMD = 0XDC00;	
					id |= TFTLCD->TFTLCD_DAT;		//����0X00	
					//NT35510���ص�ID��8000H,Ϊ��������,����ǿ������Ϊ5510
					if(id == 0x8000) id = 0x5510;
					
					//Ҳ����NT5510,���Կ����ǲ���SSD1963
					if(id!=0X5510)
					{
						TFTLCD->TFTLCD_CMD = 0XA1;
						id = TFTLCD->TFTLCD_DAT;
						id = TFTLCD->TFTLCD_DAT;	//����0X57
						id<<=8;	 
						id |= TFTLCD->TFTLCD_DAT;	//����0X61
            //SSD1963���ص�ID��5761H,Ϊ��������,����ǿ������Ϊ1963						
						if(id==0X5761)id=0X1963;
					}
				}
			}
		}
	}
	
	return id;
}






