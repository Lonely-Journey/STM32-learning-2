#include "norflash.h"
#include "spi.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\norflash.c
*              .\Drivers\BSP\inc\norflash.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-01
* @brief     ��ʶ��norflashоƬID����
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��NorFlash_Init
* @brief    ��NorFlashоƬ������ų�ʼ��
* @param    ��void
* @retval   ��void
* @details  ��
*             SPI_CS     PB12
* @fn       ��
************************************************************/
void NorFlash_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;     /* GPIOx���ýṹ�� */

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);    /* ʱ��ʹ��   GPIOB */

    /* GPIOx��ʼ������ */
    /* GPIOx�˿����� */
    /* PB12 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_12;        /* PB12 �˿����� */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;   /* ������� */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   /* IO���ٶ�Ϊ50MHz */
    GPIO_Init(GPIOB, &GPIO_InitStruct);              /* �����趨������ʼ��PB12 */
    /* �˿�������ݼĴ���(Ƭѡ����) */
    GPIO_SetBits(GPIOB,GPIO_Pin_12);                 /* �˿�����Ĵ��������(��������) */

    SPI2_Init();         /* SPI��ʼ�� */
}



/**********************************************************
* @funcName ��NorFlash_Read_ID
* @brief    ����ȡnorflashоƬ��ID
* @param    ��void
* @retval   ��uint16_t -- ����оƬID
* @details  ��
* @fn       ��
************************************************************/
uint16_t NorFlash_Read_ID(void)
{
    uint16_t id;

    SPI_CS_L;                               /* Ƭѡʹ�� */
    SPI2_RS_Byte(FLASH_ManufactDeviceID);   /* ���Ͷ� ID ���� */
    SPI2_RS_Byte(0);                        /* д��һ���ֽ� */
    SPI2_RS_Byte(0);
    SPI2_RS_Byte(0);
    id =  SPI2_RS_Byte(0xFF) << 8;          /* ��ȡ��8λ�ֽ� */
    id |= SPI2_RS_Byte(0xFF);               /* ��ȡ��8λ�ֽ� */
    SPI_CS_H;                               /* Ƭѡʧ�� */

    /* оƬID�б�ο�norfalsh.h��ĺ궨�� */
    printf("оƬID : %x\r\n", id);

    return id;
}































