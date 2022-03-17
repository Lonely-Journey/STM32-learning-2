#include "stm32f4xx.h" 
#include "lcd.h"
#include "font.h"
#include "w25q64.h"

//��ʱ��������
static void delay_ms(u32 ms)
{
	u32 i = 168 / 4 * 1000 * ms;   //��ϵͳʱ��Ƶ�ʾ�����
	while(i)
	{
		i--;
	}
}


typedef struct _HEADCOLOR
{
   unsigned char scan;
   unsigned char gray;
   unsigned short w;
   unsigned short h;
   unsigned char is565;
   unsigned char rgb;
}HEADCOLOR;

/****************************************************************************
*��������  ����LCD��Ӧ��GPIO�ڽ��г�ʼ������
*������    ��LCD_IO_Init
*��������  ��void
*��������ֵ��void
*����      ��
      �����ߣ�//ͨ����� ��������   �ٶ�100M
            DO0---PD14	DO1---PD15	DO2---PD0	DO3---PD1
            DO4---PE7	DO5---PE8	DO6---PE9	DO7---PE10
            DO8---PE11	DO9---PE12	DO10---PE13	DO11---PE14
            DO12---PE15	DO13---PD8	DO14---PD9	DO15---PD10
      �����ߣ�//ͨ�����  ����      �ٶ�100M
            WR---PD5	RD---PD4	CS---PG12 	
            RS/DC(����/����)---PF12	 
            BL�����⣩--PB15   
      ����
           PB��15
           PD��0-1  4-5  8-10  14-15  
           PE��7-15
           PF��12
           PG��12           
***************************************************************************/
void LCD_IO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruc;
  
  //ʱ��ʹ��  GPIOB  GPIOD  GPIOE  GPIOF  GPIOG
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	
  /*GPIOx�˿�����*/
  //PB15
  GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_15;          //ѡ��PB15
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_OUT;        //ͨ�����
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;     //��������
  GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;        //�������
  GPIO_InitStruc.GPIO_Speed = GPIO_Speed_100MHz;    //�ٶ�Ϊ100MHz
  GPIO_Init(GPIOB,&GPIO_InitStruc);                 //�����趨������ʼ��PB15
	//PD��0-1  4-5  8-10  14-15 
	GPIO_InitStruc.GPIO_Pin = (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15);  //ѡ��PD0/1/4/5/8/9/10/14/15
	GPIO_Init(GPIOD,&GPIO_InitStruc); //�����趨������ʼ��PD0/1/4/5/8/9/10/14/15
	//PE��7-15
	GPIO_InitStruc.GPIO_Pin = (GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //ѡ��PE��7-15
	GPIO_Init(GPIOE,&GPIO_InitStruc); //�����趨������ʼ��PE��7-15
	//PF12
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_12; //ѡ��PF12
	GPIO_Init(GPIOF,&GPIO_InitStruc);      //�����趨������ʼ��PF12
	//PG12
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_12; //ѡ��PG12
	GPIO_Init(GPIOG,&GPIO_InitStruc);      //�����趨������ʼ��PG12
	
	//Ƭѡ���ߣ�����
  LCD_CS_H;
  //��������ѡ�������ߡ�����
  LCD_DC_H;
  //дʹ������
  LCD_WR_H ;
  //��ʹ������
  LCD_RD_H;
  //�ر���
  LCD_BL_L;  
}

/***************************************************************************************
*��������  ������оƬ���������
*������    ��LCD_ILI9486_CMD
*��������  ��u8 cmd    
*��������ֵ��void
*��������  ��  
*         Ƭѡ             LCD_CS_H   LCD_CS_L   
*         дʹ��           LCD_WR_H   LCD_WR_L   
*         ��ʹ��           LCD_RD_H   LCD_RD_L 
*         ��������ѡ����   LCD_DC_H   LCD_DC_L
*         ����             LCD_BL_H   LCD_BL_L  
*****************************************************************************************/
void LCD_ILI9486_CMD(u8 cmd)
{
  //Ƭѡ��
  LCD_CS_L;
  //��������ѡ��������
  LCD_DC_L;
  //��ʹ������
  LCD_RD_H;
  //дʹ������
  LCD_WR_L;
 
  //׼������
	if(cmd & 0x01) LCD_DO_0(1);    else LCD_DO_0(0);
	if(cmd & 0x02) LCD_DO_1(1);    else LCD_DO_1(0);
	if(cmd & 0x04) LCD_DO_2(1);    else LCD_DO_2(0);
	if(cmd & 0x08) LCD_DO_3(1);    else LCD_DO_3(0);
	if(cmd & 0x10) LCD_DO_4(1);    else LCD_DO_4(0);
	if(cmd & 0x20) LCD_DO_5(1);    else LCD_DO_5(0);
	if(cmd & 0x40) LCD_DO_6(1);    else LCD_DO_6(0);
	if(cmd & 0x80) LCD_DO_7(1);    else LCD_DO_7(0);
  //дʹ������
  LCD_WR_H;
  //Ƭѡ����
  LCD_CS_H;
}

/***************************************************************************************
*��������  ������оƬ�������ݺ���
*������    ��LCD_ILI9486_Parameter
*��������  ��u16  data   
*��������ֵ��void
*��������  ��	
*         Ƭѡ             LCD_CS_H   LCD_CS_L   
*         дʹ��           LCD_WR_H   LCD_WR_L   
*         ��ʹ��           LCD_RD_H   LCD_RD_L 
*         ��������ѡ����   LCD_DC_H   LCD_DC_L
*         ����             LCD_BL_H   LCD_BL_L  
*****************************************************************************************/
void LCD_ILI9486_Parameter(u16 data)
{
  //Ƭѡ����
	LCD_CS_L;
  //��������ѡ������
	LCD_DC_H;    
  //��ʹ��
	LCD_RD_H;
  //дʹ������
	LCD_WR_L;

	//׼��16λ���ݵ�������
	if(data & 0x0001)  LCD_DO_0(1);    else LCD_DO_0(0);
	if(data & 0x0002)  LCD_DO_1(1);    else LCD_DO_1(0);
	if(data & 0x0004)  LCD_DO_2(1);    else LCD_DO_2(0);
	if(data & 0x0008)  LCD_DO_3(1);    else LCD_DO_3(0);
	if(data & 0x0010)  LCD_DO_4(1);    else LCD_DO_4(0);
	if(data & 0x0020)  LCD_DO_5(1);    else LCD_DO_5(0);
	if(data & 0x0040)  LCD_DO_6(1);    else LCD_DO_6(0);
	if(data & 0x0080)  LCD_DO_7(1);    else LCD_DO_7(0);
	if(data & 0x0100)  LCD_DO_8(1);    else LCD_DO_8(0);
	if(data & 0x0200)  LCD_DO_9(1);    else LCD_DO_9(0);
	if(data & 0x0400)  LCD_DO_10(1);   else LCD_DO_10(0);
	if(data & 0x0800)  LCD_DO_11(1);   else LCD_DO_11(0);
	if(data & 0x1000)  LCD_DO_12(1);   else LCD_DO_12(0);
	if(data & 0x2000)  LCD_DO_13(1);   else LCD_DO_13(0);
	if(data & 0x4000)  LCD_DO_14(1);   else LCD_DO_14(0);
	if(data & 0x8000)  LCD_DO_15(1);   else LCD_DO_15(0);
	
	//дʹ������
	LCD_WR_H;
	//Ƭѡ����
	LCD_CS_H;
}

/**************************************************************************
*��������  ��LCD��Ļ��������
*������    ��LCD_Clear
*��������  ��u16 colour
*��������ֵ��void
*��������  ��
*         Ƭѡ             LCD_CS_H   LCD_CS_L   
*         дʹ��           LCD_WR_H   LCD_WR_L   
*         ��ʹ��           LCD_RD_H   LCD_RD_L 
*         ��������ѡ����   LCD_DC_H   LCD_DC_L
*         ����             LCD_BL_H   LCD_BL_L  
*         ��������������   0x2a
*         ��������������   0x2b
*         ��ɫ��������     0x2c
*****************************************************************************/
void LCD_Clear(u16 colour)
{
	u32 i;
	i = LCD_Long * LCD_Wide;
	
	/*ȷ��������*/
	//���ͺ���������(X)  0x2A
	LCD_ILI9486_CMD(0x2A);
	//��ʼ������
	LCD_ILI9486_Parameter(0);      //���͸߰�λ
	LCD_ILI9486_Parameter(0);           //���͵Ͱ�λ
	//����������
	LCD_ILI9486_Parameter((LCD_Long-1) >> 8); //���͸߰�λ
	LCD_ILI9486_Parameter(LCD_Long-1);        //���͵Ͱ�λ
	
	/*ȷ��������*/    
	//��������������(Y)  0x2B
	LCD_ILI9486_CMD(0x2B);
	//��ʼ������
	LCD_ILI9486_Parameter(0 >> 8);      //���͸߰�λ
	LCD_ILI9486_Parameter(0);           //���͵Ͱ�λ
	//����������
	LCD_ILI9486_Parameter((LCD_Wide-1) >> 8); //���͸߰�λ
	LCD_ILI9486_Parameter(LCD_Wide-1);        //���͵Ͱ�λ
	
	/*ȷ����ɫ*/
	//������ɫ����
	LCD_ILI9486_CMD(0x2C);
	//������ɫ����
	while(i)
	{
		LCD_ILI9486_Parameter(colour);
		i--;
	}
}

/***************************************************************************************
*��������  ��LCD��Ļ��ʼ������
*������    ��LCD_Init
*��������  ��void   
*��������ֵ��void
*��������  ��
*         Ƭѡ             LCD_CS_H   LCD_CS_L   
*         дʹ��           LCD_WR_H   LCD_WR_L   
*         ��ʹ��           LCD_RD_H   LCD_RD_L 
*         ��������ѡ����   LCD_DC_H   LCD_DC_L
*         ����             LCD_BL_H   LCD_BL_L  
*         ��������������   0x2a
*         ��������������   0x2b
*         ��ɫ��������     0x2c
*****************************************************************************************/
void LCD_Init(void)
{
	/*IO�ڳ�ʼ��*/
	LCD_IO_Init();
  
  /*LCD���Ҵ���*/
	delay_ms(120); // ��ʱ120ms
	//************* Start Initial Sequence **********//
	LCD_ILI9486_CMD(0XF2);//
	LCD_ILI9486_Parameter(0x18);
	LCD_ILI9486_Parameter(0xA3);
	LCD_ILI9486_Parameter(0x12);
	LCD_ILI9486_Parameter(0x02);
	LCD_ILI9486_Parameter(0XB2);
	LCD_ILI9486_Parameter(0x12);
	LCD_ILI9486_Parameter(0xFF);
	LCD_ILI9486_Parameter(0x10);
	LCD_ILI9486_Parameter(0x00);
	LCD_ILI9486_CMD(0XF8);
	LCD_ILI9486_Parameter(0x21);
	LCD_ILI9486_Parameter(0x04);
	LCD_ILI9486_CMD(0XF9);
	LCD_ILI9486_Parameter(0x00);
	LCD_ILI9486_Parameter(0x08);
	LCD_ILI9486_CMD(0x36);             
	LCD_ILI9486_Parameter(0x08); //�� 0=RGB color filter panel, 1=BGR color filter panel)
	LCD_ILI9486_CMD(0x3A);       //����16λBPP
	LCD_ILI9486_Parameter(0x55);   
	LCD_ILI9486_CMD(0xB4);
	LCD_ILI9486_Parameter(0x01);
	LCD_ILI9486_CMD(0xB6);
	LCD_ILI9486_Parameter(0x02);
	LCD_ILI9486_Parameter(0x22);
	LCD_ILI9486_CMD(0xC1);
	LCD_ILI9486_Parameter(0x41);
	LCD_ILI9486_CMD(0xC5);
	LCD_ILI9486_Parameter(0x00);
	LCD_ILI9486_Parameter(0x07);
	LCD_ILI9486_CMD(0xE0);
	LCD_ILI9486_Parameter(0x0F);
	LCD_ILI9486_Parameter(0x1F);
	LCD_ILI9486_Parameter(0x1C);
	LCD_ILI9486_Parameter(0x0C);
	LCD_ILI9486_Parameter(0x0F);
	LCD_ILI9486_Parameter(0x08);
	LCD_ILI9486_Parameter(0x48);
	LCD_ILI9486_Parameter(0x98);
	LCD_ILI9486_Parameter(0x37);
	LCD_ILI9486_Parameter(0x0A);
	LCD_ILI9486_Parameter(0x13);
	LCD_ILI9486_Parameter(0x04);
	LCD_ILI9486_Parameter(0x11);
	LCD_ILI9486_Parameter(0x0D);
	LCD_ILI9486_Parameter(0x00);
	LCD_ILI9486_CMD(0xE1);
	LCD_ILI9486_Parameter(0x0F);
	LCD_ILI9486_Parameter(0x32);
	LCD_ILI9486_Parameter(0x2E);
	LCD_ILI9486_Parameter(0x0B);
	LCD_ILI9486_Parameter(0x0D);
	LCD_ILI9486_Parameter(0x05);
	LCD_ILI9486_Parameter(0x47);
	LCD_ILI9486_Parameter(0x75);
	LCD_ILI9486_Parameter(0x37);
	LCD_ILI9486_Parameter(0x06);
	LCD_ILI9486_Parameter(0x10);
	LCD_ILI9486_Parameter(0x03);
	LCD_ILI9486_Parameter(0x24);
	LCD_ILI9486_Parameter(0x20);
	LCD_ILI9486_Parameter(0x00);
	LCD_ILI9486_CMD(0x11);   //�˳�˯��
	delay_ms(120);
	LCD_ILI9486_CMD(0x29);   //������ʾ
	
	/*�û��Լ���д*/
	//����ƴ�
	LCD_BL_H;
	//��������
	LCD_Clear(WHITE);
}

/**************************************************************************
*��������  ��LCD��Ļ��ĳ����������ĳ����ɫ
*������    ��LCD_Clear_XY
*��������  ��u16 x,u16 y,u16 w,u16 h,u16 colour  
*��������ֵ��void
*��������  ��
*         Ƭѡ             LCD_CS_H   LCD_CS_L   
*         дʹ��           LCD_WR_H   LCD_WR_L   
*         ��ʹ��           LCD_RD_H   LCD_RD_L 
*         ��������ѡ����   LCD_DC_H   LCD_DC_L
*         ����             LCD_BL_H   LCD_BL_L  
*         ��������������   0x2a
*         ��������������   0x2b
*         ��ɫ��������     0x2c
*****************************************************************************/
void LCD_Clear_XY(u16 x,u16 y,u16 Long,u16 Wide,u16 colour)
{
  u32 i;
	i = Long * Wide;
	
	/*ȷ��������*/
	//���ͺ���������(X)  0x2A
	LCD_ILI9486_CMD(0x2A);
	//��ʼ������
	LCD_ILI9486_Parameter(x >> 8);          //���͸߰�λ
	LCD_ILI9486_Parameter(x);               //���͵Ͱ�λ
	//����������
	LCD_ILI9486_Parameter((x+Long-1) >> 8); //���͸߰�λ
	LCD_ILI9486_Parameter(x+Long-1);        //���͵Ͱ�λ
	
	/*ȷ��������*/    
	//��������������(Y)  0x2B
	LCD_ILI9486_CMD(0x2B);
	//��ʼ������
	LCD_ILI9486_Parameter(y>>8);            //���͸߰�λ
  LCD_ILI9486_Parameter(y);               //���͵Ͱ�λ
	//����������
	LCD_ILI9486_Parameter((y+Wide-1) >> 8); //���͸߰�λ
	LCD_ILI9486_Parameter(y+Wide-1);        //���͵Ͱ�λ
	
	/*ȷ����ɫ*/
	//������ɫ����
	LCD_ILI9486_CMD(0x2C);
	//������ɫ����
	while(i)
	{
		LCD_ILI9486_Parameter(colour);
		i--;
	}
}

/**************************************************************************
*��������  ��LCD��Ļ���㺯��
*������    ��LCD_Point
*��������  ��u16 x,u16 y,u16 colour 
*��������ֵ��void
*��������  ��
*         Ƭѡ             LCD_CS_H   LCD_CS_L   
*         дʹ��           LCD_WR_H   LCD_WR_L   
*         ��ʹ��           LCD_RD_H   LCD_RD_L 
*         ��������ѡ����   LCD_DC_H   LCD_DC_L
*         ����             LCD_BL_H   LCD_BL_L  
*         ��������������   0x2a
*         ��������������   0x2b
*         ��ɫ��������     0x2c
*****************************************************************************/
void LCD_Point(u16 x,u16 y,u16 colour)
{
  /*ȷ��x*/
  //����x��������
	LCD_ILI9486_CMD(0x2A);
	
  //����x��������
	LCD_ILI9486_Parameter(x>>8);
	LCD_ILI9486_Parameter(x);
	LCD_ILI9486_Parameter(x>>8);
	LCD_ILI9486_Parameter(x);
	
	/*ȷ��y*/
  //����y��������
	LCD_ILI9486_CMD(0x2B);
	//����y�������ݼ�
	LCD_ILI9486_Parameter(y>>8);
	LCD_ILI9486_Parameter(y);
	LCD_ILI9486_Parameter(y>>8);
	LCD_ILI9486_Parameter(y);
	
	/*ȷ����ɫ*/
	//������ɫ����
	LCD_ILI9486_CMD(0x2C);
	//������ɫ����
	LCD_ILI9486_Parameter(colour); 
}

/******************************************************************************
*��������  ��LCD��Ļ��Բ����(����)
*������    ��LCD_Circle
*��������  ��u16 x,u16 y,u16 r,u16 colour 
*��������ֵ��void
*��������  ��			
*         Ƭѡ             LCD_CS_H   LCD_CS_L   
*         дʹ��           LCD_WR_H   LCD_WR_L   
*         ��ʹ��           LCD_RD_H   LCD_RD_L 
*         ��������ѡ����   LCD_DC_H   LCD_DC_L
*         ����             LCD_BL_H   LCD_BL_L  
*         ��������������   0x2a
*         ��������������   0x2b
*         ��ɫ��������     0x2c
********************************************************************************/
void LCD_Circle1(u16 x,u16 y,u16 r,u16 colour)
{
	u16 i,j;
	
	for(i=x-r;i<=x+r;i++)
	{
		for(j=y-r;j<=y+r;j++)
		{
			if((i-x)*(i-x) + (j-y)*(j-y) >= (r)*(r-2) && (i-x)*(i-x) + (j-y)*(j-y) <= r*r)
			{
				LCD_Point(i,j,colour);
			}
		}
	}
}

/******************************************************************************
*��������  ��LCD��Ļ��Բ����(ʵ��Բ)
*������    ��LCD_Circle2
*��������  ��u16 x,u16 y,u16 r,u16 colour  ��ɫ
*��������ֵ��void
*��������  ��
*         Ƭѡ             LCD_CS_H   LCD_CS_L   
*         дʹ��           LCD_WR_H   LCD_WR_L   
*         ��ʹ��           LCD_RD_H   LCD_RD_L 
*         ��������ѡ����   LCD_DC_H   LCD_DC_L
*         ����             LCD_BL_H   LCD_BL_L  
*         ��������������   0x2a
*         ��������������   0x2b
*         ��ɫ��������     0x2c
********************************************************************************/
void LCD_Circle2(u16 x,u16 y,u16 r,u16 colour)
{
	u16 i,j;
	
	for(i=x-r;i<=x+r;i++)
	{
		for(j=y-r;j<=y+r;j++)
		{
			if((i-x)*(i-x) + (j-y)*(j-y)<=r*r)
			{
				LCD_Point(i,j,colour);
			}
		}
	}
}

/******************************************************************************
*��������  ��LCD��Ļ��ʾһ��16*16�ַ�
*������    ��LCD_ Character16
*��������  ��u16 x,u16 y,u8 ch,u16 colour 
*��������ֵ��void
*��������  ��					
********************************************************************************/
void LCD_Character16(u16 x,u16 y,u8 ch,u16 colour)
{
  u8 n;
	u8 i,j;
	u8 temp;
	
	/*�����Ҫ��ʾ���ַ���ո��ַ���ƫ��*/
	n = ch - ' ';
	
	/*��ʾ*/
	for(i=0;i<16;i++)
	{
		temp = model16[n*16+i];
		for(j=0;j<8;j++)
		{
			if(temp & (0x80 >> j))
			{
				LCD_Point(x+j,y+i,colour);
			}
		}
	}
}

/******************************************************************************
*��������  ��LCD��Ļ��ʾһ��24*24�ַ�
*������    ��LCD_ Character24
*��������  ��u16 x,u16 y,u8 ch,u16 colour 
*��������ֵ��void
*��������  ��					
********************************************************************************/
void LCD_Character24(u16 x,u16 y,u8 ch,u16 colour)
{
  u8 n;
	u8 i,j;
	u16 temp;
  u8 temp1;
  u8 temp2;
	
	/*�����Ҫ��ʾ���ַ���ո��ַ���ƫ��*/
	n = ch - ' ';
	
	/*��ʾ*/
	for(i=0;i<24;i++)
	{
		temp1 = model24[n*48+i*2];
    temp2 = model24[n*48+i*2+1];
    temp = (temp1<<4) | (temp2>>4);
		for(j=0;j<12;j++)
		{
			if(temp & (0x800 >> j))
			{
        LCD_Point(x+j,y+i,colour);
			}
		}
	}
}

/******************************************************************************
*��������  ��LCD��Ļ��ʾһ��32*32�ַ�
*������    ��LCD_ Character32
*��������  ��u16 x,u16 y,u8 ch,u16 colour 
*��������ֵ��void
*��������  ��					
********************************************************************************/
void LCD_Character32(u16 x,u16 y,u8 ch,u16 colour)
{
  u8 n;
	u8 i,j;
	u16 temp;
  u8 temp1;
  u8 temp2;
	
	/*�����Ҫ��ʾ���ַ���ո��ַ���ƫ��*/
	n = ch - ' ';
	
	/*��ʾ*/
	for(i=0;i<32;i++)
	{
		temp1 = model32[n*64+i*2];
    temp2 = model32[n*64+i*2+1];
    temp = (temp1<<8) | temp2;
		for(j=0;j<16;j++)
		{
			if(temp & (0x8000 >> j))
			{
        LCD_Point(x+j,y+i,colour);
			}
		}
	}
}

/******************************************************************************
*��������  ��LCD��Ļ��ʾһ��16*16�ַ���
*������    ��LCD_String16
*��������  ��u16 x,u16 y,u8 *str,u16 colour 
*��������ֵ��void
*��������  ��					
********************************************************************************/
void LCD_String16(u16 x,u16 y,u8 *str,u16 colour)
{
  while(*str != '\0')
  {
    LCD_Character16(x,y,*str,colour);
    x += 8;
    str++;
    if(x+8>LCD_Long)
    {
      x = 0;
      y += 16;
    }
  }
}

/******************************************************************************
*��������  ��LCD��Ļ��ʾһ��24*24�ַ���
*������    ��LCD_String24
*��������  ��u16 x,u16 y,u8 *str,u16 colour 
*��������ֵ��void
*��������  ��					
********************************************************************************/
void LCD_String24(u16 x,u16 y,u8 *str,u16 colour)
{
  while(*str != '\0')
  {
    LCD_Character24(x,y,*str,colour);
    x += 12;
    str++;
    if(x+12>LCD_Long)
    {
      x = 0;
      y += 24;
    }
  }
}

/******************************************************************************
*��������  ��LCD��Ļ��ʾһ��32*32�ַ���
*������    ��LCD_String32
*��������  ��u16 x,u16 y,u8 *str,u16 colour 
*��������ֵ��void
*��������  ��					
********************************************************************************/
void LCD_String32(u16 x,u16 y,u8 *str,u16 colour)
{
  while(*str != '\0')
  {
    LCD_Character32(x,y,*str,colour);
    x += 16;
    str++;
    if(x+16>LCD_Long)
    {
      x = 0;
      y += 32;
    }
  }
}
/******************************************************************************
*��������  ��LCD��Ļ��ʾһ��32*32����
*������    ��LCD_Characters_Single32
*��������  ��u16 x,u16 y,u8 *ch,u16 colour
*��������ֵ��void
*��������  ��					
********************************************************************************/
void LCD_Characters_Single32(u16 x,u16 y,u8 *ch,u16 colour)
{
  u8 n = 0;
	u8 i,j,k;
	u32 temp=0;

  
  while(1)
  {
    if((*ch == query[n*2])&&(*(ch+1)==query[n*2+1]))
    {
      break;
    }
    n++;
  }
  for(i=0;i<32;i++)
  {
    for(k=0;k<4;k++)
    {
      temp<<=8;
      temp |= hanzi[16*8*n+k+i*4];
    }
    for(j=0;j<32;j++)
		{
			if(temp & (0x80000000 >> j))
			{
        LCD_Point(x+j,y+i,colour);
			}
		}
  }
}

/******************************************************************************
*��������  ��LCD��Ļ��ʾһ��32*32�����ַ���
*������    ��LCD_Characters_String32
*��������  ��u16 x,u16 y,u8 *str,u16 colour 
*��������ֵ��void
*��������  ��					
********************************************************************************/
void LCD_Characters_String32(u16 x,u16 y,u8 *str,u16 colour)
{
  while(*str != '\0')
  {
    LCD_Characters_Single32(x,y,str,colour);
    x += 32;
    str+=2;
    if(x+16>LCD_Long)
    {
      x = 0;
      y += 32;
    }
  }
}

/******************************************************************************
*��������  ��LCD��Ļ��ʾһ��ͼƬ(ʹ�����λ��ǰ��ͼƬģ����ǣ�����tu.c)
*������    ��LCD_Picture
*��������  ��u16 x,u16 y,u8 *buf 
*��������ֵ��void
*��������  ��					
********************************************************************************/
void LCD_Picture(u16 x,u16 y,u8 *buf)
{
  u32 i;
  
  HEADCOLOR *p;
  u16 *k;
  
  p = (HEADCOLOR *)buf;
  
  k = (u16 *)(buf+8);  //ǿ��ת�������и������ע�⣺ǿת�ǽ���һλ����λ��ǰһλ����λ
  i = p->w * p->h;     //���磺u8 str[2]={0x22,0x32}----ǿת----->u16 a = 0x3222
	                     //���磺u16 a = 0x1234  ------ǿת------>u8 *a    ��ӡ������˳���� 0x34  0x12
	/*ȷ��������*/
	//���ͺ���������(X)  0x2A
	LCD_ILI9486_CMD(0x2A);
	//��ʼ������
	LCD_ILI9486_Parameter(x >> 8);          //���͸߰�λ
	LCD_ILI9486_Parameter(x);               //���͵Ͱ�λ
	//����������
	LCD_ILI9486_Parameter((x+(p->w)-1) >> 8); //���͸߰�λ
	LCD_ILI9486_Parameter(x+(p->w)-1);        //���͵Ͱ�λ
	
	/*ȷ��������*/    
	//��������������(Y)  0x2B
	LCD_ILI9486_CMD(0x2B);
	//��ʼ������
	LCD_ILI9486_Parameter(y>>8);            //���͸߰�λ
  LCD_ILI9486_Parameter(y);               //���͵Ͱ�λ
	//����������
	LCD_ILI9486_Parameter((y+(p->h)-1) >> 8); //���͸߰�λ
	LCD_ILI9486_Parameter(y+(p->h)-1);        //���͵Ͱ�λ
	
	/*ȷ����ɫ*/
	//������ɫ����
	LCD_ILI9486_CMD(0x2C);
	//������ɫ���� 
	while(i)
	{
    LCD_ILI9486_Parameter(*k);
    k++;
    i--;
	}
}

/******************************************************************************
*��������  ��LCD��Ļ��ʾһ��ͼƬ(ʹ�����λ��ǰ��ͼƬģ����ǣ�����tu2.c)
*������    ��LCD_Picture2
*��������  ��u16 x,u16 y,u8 *buf 
*��������ֵ��void
*��������  ��					
********************************************************************************/
void LCD_Picture2(u16 x,u16 y,u8 *buf)
{
  u32 i;
  u32 k = 0;
  
  i = 320 * 480;
	
	/*ȷ��������*/
	//���ͺ���������(X)  0x2A
	LCD_ILI9486_CMD(0x2A);
	//��ʼ������
	LCD_ILI9486_Parameter(x >> 8);          //���͸߰�λ
	LCD_ILI9486_Parameter(x);               //���͵Ͱ�λ
	//����������
	LCD_ILI9486_Parameter((x+(320)-1) >> 8); //���͸߰�λ
	LCD_ILI9486_Parameter(x+(320)-1);        //���͵Ͱ�λ
	
	/*ȷ��������*/    
	//��������������(Y)  0x2B
	LCD_ILI9486_CMD(0x2B);
	//��ʼ������
	LCD_ILI9486_Parameter(y>>8);            //���͸߰�λ
  LCD_ILI9486_Parameter(y);               //���͵Ͱ�λ
	//����������
	LCD_ILI9486_Parameter((y+(480)-1) >> 8); //���͸߰�λ
	LCD_ILI9486_Parameter(y+(480)-1);        //���͵Ͱ�λ
	
	/*ȷ����ɫ*/
	//������ɫ����
	LCD_ILI9486_CMD(0x2C);
	//������ɫ���� 
	while(i)
	{
    LCD_ILI9486_Parameter((buf[k*2]<<8)|(buf[k*2+1]));
    k++;
    i--;
	}
}

/******************************************************************************
*��������  ��LCD��Ļ��ʾһ��16*16�ַ�/����
*������    ��LCD_Show_HZ16
*��������  ��u16 x,u16 y,u8 *str,u16 colour 
*��������ֵ��void
*��������  ��					
********************************************************************************/
void LCD_Show_HZ16(u16 x,u16 y,u8 *str,u16 colour,u16 backdrop)
{
  u32 n;
  u32 flash_addr;
  u8 model_ch[16];
  u8 model_hz[32];
  u8 i,j,temp1;
  u16 temp2;
  u32 H,L;  //���롢λ��
  
  while(*str != '\0')
  {
    if((32<=*str) && (*str<=127))
    {
      n = *str - 0;
      flash_addr = 0x0b0000 + 0 + n * 16;
      W25Q64_Read_Bytes(flash_addr,16,model_ch);
      for(i=0;i<16;i++)
      {
        temp1 = model_ch[i];
        for(j=0;j<8;j++)
        {
          if(temp1 & (0x80>>j))
          {
            LCD_Point(x+j,y+i,colour);
          }
          else
          {
            LCD_Point(x+j,y+i,backdrop);
          }
        }
      }
      x += 8;
      str++;
      if(x > (LCD_Wide-1-8))
			{
				x=0;
				y+=16;
			}
    }
    else
    {
      H = (*str - 0xa1) * 94;
      L = *(str + 1) - 0xa1;
      n = H + L;
      flash_addr = 0x0b0000 + 0x0000280C + n * 32;
      W25Q64_Read_Bytes(flash_addr,32,model_hz);
      for(i=0;i<16;i++)
      {
        temp2 = (model_hz[i*2]<<8) | (model_hz[i*2+1]);
        for(j=0;j<16;j++)
        {
          if(temp2 & 0x8000>>j)
          {
            LCD_Point(x+j,y+i,colour);
          }
          else
          {
            LCD_Point(x+j,y+i,backdrop);
          }
        }
      }
      x += 16;
      str += 2;
      if(x > (LCD_Wide-1-16))
			{
				x=0;
				y+=16;
			}
    }
  }
}

/***********************************************************
*��������  ��LCD��Ļ��ʾһ��32*32�ַ�/����
*������    ��LCD_Show_HZ16
*��������  ��u16 x,u16 y,u8 *str,u16 colour 
*��������ֵ��void
*��������  ��					
*************************************************************/
void LCD_Show_HZ32(u16 x,u16 y,u8 *str,u16 colour,u16 backdrop)
{
  u32 n;
  u32 flash_addr;
  u8 model_ch[64];
  u8 model_hz[128];
  u8 i,j,k;
  u16 temp1;
  u32 temp2;
  u32 H,L;  //���롢λ��
  
  while(*str != '\0')
  {
    if((32<=*str) && (*str<=127))
    {
      n = *str - 0;
      flash_addr = 0x0b0000 + 0x00000806 + n * 64;
      W25Q64_Read_Bytes(flash_addr,64,model_ch);
      for(i=0;i<32;i++)
      {
        temp1 = (model_ch[i*2]<<8) | (model_ch[i*2+1]);
        for(j=0;j<16;j++)
        {
          if(temp1 & (0x8000>>j))
          {
            LCD_Point(x+j,y+i,colour);
          }
          else
          {
            LCD_Point(x+j,y+i,backdrop);
          }
        }
      }
      x += 16;
      str++;
      if(x > (LCD_Wide-1-16))
			{
				x=0;
				y+=32;
			}
    }
    else
    {
      H = (*str - 0xa1) * 94;
      L = *(str + 1) - 0xa1;
      n = H + L;
      flash_addr = 0x0b0000 + 0x00042652 + n * 128;
      W25Q64_Read_Bytes(flash_addr,128,model_hz);
      for(i=0;i<32;i++)
      {
        for(k=0;k<4;k++)
        {
          temp2 <<= 8;
          temp2 |= model_hz[k+i*4];
        }
        for(j=0;j<32;j++)
        {
          if(temp2 & 0x80000000>>j)
          {
            LCD_Point(x+j,y+i,colour);
          }
          else
          {
            LCD_Point(x+j,y+i,backdrop);
          }
        }
      }
      x += 32;
      str += 2;
      if(x > (LCD_Wide-1-32))
			{
				x=0;
				y+=32;
			}
    }
  }
}

















