#include "capture.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\capture.c
*              .\Drivers\BSP\inc\capture.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-27
* @brief     ��������ϰ����
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��TIM4_Capture_Init
* @brief    ����ͨ�ö�ʱ��4���г�ʼ�����ò����������ź�
* @param    ��uint16_t arr (����ֵ)
* @param    ��uint16_t psc (Ԥ��Ƶֵ)
* @retval   ��void
* @details  ��
*            ���������  PB6   TIM4_CH1  ����ͨ��1  ӳ��ͨ��IC1��IC2
* @fn       ��
************************************************************/
void TIM4_Capture_Init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef        GPIO_InitStruct;             /* GPIOx���ýṹ�� */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      /* TIMx���ýṹ�� */
    TIM_ICInitTypeDef       TIM_ICInitStruct;            /* ����ͨ�����ýṹ�� */
    NVIC_InitTypeDef        NVIC_InitStruct;             /* NVIC(�ж�)���ýṹ�� */

    /* ʱ��ʹ��   GPIOB   TIM4 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); /* ʹ��GPIOBʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  /* ʹ��TIM4ʱ�� */

    /* GPIO������ */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_6;               /* PB6 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;             /* ���ù��� */
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;         /* ���������� */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;        /* �ٶ�100MHz */
    GPIO_Init(GPIOB,&GPIO_InitStruct);                     /* �����趨������ʼ��PB6 */

    /* TIM4��Ӧ���Ÿ���ӳ�� */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);  /* PB6 ����ΪTIM4 */

    /* TIMx���� */
    TIM_TimeBaseInitStruct.TIM_Period        = arr;               /* �Զ���װ��ֵ(TIMx->ARR�Ĵ���) */
    TIM_TimeBaseInitStruct.TIM_Prescaler     = psc;               /* ��ʱ����Ƶ(TIMx->PSC�Ĵ���) */
    TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;/* ���ϼ���ģʽ(TIMx->CR1�Ĵ����ĵ�4λ)(��ʱ��9ֻ�ܵ�������) */
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;      /* ʱ�ӷ�Ƶ:tDTS=tCK_INT(TIMx->CR1�Ĵ����ĵ�8-9λ) */
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);              /* ��ʼ��TIM4 */

    /* ����ͨ������ */
    /* ͨ��1ӳ�䵽T1 */
    TIM_ICInitStruct.TIM_Channel     = TIM_Channel_1;             /* ѡ�������ͨ�� */
    TIM_ICInitStruct.TIM_ICPolarity  = TIM_ICPolarity_Rising;     /* �����ز���(TIMx->CCER�Ĵ����ĵ�1λ) */
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;  /* ӳ�䵽TI1��(TIMx->CCMR1�Ĵ���������ģʽ�ĵ�0-1λ) */
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;            /* ���������Ƶ������Ƶ(TIMx->CCMR1�Ĵ���������ģʽ�ĵ�2-3λ) */
    TIM_ICInitStruct.TIM_ICFilter    = 0x00;                      /* ���������˲��������˲�(TIMx->CCMR1�Ĵ���������ģʽ�ĵ�4-7λ) */
    TIM_ICInit(TIM4, &TIM_ICInitStruct);                          /* ����ͨ�����ó�ʼ�� */
    /* ͨ��1ӳ�䵽T2 */
    /* ��Ϊ����һ��������һЩ��������ı� */
    TIM_ICInitStruct.TIM_Channel     = TIM_Channel_2;             /* ѡ�������ͨ�� */
    TIM_ICInitStruct.TIM_ICPolarity  = TIM_ICPolarity_Falling;    /* �½��ز���(TIMx->CCER�Ĵ����ĵ�1λ) */
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_IndirectTI;/* ӳ�䵽ͨ��1��(TI1)��(����������ֲ�������)(TIMx->CCMR1�Ĵ���������ģʽ�ĵ�0-1λ) */
    TIM_ICInit(TIM4, &TIM_ICInitStruct);                          /* ����ͨ�����ó�ʼ�� */

    /* ����ѡ��TI1 ���ؼ����(TIMx->SMCR�Ĵ����ĵ�4-6) */
    TIM_SelectInputTrigger(TIM4,TIM_TS_TI1FP1);
    /* ��ģʽѡ��(TIMx->SMCR�Ĵ����ĵ�0-2) */
    TIM_SelectSlaveMode(TIM4,TIM_SlaveMode_Reset);                 /* ��ģʽѡ�񣺸�λģʽ(TIMx->SMCR�Ĵ����ĵ�0-2λ) */

    /* ��ʱ���ж���ʽѡ��ʹ�� */
    TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);        /* ����CC1IE�����ж� */
    TIM_ITConfig(TIM4,TIM_IT_CC2,ENABLE);        /* ����CC2IE�����ж� */

    /* TIM4�ж�����-----NVIC ���� */
    NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;              /* TIM4�ж�ͨ�� */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;    /* ��ռ���ȼ�1 */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;           /* �����ȼ�1 */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;              /* IRQͨ��ʹ�� */
    NVIC_Init(&NVIC_InitStruct);                              /* ����ָ���Ĳ�����ʼ��VIC�Ĵ��� */

    /* ʹ����װ��Ӱ�ӼĴ������� */
    TIM_ARRPreloadConfig(TIM4, ENABLE);  /* ʹ��TIM4���ؼĴ���ARR */

    /* ʹ�ܶ�ʱ��4���� */
    TIM_Cmd(TIM4, ENABLE);               /* ʹ�ܶ�ʱ��4 */
}



/**********************************************************
* @funcName ��TIM4_IRQHandler
* @brief    ����ʱ��4�жϺ���
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void TIM4_IRQHandler(void)
{
    static uint16_t data_2;
    uint16_t data_1;
    uint16_t p_val;

    if(TIM_GetITStatus(TIM4,TIM_FLAG_CC2) != RESET)
    {
        /* �����ͨ��2�Ļ��־λ */
        TIM_ClearITPendingBit(TIM4,TIM_FLAG_CC2);
        /* �����¼� */
        /* CCR2 */
        data_2 = TIM4->CCR2;
    }

    if(TIM_GetITStatus(TIM4,TIM_FLAG_CC1) != RESET)
    {
        /* ���ͨ��1�ظ������־λ */
        TIM_ClearITPendingBit(TIM4,TIM_FLAG_CC1);
        /* ���ͨ��1�Ĳ����־λ */
        TIM_ClearITPendingBit(TIM4,TIM_FLAG_CC1OF);
        /* �����¼� */
        /* ��¼ֵCCR1 */
        data_1 = TIM_GetCapture1(TIM4);
        /* �������ں�ռ�ձ� */
        p_val = (float)data_2/data_1*100;

        printf("���ڣ�%d   �ߵ�ƽ��%d   ռ�ձȣ�%d%%\r\n",data_1,data_2,p_val);
    }
}


