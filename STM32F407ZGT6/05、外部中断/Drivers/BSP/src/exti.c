#include "exti.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\exti.c
*              .\Drivers\BSP\inc\exti.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-06-16
* @brief     ���ⲿ�жϴ���
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��EXTI0_Init
* @brief    ���ⲿ�жϳ�ʼ��
* @param    ��void
* @retval   ��void
* @details  ��
*            KEY_UP--------PA0 �ߵ�ƽ���£��͵�ƽ̧��
* @fn       ��
************************************************************/
void EXTI0_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;   /* GPIOx���ýṹ�� */
    EXTI_InitTypeDef EXTI_InitStruct;   /* �ⲿ�ж����ýṹ�� */
    NVIC_InitTypeDef NVIC_InitStruct;   /* NVIC(�ж�)���ýṹ�� */

    /* ʱ��ʹ��     SYSCFG  GPIOA */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

    /* GPIOx���� */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;       /* PA0 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;     /* ͨ������ */
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL; /* �������� */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; /* �ٶ�Ϊ50MHz */
    GPIO_Init(GPIOA,&GPIO_InitStruct);             /* �����趨������ʼ��PA0 */

    /* ӳ��ܽ� */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0); /* PA0 ���ӵ��ж���0(SYSCFG->EXTICR1�Ĵ���) */

    /*�ж�����*/
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;            /* �ⲿ�ж�0 */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   /* ��ռ���ȼ�1 */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;          /* �����ȼ�1 */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;             /* ʹ���ⲿ�ж�ͨ�� */
    NVIC_Init(&NVIC_InitStruct);                             /* ���� */

    /* �ⲿ�ж��߳�ʼ�� */
    EXTI_InitStruct.EXTI_Line    = EXTI_Line0;          /* ѡ���ⲿ�ж���0 */
    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt; /* �ж��¼� */
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; /* �����ش���    (EXTI_RTSR�Ĵ����ĵ�0λ) */
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;              /* ʹ��0�ߵ��ж� (EXTI->IMR�Ĵ����ĵ�0λ) */
    EXTI_Init(&EXTI_InitStruct);                        /* ��ʼ���ⲿ�ж�0 */
}



/**********************************************************
* @funcName ��EXTI0_IRQHandler
* @brief    ���ⲿ�ж�0������
* @param    ��void
* @retval   ��void
* @details  ��
*            KEY_UP--------PA0 �ߵ�ƽ���£��͵�ƽ̧��
* @fn       ��
************************************************************/
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetFlagStatus(EXTI_Line0) == SET)
    {
        delay_ms(20);
        if((GPIOA->IDR & (1<<0)) == 0)
        {
            GPIOF->ODR ^= (1<<9);     /* LED0��ת */
            EXTI->PR |= (1<<0);       /* д1����жϱ�־ */
        }
    }
}


