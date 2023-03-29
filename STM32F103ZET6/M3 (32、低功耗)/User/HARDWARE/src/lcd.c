#include "stm32f10x.h"
#include "lcd.h"
#include "delay.h"
#include "fsmc.h"

//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;
	

/***************************************************************************************
*��������  ������оƬ���������
*������    ��LCD_ILI9341_CMD
*��������  ��u8 cmd    
*��������ֵ��void
*��������  ��  
*****************************************************************************************/
void LCD_ILI9341_CMD(u8 cmd)
{
	/********************** ͨ��FSMC������ʽ **********************/
  //LCD_CMD1 = cmd;
  LCD->LCD_CMD = cmd;
}

/***************************************************************************************
*��������  ������оƬ�������ݺ���
*������    ��LCD_ILI9341_Parameter
*��������  ��u16  data   
*��������ֵ��void
*��������  ��	
*****************************************************************************************/
void LCD_ILI9341_Parameter(u16 data)
{
/********************** ͨ��FSMC������ʽ **********************/
  //LCD_DAT1 = data;
  LCD->LCD_DAT = data;
}

/***************************************************************************************
*��������  ������оƬ��ȡ���ݺ���
*������    ��LCD_ILI9341_ReadData
*��������  ��void   
*��������ֵ��u16
*��������  ��  
*****************************************************************************************/
u16 LCD_ILI9341_ReadData(void)
{
	return LCD->LCD_DAT;
}

/**************************************************************************
*��������  ��LCD��Ļ��������
*������    ��LCD_Clear
*��������  ��u16 colour
*��������ֵ��void
*��������  ��
*         ��������������   0x2a
*         ��������������   0x2b
*         ��ɫ��������     0x2c
*****************************************************************************/
void LCD_Clear(u16 colour)
{
  u32 i;
  
  i = lcddev.width * lcddev.height;
   
  //��������(x����)
  LCD_ILI9341_CMD(lcddev.setxcmd);
  //д��x��������
  //��ʼx����
  LCD_ILI9341_Parameter(0);
  LCD_ILI9341_Parameter(0);
  
  //��������(y����)
  LCD_ILI9341_CMD(lcddev.setycmd);
  //д��y��������
  //��ʼy����
  LCD_ILI9341_Parameter(0);
  LCD_ILI9341_Parameter(0);
  
  //��������(��ɫ)
  LCD_ILI9341_CMD(lcddev.wramcmd);
  //��ʼ��ɫ
  while(i)
  {
    LCD->LCD_DAT = colour;
    i--;
  }
}

/**************************************************************************
*��������  ������LCD���Զ�ɨ�跽��
*������    ��LCD_Scan_Dir
*��������  ��u8 dir
*��������ֵ��void
*��������  ��
*****************************************************************************/
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u16 dirreg=0;
	u16 temp;
	
	if(lcddev.dir==1)//����ʱ����
	{			   
		switch(dir)//����ת��
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;	     
		}
	}
	
	switch(dir)
	{
		case L2R_U2D://������,���ϵ���
			regval|=(0<<7)|(0<<6)|(0<<5); 
			break;
		case L2R_D2U://������,���µ���
			regval|=(1<<7)|(0<<6)|(0<<5); 
			break;
		case R2L_U2D://���ҵ���,���ϵ���
			regval|=(0<<7)|(1<<6)|(0<<5); 
			break;
		case R2L_D2U://���ҵ���,���µ���
			regval|=(1<<7)|(1<<6)|(0<<5); 
			break;	 
		case U2D_L2R://���ϵ���,������
			regval|=(0<<7)|(0<<6)|(1<<5); 
			break;
		case U2D_R2L://���ϵ���,���ҵ���
			regval|=(0<<7)|(1<<6)|(1<<5); 
			break;
		case D2U_L2R://���µ���,������
			regval|=(1<<7)|(0<<6)|(1<<5); 
			break;
		case D2U_R2L://���µ���,���ҵ���
			regval|=(1<<7)|(1<<6)|(1<<5); 
			break;	 
	}
	
	dirreg=0X36;
	regval|=0X08;
	LCD->LCD_CMD = dirreg;
	LCD->LCD_DAT = regval;
	
	//����Ļ���������
	if(regval&0X20)
	{
		if(lcddev.width<lcddev.height)//����X,Y
		{
			temp=lcddev.width;
			lcddev.width=lcddev.height;
			lcddev.height=temp;
		}
	}
	else  
	{
		if(lcddev.width>lcddev.height)//����X,Y
		{
			temp=lcddev.width;
			lcddev.width=lcddev.height;
			lcddev.height=temp;
		}
	}
  LCD_ILI9341_CMD(lcddev.setxcmd); 
	LCD_ILI9341_Parameter(0);LCD_ILI9341_Parameter(0);
	LCD_ILI9341_Parameter((lcddev.width-1)>>8);LCD_ILI9341_Parameter((lcddev.width-1)&0XFF);
	LCD_ILI9341_CMD(lcddev.setycmd); 
	LCD_ILI9341_Parameter(0);LCD_ILI9341_Parameter(0);
	LCD_ILI9341_Parameter((lcddev.height-1)>>8);LCD_ILI9341_Parameter((lcddev.height-1)&0XFF);
}

/**************************************************************************
*��������  �����ú�����
*������    ��LCD_Display_Dir
*��������  ��u8 dir:0,������1,����
*��������ֵ��void
*��������  ��
*****************************************************************************/
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)			//����
	{
		lcddev.dir = 0;	            //����
		lcddev.width = LCD_WIDTH;   //��Ļ��
		lcddev.height = LCD_HEIGHT; //��Ļ��
		
		lcddev.wramcmd=0X2C;    //��ɫָ��
		lcddev.setxcmd=0X2A;    //x����ָ��
		lcddev.setycmd=0X2B;    //y����ָ��
	}
	else
	{
		lcddev.dir = 1;	            //����
		lcddev.width = LCD_HEIGHT;  //��Ļ��
		lcddev.height = LCD_WIDTH;  //��Ļ��
		
		lcddev.wramcmd=0X2C;    //��ɫָ��
		lcddev.setxcmd=0X2A;    //x����ָ��
		lcddev.setycmd=0X2B;    //y����ָ��
	}
	
	LCD_Scan_Dir(DFT_SCAN_DIR);	//Ĭ��ɨ�跽��
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
	/******** FSMC������ʽ ********/
  /*IO�ڳ�ʼ����FSMC��ʼ��*/
  FSMC_LCD_Init();
	
	delay1_ms(10);
	lcddev.id = 9341;
	
  
  /*LCD9341���Ҵ���*/
	LCD_ILI9341_CMD(0xCF);  
	LCD_ILI9341_Parameter(0x00); 
	LCD_ILI9341_Parameter(0xC1); 
	LCD_ILI9341_Parameter(0X30); 
	LCD_ILI9341_CMD(0xED);  
	LCD_ILI9341_Parameter(0x64); 
	LCD_ILI9341_Parameter(0x03); 
	LCD_ILI9341_Parameter(0X12); 
	LCD_ILI9341_Parameter(0X81); 
	LCD_ILI9341_CMD(0xE8);  
	LCD_ILI9341_Parameter(0x85); 
	LCD_ILI9341_Parameter(0x10); 
	LCD_ILI9341_Parameter(0x7A); 
	LCD_ILI9341_CMD(0xCB);  
	LCD_ILI9341_Parameter(0x39); 
	LCD_ILI9341_Parameter(0x2C); 
	LCD_ILI9341_Parameter(0x00); 
	LCD_ILI9341_Parameter(0x34); 
	LCD_ILI9341_Parameter(0x02); 
	LCD_ILI9341_CMD(0xF7);  
	LCD_ILI9341_Parameter(0x20); 
	LCD_ILI9341_CMD(0xEA);  
	LCD_ILI9341_Parameter(0x00); 
	LCD_ILI9341_Parameter(0x00); 
	LCD_ILI9341_CMD(0xC0);    //Power control 
	LCD_ILI9341_Parameter(0x1B);   //VRH[5:0] 
	LCD_ILI9341_CMD(0xC1);    //Power control 
	LCD_ILI9341_Parameter(0x01);   //SAP[2:0];BT[3:0] 
	LCD_ILI9341_CMD(0xC5);    //VCM control 
	LCD_ILI9341_Parameter(0x30); 	 //3F
	LCD_ILI9341_Parameter(0x30); 	 //3C
	LCD_ILI9341_CMD(0xC7);    //VCM control2 
	LCD_ILI9341_Parameter(0XB7); 
	LCD_ILI9341_CMD(0x36);    // Memory Access Control 
	LCD_ILI9341_Parameter(0x48); 
	LCD_ILI9341_CMD(0x3A);   
	LCD_ILI9341_Parameter(0x55); 
	LCD_ILI9341_CMD(0xB1);   
	LCD_ILI9341_Parameter(0x00);   
	LCD_ILI9341_Parameter(0x1A); 
	LCD_ILI9341_CMD(0xB6);    // Display Function Control 
	LCD_ILI9341_Parameter(0x0A); 
	LCD_ILI9341_Parameter(0xA2); 
	LCD_ILI9341_CMD(0xF2);    // 3Gamma Function Disable 
	LCD_ILI9341_Parameter(0x00); 
	LCD_ILI9341_CMD(0x26);    //Gamma curve selected 
	LCD_ILI9341_Parameter(0x01); 
	LCD_ILI9341_CMD(0xE0);    //Set Gamma 
	LCD_ILI9341_Parameter(0x0F); 
	LCD_ILI9341_Parameter(0x2A); 
	LCD_ILI9341_Parameter(0x28); 
	LCD_ILI9341_Parameter(0x08); 
	LCD_ILI9341_Parameter(0x0E); 
	LCD_ILI9341_Parameter(0x08); 
	LCD_ILI9341_Parameter(0x54); 
	LCD_ILI9341_Parameter(0XA9); 
	LCD_ILI9341_Parameter(0x43); 
	LCD_ILI9341_Parameter(0x0A); 
	LCD_ILI9341_Parameter(0x0F); 
	LCD_ILI9341_Parameter(0x00); 
	LCD_ILI9341_Parameter(0x00); 
	LCD_ILI9341_Parameter(0x00); 
	LCD_ILI9341_Parameter(0x00); 		 
	LCD_ILI9341_CMD(0XE1);    //Set Gamma 
	LCD_ILI9341_Parameter(0x00); 
	LCD_ILI9341_Parameter(0x15); 
	LCD_ILI9341_Parameter(0x17); 
	LCD_ILI9341_Parameter(0x07); 
	LCD_ILI9341_Parameter(0x11); 
	LCD_ILI9341_Parameter(0x06); 
	LCD_ILI9341_Parameter(0x2B); 
	LCD_ILI9341_Parameter(0x56); 
	LCD_ILI9341_Parameter(0x3C); 
	LCD_ILI9341_Parameter(0x05); 
	LCD_ILI9341_Parameter(0x10); 
	LCD_ILI9341_Parameter(0x0F); 
	LCD_ILI9341_Parameter(0x3F); 
	LCD_ILI9341_Parameter(0x3F); 
	LCD_ILI9341_Parameter(0x0F); 
	LCD_ILI9341_CMD(0x2B); 
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x01);
	LCD_ILI9341_Parameter(0x3f);
	LCD_ILI9341_CMD(0x2A); 
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0xef);	 
	LCD_ILI9341_CMD(0x11); //Exit Sleep
	delay_ms(120);
	LCD_ILI9341_CMD(0x29); //display on	

  
  /*�û��Լ���д*/
	//������ĻΪ����
	LCD_Display_Dir(0);  
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
*         ��������������   0x2a
*         ��������������   0x2b
*         ��ɫ��������     0x2c
*****************************************************************************/
void LCD_Clear_XY(u16 x,u16 y,u16 width,u16 height,u16 color)
{
  u32 i;
  i = width * height;
  //��������(x����)
  LCD_ILI9341_CMD(lcddev.setxcmd);
  //д��x��������
  //��ʼx����
  LCD_ILI9341_Parameter(x>>8);
  LCD_ILI9341_Parameter(x);
  //ĩβx����
  LCD_ILI9341_Parameter((x+width) >> 8);
  LCD_ILI9341_Parameter(x+width);
  
  //��������(y����)
  LCD_ILI9341_CMD(lcddev.setycmd);
  //д��y��������
  //��ʼy����
  LCD_ILI9341_Parameter(y>>8);
  LCD_ILI9341_Parameter(y);
  //ĩβy����
  LCD_ILI9341_Parameter((y+height) >> 8);
  LCD_ILI9341_Parameter(y+height);
  
  //��������(��ɫ)
  LCD_ILI9341_CMD(lcddev.wramcmd);
  //��ʼ��ɫ
  while(i)
  {
    LCD->LCD_DAT = color;
    i--;
  }
}

/**************************************************************************
*��������  ����ָ�����������ָ����ɫ��
*������    ��LCD_Color_Fill
*��������  ��u16 x1,u16 y1,u16 x2,u16 y2,u16 *color 
*��������ֵ��void
*��������  ��  
*         ��������������   0x2a
*         ��������������   0x2b
*         ��ɫ��������     0x2c
*****************************************************************************/
void LCD_Color_Fill(u16 x1,u16 y1,u16 x2,u16 y2,u16 *color)
{  
	u16 height,width;
	u16 i,j,y=0;
	width = x2 - x1 + 1; 			//�õ����Ŀ��
	height = y2 - y1 + 1;			//�߶�
 	for(i=0;i<height;i++)
	{
		//��������(x����)
		LCD_ILI9341_CMD(lcddev.setxcmd);
		LCD_ILI9341_Parameter(x1>>8);
		LCD_ILI9341_Parameter(x1&0xff);
		//��������(y����)
		y=y1+i;
		LCD_ILI9341_CMD(lcddev.setycmd);
		LCD_ILI9341_Parameter(y>>8);
		LCD_ILI9341_Parameter(y&0xff);
		
		//��������(��ɫ)
		LCD_ILI9341_CMD(lcddev.wramcmd);
		for(j=0;j<width;j++) LCD->LCD_DAT=color[i*width+j];
	}		  
} 

/**************************************************************************
*��������  ��LCD��Ļ���㺯��
*������    ��LCD_Point
*��������  ��u16 x,u16 y,u16 colour 
*��������ֵ��void
*��������  ��
*         ��������������   0x2a
*         ��������������   0x2b
*         ��ɫ��������     0x2c
*****************************************************************************/
void LCD_Point(u16 x,u16 y,u16 color)
{
  //��������(x����)
  LCD_ILI9341_CMD(lcddev.setxcmd);
  //д��x��������
  //��ʼx����
  LCD_ILI9341_Parameter(x >> 8);
  LCD_ILI9341_Parameter(x);
  
  //��������(y����)
  LCD_ILI9341_CMD(lcddev.setycmd);
  //д��y��������
  //��ʼy����
  LCD_ILI9341_Parameter(y >> 8);
  LCD_ILI9341_Parameter(y);
  
  //��������(��ɫ)
  LCD->LCD_CMD = lcddev.wramcmd;
  //��ʼ��ɫ
  LCD->LCD_DAT=color;
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
void LCD_Circle(u16 x,u16 y,u16 r,u16 color)
{
  u16 i,j;
	
	for(i=x-r;i<=x+r;i++)
	{
		for(j=y-r;j<=y+r;j++)
		{
			if((i-x)*(i-x) + (j-y)*(j-y) >= (r)*(r-2) && (i-x)*(i-x) + (j-y)*(j-y) <= r*r)
			{
				LCD_Point(i,j,color);
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
void LCD_Circle2(u16 x,u16 y,u16 r,u16 color)
{
	u16 i,j;
	
	for(i=x-r;i<=x+r;i++)
	{
		for(j=y-r;j<=y+r;j++)
		{
			if((i-x)*(i-x) + (j-y)*(j-y)<=r*r)
			{
				LCD_Point(i,j,color);
			}
		}
	}
}




























