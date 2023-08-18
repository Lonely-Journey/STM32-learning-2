#include "key.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\key.c
*              .\Drivers\BSP\inc\key.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-06-16
* @brief     �������������ô���
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��KEY_Init
* @brief    ����KEY��Ӧ��GPIO�ڽ��г�ʼ������
* @param    ��void
* @retval   ��void
* @details  ��
*            KEY0     PE4
*            KEY1     PE3
*            KEY2     PE2
*            KEY_UP   PA0
*                                 -------------���ģʽ
* @fn       ��
************************************************************/
void KEY_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;                    /* GPIOx���ýṹ�� */

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOA,GPIOEʱ��

    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4; /* KEY0 KEY1 KEY2��Ӧ���� */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;                     /* ��ͨ����ģʽ */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;                /* 100M */
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;                     /* ���� */
    GPIO_Init(GPIOE, &GPIO_InitStruct);                            /* ��ʼ��GPIOE2,3,4 */


    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;      /* WK_UP��Ӧ����PA0 */
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN ; /* ���� */
    GPIO_Init(GPIOA, &GPIO_InitStruct);          /* ��ʼ��GPIOA0 */
}



/**********************************************************
* @funcName ��KEY_Scan
* @brief    ������ɨ�躯��
* @param    ��void
* @retval   ��uint8_t   1--KEY0  2--KEY1  3--KEY2  4--KEY_UP
* @details  ��
*            KEY0��KEY1 �͵�ƽ���£��ߵ�ƽ̧��
*            KEY_UP     �ߵ�ƽ���£��͵�ƽ̧��
* @fn       ��
************************************************************/
uint8_t KEY_Scan(void)
{
    uint8_t temp = 0;
    static uint8_t key_flay = 1;

    if(key_flay && ( KEY_UP==1 || KEY0==0 || KEY1==0 || KEY2==0))
    {
        delay_ms(10);
        if( KEY_UP==1 || KEY0==0 || KEY1==0 || KEY2==0)
        {
            key_flay = 0;
            if(KEY0==0)        temp = 1;
            else if(KEY1==0)   temp = 2;
            else if(KEY2==0)   temp = 3;
            else if(KEY_UP==1) temp = 4;
        }
    }

    if(KEY_UP==0 && KEY0==1 && KEY1==1 && KEY2==1)
    {
        key_flay = 1;
    }

    return temp;
}



