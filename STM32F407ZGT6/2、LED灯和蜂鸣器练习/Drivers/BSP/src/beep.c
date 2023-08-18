#include "beep.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\beep.c
*              .\Drivers\BSP\inc\beep.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-06-16
* @brief     ��BEEP�������������ô���
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��BEEP_Init
* @brief    ����BEEP��Ӧ��GPIO�ڽ��г�ʼ������
* @param    ��void
* @retval   ��void
* @details  ��
*            BEEP     PF8
*            �ߵ�ƽ�죬�͵�ƽ����-----���ģʽ
* @fn       ��
************************************************************/
void BEEP_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;                    /* GPIOx���ýṹ�� */

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); /* ʹ��GPIOFʱ�� */

    /* ��ʼ����������Ӧ����GPIOF8 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8;              /* PF8 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;           /* ��ͨ���ģʽ */
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;           /* ������� */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;       /* 100MHz */
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;          /* ���� */
    GPIO_Init(GPIOF, &GPIO_InitStruct);                   /* ��ʼ��GPIO */

    GPIO_ResetBits(GPIOF,GPIO_Pin_8);                     /* ��������Ӧ����GPIOF8����*/
}


