#ifndef __DMA_H
#define __DMA_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"
    #include "delay.h"



    /****************    �����ⲿ����   *****************/
    void DMA1_Usart1_Init(uint8_t* data);   /* ��DMA1��ͨ��4���г�ʼ������(����USART1�ķ���ͨ��) */
    void DMA1_Enable(uint16_t cndtr);       /* ����DMA1���ݴ��� */

#endif




