#ifndef __BEEP_H
#define __BEEP_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"



    /********************   �궨��   ********************/
    #define BEEP_ON  GPIO_SetBits(GPIOF,GPIO_Pin_8)       /* �������� */
    #define BEEP_OFF GPIO_ResetBits(GPIOF,GPIO_Pin_8)     /* �ط����� */



    /****************    �����ⲿ����   *****************/
    void BEEP_Init(void);       /* BEEP��ʼ�� */

#endif

