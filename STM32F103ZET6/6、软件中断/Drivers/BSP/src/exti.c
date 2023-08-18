#include "exti.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\exti.c
*              .\Drivers\BSP\inc\exti.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-06-16
* @brief     ������жϴ���
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��EXTI5_Swier_Init
* @brief    ������ⲿ�жϳ�ʼ��
* @param    ��void
* @retval   ��void
* @details  ��
*            ����˿�
* @fn       ��
************************************************************/
void EXTI5_Swier_Init(void)
{
    NVIC_InitTypeDef   NVIC_InitStruct;    /* NVIC(�ж�)���ýṹ�� */

    /* �ⲿ�ж����� */
    /* ����ж��¼��Ĵ��� */
    EXTI->SWIER &= ~(1<<5);

    /* �ж����� */
    NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;          /* �ⲿ�ж�5 */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   /* ��ռ���ȼ�1 */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;          /* �����ȼ�1 */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;             /* ʹ���ⲿ�ж�ͨ�� */
    NVIC_Init(&NVIC_InitStruct);                             /* �����ⲿ�ж� */

    /* �ж����μĴ��� */
    EXTI->IMR |= (1<<5);
}



/**********************************************************
* @funcName ��EXTI9_5_IRQHandler
* @brief    ���ⲿ�жϷ�����
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetFlagStatus(EXTI_Line5) == SET)
    {
        GPIOB->ODR ^= (1<<5);            /* LED0��ת */
        EXTI->PR |= (1<<5);              /* д1����жϱ�־ */
    }
}







