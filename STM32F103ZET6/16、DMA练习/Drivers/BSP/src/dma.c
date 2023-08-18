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
* @funcName ��DMA1_Usart1_Init
* @brief    ����DMA1��ͨ��4���г�ʼ������(����USART1�ķ���ͨ��)
* @param    ��uint8_t* data (DMA�洢����ַ)
* @retval   ��void
* @details  ��
* @fn       ��
*            DMA1���洢�������ݰ��˵�����1����
************************************************************/
void DMA1_Usart1_Init(uint8_t* data)
{
    DMA_InitTypeDef   DMA_InitStruct;      /* DMA���ýṹ�� */

    /* ʱ��ʹ��  DMA1 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);   /* DMA1ʱ��ʹ�� */
    delay_ms(5);                                        /* �ȴ�DMAʱ���ȶ� */

    /* ��ʼ��ͨ��������ΪĬ������ */
    DMA_DeInit(DMA1_Channel4);

    /* DMA���� */
    DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&USART1->DR;             /* DMA�����ַ������1(DMA_CPARx�Ĵ���) */
    DMA_InitStruct.DMA_MemoryBaseAddr     = (u32)data;                    /* DMA�洢��0��ַ������һ�������ַ(DMA_CMARx�Ĵ���) */
    DMA_InitStruct.DMA_DIR                = DMA_DIR_PeripheralDST;        /* �洢��������ģʽ�����ݴӴ洢�������跽����(DMA_CCRx�Ĵ����ĵ�4λ) */
    DMA_InitStruct.DMA_BufferSize         = 0;                            /* ���ݴ�����(DMA_CNDTRx�Ĵ���) �Ȳ����� */
    DMA_InitStruct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;    /* ���������ģʽ�������ַָ��̶�(DMA_CCRx�Ĵ����ĵ�6λ) */
    DMA_InitStruct.DMA_MemoryInc          = DMA_MemoryInc_Enable;         /* �洢������ģʽ��ÿ�����ݴ���󣬴洢����ַָ�����(DMA_CCRx�Ĵ����ĵ�7λ) */
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  /* �������ݳ���8bit��ÿ�δ��������8�ֽ�����(DMA_CCRx�Ĵ����ĵ�8-9λ) */
    DMA_InitStruct.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;      /* �洢�����ݳ��ȣ�ÿ�δӴ洢������8�ֽ�����(DMA_CCRx�Ĵ����ĵ�10-11λ) */
    DMA_InitStruct.DMA_Mode               = DMA_Mode_Normal;              /* ѭ��ģʽѡ�񣺽�ֹѭ��ģʽ(DMA_CCRx�Ĵ����ĵ�5λ) */
    DMA_InitStruct.DMA_Priority           = DMA_Priority_Medium;          /* ���ȼ����е����ȼ�(DMA_CCRx�Ĵ����ĵ�12-13λ) */
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;                             /* �Ǵ洢�����洢��ģʽ(DMA_CCRx�Ĵ����ĵ�14λ) */
    DMA_Init(DMA1_Channel4, &DMA_InitStruct);                             /* ��ʼ��DMA */

    /* ���ڵ�DMA����/���չ���ʹ�� */
    USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);                        /* ʹ�ܴ��ڵķ��͹���(USART->CR3�Ĵ����ĵ�7λ) */
}



/**********************************************************
* @funcName ��DMA1_Enable
* @brief    ������DMA1���ݴ���
* @param    ��uint16_t cndtr (DMA����������)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void DMA1_Enable(uint16_t cndtr)
{
    DMA_Cmd(DMA1_Channel4, DISABLE);         /* �ر�DMA���� (DMA_CCR�Ĵ����ĵ�0λ) */

    while (DMA1_Channel4->CCR & (1 << 0));   /* ȷ��DMA���Ա����� */
    DMA1_Channel4->CNDTR = cndtr;            /* DMA���������� */

    DMA_Cmd(DMA1_Channel4, ENABLE);          /* ����DMA���� (DMA_CCR�Ĵ����ĵ�0λ) */
}










