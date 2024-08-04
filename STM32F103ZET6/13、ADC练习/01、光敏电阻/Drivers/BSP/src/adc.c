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
* @funcName ��ADC3_In6_Init
* @brief    ��ADC3�ĵ�6��ͨ����ʼ������
* @param    ��void
* @retval   ��void
* @details  ��
*             PF8----------ADC3_IN6   ģ��ģʽ
* @fn       ��
************************************************************/
void ADC3_In6_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;     /* GPIOx���ýṹ�� */
    ADC_InitTypeDef   ADC_InitStruct;      /* ADCxһ�����ýṹ�� */

    /* ʱ��ʹ��  GPIOF  ADC3 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);

    /* GPIOx��ʼ������ */
    /* GPIOx�˿����� */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8;         /* PF8�˿����� */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AIN;      /* ģ���������� */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   /* IO���ٶ�Ϊ50MHz */
    GPIO_Init(GPIOF, &GPIO_InitStruct);              /* �����趨������ʼ��PF8 */

    ADC_DeInit(ADC3);                                /* ADC3�ӿڸ�λ,������ADC3��ȫ���Ĵ�������Ϊȱʡֵ(RCC_APB2RSTR�Ĵ����ĵ�15λ) */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);                /* �����߽���6��Ƶ12MHZ */

    /*ADCx��ʼ������*/
    ADC_InitStruct.ADC_Mode               = ADC_Mode_Independent;       /* ����ģʽ(ADC_CR1�Ĵ����ĵ�16-19λ) */
    ADC_InitStruct.ADC_ScanConvMode       = DISABLE;                    /* ��ɨ��(ADC_CR1�Ĵ����ĵ�8λ) */
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                    /* ����ת��ģʽ(ADC_CR2�Ĵ����ĵ�1λ) */
    ADC_InitStruct.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;  /* ������ʽ���������(ADC_CR2�Ĵ����ĵ�17-19λ) */
    ADC_InitStruct.ADC_DataAlign          = ADC_DataAlign_Right;        /* ���ݶ��룺�Ҷ���(ADC_CR2�Ĵ����ĵ�11λ) */
    ADC_InitStruct.ADC_NbrOfChannel       = 1;                          /* �м���ת��(ADC_SQR1�Ĵ����ĵ�20-23λ) */
    ADC_Init(ADC3, &ADC_InitStruct);                                    /* ADC3��ʼ�� */

    /* ADC����ͨ������ƫ�򼰲���ʱ�����ú��� */
    /* ADC3�ĵ�һ��ת������ͨ��6������ʱ��239.5����    (������ADCx->SMPRx) */
    ADC_RegularChannelConfig(ADC3, ADC_Channel_6, 1, ADC_SampleTime_239Cycles5);

    /* ADC3ʹ�ܺ��� */
    ADC_Cmd(ADC3, ENABLE);

    /* У׼����(���Ǳ����) */ 
    /* ��λ��������ò���Ӳ���������У׼�Ĵ�������ʼ�����λ��������� */
    ADC_ResetCalibration(ADC3);                 /* ʹ�ܸ�λУ׼ */
    while(ADC_GetResetCalibrationStatus(ADC3)); /* �ȴ�У׼���� */
    /* ��λ����������Կ�ʼУ׼������У׼����ʱ��Ӳ����� */
    ADC_StartCalibration(ADC3);                 /* ����ADУ׼ */
    while(ADC_GetCalibrationStatus(ADC3));      /* �ȴ�У׼���� */
}



/**********************************************************
* @funcName ��ADC3_In6_Data
* @brief    ����ȡADC3ͨ��6��ת������
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
*            ����ת��������������
************************************************************/
uint16_t ADC3_In6_Data(void)
{
    uint16_t data;

    /* ��������ͨ��ת������(ADC_CR2�Ĵ�����22λ) */
    ADC_SoftwareStartConvCmd(ADC3, ENABLE);
    /* �ȴ�ת�����(ADC_SR�Ĵ����ĵ�1λ) */
    while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));
    /* ��ȡת���������(ADC_DR�Ĵ���) */
    data = ADC_GetConversionValue(ADC3);

    return data;
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
        sum+=ADC3_In6_Data();
    }

    return (sum>>4);
}



