#include "sys.h"

/*****************************************************
*��������  ���Զ������Ź���������
*������    ��IWDG_Init
*��������  ��u16 rlr
*��������ֵ��void
*����      ��ÿ��625��Ϊ1s
********************************************************/
void IWDG_Init(u16 rlr)
{
	//�����ڲ���������(RCC_CSR�Ĵ����ĵ�0λ)
	RCC_LSICmd(ENABLE);
	//�ȴ��ڲ����������������(RCC_CSR�Ĵ����ĵ�1λ)
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	
	//���д����
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //����0x5555 (IWDG->KR�Ĵ���)
	//����IWDG��Ƶϵ��
	IWDG_SetPrescaler(0x3);                       //32��Ƶ(IWDG_PR�Ĵ���)
	//���ؼĴ���
	IWDG_SetReload(rlr);                          //����IWDGװ��ֵ(IWDG->RLR�Ĵ���)
	//����ǰ����ιһ�ι�
	IWDG_ReloadCounter();                         //0xAAAA(IWDG->KR�Ĵ���)
	//ʹ�ܿ��Ź�
	IWDG_Enable(); 
}

















