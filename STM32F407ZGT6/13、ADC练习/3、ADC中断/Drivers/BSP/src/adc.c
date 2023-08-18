#include "adc.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\adc.c
*              .\Drivers\BSP\inc\adc.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-27
* @brief     ��ADC���ô���
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/***************************************
* F407��ADCʱ��Ƶ�ʹٷ�����: ������36Mhz
* F103��ADCʱ��Ƶ�ʹٷ�����: ������14Mhz
****************************************/



/**********************************************************
* @funcName ��ADC3_In5_Init
* @brief    ��ADC3�ĵ�5��ͨ����ʼ������
* @param    ��void
* @retval   ��void
* @details  ��
*             PF7----------ADC3_IN5   ģ��ģʽ
* @fn       ��
************************************************************/
void ADC3_In5_Init(void)
{
    GPIO_InitTypeDef      GPIO_InitStruct;       /* GPIOx���ýṹ�� */
    ADC_CommonInitTypeDef ADC_CommonInitStruct;  /* ADCxһ�����ýṹ�� */
    ADC_InitTypeDef       ADC_InitStruct;        /* ADCx��ʼ�����ýṹ�� */
    NVIC_InitTypeDef      NVIC_InitStruct;       /* �ж����ýṹ�� */

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); /* ʹ��GPIOFʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);  /* ʹ��ADC3ʱ�� */

    /* PF7 */
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_7;        /* PF7 */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;      /* ģ������ */
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ; /* ���������� */
    GPIO_Init(GPIOF, &GPIO_InitStruct);            /* GPIO��ʼ */
    /* ADC���� */
    ADC_CommonInitStruct.ADC_Mode             = ADC_Mode_Independent;         /* ����ģʽ(ADC->CCR�Ĵ����ĵ�0-4λ) */
    ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; /* ���������׶�֮����ӳ�5��ʱ��(ADC->CCR�Ĵ����ĵ�8-11λ) */
    ADC_CommonInitStruct.ADC_DMAAccessMode    = ADC_DMAAccessMode_Disabled;   /* DMAʧ��(ADC->CCR�Ĵ����ĵ�14-15λ) */
    ADC_CommonInitStruct.ADC_Prescaler        = ADC_Prescaler_Div4;           /* Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz(ADC->CCR�Ĵ����ĵ�16-17λ) */
    ADC_CommonInit(&ADC_CommonInitStruct);                                    /* ADCxһ�����ó�ʼ��(ADC->CCR) */

    ADC_InitStruct.ADC_Resolution           = ADC_Resolution_12b;             /* 12λģʽ(ADC->CR1�Ĵ����ĵ�24��25λ) */
    ADC_InitStruct.ADC_ScanConvMode         = DISABLE;                        /* ��ɨ��ģʽ(ADC->CR1�Ĵ����ĵ�8λ) */
    ADC_InitStruct.ADC_ContinuousConvMode   = DISABLE;                        /* �ر�����ת��(ADC->CR2�Ĵ����ĵ�1λ) */
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  /* ��ֹ������⣬ʹ���������(ADC->CR2�Ĵ����ĵ�20-21λ/��28-29λ) */
    ADC_InitStruct.ADC_DataAlign            = ADC_DataAlign_Right;            /* �Ҷ��� (ADC->CR2�Ĵ����ĵ�11λ) */
    ADC_InitStruct.ADC_NbrOfConversion      = 1;                              /* 1��ת���ڹ��������� Ҳ����ֻת����������1(ADC->SQR1�Ĵ����ĵ�20-23λ) */
    ADC_Init(ADC3, &ADC_InitStruct);                                          /* ADCx���ó�ʼ�� */

    /* ADC����ͨ������ƫ�򼰲���ʱ�����ú��� */
    /* ADC3�ĵ�һ��ת��ʱͨ��5������ʱ��480������        (������ADCx->SMPRx) */
    ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1,ADC_SampleTime_480Cycles); 

    /* �ж����� */
    ADC_ITConfig(ADC3, ADC_IT_EOC, ENABLE);   /* ��ADC3��EOC�ж�(ADC->CR1�Ĵ����ĵ�5λ) */

    //ADC1�ж�����-----NVIC ����
    NVIC_InitStruct.NVIC_IRQChannel = ADC_IRQn;            /* ADC�ж�ͨ�� */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority= 1;  /* ��ռ���ȼ�1 */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;        /* �����ȼ�1 */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;           /* IRQͨ��ʹ�� */
    NVIC_Init(&NVIC_InitStruct);                           /* ����ָ���Ĳ�����ʼ��NVIC�Ĵ��� */

    ADC_Cmd(ADC3, ENABLE);    /* ����ADת���� */
    
    /* ��������ͨ��ת������(ADC3->CR2�Ĵ����ĵ�30λ) */ 
    ADC_SoftwareStartConv(ADC3);
}



/**********************************************************
* @funcName ��ADC_IRQHandler
* @brief    ��ADC�жϷ�����
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void ADC_IRQHandler(void)
{
    uint16_t data;

    if(ADC_GetITStatus(ADC3,ADC_IT_EOC) == SET)
    { 
        /* ��ȡת��������� */
        data = ADC_GetConversionValue(ADC3);
        printf("data:%d\r\n",data);
    }

    /* �����־ */
    ADC_ClearITPendingBit(ADC3,ADC_IT_EOC);
}









