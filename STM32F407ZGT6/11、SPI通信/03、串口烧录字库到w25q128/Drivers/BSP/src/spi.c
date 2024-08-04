#include "spi.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\spi.c
*              .\Drivers\BSP\inc\spi.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-01
* @brief     ��SPI���ô���
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��SPI1_Init
* @brief    ����SPI1ͨ��Ҫ�õ�GPIO�ڽ��г�ʼ���Ͷ�SPI1�ļĴ�����������
* @param    ��void
* @retval   ��void
* @details  ��
*            PB3   SPI1_SCK   ʱ����  ��������
*            PB4   SPI1_MISO  ������  ��������
*            PB5   SPI1_MOSI  �����  ��������
*            ���ԣ�SCKΪ�͵�ƽΪ����״̬
*            ��λ�����ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ
* @fn       ��
************************************************************/
void SPI1_Init(void)
{
    GPIO_InitTypeDef   GPIO_InitStruct;    /* GPIOx���ýṹ�� */
    SPI_InitTypeDef    SPI_InitStruct;     /* SPI���ýṹ�� */

    /* ʱ��ʹ��   GPIOB   SPI1 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);    /* ʹ��GPIOBʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);     /* ʹ��SPI1ʱ�� */

    /* GPIOFB3,4,5��ʼ������ */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;   /* PB3~5 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;                       /* ���ù��� */
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;                      /* ������� */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;                  /* 100MHz */
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;                       /* ���� */
    GPIO_Init(GPIOB, &GPIO_InitStruct);                              /* ��ʼ��GPIOx */
    /* IO���� */
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);            /* PB3����Ϊ SPI1 */
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);            /* PB4����Ϊ SPI1 */
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);            /* PB5����Ϊ SPI1 */

    SPI_InitStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;  /* ����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��(SPI->CR1�ĵ�15��10λ) */
    SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;                  /* ����SPI����ģʽ:����Ϊ��SPI(SPI->CR1�ĵ�2λ) */
    SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;                  /* ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ(SPI->CR1�ĵ�11λ) */
    SPI_InitStruct.SPI_CPOL              = SPI_CPOL_High;                    /* ����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ(SPI->CR1�ĵ�1λ) */
    SPI_InitStruct.SPI_CPHA              = SPI_CPHA_2Edge;                   /* ����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ����� (SPI->CR1�ĵ�0λ) */
    SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;                     /* NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����(SPI->CR1�ĵ�9λ) */
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;          /* ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ4(SPI->CR1�ĵ�3-5λ) */
    SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;                 /* ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ(SPI->CR1�ĵ�7λ) */
    SPI_InitStruct.SPI_CRCPolynomial      = 7;                               /* CRCֵ����Ķ���ʽ */
    SPI_Init(SPI1, &SPI_InitStruct);  /* ����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ��� */

    SPI_Cmd(SPI1, ENABLE);            /* ʹ��SPI���� */

    SPI1_RS_Byte(0xff);               /* �������� */
}



/**********************************************************
* @funcName ��SPI1_RS_Byte
* @brief    ��SPI1����/����һ���ֽ�����(8λ)
* @param    ��uint8_t data
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
uint8_t SPI1_RS_Byte(uint8_t data)
{
    /* �жϷ��ͻ������Ƿ�Ϊ�� */
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, data);
    /* �жϽ��ջ������Ƿ�Ϊ�� */
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    data = SPI_I2S_ReceiveData(SPI1);

    return data;
}

