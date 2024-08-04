#ifndef __LED_H
#define __LED_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"



    /********************   �궨��   ********************/
    /* ���� */
    #define LED0_ON   GPIO_ResetBits(GPIOF,GPIO_Pin_9)
    #define LED1_ON   GPIO_ResetBits(GPIOF,GPIO_Pin_10)
    /* �ص� */
    #define LED0_OFF  GPIO_SetBits(GPIOF,GPIO_Pin_9)
    #define LED1_OFF  GPIO_SetBits(GPIOF,GPIO_Pin_10)



    /****************    �����ⲿ����   *****************/
    void LED_Init(void);         /* LED��ʼ�� */

#endif








