#ifndef __TOUCH_H
#define __TOUCH_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"
    #include "delay.h"



    /********************   �궨��   ********************/
    /* Ƭѡ�� */
    #define T_CS_H    GPIO_SetBits(GPIOF,GPIO_Pin_11)      /* Ƭѡʧ�� */
    #define T_CS_L    GPIO_ResetBits(GPIOF,GPIO_Pin_11)    /* Ƭѡʹ�� */
    /* ʱ���� */
    #define T_SCK_H   GPIO_SetBits(GPIOB,GPIO_Pin_1)
    #define T_SCK_L   GPIO_ResetBits(GPIOB,GPIO_Pin_1)
    /* ����� */
    #define T_MOSI_H  GPIO_SetBits(GPIOF,GPIO_Pin_9)
    #define T_MOSI_L  GPIO_ResetBits(GPIOF,GPIO_Pin_9)
    /* ������ */
    #define T_MISO    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)
    /* �ʽӴ��� */
    #define T_PEN     GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_10)

    /* ָ�� */
    #define TOUCH_X   0xD0    //��ȡx��������
    #define TOUCH_Y   0x90    //��ȡy��������



    /****************    �����ⲿ����   *****************/
    void Touch_IO_Init(void);                /* ���败�������ų�ʼ�� */
    uint16_t Touch_Data(uint8_t command);    /* ����败�����������ݲ����յ��败�������������� */

#endif







