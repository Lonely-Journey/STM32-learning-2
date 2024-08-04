#include "touch.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\touch.c
*              .\Drivers\BSP\inc\touch.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-03
* @brief     �����败�����������úͲ�������
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��Touch_IO_Init
* @brief    ���Ե��败����оƬ��Ӧ��GPIO�ڽ��г�ʼ������
* @param    ��void
* @retval   ��void
* @details  ��
*            CS     PC13  �������
*            SCK    PB0   �������
*            PEN    PB1   ͨ������
*            MISO   PB2   ͨ������
*            MOSI   PF11  �������
* @fn       ��
************************************************************/
void Touch_IO_Init(void)
{
    GPIO_InitTypeDef   GPIO_InitStruct;    /* GPIOx���ýṹ�� */

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOF, ENABLE);   /* ʹ��GPIOB,C,Fʱ�� */

    /* GPIOB1,2��ʼ������ */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_2;  /* PB1/PB2 ����Ϊ�������� */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;             /* ����ģʽ */
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;            /* ������� */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;        /* 100MHz */
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;             /* ���� */
    GPIO_Init(GPIOB, &GPIO_InitStruct);                    /* ��ʼ��GPIO */
    /* PB0 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;               /* PB0����Ϊ������� */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;            /* ���ģʽ */
    GPIO_Init(GPIOB, &GPIO_InitStruct);                    /* ��ʼ��GPIO */
    /* PC13 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_13;              /* PC13����Ϊ������� */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;            /* ���ģʽ */
    GPIO_Init(GPIOC, &GPIO_InitStruct);                    /* ��ʼ��GPIO */
    /* PF11 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;                /* PF11����������� */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;             /* ���ģʽ */
    GPIO_Init(GPIOF, &GPIO_InitStruct);                    /* ��ʼ��GPIO */

    /* ��ʼ��״̬���� */
    T_CS_H;
    T_SCK_L;
    T_MOSI_L;
}



/**********************************************************
* @funcName ��Touch_Data
* @brief    ���Ե��败����оƬ��Ӧ��GPIO�ڽ��г�ʼ������
* @param    ��uint8_t command (ָ��)
* @retval   ��uint16_t
* @details  ��
*            Ƭѡ��        T_CS_H       T_CS_L  
*            ʱ����        T_SCK_H      T_SCK_L  
*            ���������    T_MOSI_H     T_MOSI_L
*            ����������    T_MISO
*            �ʽӴ���      T_PEN
* @fn       ��
************************************************************/
uint16_t Touch_Data(uint8_t command)
{
    uint8_t i;
    uint16_t data = 0;

    T_MOSI_L;            /* ���������� */
    T_CS_L;              /* Ƭѡʹ�� */

    for(i=0;i<8;i++)
    {
        T_SCK_L;             /* ʱ�������� */

        /* �ж�ָ��λ��ƽ */
        if(command & 0x80)T_MOSI_H;
        else T_MOSI_L;

        command <<= 1;  /* ָ���������� */

        T_SCK_H;          /* ʱ�������� */
        delay_us(1);      /* ��ʱ�����ڴӻ���ȡ���� */
    }
    T_SCK_L;            /* ʱ�������� */
    T_MOSI_L;           /* ���������� */
    delay_us(6);        /* ��ʱ6us�����ڵȴ�ADC������ת���� */

    /* �ճ�һ�������� */
    T_SCK_L;
    delay_us(1);
    T_SCK_H;
    delay_us(1);

    for(i=0;i<12;i++)
    {
        data <<= 1;              /* ָ���������� */
        T_SCK_L;                 /* ʱ�������� */
        delay_us(1);             /* ��ʱ���������ݽ��� */
        T_SCK_H;                 /* ʱ�������� */
        if(T_MISO) data |= 0x1;  /* ��ȡ����λ��ƽ */
        delay_us(1);             /* ��ʱ���������ݽ��� */
    }

    T_CS_H;    /* Ƭѡ���� */
    T_SCK_L;   /* ʱ�������� */

    return data;
}



