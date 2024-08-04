#ifndef __IIC_H
#define __IIC_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"
    #include "delay.h"



    /********************   �궨��   ********************/
    /* SCL */
    #define SCL_H      GPIO_SetBits(GPIOB,GPIO_Pin_8)     /* ʱ�������� */
    #define SCL_L      GPIO_ResetBits(GPIOB,GPIO_Pin_8)   /* ʱ�������� */

    /* SDA */
    /* ����� */
    #define SDA_OUT_H  GPIO_SetBits(GPIOB,GPIO_Pin_9)     /* ���ģʽ������������ */
    #define SDA_OUT_L  GPIO_ResetBits(GPIOB,GPIO_Pin_9)   /* ���ģʽ������������ */
    /* ���룺 */
    #define SDA_IN     GPIO_SetBits(GPIOB,GPIO_Pin_9)              /* ����ģʽ */
    #define SDA_INT    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)     /* ���յ���ֵ */



    /****************    �����ⲿ����   *****************/
    void IIC_Init(void);                /* IIC��ʼ�� */
    void IIC_Start(void);               /* IIC��ʼ�ź� */
    void IIC_Stop(void);                /* IICֹͣ�ź� */
    void IIC_Send_Ack(uint8_t ack);     /* IICӦ���ź� */
    uint8_t IIC_Receive_Ack(void);      /* ����IICӦ���ź� */
    void IIC_Send_Byte(uint8_t data);   /* ����һ���ֽ����� */
    uint8_t IIC_Receive_Byte(void);     /* ����һ���ֽ����� */

#endif



