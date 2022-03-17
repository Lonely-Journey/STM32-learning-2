#include "sys.h"
#include "OLED.h"
#include "stdio.h"
#include "matrix.h"
#include "delay.h"


/*************************************************************************
*��������  ����ʼ��OLED��Ӧ��GPIO��
*������    ��OLED_IO_Init
*��������  ��void
*��������ֵ��void
*����      ��
*            O_MOSI ------- PB15  �����������
*            O_RST  ------- PB14  Ӳ��λOLED
*            O_CLK  ------- PB13  ʱ����
*            O_DC   ------- PB12  ����������(0����д��� 1����д����)
***************************************************************************/
void OLED_IO_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;         //GPIOx���ýṹ��
  
  //GPIOxʱ��ʹ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	     //ʹ��PB�˿�ʱ��
  
  //GPIOx����
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	//OLED  PB12/13/14/15 �˿�����
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOA PA2
  
}

/**********************************************************************
*��������  ����SSD1306д��һ���ֽڵ����ݻ�����
*������    ��OLED_WR_Byte
*��������  ��u8 data    u8 DC
*��������ֵ��void
*����      ��
*            ��������ѡ����    OLED_DC_L    OLED_DC_H
*            ʱ����            OLED_CLK_L   OLED_CLK_H
*            Ӳ��λOLED        OLED_RST_L   OLED_RST_H
*            �����������      OLED_MOSI_L  OLED_MOSI_H
*
*            ������DCΪ 1 ��ʱ�������� �� Ϊ 0 ��ʱ��������
************************************************************************/
void OLED_WR_Byte(u8 data,u8 DC)
{
  u8 i;
  
  //����������
  OLED_MOSI_L;
  
  //�жϽ�����Ҫ���͵� data �����������
  if(DC) OLED_DC_H;     //��������
  else OLED_DC_L;       //��������
  
  //����1���ֽ�����/����
  for(i=0;i<8;i++)
  {
    //����ʱ����
    OLED_CLK_L;
    //�ж�����/���ݵĵ�7λ�� 0 ���� 1
    if(data & 0x80) OLED_MOSI_H;   //����������
    else OLED_MOSI_L;              //����������
    //����ʱ����
    OLED_CLK_H;
    //����/��������
    data <<= 1;
  }
}

/******************************************************************************************
*��������  ����OLE������(��������Ļ�ǰ���)
*������    ��OLED_Clear
*��������  ��void
*��������ֵ��void
*����      ��        
*            ����              OLED_COM     0      
*            ����              OLED_DATA    1      
*
*                              ָ�
*                    0x00~0x0f:  ����8λ��ʼ�е�ַ�ĵ���λ��
*                    0x10~0x1f:  ����8λ��ʼ�е�ַ�ĸ���λ��
*        0x81ָ����0x00~0xff:  ���öԱȶȣ���ֵԽ��Խ����
*     0x8Dָ����0X14����0x10:  �������߹رյ�ɱá�
*                   0xAE��0xAF:  ������Ļ��ʾ�أ�˯��ģʽ��������ʾ��������ģʽ��
*                    0xb0~0xb7:  ����ҳ��ַ����8ҳ��ÿҳ8�����ص㣩������OLED��128*64�����ص�
*
*        ע�⣺  
*              ����˵��ҳָ�����У�һҳ���ư��У�һ����8ҳ(64��)
*              �������������ָ����ƣ��ֱ���Ƶ���λ�͸���λ��һ����128��
*              ��������ʱ��һ���ֽ����ݿ���8��1�е�״̬
**********************************************************************************************/
void OLED_Clear(void)
{
  u8 i,j;
  for(i=0;i<8;i++)
  {
    OLED_WR_Byte(0xb0+i,OLED_COM);   //ѡ��0+iҳ(����8*i ~ 8*(i+1)-1��)
    OLED_WR_Byte(0x00,OLED_COM);     //������ʾλ�á��е͵�ַ
    OLED_WR_Byte(0x10,OLED_COM);     //������ʾλ�á��иߵ�ַ  �ӵ�0�п�ʼ
    for(j=0;j<128;j++) OLED_WR_Byte(0,OLED_DATA); //һ���ֽڿ���8��1��
  }
}

/******************************************************************************************
*��������  ��������ʾ����
*������    ��OLED_XY
*��������  ��u8 x    u8 y
*��������ֵ��void
*����      �� 
*            ����              OLED_COM     0      
*            ����              OLED_DATA    1      
*
*                              ָ�
*                    0x00~0x0f:  ����8λ��ʼ�е�ַ�ĵ���λ��
*                    0x10~0x1f:  ����8λ��ʼ�е�ַ�ĸ���λ��
*        0x81ָ����0x00~0xff:  ���öԱȶȣ���ֵԽ��Խ����
*     0x8Dָ����0X14����0x10:  �������߹رյ�ɱá�
*                   0xAE��0xAF:  ������Ļ��ʾ�أ�˯��ģʽ��������ʾ��������ģʽ��
*                    0xb0~0xb7:  ����ҳ��ַ����8ҳ��ÿҳ8�����ص㣩������OLED��128*64�����ص�
*
*        ע�⣺  
*              ����˵��ҳָ�����У�һҳ���ư��У�һ����8ҳ(64��)
*              �������������ָ����ƣ��ֱ���Ƶ���λ�͸���λ��һ����128��
*              ��������ʱ��һ���ֽ����ݿ���8��1�е�״̬
**********************************************************************************************/
void OLED_XY(u8 x,u8 y)
{
  OLED_WR_Byte(0xb0+y,OLED_COM);           //������һҳ(�İ���)
  OLED_WR_Byte(0x00|(x&0x0f),OLED_COM);    //������һ�еĵ�8λ
  OLED_WR_Byte(0x10|(x>>4),OLED_COM);      //������һ�еĸ�8λ
}

/******************************************************************************************
*��������  ������OLED��
*������    ��OLED_Open
*��������  ��void 
*��������ֵ��void
*����      �� 
*            ����              OLED_COM     0      
*            ����              OLED_DATA    1      
*
*                              ָ�
*                    0x00~0x0f:  ����8λ��ʼ�е�ַ�ĵ���λ��
*                    0x10~0x1f:  ����8λ��ʼ�е�ַ�ĸ���λ��
*        0x81ָ����0x00~0xff:  ���öԱȶȣ���ֵԽ��Խ����
*     0x8Dָ����0X14����0x10:  �������߹رյ�ɱá�
*                   0xAE��0xAF:  ������Ļ��ʾ�أ�˯��ģʽ��������ʾ��������ģʽ��
*                    0xb0~0xb7:  ����ҳ��ַ����8ҳ��ÿҳ8�����ص㣩������OLED��128*64�����ص�
*
*        ע�⣺  
*              ����˵��ҳָ�����У�һҳ���ư��У�һ����8ҳ(64��)
*              �������������ָ����ƣ��ֱ���Ƶ���λ�͸���λ��һ����128��
*              ��������ʱ��һ���ֽ����ݿ���8��1�е�״̬
**********************************************************************************************/
void OLED_Open(void)
{
  //������ɱ�
  OLED_WR_Byte(0x8D,OLED_COM);
  OLED_WR_Byte(0x14,OLED_COM);
  //��OLED������������ģʽ
  OLED_WR_Byte(0xAF,OLED_COM);
}


/******************************************************************************************
*��������  ���ر�OLED��
*������    ��OLED_Close
*��������  ��void 
*��������ֵ��void
*����      �� 
*            ����              OLED_COM     0      
*            ����              OLED_DATA    1      
*
*                              ָ�
*                    0x00~0x0f:  ����8λ��ʼ�е�ַ�ĵ���λ��
*                    0x10~0x1f:  ����8λ��ʼ�е�ַ�ĸ���λ��
*        0x81ָ����0x00~0xff:  ���öԱȶȣ���ֵԽ��Խ����
*     0x8Dָ����0X14����0x10:  �������߹رյ�ɱá�
*                   0xAE��0xAF:  ������Ļ��ʾ�أ�˯��ģʽ��������ʾ��������ģʽ��
*                    0xb0~0xb7:  ����ҳ��ַ����8ҳ��ÿҳ8�����ص㣩������OLED��128*64�����ص�
*
*        ע�⣺  
*              ����˵��ҳָ�����У�һҳ���ư��У�һ����8ҳ(64��)
*              �������������ָ����ƣ��ֱ���Ƶ���λ�͸���λ��һ����128��
*              ��������ʱ��һ���ֽ����ݿ���8��1�е�״̬
**********************************************************************************************/
void OLED_Close(void)
{
  //�رյ�ɱ�
  OLED_WR_Byte(0x8D,OLED_COM);
  OLED_WR_Byte(0x10,OLED_COM);
  //��OLED����˯��ģʽ
  OLED_WR_Byte(0xAE,OLED_COM);
}

/*************************************************************************
*��������  ��OLED��ʽ��ʼ��
*������    ��OLED_Init
*��������  ��void
*��������ֵ��void
*����      ��
*            ��������ѡ����    OLED_DC_L    OLED_DC_H
*            ʱ����            OLED_CLK_L   OLED_CLK_H
*            Ӳ��λOLED        OLED_RST_L   OLED_RST_H
*            �����������      OLED_MOSI_L  OLED_MOSI_H
*
*            ����              OLED_COM          0      
*            ����              OLED_DATA         1 
***************************************************************************/
void OLED_Init(void)
{
  //GPIOx��ʼ��
  OLED_IO_Init();
  
  //Ӳ��λOLED (��һ���Ǳ����)
  OLED_DC_L;
  delay_ms(100);
  OLED_RST_H;
  
  /*���Ҵ���*/
  OLED_WR_Byte(0xAE,OLED_COM);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_COM);//---set low column address
	OLED_WR_Byte(0x10,OLED_COM);//---set high column address
	OLED_WR_Byte(0x40,OLED_COM);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_COM);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_COM); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_COM);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WR_Byte(0xC8,OLED_COM);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_WR_Byte(0xA6,OLED_COM);//--set normal display
	OLED_WR_Byte(0xA8,OLED_COM);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_COM);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_COM);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_COM);//-not offset
	OLED_WR_Byte(0xd5,OLED_COM);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_COM);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_COM);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_COM);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_COM);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_COM);
	OLED_WR_Byte(0xDB,OLED_COM);//--set vcomh
	OLED_WR_Byte(0x40,OLED_COM);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_COM);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_COM);//
	OLED_WR_Byte(0x8D,OLED_COM);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_COM);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_COM);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_COM);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_COM);//--turn on oled panel
  
  /*����*/
  OLED_Clear();
  /*��ʼ����*/
  OLED_XY(0,0);
}

/*************************************************************************
*��������  ��OLED��ʾһ��size��С���ַ�
*������    ��OLED_ShowChar
*��������  ��u8 x  u8 y  u8 size  u8 ch
*��������ֵ��void
*����      ��
***************************************************************************/
void OLED_ShowChar(u16 x,u8 y,u8 size,u8 ch)
{
  u8 i,c;
  
  //�����ַ�ƫ����
  c = ch - ' ';
  
  //�ж��Ƿ񳬳�����
  if(x+size > (OLED_LINE-1))
  {
    y=y+(size/8)*((x+size)/(OLED_LINE-1));   //yֵ��Ҫ����7
    if((x+size-(OLED_LINE-1))<size)x=0;
    else x=(x+size-(OLED_LINE-1))/size*size;
  }
  
  //ѡ�������С
  if(size == 8)
  {
    /*ע�⣺8���ַ���1ҳ���*/
    //ȷ������
    OLED_XY(x,y);
    //��ʾ��һҳ���ַ�����(1ҳ=8��)
    for(i=0;i<4;i++)
		OLED_WR_Byte(CH8[c*4+i],OLED_DATA);
  }
  else if(size == 16)
  {
    /*ע�⣺16���ַ���2ҳ���*/
    //ȷ������
    OLED_XY(x,y);
    //��ʾ��һҳ���ַ�����(1ҳ=8��)
    for(i=0;i<8;i++)
		OLED_WR_Byte(CH16[c*16+i],OLED_DATA);
    //ȷ������
    OLED_XY(x,y+1);
    //��ʾ��һҳ���ַ�����(1ҳ=8��)
    for(i=0;i<8;i++)
    OLED_WR_Byte(CH16[c*16+i+8],OLED_DATA);
  }
  else if(size == 24)
  {
    /*ע�⣺24���ַ���3ҳ���*/
    //ȷ������
    OLED_XY(x,y);
    //��ʾ��һҳ���ַ�����(1ҳ=8��)
    for(i=0;i<12;i++)
		OLED_WR_Byte(CH24[c*36+i],OLED_DATA);
    //ȷ������
    OLED_XY(x,y+1);
    //��ʾ��һҳ���ַ�����(1ҳ=8��)
    for(i=0;i<12;i++)
    OLED_WR_Byte(CH24[c*36+i+12],OLED_DATA);
    //ȷ������
    OLED_XY(x,y+2);
    //��ʾ��һҳ���ַ�����(1ҳ=8��)
    for(i=0;i<12;i++)
    OLED_WR_Byte(CH24[c*36+i+24],OLED_DATA);
  }
  else if(size == 32)
  {
    /*ע�⣺24���ַ���4ҳ���*/
    //ȷ������
    OLED_XY(x,y);
    //��ʾ��һҳ���ַ�����(1ҳ=8��)
    for(i=0;i<16;i++)
		OLED_WR_Byte(CH32[c*64+i],OLED_DATA);
    //ȷ������
    OLED_XY(x,y+1);
    //��ʾ��һҳ���ַ�����(1ҳ=8��)
    for(i=0;i<16;i++)
    OLED_WR_Byte(CH32[c*64+i+16],OLED_DATA);
    //ȷ������
    OLED_XY(x,y+2);
    //��ʾ��һҳ���ַ�����(1ҳ=8��)
    for(i=0;i<16;i++)
    OLED_WR_Byte(CH32[c*64+i+32],OLED_DATA);
    //ȷ������
    OLED_XY(x,y+3);
    //��ʾ��һҳ���ַ�����(1ҳ=8��)
    for(i=0;i<16;i++)
    OLED_WR_Byte(CH32[c*64+i+48],OLED_DATA);
  }
}

/*************************************************************************
*��������  ��OLED��ʾһ��size��С���ַ���
*������    ��OLED_ShowString
*��������  ��u8 x  u8 y  u8 size  u8 *str
*��������ֵ��void
*����      ��
***************************************************************************/
void OLED_ShowString(u8 x,u8 y,u8 size,u8 *str)
{
  u8 i=0;
  
  while(*str != '\0')
  {
    OLED_ShowChar(x+i*(size/2),y,size,*str);
    str++;
    i++;
  }
}

/*************************************************************************
*��������  ��OLED��ʾһ��size��С�ĺ���
*������    ��OLED_ShowHanzi
*��������  ��u8 x  u8 y  u8 size  u8 ch
*��������ֵ��void
*����      ��
***************************************************************************/
void OLED_ShowHanzi(u16 x,u8 y,u8 size,u8 *hz)
{
  u8 i,h=0;
  
  //���㺺��ƫ����
  while(1)
  {
    if((*hz == HZ[h*2])&&(*(hz+1)==HZ[h*2+1]))
    {
      break;
    }
    h++;
  }
  //�ж��Ƿ񳬳�����
  if(x+size > (OLED_LINE-1))
  {
    y=y+(size/8)*((x+size)/(OLED_LINE-1));   //yֵ��Ҫ����7
    if((x+size-(OLED_LINE-1))<size)x=0;
    else x=(x+size-(OLED_LINE-1))/size*size;
  }
  
  if(size == 16)
  {
    /*ע�⣺16���ַ���2ҳ���*/
    //ȷ������
    OLED_XY(x,y);
    //��ʾ��һҳ���ַ�����(1ҳ=8��)
    for(i=0;i<16;i++)
		OLED_WR_Byte(HZ16[h*32+i],OLED_DATA);
    //ȷ������
    OLED_XY(x,y+1);
    //��ʾ��һҳ���ַ�����(1ҳ=8��)
    for(i=0;i<16;i++)
    OLED_WR_Byte(HZ16[h*32+i+16],OLED_DATA);
  }
  if(size == 24)
  {
    /*ע�⣺16���ַ���2ҳ���*/
    //ȷ������
    OLED_XY(x,y);
    //��ʾ��һҳ���ַ�����(1ҳ=8��)
    for(i=0;i<24;i++)
		OLED_WR_Byte(HZ24[h*72+i],OLED_DATA);
    //ȷ������
    OLED_XY(x,y+1);
    //��ʾ��һҳ���ַ�����(1ҳ=8��)
    for(i=0;i<24;i++)
    OLED_WR_Byte(HZ24[h*72+i+24],OLED_DATA);
    //ȷ������
    OLED_XY(x,y+2);
    //��ʾ��һҳ���ַ�����(1ҳ=8��)
    for(i=0;i<24;i++)
    OLED_WR_Byte(HZ24[h*72+i+48],OLED_DATA);
  }
}

/*************************************************************************
*��������  ��OLED��ʾһ��size��С�ĺ���
*������    ��OLED_HanziString
*��������  ��u8 x  u8 y  u8 size  u8 *str
*��������ֵ��void
*����      ��
***************************************************************************/
void OLED_HanziString(u8 x,u8 y,u8 size,u8 *str)
{
  u16 i=0;
  
  while(*str != '\0')
  {
    OLED_ShowHanzi(x+i*size,y,size,str);
    str+=2;
    i++;
  }
}

/*************************************************************************
*��������  ��OLED��ʾһ��size��С�ĺ���/�ַ�
*������    ��OLED_Show
*��������  ��u8 x  u8 y  u8 size  u8 *str
*��������ֵ��void
*����      ��
*             ע�⣺sizeֻ����16��24
***************************************************************************/
void OLED_Show(u8 x,u8 y,u8 size,u8 *str)
{
  u16 temp=0;
  
  while(*str != '\0')
  {
    if((32<=*str) && (*str<=127))
    {
      OLED_ShowChar(x+temp,y,size,*str);
      temp += (size/2);
      str++;
    }
    else
    {
      OLED_ShowHanzi(x+temp,y,size,str);
      temp += size;
      str+=2;
    }
  }
}

/*************************************************************************
*��������  ��OLED��ʾһ��ͼƬ
*������    ��OLED_ShowPicture
*��������  ��u8 x  u8 y  u8 *tu
*��������ֵ��void
*����      ��
***************************************************************************/
void OLED_ShowPicture(u8 x,u8 y,u8 *tu)
{
  u8 i,j;
  for(i=0;i<8;i++)
  {
    OLED_XY(x,y+i);
    for(j=0;j<64;j++) 
    {
      OLED_WR_Byte(*tu,OLED_DATA); //һ���ֽڿ���8��1��
      tu++;
    }
  }
}















