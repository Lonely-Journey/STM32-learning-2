#include "led.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\led.c
*              .\Drivers\BSP\inc\led.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-06-16
* @brief     ��LED���������ô���
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��LED_Init
* @brief    ����LED��Ӧ��GPIO�ڽ��г�ʼ������
* @param    ��void
* @retval   ��void
* @details  ��
*            LED0     PF9
*            LED1     PF10
*            �ߵ�ƽ�𣬵͵�ƽ��-----���ģʽ
* @fn       ��
************************************************************/
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;                    /* GPIOx���ýṹ�� */

    /* �˿�ʱ��ʹ��   GPIOF */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);   /* ʹ��GPIOFʱ�� */

    /* GPIOF9,F10��ʼ������ */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;  /* LED0��LED1��ӦIO�� */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;             /* ��ͨ���ģʽ */
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;             /* ������� */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;         /* 100MHz */
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;              /* ���� */
    GPIO_Init(GPIOF, &GPIO_InitStruct);                     /* ��ʼ��GPIO */

    GPIO_SetBits(GPIOF, GPIO_Pin_9|GPIO_Pin_10);            /* GPIOF9,F10���øߣ����� */
} 








