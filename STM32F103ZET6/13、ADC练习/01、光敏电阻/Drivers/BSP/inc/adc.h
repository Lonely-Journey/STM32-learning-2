#ifndef __ADC_H
#define __ADC_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"



    /****************    �����ⲿ����   *****************/
    void ADC3_In6_Init(void);       /* ADC3��ʼ�� */
    uint16_t ADC3_In6_Data(void);   /* ��ȡADC3ת����ֵ */
    uint16_t Oversampling(void);    /* ������ */

#endif
