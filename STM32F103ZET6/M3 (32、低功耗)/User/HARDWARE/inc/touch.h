#ifndef _TOUCH_H
#define _TOUCH_H

  #include "stm32f10x.h"
	
	//������ͨ�Ĵ�����x��y�����ȡ
  typedef struct touch
  {
    u16 x;
    u16 y;
  }TOUCH_XY;
  //���ڽ���LCD�ʹ������Ĺ�ϵ��
  typedef struct adjust
  {
    float a_x;  //lcd �� ������ x����ı���ϵ��
    signed short b_x;    //lcd �� ������ x�����ƫ����
    float c_y;  //lcd �� ������ y����ı���ϵ��
    signed short d_y;    //lcd �� ������ y�����ƫ����
  }ADJUST;

  //Ƭѡ��
  #define T_CS_H    GPIO_SetBits(GPIOF,GPIO_Pin_11)
  #define T_CS_L    GPIO_ResetBits(GPIOF,GPIO_Pin_11)
  //ʱ����
  #define T_SCK_H   GPIO_SetBits(GPIOB,GPIO_Pin_1)
  #define T_SCK_L   GPIO_ResetBits(GPIOB,GPIO_Pin_1);
  //�����
  #define T_MOSI_H  GPIO_SetBits(GPIOF,GPIO_Pin_9)
  #define T_MOSI_L  GPIO_ResetBits(GPIOF,GPIO_Pin_9);
  //������
  #define T_MISO    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)
  //�ʽӴ���
  #define T_PEN     GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_10)
  
  //ָ��
  #define TOUCH_X   0xD0    //��ȡx��������
  #define TOUCH_Y   0x90    //��ȡy��������
  
  void Touch_Init(void);
  u16 Touch_Data(u8 command);
	TOUCH_XY Touch_Smoothing(void);
  void LCD_Calibration_Cross(u16 x,u16 y,u16 colour);
  void LCD_Touch_Calibration(void);
  TOUCH_XY LCD_Touch_Transition(void);
	u8 LCD_Key_Scan(u16 x,u16 y,u16 Long,u16 Wide);

#endif















