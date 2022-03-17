#ifndef _IIC_H
#define _IIC_H

  #include "sys.h"
  //SCL:
  #define SCL_H      GPIO_SetBits(GPIOB,GPIO_Pin_6)    //ʱ��������
  #define SCL_L      GPIO_ResetBits(GPIOB,GPIO_Pin_6)  //ʱ��������
  //SDA
  //�����
  #define SDA_OUT_H  GPIO_SetBits(GPIOB,GPIO_Pin_7)    //���ģʽ������������
  #define SDA_OUT_L  GPIO_ResetBits(GPIOB,GPIO_Pin_7)  //���ģʽ������������
  //���룺
  #define SDA_IN     GPIO_SetBits(GPIOB,GPIO_Pin_7)           //����ģʽ
  #define SDA_INT    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)  //���յ���ֵ

  void IIC_Init(void);
  void IIC_Start(void);
  void IIC_Stop(void);
  void IIC_Send_Ack(u8 ack);
  u8 IIC_Receive_Ack(void);
  void IIC_Send_Byte(u8 data);
  u8 IIC_Receive_Byte(void);

#endif













