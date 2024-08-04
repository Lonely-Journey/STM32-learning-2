#include "iwdg.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\iwdg.c
*              .\Drivers\BSP\inc\iwdg.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-08-05
* @brief     ���������Ź���ϰ����
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��IWDG_Init
* @brief    ���Զ������Ź���������
* @param    ��uint8_t prer (Ԥ��Ƶ����Ƶ��:0~7(ֻ�е�3λ��Ч!))
*   @arg    ����Ƶ���� = 4 * 2^prer. �����ֵֻ����256!
* @param    ��uint16_t rlr (�Զ���װ��ֵ,0~0XFFF)
* @retval   ��void
* @details  ��
* @fn       ��
*           ʱ�����(���):Tout=((4 * 2^prer) * rlr) / 32 (ms).
************************************************************/
void IWDG_Init(uint8_t prer, uint16_t rlr)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); /* �ؼ��ּĴ�����ʹ�ܶ�IWDG->PR��IWDG->RLR��д������0x5555 ���д���� */
    IWDG_SetPrescaler(prer);                      /* ���÷�Ƶϵ�� */
    IWDG_SetReload(rlr);                          /* �Ӽ��ؼĴ��� IWDG->RLR */
    IWDG_ReloadCounter();                         /* ιһ�ι� */
    IWDG_Enable();                                /* ʹ�ܿ��Ź� */
}




/**********************************************************
* @funcName ��IWDG_Feed
* @brief    ��ι�������Ź�
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void IWDG_Feed(void)
{
    IWDG_ReloadCounter();    /* ιһ�ι� */
}



