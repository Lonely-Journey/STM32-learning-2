#include "adc.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\adc.c
*              .\Drivers\BSP\inc\adc.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-27
* @brief     ��ADC��оƬ�ڲ��¶ȴ���
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/*****************************************
* F407��ADCʱ��Ƶ�ʹٷ�����: ������36Mhz
* F103��ADCʱ��Ƶ�ʹٷ�����: ������14Mhz
******************************************/



/**********************************************************
* @funcName ��ADC1_In16_Init
* @brief    ��ADC1�ĵ�16��ͨ����ʼ������
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
*            �������ţ����ڲ�оƬ�¶�
************************************************************/
void ADC1_In16_Init(void)
{
    ADC_CommonInitTypeDef   ADC_CommonInitStruct;   /* ADCxһ�����ýṹ�� */
    ADC_InitTypeDef         ADC_InitStruct;         /* ADCx��ʼ�����ýṹ�� */

    /* ʱ��ʹ��   ADC1 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    ADC_TempSensorVrefintCmd(ENABLE);//ʹ���ڲ��¶ȴ�����(ADC->CCR�Ĵ����ĵ�23λ)

    /* ADCx�Ĵ������� */
    ADC_CommonInitStruct.ADC_Mode             = ADC_Mode_Independent;         /* ����ģʽ(ADC->CCR�Ĵ����ĵ�0-4λ) */
    ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; /* ���������׶�֮����ӳ�5��ʱ��(ADC->CCR�Ĵ����ĵ�8-11λ) */
    ADC_CommonInitStruct.ADC_DMAAccessMode    = ADC_DMAAccessMode_Disabled;   /* DMAʧ��(ADC->CCR�Ĵ����ĵ�14-15λ) */
    ADC_CommonInitStruct.ADC_Prescaler        = ADC_Prescaler_Div4;           /* Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz(ADC->CCR�Ĵ����ĵ�16-17λ) */
    ADC_CommonInit(&ADC_CommonInitStruct);                                    /* ADCxһ�����ó�ʼ��(ADC->CCR) */

    ADC_InitStruct.ADC_Resolution           = ADC_Resolution_12b;             /* 12λģʽ(ADC->CR1�Ĵ����ĵ�24��25λ) */
    ADC_InitStruct.ADC_ScanConvMode         = DISABLE;                        /* ��ɨ��ģʽ(ADC->CR1�Ĵ����ĵ�8λ) */
    ADC_InitStruct.ADC_ContinuousConvMode   = DISABLE;                        /* �ر�����ת��(ADC->CR2�Ĵ����ĵ�1λ) */
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  /* ��ֹ������⣬ʹ���������(ADC->CR2�Ĵ����ĵ�20-21λ/��28-29λ) */
    ADC_InitStruct.ADC_DataAlign            = ADC_DataAlign_Right;            /* �Ҷ���	(ADC->CR2�Ĵ����ĵ�11λ) */
    ADC_InitStruct.ADC_NbrOfConversion      = 1;                              /* 1��ת���ڹ��������� Ҳ����ֻת����������1(ADC->SQR1�Ĵ����ĵ�20-23λ) */
    ADC_Init(ADC1, &ADC_InitStruct);                                          /* ADCx���ó�ʼ�� */

    /* ADC����ͨ������ƫ�򼰲���ʱ�����ú��� */
    /* ADC1�ĵ�һ��ת��ʱͨ��16������ʱ��480������ (������ADCx->SMPRx) */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1,ADC_SampleTime_480Cycles);  

    ADC_Cmd(ADC1, ENABLE);  /* ʹ��ADCx��������ADת���� */
}



/**********************************************************
* @funcName ��ADC1_In16_Data
* @brief    ����ȡADC1ͨ��16��ת������
* @param    ��void
* @retval   ��uint16_t
* @details  ��
* @fn       ��
*            ���оƬ�¶�
************************************************************/
uint16_t ADC1_In16_Data(void)
{
    /* ��������ͨ��ת������(ADC1->CR2�Ĵ����ĵ�30λ) */
    ADC_SoftwareStartConv(ADC1);
    /* �ȴ�ת����� */
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){};/* �ȴ�ת������(ADC1->SR�Ĵ����ĵ�1λ) */
    /* ��ȡת���������(ADC1->DR) */
    return ADC_GetConversionValue(ADC1);   
}



/**********************************************************
* @funcName ��Oversampling
* @brief    ��ADC�������㷨����߷ֱ��ʣ���12�ֱ��ʱ�Ϊ16�ֱ���
* @param    ��void
* @retval   ��uint16_t  ---  (ADC��16�ֱ�������)
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
        sum+=ADC1_In16_Data();
    }

    return (sum>>4);
}



/**********************************************************
* @funcName ��ADC1_Chip_Temperature1
* @brief    ����ADCֵת��ΪоƬ�¶�ֵ(�ǹ�����)
* @param    ��void
* @retval   ��double
* @details  ��
* @fn       ��
*            ���ڼ��оƬ�¶�
************************************************************/
double ADC1_Chip_Temperature1(void)
{
    uint8_t i;
    uint32_t data;
    double sum = 0;
    double stm_t[6];

    for(i=0;i<5;i++)
    {
        data = ADC1_In16_Data();
        stm_t[i]=(float)data*(3.3/4096);     /* ��ѹֵ */
        stm_t[i]=(1.43-stm_t[i])/0.0043+25;  /* ת��Ϊ�¶�ֵ */
        sum += stm_t[i];
    }

    stm_t[5] = sum/5;

    return stm_t[5];
}



/**********************************************************
* @funcName ��ADC1_Chip_Temperature2
* @brief    ����ADCֵת��ΪоƬ�¶�ֵ(������)
* @param    ��void
* @retval   ��double
* @details  ��
* @fn       ��
*            ���ڼ��оƬ�¶�
************************************************************/
double ADC1_Chip_Temperature2(void)
{
    uint8_t i;
    uint32_t data;
    double sum = 0;
    double stm_t[6];

    for(i=0;i<5;i++)
    {
        data = Oversampling();
        stm_t[i]=(float)data*(3.3/65536);     /* ��ѹֵ */
        stm_t[i]=(1.43-stm_t[i])/0.0043+25;  /* ת��Ϊ�¶�ֵ */
        sum += stm_t[i];
    }

    stm_t[5] = sum/5;

    return stm_t[5];
}



