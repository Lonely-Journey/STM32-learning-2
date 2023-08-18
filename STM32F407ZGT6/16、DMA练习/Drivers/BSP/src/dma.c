#include "dma.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\dma.c
*              .\Drivers\BSP\inc\dma.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-08-05
* @brief     ��DMA��ϰ����
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��DMA2_Usart1_Init
* @brief    ����DMA2��ͨ��4���г�ʼ������(����USART1�ķ���ͨ��)
* @param    ��uint8_t* data (DMA�洢����ַ)
* @retval   ��void
* @details  ��
* @fn       ��
*            DMA2���洢�������ݰ��˵�����1����
************************************************************/
void DMA2_Usart1_Init(uint8_t ch, uint8_t* data)
{
    DMA_InitTypeDef   DMA_InitStruct;    /* DMA���ýṹ�� */

    /* ʱ��ʹ��  DMA2 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);  /* DMA2ʱ��ʹ�� */

    /* ��ʼ��������ͨ��������ΪĬ������ */
    DMA_DeInit(DMA2_Stream7);
    /* �ж�DMAx��������ʹ���Ƿ񱻹ر�(����رղ�������) */
    while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);   /* �ȴ�DMA������ */

    /* DMA���� */
    DMA_InitStruct.DMA_Channel            = DMA_Channel_4;                /* ͨ��ѡ��ͨ��4(DMA_SxCR�Ĵ����ĵ�25-27λ) */
    DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&USART1->DR;             /* DMA�����ַ������1(DMA->SxPAR�Ĵ���) */
    DMA_InitStruct.DMA_Memory0BaseAddr    = (u32)data;                    /* DMA�洢��0��ַ������һ�������ַ(DMA->SxM0AR�Ĵ���) */
    DMA_InitStruct.DMA_DIR                = DMA_DIR_MemoryToPeripheral;   /* �洢��������ģʽ�����ݴӴ洢�������跽����(DMA_SxCR�Ĵ����ĵ�6λ) */
    DMA_InitStruct.DMA_BufferSize         = 0;                            /* ���ݴ�����(DMA_SxCR�Ĵ���) */
    DMA_InitStruct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;    /* ���������ģʽ�������ַָ��̶�(DMA_SxCR�Ĵ����ĵ�9λ) */
    DMA_InitStruct.DMA_MemoryInc          = DMA_MemoryInc_Enable;         /* �洢������ģʽ��ÿ�����ݴ���󣬴洢����ַָ�����(DMA_SxCR�Ĵ����ĵ�10λ) */
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  /* �������ݳ���8bit��ÿ�δ��������8�ֽ�����(DMA_SxCR�Ĵ����ĵ�11-12λ) */
    DMA_InitStruct.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;      /* �洢�����ݳ��ȣ�ÿ�δӴ洢������8�ֽ�����(DMA_SxCR�Ĵ����ĵ�13-14λ) */
    DMA_InitStruct.DMA_Mode               = DMA_Mode_Normal;              /* ѭ��ģʽѡ�񣺽�ֹѭ��ģʽ(DMA_SxCR�Ĵ����ĵ�8λ) */
    DMA_InitStruct.DMA_Priority           = DMA_Priority_Medium;          /* �����ȼ����е����ȼ�(DMA_SxCR�Ĵ����ĵ�16-17λ) */
    DMA_InitStruct.DMA_FIFOMode           = DMA_FIFOMode_Disable;         /* ֱ��ģʽѡ�񣺽�ֱֹ��ģʽ(DMA_SxFCR�Ĵ����ĵ�2λ) */
    DMA_InitStruct.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;       /* FIFO ��ֵѡ�� FIFO ��������(DMA_SxFCR�Ĵ����ĵ�0-1λ) */
    DMA_InitStruct.DMA_MemoryBurst        = DMA_MemoryBurst_Single;       /* �洢��ͻ���������ã��洢��ͻ�����δ���(DMA_SxCR�Ĵ����ĵ�23-24λ) */
    DMA_InitStruct.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;   /* ����ͻ���������ã�����ͻ�����δ���(DMA_SxCR�Ĵ����ĵ�21-22λ) */
    DMA_Init(DMA2_Stream7, &DMA_InitStruct);                              /* ��ʼ��DMA Stream7 */
        
    //���ڵ�DMA����/���չ���ʹ��
    USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);                         /* ʹ�ܴ��ڵķ��͹���(USART->CR3�Ĵ����ĵ�7λ) */
}



/**********************************************************
* @funcName ��DMA2_Enable
* @brief    ������DMA2���ݴ���
* @param    ��uint16_t cndtr (DMA����������)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void DMA2_Enable(uint16_t ndtr)
{
    DMA_Cmd(DMA2_Stream7, DISABLE);                      /* �ر�DMA���� */
    while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);    /* ȷ��DMA���Ա����� */
    DMA_SetCurrDataCounter(DMA2_Stream7,ndtr);           /* ���ݴ����� */
    DMA_Cmd(DMA2_Stream7, ENABLE);                       /* ����DMA���� */
}





