#ifndef _TOUCH_H
#define _TOUCH_H

  #include "stm32f4xx.h"  

  //Ƭѡ��
  #define T_CS_H    GPIO_SetBits(GPIOC,GPIO_Pin_13)
  #define T_CS_L    GPIO_ResetBits(GPIOC,GPIO_Pin_13)
  //ʱ����
  #define T_SCK_H   GPIO_SetBits(GPIOB,GPIO_Pin_0)
  #define T_SCK_L   GPIO_ResetBits(GPIOB,GPIO_Pin_0)
  //�����
  #define T_MOSI_H  GPIO_SetBits(GPIOF,GPIO_Pin_11)
  #define T_MOSI_L  GPIO_ResetBits(GPIOF,GPIO_Pin_11)
  //������
  #define T_MISO    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)
  //�ʽӴ���
  #define T_PEN     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
  
  //ָ��
  #define TOUCH_X   0xD0    //��ȡx��������
  #define TOUCH_Y   0x90    //��ȡy��������
  
  void Touch_Init(void);
  u16 Touch_Data(u8 command);
  
#endif














