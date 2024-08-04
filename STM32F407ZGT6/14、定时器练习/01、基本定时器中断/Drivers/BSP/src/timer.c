#include "timer.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\timer.c
*              .\Drivers\BSP\inc\timer.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-27
* @brief     ��������ʱ���ж���ϰ����
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��TIM6_Init
* @brief    ���Զ�ʱ��6���г�ʼ������
* @param    ��uint16_t arr (����ֵ)
* @param    ��uint16_t psc (Ԥ��Ƶֵ)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void TIM6_Init(uint16_t arr, uint16_t psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx���ýṹ��
    NVIC_InitTypeDef   NVIC_InitStruct;                  //NVIC(�ж�)���ýṹ��

    /* ʱ��ʹ��  TIM6 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);  

    /* TIMx���� */
    TIM_TimeBaseInitStruct.TIM_Period        = arr;                /* �Զ���װ��ֵ(TIMx->ARR�Ĵ���) */
    TIM_TimeBaseInitStruct.TIM_Prescaler     = psc;                /* ��ʱ����Ƶ(TIMx->PSC�Ĵ���) */
    TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up; /* ���ϼ���ģʽ(TIMx->CR1�Ĵ����ĵ�4λ)(������ʱ��ֻ�е�������) */
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;       /* ʱ�ӷ�Ƶ:tDTS=tCK_INT(TIMx->CR1�Ĵ����ĵ�8-9λ)(ע�⣺������ʱ��û�����) */
    TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);                /* ��ʼ��TIM6 */

    /* TIMx�ж�ѡ��ʹ�� */
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);  /* ����ʱ��6�����ж� */

    /* TIM6�ж�����-----NVIC ���� */
    NVIC_InitStruct.NVIC_IRQChannel = TIM6_DAC_IRQn;        /* TIM6�ж�ͨ�� */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;  /* ��ռ���ȼ�1 */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;         /* �����ȼ�1 */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;            /* IRQͨ��ʹ�� */
    NVIC_Init(&NVIC_InitStruct);                            /* ����ָ���Ĳ�����ʼ��VIC�Ĵ��� */

    TIM_Cmd(TIM6,ENABLE);  /* ʹ�ܶ�ʱ��6 */
}



/**********************************************************
* @funcName ��TIM6_DAC_IRQn
* @brief    ����ʱ��6���жϷ�����
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void TIM6_DAC_IRQHandler(void)
{
    /* �ж��Ƿ�ʱ��6�ж� */
    if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET)
    {
        /* ���жϱ�־λ */
        TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
        /* �����¼� */
        GPIOF->ODR ^= (1<<10);     /* LED1��˸ */
    }
}



