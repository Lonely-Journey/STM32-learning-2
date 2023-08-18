#include "rtc.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\rtc.c
*              .\Drivers\BSP\inc\rtc.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-08-01
* @brief     ��RTC���ô���
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/* RTCʱ�� */
RTC_DATES rtc = {
    2023,       /* �� */
    8,          /* �� */
    1,          /* �� */
    0,          /* ���� 1-7 ����һ�������� */
    12,         /* ʱ */
    0,          /* �� */
    0,          /* �� */
    0           /* AM/PM, 0=AM/24H; 1=PM/12H; */
};



/**********************************************************
* @funcName ��RTC_Init
* @brief    ����RTC���г�ʼ������
* @param    ��void
* @retval   ��uint8_t  ---  (0���óɹ�  1/2����ʧ��)
* @details  ��
* @fn       ��
************************************************************/
uint8_t MyRTC_Init(void)
{
    uint16_t ssr = 0;       /* ������Ĵ���ֵ */
    uint16_t bkpflag = 0;   /* ����ֵ �ж��Ƿ�֮ǰ���ù� */
    uint16_t retry = 200;   /* �ȴ����� ������ѭ�� */
    uint32_t tempreg = 0;   /* ��żĴ���ֵ */

    rtc.week = RTC_Week(rtc.year, rtc.month, rtc.day);     /* ������������ */

    RCC->APB1ENR|=1<<28;        /* ʹ�ܵ�Դ�ӿ�ʱ�� */
    PWR->CR|=1<<8;              /* ���������ʹ��(RTC+SRAM) */

    bkpflag = RTC_Read_Bkr(0);      /* ��ȡ���ݼĴ���ֵ */

    if (bkpflag != 0X5050)          /* ֮ǰʹ�õĲ���LSE */
    {
        RCC->CSR |= 1 << 0;            /* �ڲ�������������ʹ�� */
        while (!(RCC->CSR & 0x02));    /* �ȴ��ڲ������������� */

        RCC->BDCR |= 1 << 0;           /* ���Կ����ⲿ�������� */
        while (retry && ((RCC->BDCR & 0X02) == 0))  /* �ȴ��ⲿ��������׼���� */
        {
            retry--;
            delay_ms(5);
        }

        tempreg = RCC->BDCR;        /* ��ȡBDCR��ֵ */
        tempreg &= ~(3 << 8);       /* ����8/9λ */

        if (retry == 0)tempreg |= 1 << 9;   /* LSE����ʧ��,����LSI. */
        else tempreg |= 1 << 8;             /* ѡ��LSE,��ΪRTCʱ�� */

        tempreg |= 1 << 15;     /* ʹ��RTCʱ�� */
        RCC->BDCR = tempreg;    /* ��������BDCR�Ĵ��� */

        /* �ر�RTC�Ĵ���д���� */
        RTC->WPR = 0xCA;
        RTC->WPR = 0x53;
        RTC->CR = 0;

        if(RTC_Init_Mode())
        {
            RCC->BDCR = 1 << 16;/* ��λBDCR */
            delay_ms(10);
            RCC->BDCR = 0;      /* ������λ */
            return 2;           /* ����RTC��ʼ��ģʽʧ�� */
        }

        RTC->PRER = 0XFF;       /* RTCͬ����Ƶϵ��(0~7FFF),����������ͬ����Ƶ,�������첽��Ƶ,Frtc=Fclks/((Sprec+1)*(Asprec+1)) */
        RTC->PRER |= 0X7F << 16;/* RTC�첽��Ƶϵ��(1~0X7F) */
        RTC->CR &= ~(1 << 6);   /* RTC����Ϊ,24Сʱ��ʽ */
        RTC->ISR &= ~(1 << 7);  /* �˳�RTC��ʼ��ģʽ */
        RTC->WPR = 0xFF;        /* ʹ��RTC�Ĵ���д���� */

        if (bkpflag != 0X5051)  /* BKP0�����ݼȲ���0X5050,Ҳ����0X5051,˵���ǵ�һ������,��Ҫ����ʱ������. */
        {
            RTC_Set_Time(rtc);  /* ����ʱ�� */
        }

        if (retry == 0)
        {
            RTC_Write_Bkr(0, 0X5051);   /* ����Ѿ���ʼ������,ʹ��LSI */
        }
        else 
        {
            RTC_Write_Bkr(0, 0X5050);   /* ����Ѿ���ʼ������,ʹ��LSE */
        }
    }
    else
    {
        retry = 10;     /* ����10��SSR��ֵ��û�仯,��LSE����. */
        ssr = RTC->SSR; /* ��ȡ��ʼֵ */

        while (retry)   /* ���ssr�Ĵ����Ķ�̬,���ж�LSE�Ƿ����� */
        {
            delay_ms(10);

            if (ssr == RTC->SSR)        /* �Ա� */
            {
                retry--;
            }
            else 
            {
                break;
            }
        }

        if (retry == 0) /* LSE����,������õȴ��´ν����������� */
        {
            RTC_Write_Bkr(0, 0XFFFF);   /* ��Ǵ����ֵ */
            RCC->BDCR = 1 << 16;        /* ��λBDCR */
            delay_ms(10);
            RCC->BDCR = 0;              /* ������λ */
        }
    }

    return 0;
}



/**********************************************************
* @funcName ��RTC_Read_Bkr
* @brief    ��RTC��ȡ������SRAM
* @param    ��uint8_t bkrx (�����Ĵ������,��Χ:0~41)
* @retval   ��uint8_t  ---  (��ȡ����ֵ)
* @details  ��
* @fn       ��
************************************************************/
uint16_t RTC_Read_Bkr(uint8_t bkrx)
{
    uint32_t temp = 0;
    temp = RTC_BASE + 0x50 + bkrx * 4;
    return (*(uint16_t *)temp); /* ���ض�ȡ����ֵ */
}



/**********************************************************
* @funcName ��RTC_Write_Bkr
* @brief    ��RTCд�������SRAM
* @param    ��uint8_t bkrx  (�����Ĵ������,��Χ:0~41)
* @param    ��uint16_t data (Ҫд�������,16λ����)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void RTC_Write_Bkr(uint8_t bkrx, uint16_t data)
{
    uint32_t temp = 0;
    PWR->CR |= 1 << 8;  /* ȡ��������д���� */
    temp = RTC_BASE + 0x50 + bkrx * 4;
    (*(uint16_t *)temp) = data;
}



/**********************************************************
* @funcName ��RTC_Init_Mode
* @brief    ��RTC�����ʼ��ģʽ
* @param    ��void
* @retval   ��uint8_t  ---  (0,�ɹ�;1,ʧ��)
* @details  ��
* @fn       ��
************************************************************/
static uint8_t RTC_Init_Mode(void)
{
    uint32_t retry = 0XFFFFF;

    if (RTC->ISR & (1 << 6))return 0;

    RTC->ISR |= 1 << 7; /* ����RTC��ʼ��ģʽ */

    while (retry && ((RTC->ISR & (1 << 6)) == 0x00))
    {
        retry--;    /* �ȴ�����RTC��ʼ��ģʽ�ɹ� */
    }
   
    if (retry == 0)
    {
        return 1;   /* ͬ��ʧ�� */
    }
    else 
    {
        return 0;   /* ͬ���ɹ� */
    }
}



/**********************************************************
* @funcName ��RTC_DecDcd
* @brief    ��ʮ����ת��ΪBCD��
* @param    ��uint8_t val (Ҫת����ʮ������) 
* @retval   ��uint8_t  ---  (BCD��)
* @details  ��
* @fn       ��
************************************************************/
static uint8_t RTC_DecDcd(uint8_t val)
{
    uint8_t bcdhigh = 0;

    while (val >= 10)
    {
        bcdhigh++;
        val -= 10;
    }

    return ((uint8_t)(bcdhigh << 4) | val);
}



/**********************************************************
* @funcName ��RTC_DcdDec
* @brief    ��BCD��ת��Ϊʮ��������
* @param    ��uint8_t val (Ҫת����BCD��) 
* @retval   ��uint8_t  ---  (ʮ��������)
* @details  ��
* @fn       ��
************************************************************/
static uint8_t RTC_DcdDec(uint8_t val)
{
    uint8_t temp = 0;
    temp = (val >> 4) * 10;
    return (temp + (val & 0X0F));
}



/**********************************************************
* @funcName ��RTC_Wait_Synchro
* @brief    ���ȴ�RSFͬ��
* @param    ��void
* @retval   ��uint8_t  ---  (0ͬ���ɹ�  1ͬ��ʧ��)
* @details  ��
* @fn       ��
************************************************************/
static uint8_t RTC_Wait_Synchro(void)
{
    uint32_t retry = 0XFFFFF;
    /* �ر�RTC�Ĵ���д���� */
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    RTC->ISR &= ~(1 << 5);  /* ���RSFλ */

    while (retry && ((RTC->ISR & (1 << 5)) == 0x00))
    {
        retry--;    /* �ȴ�Ӱ�ӼĴ���ͬ�� */
    }
    
    if (retry == 0)return 1;/* ͬ��ʧ�� */

    RTC->WPR = 0xFF;        /* ʹ��RTC�Ĵ���д���� */
    return 0;
}



/**********************************************************
* @funcName ��RTC_Set_Time
* @brief    ������RTC���ꡢ�¡��ա�ʱ���֡��롢AM/PM
* @param    ��RTC_DATES *rtc (Ҫ���õ�RTCʱ��ֵ)
* @retval   ��uint8_t  ---  (0���óɹ�  1����ʧ��)
* @details  ��
* @fn       ��
************************************************************/
uint8_t RTC_Set_Time(RTC_DATES rtc)
{
    uint32_t temp = 0;
    /* �ر�RTC�Ĵ���д���� */
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;

    if (RTC_Init_Mode())return 1;   /* ����RTC��ʼ��ģʽʧ�� */

    temp = (((uint32_t)rtc.ampm & 0X01) << 22) | ((uint32_t)RTC_DecDcd(rtc.hour) << 16) | ((uint32_t)RTC_DecDcd(rtc.min) << 8) | (RTC_DecDcd(rtc.sec));
    RTC->TR = temp;
    temp = (((uint32_t)rtc.week & 0X07) << 13) | ((uint32_t)RTC_DecDcd(rtc.year) << 16) | ((uint32_t)RTC_DecDcd(rtc.month) << 8) | (RTC_DecDcd(rtc.day));
    RTC->DR = temp;
    RTC->ISR &= ~(1 << 7);  /* �˳�RTC��ʼ��ģʽ */
    return 0;
}



/**********************************************************
* @funcName ��RTC_Get_Time
* @brief    ����ȡRTC���ꡢ�¡��ա����ڡ�ʱ���֡���
* @param    ��void
* @retval   ��RTC_DATES  ---  (��ȡRTCʱ��)
* @details  ��
* @fn       ��
************************************************************/
void RTC_Get_Time(void)
{
    uint32_t temp1 = 0;
    uint32_t temp2 = 0;

    while (RTC_Wait_Synchro()); /* �ȴ�ͬ�� */

    temp1 = RTC->TR;
    temp2 = RTC->DR;
    rtc.hour = RTC_DcdDec((temp1 >> 16) & 0X3F);
    rtc.min = RTC_DcdDec((temp1 >> 8) & 0X7F);
    rtc.sec = RTC_DcdDec(temp1 & 0X7F);
    rtc.ampm = temp1 >> 22;

    rtc.year = RTC_DcdDec((temp2 >> 16) & 0XFF);
    rtc.month = RTC_DcdDec((temp2 >> 8) & 0X1F);
    rtc.day = RTC_DcdDec(temp2 & 0X3F);
    rtc.week = (temp2 >> 13) & 0X07;
}




/**********************************************************
* @funcName ��RTC_Week
* @brief    ��ͨ�������ջ�ȡ��������
* @param    ��uint16_t year (���)
* @param    ��uint8_t month (�·�)
* @param    ��uint8_t day   (����)
* @retval   ��uint8_t  ---  (1~7,������һ~����)
* @details  ��
* @fn       ��
************************************************************/
uint8_t RTC_Week(uint16_t year, uint8_t month, uint8_t day)
{
    uint16_t temp2;
    uint8_t yearH, yearL;
    /* ���������ݱ� */
    uint8_t table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};

    yearH = year / 100;
    yearL = year % 100;

    /*  ���Ϊ21����,�������100 */
    if (yearH > 19)yearL += 100;

    /*  ����������ֻ��1900��֮��� */
    temp2 = yearL + yearL / 4;
    temp2 = temp2 % 7;
    temp2 = temp2 + day + table_week[month - 1];

    if (yearL % 4 == 0 && month < 3)temp2--;

    temp2 %= 7;

    if (temp2 == 0)temp2 = 7;

    return temp2;
}



/**********************************************************
* @funcName ��RTC_Set_Alarm
* @brief    ������RTC����
* @param    ��uint8_t week   (����)
* @param    ��uint8_t hour   (Сʱ)
* @param    ��uint8_t min    (����)
* @param    ��uint8_t sec    (����)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void RTC_Set_Alarm(uint8_t week, uint8_t hour, uint8_t min, uint8_t sec)
{
    NVIC_InitTypeDef   NVIC_InitStruct;    /* NVIC(�ж�)���ýṹ�� */

    /* �ر�RTC�Ĵ���д���� */
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    RTC->CR &= ~(1 << 8);   /* �ر�����A */

    while ((RTC->ISR & 0X01) == 0); /* �ȴ�����A�޸����� */

    RTC->ALRMAR = 0;        /* ���ԭ������ */
    RTC->ALRMAR |= 1 << 30; /* ���������� */
    RTC->ALRMAR |= 0 << 22; /* 24Сʱ�� */
    RTC->ALRMAR |= (uint32_t)RTC_DecDcd(week) << 24;   /* �������� */
    RTC->ALRMAR |= (uint32_t)RTC_DecDcd(hour) << 16;   /* Сʱ���� */
    RTC->ALRMAR |= (uint32_t)RTC_DecDcd(min) << 8;     /* �������� */
    RTC->ALRMAR |= (uint32_t)RTC_DecDcd(sec);          /* �������� */
    RTC->ALRMASSR = 0;      /* ��ʹ��SUB SEC */
    RTC->CR |= 1 << 12;     /* ��������A�ж� */
    RTC->CR |= 1 << 8;      /* ��������A */
    RTC->WPR = 0XFF;        /* ��ֹ�޸�RTC�Ĵ��� */

    RTC->ISR &= ~(1 << 8);  /* ���RTC����A�ı�־ */
    EXTI->PR = 1 << 17;     /* ���LINE17�ϵ��жϱ�־λ */
    EXTI->IMR |= 1 << 17;   /* ����line17�ϵ��ж� */
    EXTI->RTSR |= 1 << 17;  /* line17���¼��������ش��� */

    /* RTC�����ж�����-----NVIC ���� */
    NVIC_InitStruct.NVIC_IRQChannel = RTC_Alarm_IRQn;      /* RTC�����ж�ͨ�� */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority= 1;  /* ��ռ���ȼ�1 */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;        /* �����ȼ�1 */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;           /* IRQͨ��ʹ�� */
    NVIC_Init(&NVIC_InitStruct);                           /* ����ָ���Ĳ�����ʼ��VIC�Ĵ���*/
}



/**********************************************************
* @funcName ��RTC_Alarm_IRQHandler
* @brief    ��RTC�����ж�
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void RTC_Alarm_IRQHandler(void)
{
    if (RTC->ISR & (1 << 8))    /* ALARM A�ж�? */
    {
        RTC->ISR &= ~(1 << 8);  /* ����жϱ�־ */
        printf("ALARM A!\r\n");
    }

    EXTI->PR |= 1 << 17;        /* ����ж���17���жϱ�־ */
}



/**********************************************************
* @funcName ��RTC_Set_WakeUp
* @brief    �������Ի��Ѷ�ʱ������
* @param    ��wksel
*   @arg    ��000,RTC/16;001,RTC/8;010,RTC/4;011,RTC/2;100,RTC
*   @arg    ��10x,ck_spre,1Hz;11x,1Hz,��cntֵ����2^16(��cnt+2^16)
*   @note   ��ע��:RTC����RTC��ʱ��Ƶ��,��RTCCLK!
* @param    ��cnt (�Զ���װ��ֵ.����0,�����ж�.)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void RTC_Set_WakeUp(uint8_t wksel, uint16_t cnt)
{
    NVIC_InitTypeDef   NVIC_InitStruct;    /* NVIC(�ж�)���ýṹ�� */

    /* �ر�RTC�Ĵ���д���� */
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    RTC->CR &= ~(1 << 10);  /* �ر�WAKE UP */

    while ((RTC->ISR & 0X04) == 0);	/* �ȴ�WAKE UP�޸����� */

    RTC->CR &= ~(7 << 0);   /* ���ԭ�������� */
    RTC->CR |= wksel & 0X07;/* �����µ�ֵ */
    RTC->WUTR = cnt;        /* ����WAKE UP�Զ���װ�ؼĴ���ֵ */
    RTC->ISR &= ~(1 << 10); /* ���RTC WAKE UP�ı�־ */
    RTC->CR |= 1 << 14;     /* ����WAKE UP ��ʱ���ж� */
    RTC->CR |= 1 << 10;     /* ����WAKE UP ��ʱ�� */
    RTC->WPR = 0XFF;        /* ��ֹ�޸�RTC�Ĵ��� */
    EXTI->PR = 1 << 19;     /* ���LINE19�ϵ��жϱ�־λ */
    EXTI->IMR |= 1 << 19;   /* ����line19�ϵ��ж� */
    EXTI->RTSR |= 1 << 19;  /* line19���¼��������ش��� */

    /* RTC�����ж�����-----NVIC ���� */
    NVIC_InitStruct.NVIC_IRQChannel = RTC_WKUP_IRQn;       /* RTC�����ж�ͨ�� */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority= 1;  /* ��ռ���ȼ�1 */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;        /* �����ȼ�1 */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;           /* IRQͨ��ʹ�� */
    NVIC_Init(&NVIC_InitStruct);                           /* ����ָ���Ĳ�����ʼ��VIC�Ĵ���*/
}



/**********************************************************
* @funcName ��RTC_WKUP_IRQHandler
* @brief    ��RTC�����ж�
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void RTC_WKUP_IRQHandler(void)
{ 
    if (RTC->ISR & (1 << 10))   /* WK_UP�ж� */
    {
        RTC->ISR &= ~(1 << 10); /* ����жϱ�־ */
        printf("ʵʱʱ��:%d-%d-%d week:%d %d:%d:%d\r\n", rtc.year, rtc.month, rtc.day, rtc.week , rtc.hour, rtc.min, rtc.sec);
    }

    EXTI->PR |= 1 << 19;        /* ����ж���19���жϱ�־ */
}









