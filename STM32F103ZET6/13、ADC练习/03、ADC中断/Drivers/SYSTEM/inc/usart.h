#ifndef __USART_H
#define __USART_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"
    #include "stdio.h"



    /****************    �����ⲿ����   *****************/
    void USART1_Init(uint32_t baud);           /* ����1��ʼ�� */
    uint8_t USART1_Receive_Byte(void);         /* ����1����һ���ֽ� */

#endif


