#ifndef __USART_H
#define __USART_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h" 
    #include "stdio.h"



    /****************    �����ⲿ����   *****************/
    /*����1*/
    void USART1_Init(uint32_t baud);          /* ����1��ʼ�� */
    void USART1_Send_Byte(uint8_t data);      /* ����1����һ���ֽ� */
    uint8_t USART1_Receive_Byte(void);        /* ����1����һ���ֽ� */

#endif












