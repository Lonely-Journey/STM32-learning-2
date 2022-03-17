#include "sys.h"

/*****************************************************
*��������  ����DMA1��ͨ��4���г�ʼ������
*������    ��DMA1_Channels4_Init
*��������  ��void
*��������ֵ��u16 ndtr,u8 *data 
*����      ��
*           DMA���洢�������ݰ��˵�����1����
********************************************************/
void DMA1_Channels4_Init(u16 ndtr,u8 *data)
{
	DMA_InitTypeDef   DMA_InitStruct;    //DMA���ýṹ��
	
	//ʱ��ʹ��  DMA1
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//DMA1ʱ��ʹ��
	
	//��ʼ��ͨ��������ΪĬ������
	DMA_DeInit(DMA1_Channel4);
		
	/*DMA����*/
  DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&USART1->DR;           //DMA�����ַ������1(DMA_CPARx�Ĵ���)
  DMA_InitStruct.DMA_MemoryBaseAddr = (u32)data;                      //DMA�洢��0��ַ������һ�������ַ(DMA_CMARx�Ĵ���)
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;                     //�洢��������ģʽ�����ݴӴ洢�������跽����(DMA_CCRx�Ĵ����ĵ�4λ)
  DMA_InitStruct.DMA_BufferSize = ndtr;                               //���ݴ�����(DMA_CNDTRx�Ĵ���)
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;       //���������ģʽ�������ַָ��̶�(DMA_CCRx�Ĵ����ĵ�6λ)
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                //�洢������ģʽ��ÿ�����ݴ���󣬴洢����ַָ�����(DMA_CCRx�Ĵ����ĵ�7λ)
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���8bit��ÿ�δ��������8�ֽ�����(DMA_CCRx�Ĵ����ĵ�8-9λ)
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;        //�洢�����ݳ��ȣ�ÿ�δӴ洢������8�ֽ�����(DMA_CCRx�Ĵ����ĵ�10-11λ)
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;                          //ѭ��ģʽѡ�񣺽�ֹѭ��ģʽ(DMA_CCRx�Ĵ����ĵ�5λ)
  DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;                  //�����ȼ����е����ȼ�(DMA_CCRx�Ĵ����ĵ�12-13λ)
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;                           //�Ǵ洢�����洢��ģʽ(DMA_CCRx�Ĵ����ĵ�14λ)
	DMA_Init(DMA1_Channel4, &DMA_InitStruct);//��ʼ��DMA
		
	//���ڵ�DMA����/���չ���ʹ��
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);        //ʹ�ܴ��ڵķ��͹���(USART->CR3�Ĵ����ĵ�7λ)  
	
	//������ʹ��
	DMA_Cmd(DMA1_Channel4, ENABLE);      //(DMA_SxCR�Ĵ����ĵ�0λ)
}
















