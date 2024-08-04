#ifndef __KEY_H
#define __KEY_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"
    #include "delay.h"



    /********************   �궨��   ********************/
    #define KEY0    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)         /* �͵�ƽ���� */
    #define KEY1    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)         /* �͵�ƽ���� */
    #define KEY2    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)         /* �͵�ƽ���� */
    #define KEY_UP  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)         /* �ߵ�ƽ���� */



    /****************    �����ⲿ����   *****************/
    void KEY_Init(void);         /* ������ʼ�� */
    uint8_t KEY_Scan(void);      /* ����ɨ�躯�� */

#endif




