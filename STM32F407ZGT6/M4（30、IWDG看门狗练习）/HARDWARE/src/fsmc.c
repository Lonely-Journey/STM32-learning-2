#include "stm32f4xx.h"   
#include "lcd.h"
#include "fsmc.h"

/*****************************************************
*��������  �������������ź���
*������    ��MyGPIO_AF_Set
*��������  ��void
*��������ֵ��void
*����      ���ú���������������ͬһ��GPIO������
********************************************************/
void MyGPIO_AF_Set(GPIO_TypeDef* GPIOx,u32 BITx,u8 AFx)
{  
	u8 pos;
	u8 i;
	for(i=0;i<16;i++)
	{
		pos = i;
		if((1<<pos)&BITx)  //�жϸ������Ƿ�Ҫ����
		{
      //pos>>3�������ж��Ǹ�λ���Ż��ǵ�λ����
			GPIOx->AFR[pos>>3]&=~(0X0F<<((pos&0X07)*4));
			GPIOx->AFR[pos>>3]|=(u32)AFx<<((pos&0X07)*4);
		}
	}
}
/*************************** FSMC �� �� LCD ******************************/
/****************************************************************************
*��������  ����LCD��Ӧ��GPIO�ڽ��г�ʼ������,���ض���GPIO�ڽ���FSMC����
*������    ��FSMC_LCD_Init
*��������  ��void
*��������ֵ��void
*����      ��
*     �����ߣ�//���� ����   �ٶ�100M
*           DO0---PD14	DO1---PD15	DO2---PD0	DO3---PD1
*           DO4---PE7	DO5---PE8	DO6---PE9	DO7---PE10
*           DO8---PE11	DO9---PE12	DO10---PE13	DO11---PE14
*           DO12---PE15	DO13---PD8	DO14---PD9	DO15---PD10
*     �����ߣ�//����  ����      �ٶ�100M
*           WR---PD5	RD---PD4	CS---PG12 	
*           RS/DC(����/����)---PF12	 
*           BL�����⣩--PB15   
*     ����
*          PB��15
*          PD��0-1  4-5  8-10  14-15  
*          PE��7-15
*          PF��12
*          PG��12           
*     ��Ҫ���õ���FSMC��Bank1������4
***************************************************************************/
void FSMC_LCD_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruc;                  //GPIOx���ýṹ�� 
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStruct;   //FSMC���ýṹ��
	FSMC_NORSRAMTimingInitTypeDef  ReadTiming;         //Ƭѡʱ��Ĵ������ýṹ�� ����ʱ����ƼĴ�����
	FSMC_NORSRAMTimingInitTypeDef  WriteTiming;        //д��ʱ��Ĵ������ýṹ��  (дʱ����ƼĴ���)
  
  //ʱ��ʹ��  GPIOB  GPIOD  GPIOE  GPIOF  GPIOG  FSMC
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);       //ʹ��FSMCʱ��
	
  /*GPIOx�˿�����*/
  //PB15
  GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_15;          //ѡ��PB15
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_OUT;        //ͨ�����
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;     //��������
  GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;        //�������
  GPIO_InitStruc.GPIO_Speed = GPIO_Speed_100MHz;    //�ٶ�Ϊ100MHz
  GPIO_Init(GPIOB,&GPIO_InitStruc);                 //�����趨������ʼ��PB15
	
	//PD��0-1  4-5  8-10  14-15 
	GPIO_InitStruc.GPIO_Pin = (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15);  //ѡ��PD0/1/4/5/8/9/10/14/15
	GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_AF;          //���ù���
	GPIO_Init(GPIOD,&GPIO_InitStruc); //�����趨������ʼ��PD0/1/4/5/8/9/10/14/15
  MyGPIO_AF_Set(GPIOD,0xc733,12);   //�˿ڸ���ΪFSMC    ����
	
	//PE��7-15
	GPIO_InitStruc.GPIO_Pin = (GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //ѡ��PE��7-15
	GPIO_Init(GPIOE,&GPIO_InitStruc); //�����趨������ʼ��PE��7-15
  MyGPIO_AF_Set(GPIOE,0xff80,12);   //�˿ڸ���ΪFSMC    ����
	
	//PF12
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_12; //ѡ��PF12
	GPIO_Init(GPIOF,&GPIO_InitStruc);      //�����趨������ʼ��PF12
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource12,GPIO_AF_FSMC);   //�˿ڸ���ΪFSMC
	
	//PG12
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_12; //ѡ��PG12
	GPIO_Init(GPIOG,&GPIO_InitStruc);      //�����趨������ʼ��PG12
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource12,GPIO_AF_FSMC);   //�˿ڸ���ΪFSMC
	
	/********** �ر��� **********/
  LCD_BL_L;
	
  /********* ����FSMC *********/
/***********************************************************************
*ע�⣺bank1��NE1~4,ÿһ��������һ��BCR+TCR+BWTR�������ܹ�12���Ĵ�����
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
*************************************************************************/
	ReadTiming.FSMC_AddressSetupTime = 0X5;	       //��ַ���ý׶εĳ���ʱ�䣺5��HCLK 1/168M=6ns*5=30ns	          (FSMC->BTR�Ĵ����ĵ�0-3λ)
  ReadTiming.FSMC_AddressHoldTime = 0x0;	       //��ַ���ֽ׶εĳ���ʱ��:Reserved(ģʽAδ�õ�)                (FSMC->BTR�Ĵ����ĵ�4-7λ)
  ReadTiming.FSMC_DataSetupTime = 8;			       //���ݽ׶εĳ���ʱ��:16��HCLK=6*8=48ns                        (FSMC->BTR�Ĵ����ĵ�8-15λ)
  ReadTiming.FSMC_BusTurnAroundDuration = 0x0;   //������ת�׶εĳ���ʱ�䣺0��HCLK ����                        (FSMC->BTR�Ĵ����ĵ�16-19λ)
  ReadTiming.FSMC_CLKDivision = 0x0;             //CLK �źŵ�ʱ�ӷ�Ƶ�ȣ�����                                  (FSMC->BTR�Ĵ����ĵ�20-23λ)
  ReadTiming.FSMC_DataLatency = 0x0;             //ͬ��ͻ�� NOR Flash �������ӳ٣�2�� CLK ʱ�����ڵ������ӳ�    (FSMC->BTR�Ĵ����ĵ�24-27λ)
  ReadTiming.FSMC_AccessMode = FSMC_AccessMode_A;//����ģʽ������ģʽ A                                        (FSMC->BTR�Ĵ����ĵ�28-29λ) 
    
	//SRAM/NOR-Flash д��ʱ��Ĵ���   (дʱ����ƼĴ���)
	WriteTiming.FSMC_AddressSetupTime =0x5;	        //��ַ���ý׶εĳ���ʱ�䣺5��HCLK 1/168M=6ns*5=30ns	         (FSMC->BWTR�Ĵ����ĵ�0-3λ)
  WriteTiming.FSMC_AddressHoldTime = 0x0;	        //��ַ���ֽ׶εĳ���ʱ��:Reserved(ģʽAδ�õ�)                (FSMC->BWTR�Ĵ����ĵ�4-7λ)	
  WriteTiming.FSMC_DataSetupTime = 8;		          //���ݽ׶εĳ���ʱ��:8��HCLK=6*8=48ns                         (FSMC->BWTR�Ĵ����ĵ�8-15λ)
  WriteTiming.FSMC_BusTurnAroundDuration = 0x0;   //������ת�׶εĳ���ʱ�䣺0��HCLK ʱ������                    (FSMC->BWTR�Ĵ����ĵ�16-19λ)
  WriteTiming.FSMC_CLKDivision = 0x0;             //CLK �źŵ�ʱ�ӷ�Ƶ�ȣ� ����                                (FSMC->BWTR�Ĵ����ĵ�20-23λ)
  WriteTiming.FSMC_DataLatency = 0x0;             //ͬ��ͻ�� NOR Flash �������ӳ٣�2 �� CLK ʱ�����ڵ������ӳ�  (FSMC->BWTR�Ĵ����ĵ�24-27λ)
  WriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;//����ģʽ������ģʽ A                                       (FSMC->BWTR�Ĵ����ĵ�28-29λ)   
    
	//SRAM/NOR-Flash Ƭѡ���ƼĴ���
	FSMC_NORSRAMInitStruct.FSMC_Bank                  = FSMC_Bank1_NORSRAM4;                     //ѡ���������bank1(��1)��NE4(����4)
  FSMC_NORSRAMInitStruct.FSMC_DataAddressMux        = FSMC_DataAddressMux_Disable;             //��ַ/���ݸ���ʹ��λ����ʹ��                             (FSMC->BCR�Ĵ����ĵ�1λ)
  FSMC_NORSRAMInitStruct.FSMC_MemoryType            = FSMC_MemoryType_SRAM;                    //�洢�����ͣ�SRAM�� ROM����                              (FSMC->BCR�Ĵ����ĵ�2-3λ)  
  FSMC_NORSRAMInitStruct.FSMC_MemoryDataWidth       = FSMC_MemoryDataWidth_16b;                //�洢���������߿�ȣ�16bit                               (FSMC->BCR�Ĵ����ĵ�4-5λ) 
  FSMC_NORSRAMInitStruct.FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable;            //ͻ��ʹ��λ����ֹͻ������ģʽ                            (FSMC->BCR�Ĵ����ĵ�8λ)
  FSMC_NORSRAMInitStruct.FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low;             //�ȴ��źż���λ��NWAIT �ߵ�ƽ��Ч                        (FSMC->BCR�Ĵ����ĵ�9λ)
	FSMC_NORSRAMInitStruct.FSMC_AsynchronousWait      = FSMC_AsynchronousWait_Disable;           //�첽�����ڼ�ĵȴ��źţ������첽Э��ʱ������ NWAIT �ź� (FSMC->BCR�Ĵ����ĵ�15λ)
  FSMC_NORSRAMInitStruct.FSMC_WrapMode              = FSMC_WrapMode_Disable;                   //����ͻ��ģʽ�Ƿ�֧�֣�δʹ��ֱ�ӻ���ͻ��                (FSMC->BCR�Ĵ����ĵ�10λ)
  FSMC_NORSRAMInitStruct.FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState;   //�ȴ�ʱ�����ã�NWAIT �ź��ڵȴ�����֮ǰ��һ������������Ч(FSMC->BCR�Ĵ����ĵ�11λ)
  FSMC_NORSRAMInitStruct.FSMC_WriteOperation        = FSMC_WriteOperation_Enable;	             //д��ʹ��λ��FSMC �ڴ洢������ʹ����д�����             (FSMC->BCR�Ĵ����ĵ�12λ)
  FSMC_NORSRAMInitStruct.FSMC_WaitSignal            = FSMC_WaitSignal_Disable;                 //�ȴ�ʹ��λ����ֹ NWAIT �ź�                             (FSMC->BCR�Ĵ����ĵ�13λ)
  FSMC_NORSRAMInitStruct.FSMC_ExtendedMode          = FSMC_ExtendedMode_Enable;                //����д�����Ƿ������ͬʱ�򣺶�дʹ�ò���ͬ��ʱ��        (FSMC->BCR�Ĵ����ĵ�14λ)
  FSMC_NORSRAMInitStruct.FSMC_WriteBurst            = FSMC_WriteBurst_Disable;                 //д��ͻ��ʹ�ܣ�ʼ�����첽ģʽ�½���д�����              (FSMC->BCR�Ĵ����ĵ�19λ)
	//SRAM/NOR-Flash Ƭѡʱ��Ĵ��� ����ʱ����ƼĴ�����
  FSMC_NORSRAMInitStruct.FSMC_ReadWriteTimingStruct = &ReadTiming;
	//SRAM/NOR-Flash д��ʱ��Ĵ���   (дʱ����ƼĴ���)
  FSMC_NORSRAMInitStruct.FSMC_WriteTimingStruct     = &WriteTiming;                       

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStruct);  //��ʼ��FSMC����

 	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // ʹ��BANK1����4	
}



/***************************FSMC �� �� �ⲿSRAN******************************/
/************************************************************************
*��������  �����ض���GPIO�ڽ���FSMC���ã�������FSMC�����������ⲿSRAN
*������    ��FSMC_SRAM_Init
*��������  ��void
*��������ֵ��void
*��������  ��
*     ��Ҫ���õ����ţ� 
*                   PD��0/1/4/5/8/9/10/11/12/13/14/15 
*                   PE��0/1/7/8/9/10/11/12/13/14/15
*                   PF��0/1/2/3/4/5/12/13/14/15
*                   PG��0/1/2/3/4/5/10
*              ��Ҫ���õ���FSMC��Bank1������3
*************************************************************************/
void FSMC_SRAM_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruc;                  //GPIOx���ýṹ�� 
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStruct;   //FSMC���ýṹ��
	FSMC_NORSRAMTimingInitTypeDef  ReadTiming;         //Ƭѡʱ��Ĵ������ýṹ�� ����ʱ����ƼĴ�����
	FSMC_NORSRAMTimingInitTypeDef  WriteTiming;        //д��ʱ��Ĵ������ýṹ��  (дʱ����ƼĴ���)
  
  //ʱ��ʹ��   GPIOD  GPIOE  GPIOF  GPIOG  FSMC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);       //ʹ��FSMCʱ��
	
	//�ر�ע�⣡��������
  //PG10,AF12(CS�ŵ���ǰ��,��ֹ��λ��CS�Ƿ����,�ƻ�ԭ������)
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource10,GPIO_AF_FSMC);   //�˿ڸ���ΪFSMC
	
  /*GPIOx�˿�����*/
	//PD��0-1  4-5  8-15 
	GPIO_InitStruc.GPIO_Pin = (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //ѡ��PD0/1/4/5/8/9/10/11/12/13/14/15
	GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_AF;          //���ù���
	GPIO_Init(GPIOD,&GPIO_InitStruc); //�����趨������ʼ��PD0/1/4/5/8/9/10/11/12/13/14/15
  MyGPIO_AF_Set(GPIOD,0xff33,12);   //�˿ڸ���ΪFSMC    ����
	
	//PE��0-1  7-15   
	GPIO_InitStruc.GPIO_Pin = (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //ѡ��PE0/1/7/8/9/10/11/12/13/14/15
	GPIO_Init(GPIOE,&GPIO_InitStruc); //�����趨������ʼ��PE��0/1/7/8/9/10/11/12/13/14/15
  MyGPIO_AF_Set(GPIOE,0xff83,12);   //�˿ڸ���ΪFSMC    ����
	
	//PF0-5 12-15     
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; //ѡ��PF0/1/2/3/4/5/12/13/14/15
	GPIO_Init(GPIOF,&GPIO_InitStruc);      //�����趨������ʼ��PF0/1/2/3/4/5/12/13/14/15
	MyGPIO_AF_Set(GPIOF,0xf03f,12);   //�˿ڸ���ΪFSMC
	
	//PG0-5 10   
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_10;   //ѡ��PG0/1/2/3/4/5/10
	GPIO_Init(GPIOG,&GPIO_InitStruc);      //�����趨������ʼ��PG0/1/2/3/4/5/10
	MyGPIO_AF_Set(GPIOG,0x003f,12);   //�˿ڸ���ΪFSMC
	
	/********** �ر��� **********/
  LCD_BL_L;
	
  /********* ����FSMC *********/
/***********************************************************************
*ע�⣺bank1��NE1~4,ÿһ��������һ��BCR+TCR+BWTR�������ܹ�12���Ĵ�����
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
*************************************************************************/
	//SRAM/NOR-Flash Ƭѡʱ��Ĵ��� ����ʱ����ƼĴ�����
	ReadTiming.FSMC_AddressSetupTime = 0X5;	       //��ַ���ý׶εĳ���ʱ�䣺5��HCLK 1/168M=6ns*5=30ns	         (FSMC->BTR�Ĵ����ĵ�0-3λ)
  ReadTiming.FSMC_AddressHoldTime = 0x0;	       //��ַ���ֽ׶εĳ���ʱ��:Reserved(ģʽAδ�õ�)                (FSMC->BTR�Ĵ����ĵ�4-7λ)
  ReadTiming.FSMC_DataSetupTime = 0x5;			     //���ݽ׶εĳ���ʱ��:5��HCLK=6*5=30ns                         (FSMC->BTR�Ĵ����ĵ�8-15λ)
  ReadTiming.FSMC_BusTurnAroundDuration = 0xF;   //������ת�׶εĳ���ʱ�䣺 15��HCLK ʱ������                  (FSMC->BTR�Ĵ����ĵ�16-19λ)
  ReadTiming.FSMC_CLKDivision = 0xF;             //CLK �źŵ�ʱ�ӷ�Ƶ�ȣ� 16��HCLK ����                        (FSMC->BTR�Ĵ����ĵ�20-23λ)
  ReadTiming.FSMC_DataLatency = 0xF;             //ͬ��ͻ�� NOR Flash �������ӳ٣�17 �� CLK ʱ�����ڵ������ӳ� (FSMC->BTR�Ĵ����ĵ�24-27λ)
  ReadTiming.FSMC_AccessMode = FSMC_AccessMode_A;//����ģʽ������ģʽ A                                        (FSMC->BTR�Ĵ����ĵ�28-29λ) 
    
	//SRAM/NOR-Flash д��ʱ��Ĵ���   (дʱ����ƼĴ���)
	WriteTiming.FSMC_AddressSetupTime =0x2;	        //��ַ���ý׶εĳ���ʱ�䣺2��HCLK 1/168M=6ns*2=12ns	           (FSMC->BWTR�Ĵ����ĵ�0-3λ)
  WriteTiming.FSMC_AddressHoldTime = 0x0;	        //��ַ���ֽ׶εĳ���ʱ��:Reserved(ģʽAδ�õ�)                 (FSMC->BWTR�Ĵ����ĵ�4-7λ)	
  WriteTiming.FSMC_DataSetupTime = 0x3;		        //���ݽ׶εĳ���ʱ��:3��HCLK=6*3=18ns                          (FSMC->BWTR�Ĵ����ĵ�8-15λ)
  WriteTiming.FSMC_BusTurnAroundDuration = 0x3;   //������ת�׶εĳ���ʱ�䣺3��HCLK ʱ������                     (FSMC->BWTR�Ĵ����ĵ�16-19λ)
  WriteTiming.FSMC_CLKDivision = 0x8;             //CLK �źŵ�ʱ�ӷ�Ƶ�ȣ� 8��HCLK ����                         (FSMC->BWTR�Ĵ����ĵ�20-23λ)
  WriteTiming.FSMC_DataLatency = 0xF;             //ͬ��ͻ�� NOR Flash �������ӳ٣�17 �� CLK ʱ�����ڵ������ӳ�  (FSMC->BWTR�Ĵ����ĵ�24-27λ)
  WriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;//����ģʽ������ģʽ A                                         (FSMC->BWTR�Ĵ����ĵ�28-29λ)  
    
	//SRAM/NOR-Flash Ƭѡ���ƼĴ���
	FSMC_NORSRAMInitStruct.FSMC_Bank                  = FSMC_Bank1_NORSRAM3;                     //ѡ���������bank1(��1)��NE3(����4)
  FSMC_NORSRAMInitStruct.FSMC_DataAddressMux        = FSMC_DataAddressMux_Disable;             //��ַ/���ݸ���ʹ��λ����ʹ��                             (FSMC->BCR�Ĵ����ĵ�1λ)
  FSMC_NORSRAMInitStruct.FSMC_MemoryType            = FSMC_MemoryType_SRAM;                    //�洢�����ͣ�SRAM�� ROM����                              (FSMC->BCR�Ĵ����ĵ�2-3λ)  
  FSMC_NORSRAMInitStruct.FSMC_MemoryDataWidth       = FSMC_MemoryDataWidth_16b;                //�洢���������߿�ȣ�16bit                               (FSMC->BCR�Ĵ����ĵ�4-5λ) 
  FSMC_NORSRAMInitStruct.FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable;            //ͻ��ʹ��λ����ֹͻ������ģʽ                            (FSMC->BCR�Ĵ����ĵ�8λ)
  FSMC_NORSRAMInitStruct.FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low;             //�ȴ��źż���λ��NWAIT �ߵ�ƽ��Ч                        (FSMC->BCR�Ĵ����ĵ�9λ)
	FSMC_NORSRAMInitStruct.FSMC_AsynchronousWait      = FSMC_AsynchronousWait_Disable;           //�첽�����ڼ�ĵȴ��źţ������첽Э��ʱ������ NWAIT �ź� (FSMC->BCR�Ĵ����ĵ�15λ)
  FSMC_NORSRAMInitStruct.FSMC_WrapMode              = FSMC_WrapMode_Disable;                   //����ͻ��ģʽ�Ƿ�֧�֣�δʹ��ֱ�ӻ���ͻ��                (FSMC->BCR�Ĵ����ĵ�10λ)
  FSMC_NORSRAMInitStruct.FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState;   //�ȴ�ʱ�����ã�NWAIT �ź��ڵȴ�����֮ǰ��һ������������Ч(FSMC->BCR�Ĵ����ĵ�11λ)
  FSMC_NORSRAMInitStruct.FSMC_WriteOperation        = FSMC_WriteOperation_Enable;	             //д��ʹ��λ��FSMC �ڴ洢������ʹ����д�����             (FSMC->BCR�Ĵ����ĵ�12λ)
  FSMC_NORSRAMInitStruct.FSMC_WaitSignal            = FSMC_WaitSignal_Disable;                 //�ȴ�ʹ��λ����ֹ NWAIT �ź�                             (FSMC->BCR�Ĵ����ĵ�13λ)
  FSMC_NORSRAMInitStruct.FSMC_ExtendedMode          = FSMC_ExtendedMode_Disable;               //����д�����Ƿ������ͬʱ�򣺶�дʹ����ͬ��ʱ��          (FSMC->BCR�Ĵ����ĵ�14λ)
  FSMC_NORSRAMInitStruct.FSMC_WriteBurst            = FSMC_WriteBurst_Disable;                 //д��ͻ��ʹ�ܣ�ʼ�����첽ģʽ�½���д�����              (FSMC->BCR�Ĵ����ĵ�19λ)
	//SRAM/NOR-Flash Ƭѡʱ��Ĵ��� ����ʱ����ƼĴ�����
  FSMC_NORSRAMInitStruct.FSMC_ReadWriteTimingStruct = &ReadTiming;
	//SRAM/NOR-Flash д��ʱ��Ĵ���   (дʱ����ƼĴ���)
  FSMC_NORSRAMInitStruct.FSMC_WriteTimingStruct     = &WriteTiming;                       

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStruct);  //��ʼ��FSMC����

 	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);  // ʹ��BANK1����3	
}











