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
*            BEEP     PB8
*            �ߵ�ƽ�죬�͵�ƽ����-----���ģʽ
* @fn       ��
************************************************************/
void BEEP_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;                  /* GPIOx���ýṹ�� */

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  /* ʹ��PB�˿�ʱ�� */

    /* PB8 */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;            /* PB8 �˿����� */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;      /* ������� */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      /* IO���ٶ�Ϊ50MHz */
    GPIO_Init(GPIOB, &GPIO_InitStructure);                 /* �����趨������ʼ��PB8 */

    /* �˿�������ݼĴ�������   beep�ĳ�ʼ״̬ */
    GPIO_ResetBits(GPIOB,GPIO_Pin_8);                      /* PB8 ����� */
}



