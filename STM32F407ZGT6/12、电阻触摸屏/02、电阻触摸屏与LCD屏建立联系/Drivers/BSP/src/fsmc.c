#include "fsmc.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\fsmc.c
*              .\Drivers\BSP\inc\fsmc.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-01
* @brief     ��FSMC���ô���
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��FSMC_LCD_Init
* @brief    ����LCD��Ӧ��GPIO�ڽ��г�ʼ������,���ض���GPIO�ڽ���FSMC����
* @param    ��void
* @retval   ��void
* @details  ��
*     �����ߣ�
*           DO0---PD14    DO1---PD15    DO2---PD0    DO3---PD1
*           DO4---PE7     DO5---PE8     DO6---PE9    DO7---PE10
*           DO8---PE11    DO9---PE12    DO10---PE13  DO11---PE14
*           DO12---PE15   DO13---PD8    DO14---PD9   DO15---PD10
*
*     �����ߣ� ͨ�����   ����   �ٶ�50M
*           WR---PD5    RD---PD4    CS---PG12
*           RS/DC(����/����)---PF12
*           BL�����⣩--PB15
*     ����
*         PB��15
*         PD��0-1   4-5   8-10   14-15
*         PE��7-15
*         PF��12
*         PG��12
*
*     ��Ҫ���õ���FSMC��Bank1������4
* @fn       ��
************************************************************/
void FSMC_LCD_Init(void)
{
    GPIO_InitTypeDef               GPIO_InitStruct;            /* GPIOx���ýṹ�� */
    FSMC_NORSRAMInitTypeDef        FSMC_NORSRAMInitStruct;     /* FSMC���ýṹ�� */
    FSMC_NORSRAMTimingInitTypeDef  ReadWriteTiming;            /* Ƭѡʱ��Ĵ������ýṹ��(��ʱ����ƼĴ���) */
    FSMC_NORSRAMTimingInitTypeDef  WriteTiming;                /* д��ʱ��Ĵ������ýṹ��(дʱ����ƼĴ���) */

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG, ENABLE);/* ʹ��PD,PE,PF,PGʱ�� */
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);   /* ʹ��FSMCʱ�� */

    /* PB��   PB��15 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_15;       /* PB15 �������,���Ʊ��� */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;     /* ��ͨ���ģʽ */
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;     /* ������� */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz; /* 100MHz */
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;      /* ���� */
    GPIO_Init(GPIOB, &GPIO_InitStruct);             /* ��ʼ��GPIO */

    /* PD��   PD��0-1   4-5   8-10   14-15 */
    GPIO_InitStruct.GPIO_Pin   = (3<<0)|(3<<4)|(7<<8)|(3<<14);  /* PD0,1,4,5,8,9,10,14,15 AF OUT */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;      /* ������� */
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;     /* ������� */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz; /* 100MHz */
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;      /* ���� */
    GPIO_Init(GPIOD, &GPIO_InitStruct);             /* ��ʼ��GPIO */

    /* PE��   PE��7-15 */
    GPIO_InitStruct.GPIO_Pin   = (0X1FF<<7);        /* PE7~15 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;      /* ������� */
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;     /* ������� */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz; /* 100MHz */
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;      /* ���� */
    GPIO_Init(GPIOE, &GPIO_InitStruct);             /* ��ʼ��GPIO */

    /* PF��   PF��12 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_12;        /* PF12 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;       /* ������� */
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;      /* ������� */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;  /* 100MHz */
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;       /* ���� */
    GPIO_Init(GPIOF, &GPIO_InitStruct);              /* ��ʼ��GPIO */

    /* PG��   PG��12 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_12;        /* PF12 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;       /* ������� */
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;      /* ������� */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;  /* 100MHz */
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;       /* ���� */
    GPIO_Init(GPIOG, &GPIO_InitStruct);              /* ��ʼ��GPIO */

    /* �˿ڸ��� */
    /* PD��   PD��0-1   4-5   8-10   14-15 */
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);
    /* PE��   PE��7-15 */
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);
    /* PF��   PF��12 */
    GPIO_PinAFConfig(GPIOF,GPIO_PinSource12,GPIO_AF_FSMC);
    /* PG��   PG��12 */
    GPIO_PinAFConfig(GPIOG,GPIO_PinSource12,GPIO_AF_FSMC);

    /********** �ر��� **********/
    GPIOB->ODR &= ~(1<<15);

    /***************FSMC�Ĵ�������***************/
    /***
    * ע�⣺bank1��NE1~4,ÿһ��������һ��BCR+TCR+BWTR�������ܹ�12���Ĵ�����
    *
    *      FSMC_BCRx �� FSMC_BTRx����ϳ� BTCR[8]�Ĵ����飬���ǵĶ�Ӧ��ϵ���£�
    *      BTCR[0]��Ӧ FSMC_BCR1��BTCR[1]��Ӧ FSMC_BTR1
    *      BTCR[2]��Ӧ FSMC_BCR2��BTCR[3]��Ӧ FSMC_BTR2
    *      BTCR[4]��Ӧ FSMC_BCR3��BTCR[5]��Ӧ FSMC_BTR3
    *      BTCR[6]��Ӧ FSMC_BCR4��BTCR[7]��Ӧ FSMC_BTR4
    *
    *      FSMC_BWTRx ����ϳ� BWTR[7]�����ǵĶ�Ӧ��ϵ���£�
    *      BWTR[0]��Ӧ FSMC_BWTR1��BWTR[2]��Ӧ FSMC_BWTR2��
    *      BWTR[4]��Ӧ FSMC_BWTR3��BWTR[6]��Ӧ FSMC_BWTR4��
    *      BWTR[1]��BWTR[3]�� BWTR[5]������û���õ���
    */
    /* SRAM/NOR-Flash Ƭѡʱ��Ĵ��� ����ʱ����ƼĴ�����*/
    ReadWriteTiming.FSMC_AddressSetupTime      = 0XF;    /* ��ַ����ʱ�䣨ADDSET��Ϊ16��HCLK 1/168M=6ns*16=96ns (FSMC->BTR�Ĵ����ĵ�0-3λ) */
    ReadWriteTiming.FSMC_AddressHoldTime       = 0;      /* ��ַ���ֽ׶εĳ���ʱ��:Reserved(ģʽAδ�õ�) (FSMC->BTR�Ĵ����ĵ�4-7λ) */
    ReadWriteTiming.FSMC_DataSetupTime         = 60;     /* ���ݱ���ʱ��Ϊ60��HCLK	=6*60=360ns (FSMC->BTR�Ĵ����ĵ�8-15λ) */
    ReadWriteTiming.FSMC_BusTurnAroundDuration = 0;      /* ������ת�׶εĳ���ʱ�䣺0��HCLK ���� (FSMC->BTR�Ĵ����ĵ�16-19λ) */
    ReadWriteTiming.FSMC_CLKDivision           = 0;      /* CLK �źŵ�ʱ�ӷ�Ƶ�ȣ����� (FSMC->BTR�Ĵ����ĵ�20-23λ) */
    ReadWriteTiming.FSMC_DataLatency           = 0;      /* ͬ��ͻ�� NOR Flash �������ӳ٣�2�� CLK ʱ�����ڵ������ӳ� (FSMC->BTR�Ĵ����ĵ�24-27λ) */
    ReadWriteTiming.FSMC_AccessMode            = FSMC_AccessMode_A;   /* ����ģʽ������ģʽ A (FSMC->BTR�Ĵ����ĵ�28-29λ) */

    /* SRAM/NOR-Flash д��ʱ��Ĵ���   (дʱ����ƼĴ���) */
    WriteTiming.FSMC_AddressSetupTime      = 9;      /* ��ַ����ʱ�䣨ADDSET��Ϊ9��HCLK =54ns (FSMC->BWTR�Ĵ����ĵ�0-3λ) */
    WriteTiming.FSMC_AddressHoldTime       = 0;      /* ��ַ���ֽ׶εĳ���ʱ��:Reserved(ģʽAδ�õ�) (FSMC->BWTR�Ĵ����ĵ�4-7λ) */
    WriteTiming.FSMC_DataSetupTime         = 8;      /* ���ݱ���ʱ��Ϊ6ns*8��HCLK=48ns (FSMC->BWTR�Ĵ����ĵ�8-15λ) */
    WriteTiming.FSMC_BusTurnAroundDuration = 0;      /* ������ת�׶εĳ���ʱ�䣺0��HCLK ʱ������ (FSMC->BWTR�Ĵ����ĵ�16-19λ) */
    WriteTiming.FSMC_CLKDivision           = 0;      /* CLK �źŵ�ʱ�ӷ�Ƶ�ȣ����� (FSMC->BWTR�Ĵ����ĵ�20-23λ) */
    WriteTiming.FSMC_DataLatency           = 0;      /* ͬ��ͻ�� NOR Flash �������ӳ٣�2 �� CLK ʱ�����ڵ������ӳ� (FSMC->BWTR�Ĵ����ĵ�24-27λ) */
    WriteTiming.FSMC_AccessMode            = FSMC_AccessMode_A;   /* ����ģʽ������ģʽ A (FSMC->BWTR�Ĵ����ĵ�28-29λ) */

    FSMC_NORSRAMInitStruct.FSMC_Bank                  = FSMC_Bank1_NORSRAM4;                    /* ѡ���������bank1(��1)��NE4(����4) */
    FSMC_NORSRAMInitStruct.FSMC_DataAddressMux        = FSMC_DataAddressMux_Disable;            /* ��ַ/���ݸ���ʹ��λ����ʹ�� (FSMC->BCR�Ĵ����ĵ�1λ) */
    FSMC_NORSRAMInitStruct.FSMC_MemoryType            = FSMC_MemoryType_SRAM;                   /* �洢�����ͣ�SRAM�� ROM���� (FSMC->BCR�Ĵ����ĵ�2-3λ) */
    FSMC_NORSRAMInitStruct.FSMC_MemoryDataWidth       = FSMC_MemoryDataWidth_16b;               /* �洢���������߿�ȣ�16bit (FSMC->BCR�Ĵ����ĵ�4-5λ) */
    FSMC_NORSRAMInitStruct.FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable;           /* ͻ��ʹ��λ����ֹͻ������ģʽ (FSMC->BCR�Ĵ����ĵ�8λ) */
    FSMC_NORSRAMInitStruct.FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low;            /* �ȴ��źż���λ��NWAIT �͵�ƽ��Ч (FSMC->BCR�Ĵ����ĵ�9λ) */
    FSMC_NORSRAMInitStruct.FSMC_AsynchronousWait      = FSMC_AsynchronousWait_Disable;          /* �첽�����ڼ�ĵȴ��źţ������첽Э��ʱ������ NWAIT �ź� (FSMC->BCR�Ĵ����ĵ�15λ) */
    FSMC_NORSRAMInitStruct.FSMC_WrapMode              = FSMC_WrapMode_Disable;                  /* ����ͻ��ģʽ�Ƿ�֧�֣�δʹ��ֱ�ӻ���ͻ�� (FSMC->BCR�Ĵ����ĵ�10λ) */
    FSMC_NORSRAMInitStruct.FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState;  /* �ȴ�ʱ�����ã�NWAIT �ź��ڵȴ�����֮ǰ��һ������������Ч (FSMC->BCR�Ĵ����ĵ�11λ) */
    FSMC_NORSRAMInitStruct.FSMC_WriteOperation        = FSMC_WriteOperation_Enable;             /* д��ʹ��λ��FSMC �ڴ洢������ʹ����д����� (FSMC->BCR�Ĵ����ĵ�12λ) */
    FSMC_NORSRAMInitStruct.FSMC_WaitSignal            = FSMC_WaitSignal_Disable;                /* �ȴ�ʹ��λ����ֹ NWAIT �ź� (FSMC->BCR�Ĵ����ĵ�13λ) */
    FSMC_NORSRAMInitStruct.FSMC_ExtendedMode          = FSMC_ExtendedMode_Enable;               /* ����д�����Ƿ������ͬʱ�򣺶�дʹ�ò���ͬ��ʱ�� (FSMC->BCR�Ĵ����ĵ�14λ) */
    FSMC_NORSRAMInitStruct.FSMC_WriteBurst            = FSMC_WriteBurst_Disable;                /* д��ͻ��ʹ�ܣ�ʼ�����첽ģʽ�½���д����� (FSMC->BCR�Ĵ����ĵ�19λ) */
    FSMC_NORSRAMInitStruct.FSMC_ReadWriteTimingStruct = &ReadWriteTiming;                       /* ��дʱ�� */
    FSMC_NORSRAMInitStruct.FSMC_WriteTimingStruct     = &WriteTiming;                           /* дʱ�� */
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStruct);                                                  /* ��ʼ��FSMC���� */

    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  /* ʹ��BANK1 */
}




