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
    ADC_InitTypeDef   ADC_InitStruct;         /* ADCxһ�����ýṹ�� */

    /* ʱ��ʹ��  ADC1 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

    ADC_DeInit(ADC1);                    /* ADC1�ӿڸ�λ,������ADC1��ȫ���Ĵ�������Ϊȱʡֵ(RCC_APB2RSTR�Ĵ����ĵ�15λ) */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);    /* �����߽���6��Ƶ12MHZ */

    /* ADCx��ʼ������ */
    ADC_InitStruct.ADC_Mode               = ADC_Mode_Independent;       /* ����ģʽ(ADC_CR1�Ĵ����ĵ�16-19λ) */
    ADC_InitStruct.ADC_ScanConvMode       = DISABLE;                    /* ��ɨ��(ADC_CR1�Ĵ����ĵ�8λ) */
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                    /* ����ת��ģʽ(ADC_CR2�Ĵ����ĵ�1λ) */
    ADC_InitStruct.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;  /* ������ʽ���������(ADC_CR2�Ĵ����ĵ�17-19λ) */
    ADC_InitStruct.ADC_DataAlign          = ADC_DataAlign_Right;        /* ���ݶ��룺�Ҷ���(ADC_CR2�Ĵ����ĵ�11λ) */
    ADC_InitStruct.ADC_NbrOfChannel       = 1;                          /* �м���ת��(ADC_SQR1�Ĵ����ĵ�20-23λ) */
    ADC_Init(ADC1, &ADC_InitStruct);                                    /* ADC1��ʼ�� */

    /* �����ڲ��¶ȴ�����(ADC_CR2�Ĵ����ĵ�23λ) */
    ADC_TempSensorVrefintCmd(ENABLE); 

    /* ADC����ͨ������ƫ�򼰲���ʱ�����ú��� */
    /* ADC1�ĵ�һ��ת������ͨ��16������ʱ��239.5����    (������ADCx->SMPRx) */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_239Cycles5); 

    /* ADC1ʹ�ܺ��� */
    ADC_Cmd(ADC1, ENABLE);

    /* У׼����(���Ǳ���� )*/ 
    /* ��λ��������ò���Ӳ���������У׼�Ĵ�������ʼ�����λ��������� */
    ADC_ResetCalibration(ADC1);                 /* ʹ�ܸ�λУ׼ */
    while(ADC_GetResetCalibrationStatus(ADC1)); /* �ȴ�У׼���� */
    /* ��λ����������Կ�ʼУ׼������У׼����ʱ��Ӳ����� */
    ADC_StartCalibration(ADC1);                 /* ����ADCУ׼ */
    while(ADC_GetCalibrationStatus(ADC1));      /* �ȴ�У׼���� */
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
    uint16_t data;

    /* ��������ͨ��ת������(ADC_CR2�Ĵ�����22λ) */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    /* �ȴ�ת�����(ADC_SR�Ĵ����ĵ�1λ) */
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
    /* ��ȡת���������(ADC_DR�Ĵ���) */
    data = ADC_GetConversionValue(ADC1);

    return data;
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

