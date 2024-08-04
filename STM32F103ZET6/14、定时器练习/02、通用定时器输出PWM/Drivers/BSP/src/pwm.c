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
* @funcName ��TIM3_PWM_Init
* @brief    ����ͨ�ö�ʱ��3���г�ʼ�����ò����PWM
* @param    ��uint16_t arr (����ֵ)
* @param    ��uint16_t psc (Ԥ��Ƶֵ)
* @param    ��uint16_t ccr (�Ƚ�ֵ/ռ�ձ�)
* @retval   ��void
* @details  ��
*            PWM�����  PB5   TIM3_CH2  ���ͨ��2
* @fn       ��
*            ��Ч��ƽΪ�ߵ�ƽ
*            ��ʱ�����ϼ���
*            �������Ч��ƽ�������Ч��ƽ
************************************************************/
void TIM3_PWM_Init(uint16_t arr, uint16_t psc, uint16_t ccr)
{
    GPIO_InitTypeDef   GPIO_InitStruct;               /* GPIOx���ýṹ�� */
    TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStruct; /* TIMx���ýṹ�� */
    TIM_OCInitTypeDef  TIM_OCInitStruct;              /* ���ͨ�����ýṹ�� */

    /* ʱ��ʹ��  GPIOB  TIM3 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* ������ӳ��Ĵ��� */
    /* AFIOʱ��ʹ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    /* ����ӳ��,ʹPB5���PWMͨ��2 */
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);

    /* GPIOx���� */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_5;          /* PB5 �˿����� */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;     /* ����������� */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;    /* IO���ٶ�Ϊ50MHz */
    GPIO_Init(GPIOB, &GPIO_InitStruct);               /* �����趨������ʼ��PB5 */

    /* TIMx��ʼ������ */
    TIM_TimeBaseInitStruct.TIM_Period        = arr;                 /* ����ֵ(TIMx_ARR�Ĵ���) */
    TIM_TimeBaseInitStruct.TIM_Prescaler     = psc;                 /* ��Ƶ(TIMx_PSC�Ĵ���) */
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        /* TDTS = Tck_tim(TIMx_CR1�Ĵ����ĵ�9-8λ)(������ʱ��û��) */
    TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;  /* ���ϼ���(TIMx_CR1�Ĵ����ĵ�4λ)(������ʱ��ֻ����) */
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);                 /* ��ʼ��TIM3 */

    /* PWM ͨ����ʼ������ */
    /* ͨ��2 ������� */
    TIM_OCInitStruct.TIM_OCMode      = TIM_OCMode_PWM1;             /* ����ΪPWMģʽ1(TIMx_CCMR1��OC1Mλ) */
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;      /* ͨ��ģʽ��ѡ��Ϊ���ģʽ(TIMx_CCER��CC1E) */
    TIM_OCInitStruct.TIM_Pulse       = ccr;                         /* ���ñȽϼĴ���(TIMx->CCR2) */
    TIM_OCInitStruct.TIM_OCPolarity  = TIM_OCPolarity_Low;           /* ������Ч��ƽΪ�͵�ƽ(TIMx_CCER��CC1Pλ) */
    TIM_OC2Init(TIM3, &TIM_OCInitStruct);                           /* ͨ��2��ʼ������ */
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);               /* ͨ��2�ȽϼĴ���Ӱ�ӼĴ���ʹ�� */

    /* ʹ����װ��Ӱ�ӼĴ������� */
    TIM_ARRPreloadConfig(TIM3, ENABLE);                             /* ʹ��TIM3���ؼĴ���ARR */

    /* ʹ�ܶ�ʱ��3���� */
    TIM_Cmd(TIM3, ENABLE);                                          /* ʹ�ܶ�ʱ��3 */
}

