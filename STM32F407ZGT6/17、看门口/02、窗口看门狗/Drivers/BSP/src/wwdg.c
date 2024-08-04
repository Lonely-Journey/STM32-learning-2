#include "wwdg.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\wwdg.c
*              .\Drivers\BSP\inc\wwdg.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-08-05
* @brief     �����ڿ��Ź���ϰ����
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/* ����WWDG������������ֵ,Ĭ��Ϊ���. */
uint8_t g_wwdg_cnt = 0x7f;


/**********************************************************
* @funcName ��WWDG_Init
* @brief    ����ʼ�����ڿ��Ź�
* @param    ��uint8_t tr (������ֵ)
* @param    ��uint8_t wr (����ֵ)
* @param    ��uint8_t fprer (��Ƶϵ����WDGTB��,��Χ:0~7,��ʾ2^WDGTB��Ƶ)
* @retval   ��void
* @details  ��
* @fn       ��
*           Fwwdg=PCLK1/(4096*2^fprer). һ��PCLK1=36Mhz
*           ���ʱ��=(4096*2^fprer)*(tr-0X3F)/PCLK3
*           ����fprer=4,tr=7f,PCLK1=36Mhz
*           �����ʱ��=4096*16*64/36Mhz=116.50ms
************************************************************/
void WWDG_Init(uint8_t tr, uint8_t wr, uint8_t fprer)
{
    NVIC_InitTypeDef NVIC_InitStruct;        /* NVIC(�ж�)���ýṹ�� */

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);    /* ʹ�ܴ��ڿ��Ź�ʱ�� */

    g_wwdg_cnt = tr & g_wwdg_cnt;   /* ��ʼ��WWDG_CNT. */
    WWDG_SetPrescaler(fprer);       /* ���÷�Ƶֵ */
    WWDG_SetWindowValue(wr);        /* ���ô���ֵ */

    NVIC_InitStruct.NVIC_IRQChannel = WWDG_IRQn;          /* ���ڿ��Ź��ж� */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;  /* ��ռ���ȼ�Ϊ1 */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;         /* �����ȼ�Ϊ1 */
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;            /* ʹ�ܴ��ڿ��Ź� */
    NVIC_Init(&NVIC_InitStruct);                          /* ���ô��ڿ��Ź��ж� */

    WWDG_ClearFlag();//�����ǰ�����жϱ�־λ
    WWDG_EnableIT();//������ǰ�����ж�
 
    WWDG_Enable(g_wwdg_cnt);  //���ü���ֵ�����������Ź�
}



/**********************************************************
* @funcName ��WWDG_Set_Counter
* @brief    ��������WWDG��������ֵ
* @param    ��uint8_t cnt (����ֵ)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void WWDG_Set_Counter(uint8_t cnt)
{
    WWDG->CR = (cnt & 0x7F);   /* ������7λ������ */
}



/**********************************************************
* @funcName ��WWDG_IRQHandler
* @brief    �����ڿ��Ź��жϷ������
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void WWDG_IRQHandler(void)
{
    WWDG->SR = 0X00;                /* �����ǰ�����жϱ�־λ */
    WWDG_Set_Counter(g_wwdg_cnt);   /* ���贰�ڿ��Ź���ֵ! */
    GPIOF->ODR ^= (1<<10);          /* LED1�̵���˸ */
}



