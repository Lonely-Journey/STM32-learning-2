#include "pwm.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\pwm.c
*              .\Drivers\BSP\inc\pwm.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-27
* @brief     ��pwm��ϰ����
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��TIM14_PWM_Init
* @brief    ����ͨ�ö�ʱ��14���г�ʼ�����ò����PWM
* @param    ��uint16_t arr (����ֵ)
* @param    ��uint16_t psc (Ԥ��Ƶֵ)
* @param    ��uint16_t ccr (�Ƚ�ֵ/ռ�ձ�)
* @retval   ��void
* @details  ��
*            PWM�����  PF9   TIM14_CH1  ���ͨ��1
* @fn       ��
*            ��Ч��ƽΪ�ߵ�ƽ
*            ��ʱ�����ϼ���
*            �������Ч��ƽ�������Ч��ƽ
************************************************************/
void TIM14_PWM_Init(uint16_t arr, uint16_t psc, uint16_t ccr)
{
    GPIO_InitTypeDef        GPIO_InitStruct;             /* GPIOx���ýṹ�� */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      /* TIMx���ýṹ�� */
    TIM_OCInitTypeDef       TIM_OCInitStruct;            /* ͨ�����ýṹ�� */

    /* ʹ��ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);    /* TIM14ʱ��ʹ�� */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);   /* ʹ��GPIOFʱ�� */

    /* ���� */
    GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14);  /* PF9����Ϊ��ʱ��14 */

    /* �������� */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;        /* PF9 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;      /* ���ù��� */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz; /* �ٶ�100MHz */
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;     /* ���츴����� */
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;      /* ���� */
    GPIO_Init(GPIOF,&GPIO_InitStruct);              /* ��ʼ��PF9 */

    /* ��ʱ������ */
    TIM_TimeBaseInitStruct.TIM_Prescaler     = psc;                /* ��ʱ����Ƶ(TIMx->PSC�Ĵ���)*/
    TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up; /* ���ϼ���ģʽ(TIMx->CR1�Ĵ����ĵ�4λ)(ͨ�ö�ʱ��12ֻ�е�������) */
    TIM_TimeBaseInitStruct.TIM_Period        = arr;                /* �Զ���װ��ֵ(TIMx->ARR�Ĵ���) */
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;       /* ʱ�ӷ�Ƶ:tDTS=tCK_INT(TIMx->CR1�Ĵ����ĵ�8-9λ) */
    TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStruct);               /* ��ʼ����ʱ��14 */

    /* ��ʼ��TIM14 Channel1 PWMģʽ */
    TIM_OCInitStruct.TIM_OCMode      = TIM_OCMode_PWM1;        /* ����ΪPWMģʽ1(TIMx_CCMR1�Ĵ��������ģʽ�ĵ�4-6λ) */
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; /* ͨ��ѡ�����ģʽ(TIMx->CCER�Ĵ����ĵ�0λ) */
    TIM_OCInitStruct.TIM_OCPolarity  = TIM_OCPolarity_Low;     /* ������Ч��ƽΪ�͵�ƽ(TIMx->CCER�Ĵ����ĵ�1λ) */
    TIM_OC1Init(TIM14, &TIM_OCInitStruct);                     /* ����Tָ���Ĳ�����ʼ������TIM14OC1 */

    TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);  /* ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ��� */
    TIM_ARRPreloadConfig(TIM14,ENABLE);                 /* ARRʹ�� */
    TIM_Cmd(TIM14, ENABLE);                             /* ʹ��TIM14 */
}



