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
*            CS     PF11  �������
*            SCK    PB1   �������
*            PEN    PF10  ͨ������
*            MISO   PB2   ͨ������
*            MOSI   PF9   �������
* @fn       ��
************************************************************/
void Touch_IO_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;       /* GPIOx���ýṹ�� */

    /* ʱ��ʹ��   GPIOB   GPIOF */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);

    /* GPIOx��ʼ������ */
    /* GPIOx�˿����� */
    /* ��������� */
    /* PB1 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_1;          /* PB1 �˿����� */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;    /* ������� */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;    /* IO���ٶ�Ϊ50MHz */
    GPIO_Init(GPIOB, &GPIO_InitStruct);               /* �����趨������ʼ��PB1 */
    /* PF9��PF11 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11;/* PF9��PF11�˿����� */
    GPIO_Init(GPIOF, &GPIO_InitStruct);               /* �����趨������ʼ��PF9��PF11 */

    /* ��������� */
    /* PB2 */
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_2;           /* PB2�˿����� */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU ;       /* ���������ģʽ */
    GPIO_Init(GPIOB, &GPIO_InitStruct);               /* �����趨������ʼ��PB1 */
    /* PF10 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;           /* PF10�˿����� */
    GPIO_Init(GPIOF, &GPIO_InitStruct);               /* �����趨������ʼ��PF10 */

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





