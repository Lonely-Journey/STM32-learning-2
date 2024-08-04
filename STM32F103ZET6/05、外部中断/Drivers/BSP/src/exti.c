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
* @funcName ��EXTI4_Init
* @brief    ���ⲿ�жϳ�ʼ��
* @param    ��void
* @retval   ��void
* @details  ��
*            KEY0--------PE4 �͵�ƽ���£��ߵ�ƽ̧��
* @fn       ��
************************************************************/
void EXTI4_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;        /* GPIOx���ýṹ�� */
    EXTI_InitTypeDef  EXTI_InitStruct;        /* �ⲿ�ж������ýṹ�� */
    NVIC_InitTypeDef  NVIC_InitStruct;        /* �ж����ýṹ�� */

    /* ʱ��ʹ��  AFIO  GPIOE */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   /* ʹ�ܸ��ù���ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);  /* GPIOE */

    /* GPIOx���� */
    /* PE */
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_4;       /* PE4�˿����� */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;    /* �������� */
    GPIO_Init(GPIOE, &GPIO_InitStruct);           /* �����趨������ʼ��GPIE PE4 */

    /* �ⲿ�ж����� */
    /* �ⲿ�ж����üĴ������� */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
    /* ��ʼ�����ⲿ�ж��� */
    EXTI_InitStruct.EXTI_Line    = EXTI_Line4;             /* ѡ���ⲿ�ж���4 */
    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt;    /* �ж��¼� */
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;   /* �½��ش���    (EXTI_RTSR�Ĵ����ĵ�0λ) */
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;                 /* ʹ��4�ߵ��ж� (EXTI->IMR�Ĵ����ĵ�0λ) */
    EXTI_Init(&EXTI_InitStruct);                           /* ��ʼ���ⲿ�ж�4 */

    /* �ж����� */
    NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;          /* �ⲿ�ж�4 */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; /* ��ռ���ȼ�1 */ 
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;        /* �����ȼ�1 */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;           /* ʹ���ⲿ�ж�ͨ�� */
    NVIC_Init(&NVIC_InitStruct);                           /* �����ж����ȼ� */
}



/**********************************************************
* @funcName ��EXTI4_IRQHandler
* @brief    ���ⲿ�жϷ�����
* @param    ��void
* @retval   ��void
* @details  ��
*            KEY0--------PE4 �͵�ƽ���£��ߵ�ƽ̧��
* @fn       ��
************************************************************/
void EXTI4_IRQHandler(void)
{
    if(EXTI_GetFlagStatus(EXTI_Line4) == SET)
    {
        delay_ms(20);
        if((GPIOE->IDR & (1<<4)) == 0)
        {
            GPIOB->ODR ^= (1<<5);     /* LED0��ת */
            EXTI->PR |= (1<<4);       /* д1����жϱ�־ */
        }
    }
}












