#ifndef _OLED_H
#define _OLED_H

  #include "sys.h"
  
  //DC  ��������ѡ����(0����д��� 1����д����)
  #define  OLED_DC_L   GPIO_ResetBits(GPIOB, GPIO_Pin_12)  //����DC��ѡ������
  #define  OLED_DC_H   GPIO_SetBits(GPIOB, GPIO_Pin_12)    //����DC��ѡ������
  //CLK  ʱ����
  #define  OLED_CLK_L   GPIO_ResetBits(GPIOB, GPIO_Pin_13)  //����CLK
  #define  OLED_CLK_H   GPIO_SetBits(GPIOB, GPIO_Pin_13)    //����CLK 
  //RST  Ӳ��λOLED
  #define  OLED_RST_L   GPIO_ResetBits(GPIOB, GPIO_Pin_14)  //����RST
  #define  OLED_RST_H   GPIO_SetBits(GPIOB, GPIO_Pin_14)    //����RST 
  //MOSI �����������
  #define  OLED_MOSI_L   GPIO_ResetBits(GPIOB, GPIO_Pin_15)  //����MOSI
  #define  OLED_MOSI_H   GPIO_SetBits(GPIOB, GPIO_Pin_15)    //����MOSI

  //���ݺ������ѡ��궨��
  #define  OLED_COM     0      //����
  #define  OLED_DATA    1      //����
  
  //OLED����С
  #define  OLED_LINE       128    //128������
  #define  OLED_ROW        8      //8*8������(1ҳ8�У�����8ҳ)
   
  void OLED_IO_Init(void);
  void OLED_WR_Byte(u8 data,u8 DC);
  void OLED_Clear(void);
  void OLED_XY(u8 x,u8 y);
  void OLED_Open(void);
  void OLED_Close(void);
  void OLED_Init(void);
  
  void OLED_ShowChar(u16 x,u8 y,u8 size,u8 ch);
  void OLED_ShowString(u8 x,u8 y,u8 size,u8 *str);
  void OLED_ShowHanzi(u16 x,u8 y,u8 size,u8 *hz);
  void OLED_HanziString(u8 x,u8 y,u8 size,u8 *str);
  
  void OLED_Show(u8 x,u8 y,u8 size,u8 *str);
  
  void OLED_ShowPicture(u8 x,u8 y,u8 *tu);
  
#endif











