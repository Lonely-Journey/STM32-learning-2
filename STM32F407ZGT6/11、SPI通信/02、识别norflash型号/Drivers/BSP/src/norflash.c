#include "norflash.h"
#include "spi.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\w25q128.c
*              .\Drivers\BSP\inc\w25q128.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-01
* @brief     ��w25q128��д��������
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/***
*W25Q128�洢оƬ
*������С             ��16Mbyte == 128Mbit
*���п���             ��256��
*ÿ������������       ��16����
*ÿ��������ҳ         ��16ҳ
*ÿҳ�ֽ�             ��256�ֽ�
*��ַ��д���ʮ����������0x9fffff
*        9f:����   f������    f��ҳ    ff���ֽ�
*/



/**********************************************************
* @funcName ��NorFlash_Init
* @brief    ��NorFlash��ʼ��
* @param    ��void
* @retval   ��void
* @details  ��
*             SPI_CS     PB14
* @fn       ��
************************************************************/
void NorFlash_Init(void)
{
    GPIO_InitTypeDef   GPIO_InitStruct;    /* GPIOx���ýṹ�� */

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); /* ʹ��GPIOBʱ�� */

    /* GPIOB14 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_14;       /* PB14 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;     /* ��� */
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;     /* ������� */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz; /* 100MHz */
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;      /* ���� */
    GPIO_Init(GPIOB, &GPIO_InitStruct);             /* ��ʼ�� */

    GPIO_SetBits(GPIOB, GPIO_Pin_14);               /* ���͵�ƽ */

    SPI1_Init();                                    /* SPI��ʼ�� */
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
    SPI1_RS_Byte(FLASH_ManufactDeviceID);   /* ���Ͷ� ID ���� */
    SPI1_RS_Byte(0);                        /* д��һ���ֽ� */
    SPI1_RS_Byte(0);
    SPI1_RS_Byte(0);
    id =  SPI1_RS_Byte(0xFF) << 8;          /* ��ȡ��8λ�ֽ� */
    id |= SPI1_RS_Byte(0xFF);               /* ��ȡ��8λ�ֽ� */
    SPI_CS_H;                               /* Ƭѡʧ�� */

    /* оƬID�б�ο�norfalsh.h��ĺ궨�� */
    printf("оƬID : %x\r\n", id);

    return id;
}



