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
    /* ADC3�ĵ�һ��ת��ʱͨ��5������ʱ��480������ (������ADCx->SMPRx) */
    ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1,ADC_SampleTime_480Cycles); 

    ADC_Cmd(ADC3, ENABLE);    /* ����ADת���� */
}



/**********************************************************
* @funcName ��ADC3_In5_Data
* @brief    ����ȡADC3ͨ��5��ת������
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
*            ����ת��������������
************************************************************/
uint16_t ADC3_In5_Data(void)
{
    ADC_SoftwareStartConv(ADC3);                     /* ��������ת��ͨ�� */
    while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));  /* �ȴ�ת������ */
    return ADC_GetConversionValue(ADC3);             /* �������һ��ADC3�������ת����� */
}



/**********************************************************
* @funcName ��Oversampling
* @brief    ��ADC�������㷨����߷ֱ��ʣ���12�ֱ��ʱ�Ϊ16�ֱ���
* @param    ��void
* @retval   ��uint32_t --- ADC��16�ֱ�������
* @details  ��
* @fn       ��
*            ����ת��������������
************************************************************/
uint16_t Oversampling(void)
{
    int i;
    uint32_t sum=0;

    for(i=0; i<256; i++)
    {
        sum+=ADC3_In5_Data();
    }

    return (sum>>4);
}









