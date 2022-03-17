#ifndef _LCD_H

  #include "sys.h"
  
    /*******������********/
  //Ƭѡ
  #define LCD_CS_H    GPIO_SetBits(GPIOG,GPIO_Pin_12)
  #define LCD_CS_L   	GPIO_ResetBits(GPIOG,GPIO_Pin_12) 
  //дʹ��
  #define LCD_WR_H   	GPIO_SetBits(GPIOD,GPIO_Pin_5)
  #define LCD_WR_L   	GPIO_ResetBits(GPIOD,GPIO_Pin_5)
  //��ʹ��
  #define LCD_RD_H   	GPIO_SetBits(GPIOD,GPIO_Pin_4) 
  #define LCD_RD_L   	GPIO_ResetBits(GPIOD,GPIO_Pin_4) 
  //��������ѡ����
  #define LCD_DC_H   	GPIO_SetBits(GPIOG,GPIO_Pin_0) 
  #define LCD_DC_L   	GPIO_ResetBits(GPIOG,GPIO_Pin_0) 
  //����
  #define LCD_BL_H    GPIO_SetBits(GPIOB,GPIO_Pin_0)
  #define LCD_BL_L    GPIO_ResetBits(GPIOB,GPIO_Pin_0)

  #define LCD_DO_0(a)   (a) ? (GPIOD->ODR |= (1<<14)) : (GPIOD->ODR &= ~(1<<14))
  #define LCD_DO_1(a)   (a) ? (GPIOD->ODR |= (1<<15)) : (GPIOD->ODR &= ~(1<<15))
  #define LCD_DO_2(a)   (a) ? (GPIOD->ODR |= (1<<0))  : (GPIOD->ODR &= ~(1<<0))
  #define LCD_DO_3(a)   (a) ? (GPIOD->ODR |= (1<<1))  : (GPIOD->ODR &= ~(1<<1))
  #define LCD_DO_4(a)   (a) ? (GPIOE->ODR |= (1<<7))  : (GPIOE->ODR &= ~(1<<7))
  #define LCD_DO_5(a)   (a) ? (GPIOE->ODR |= (1<<8))  : (GPIOE->ODR &= ~(1<<8))
  #define LCD_DO_6(a)   (a) ? (GPIOE->ODR |= (1<<9))  : (GPIOE->ODR &= ~(1<<9))
  #define LCD_DO_7(a)   (a) ? (GPIOE->ODR |= (1<<10)) : (GPIOE->ODR &= ~(1<<10))
  #define LCD_DO_8(a)   (a) ? (GPIOE->ODR |= (1<<11)) : (GPIOE->ODR &= ~(1<<11))
  #define LCD_DO_9(a)   (a) ? (GPIOE->ODR |= (1<<12)) : (GPIOE->ODR &= ~(1<<12))
  #define LCD_DO_10(a)  (a) ? (GPIOE->ODR |= (1<<13)) : (GPIOE->ODR &= ~(1<<13))
  #define LCD_DO_11(a)  (a) ? (GPIOE->ODR |= (1<<14)) : (GPIOE->ODR &= ~(1<<14))
  #define LCD_DO_12(a)  (a) ? (GPIOE->ODR |= (1<<15)) : (GPIOE->ODR &= ~(1<<15))
  #define LCD_DO_13(a)  (a) ? (GPIOD->ODR |= (1<<8))  : (GPIOD->ODR &= ~(1<<8))
  #define LCD_DO_14(a)  (a) ? (GPIOD->ODR |= (1<<9))  : (GPIOD->ODR &= ~(1<<9))
  #define LCD_DO_15(a)  (a) ? (GPIOD->ODR |= (1<<10)) : (GPIOD->ODR &= ~(1<<10))
  
  //��Ļ��С
  #define LCD_Long    240    //��
  #define LCD_Wide    320    //�� 
  //��ʾ����
  #define LCD_Crosswise  0xA8  //����
  #define LCD_Lengthways 0x08  //����
  
  //��ɫ
  //GUI��ɫ
  #define WHITE        0xFFFF
  #define BLACK        0x0000	  
  #define BLUE         0x001F  
  #define BRED         0XF81F
  #define GRED 			   0XFFE0
  #define GBLUE			   0X07FF
  #define RED          0xF800
  #define MAGENTA      0xF81F
  #define GREEN        0x07E0
  #define CYAN         0x7FFF
  #define YELLOW       0xFFE0
  #define BROWN 			 0XBC40 //��ɫ
  #define BRRED 			 0XFC07 //�غ�ɫ
  #define GRAY  			 0X8430 //��ɫ
  //PANEL����ɫ
  #define DARKBLUE     0X01CF	//����ɫ
  #define LIGHTBLUE    0X7D7C	//ǳ��ɫ  
  #define GRAYBLUE     0X5458 //����ɫ
  #define LIGHTGREEN   0X841F //ǳ��ɫ
  #define LIGHTGRAY    0XEF5B //ǳ��ɫ(PANNEL)
  #define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
  #define LGRAYBLUE    0XA651 //ǳ����ɫ(�м����ɫ)
  #define LBBLUE       0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
	
	void LCD_IO_Init(void);
  void LCD_ILI9341_CMD(u8 cmd);
  void LCD_ILI9341_Parameter(u16 data);
  void LCD_Clear(u16 colour);
  void LCD_Init(void);
  void LCD_Clear_XY(u16 x,u16 y,u16 Long,u16 Wide,u16 colour);
  void LCD_Point(u16 x,u16 y,u16 colour);
  void LCD_Circle(u16 x,u16 y,u16 r,u16 colour);
  void LCD_Circle2(u16 x,u16 y,u16 r,u16 colour);
  void LCD_Character16(u16 x,u16 y,u8 ch,u16 colour);
  void LCD_Character24(u16 x,u16 y,u8 ch,u16 colour);
  void LCD_Character32(u16 x,u16 y,u8 ch,u16 colour);
  void LCD_String16(u16 x,u16 y,u8 *str,u16 colour);
  void LCD_String24(u16 x,u16 y,u8 *str,u16 colour);
  void LCD_String32(u16 x,u16 y,u8 *str,u16 colour);
  void LCD_Characters_Single32(u16 x,u16 y,u8 *ch,u16 colour);
  void LCD_Characters_String32(u16 x,u16 y,u8 *str,u16 colour);
	void LCD_Picture(u16 x,u16 y,u8 *buf);
	void LCD_Picture2(u16 x,u16 y,u8 *buf);
	
#endif	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
