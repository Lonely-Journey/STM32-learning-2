#include "rtc.h"
#include "usart.h"



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
    0,          /* ���� */
    12,         /* ʱ */
    0,          /* �� */
    0           /* �� */
};



/**********************************************************
* @funcName ��RTC_Init
* @brief    ����RTC���г�ʼ������
* @param    ��void
* @retval   ��uint8_t  ---  (0���óɹ�  1����ʧ��)
* @details  ��
* @fn       ��
************************************************************/
uint8_t RTC_Init(void)
{
    uint16_t bkpflag = 0;   /* ����ֵ �ж��Ƿ�֮ǰ���ù� */
    uint16_t retry = 200;   /* �ȴ����� ������ѭ�� */
    uint32_t tempreg = 0;   /* ��żĴ���ֵ */
    uint32_t clockfreq = 0; /* RTC��ʹ�õ�ʱ��Ƶ�� */

    NVIC_InitTypeDef   NVIC_InitStruct;    /* NVIC(�ж�)���ýṹ�� */

    /* ʱ��ʹ��   PWR   BKP */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);   /* ʹ�ܵ�Դʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);   /* ʹ�ܱ���ʱ�� */
    /* ȡ��������д���� */
    PWR_BackupAccessCmd(ENABLE);               /* ������д������ʹ�ܺ󱸼Ĵ�������(PWR->CR�Ĵ����ĵ�8λ) */

    bkpflag = BKP_ReadBackupRegister(BKP_DR1); /* ��ȡ���ݼĴ���ֵ */

    if(bkpflag != 0X5050)          /* ֮ǰʹ�õĲ���LSE */
    {
        /*ʱ��Դѡ��*/
        RCC_LSEConfig(RCC_LSE_ON);  /* �����ⲿ��������(LSE) (RCC->BDCR�Ĵ����ĵ�0λ) */

        while (retry && (RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET))  /* �ȴ�LSE׼���� */
        {
            retry--;
            delay_ms(5);
        }

        tempreg = RCC->BDCR;        /* ��ȡBDCR��ֵ */
        tempreg &= ~(3 << 8);       /* ����8/9λ */

        if (retry == 0)             /* ����LSE(�ⲿʱ��)ʧ�ܣ�����LSI(�ڲ�ʱ��) */
        {
            RCC->CSR |= 1 << 0;                       /* LSIʹ�� */
            while (!(RCC->CSR & 0x02));               /* �ȴ�LSI���� */
            tempreg |= 1 << 9;                        /* LSE����ʧ��,����LSI. */
            clockfreq = 40000 - 1;                    /* LSIƵ��Լ40Khz(�ο�F103�����ֲ�˵��) */
            BKP_WriteBackupRegister(BKP_DR1, 0X5051); /* ����Ѿ���ʼ������,ʹ��LSI */
        }
        else
        {
            tempreg |= 1 << 8;                          /* ѡ��LSE,��ΪRTCʱ�� */
            clockfreq = 32768 - 1;                      /* LSEƵ��Ϊ32.769Khz */
            BKP_WriteBackupRegister(BKP_DR1, 0X5050);   /* ����Ѿ���ʼ������,ʹ��LSE */
        }

        tempreg |= 1 << 15;             /* ʹ��RTCʱ�� */
        RCC->BDCR = tempreg;            /* ��������BDCR�Ĵ��� */

        RTC_WaitForLastTask();          /* �ȴ�RTC�Ĵ���������� (RTC_CRL�Ĵ����ĵ�5λ) */
        RTC_WaitForSynchro();           /* �ȴ�RTC�Ĵ���ͬ��  (RTC_CRL�Ĵ����ĵ�3λ) */

        RTC_ITConfig(RTC_IT_SEC, ENABLE);   /* SECF = 1, �������ж� */
        RTC_ITConfig(RTC_IT_ALR, ENABLE);   /* ALRF = 1, ���������ж� */

        RTC_WaitForLastTask();          /* �ȴ�RTC�Ĵ���������� (RTC_CRL�Ĵ����ĵ�5λ) */

        /* RTC���� */
        RTC_EnterConfigMode();          /* ��������ģʽ(RTC_CRL�Ĵ����ĵ�4λ) */
        RTC_SetPrescaler(clockfreq);    /* ����RTCԤ��Ƶ��ֵ(0x7FFF��ST�ٷ��ṩ��һ���ο�ֵ���ɻ������Ϊ1���ӵ��ź�)(RTC_PRLL�Ĵ���) */
        RTC_ExitConfigMode();           /* �˳�����ģʽ(RTC_CRL�Ĵ����ĵ�4λ) */

        RTC_WaitForLastTask();          /* �ȴ����һ�ζ�RTC�Ĵ�����д�������(RTC_CRL�Ĵ����ĵ�5λ) */

        if(bkpflag != 0X5051)           /* BKP0�����ݼȲ���0X5050,Ҳ����0X5051,˵���ǵ�һ������,��Ҫ����ʱ������. */
        {
            RTC_Set_Time(rtc);          /* ����ʱ�� */
        }
    }
    else                /* ϵͳ������ʱ */
    {
        RTC_WaitForSynchro(); /* �ȴ�RTC�Ĵ���ͬ��  (RTC_CRL�Ĵ����ĵ�3λ) */

        retry = 100;          /* ���LSI/LSE�Ƿ��������� */

        tempreg = RTC->DIVL;  /* ��ȡDIVL�Ĵ�����ֵ */
        while(retry)
        {
            delay_ms(5);
            retry--;

            if (tempreg != RTC->DIVL)   /* �Ա�DIVL��tempreg, ����в���, ���˳� */
            {
                break;                  /* DIVL != tempreg, ˵��RTC�ڼ���, ˵������û���� */
            }
        }

        if(retry == 0)
        {
            BKP_WriteBackupRegister(BKP_DR1, 0XFFFF);   /* ��Ǵ����ֵ */
            RCC->BDCR = 1 << 16;                        /* ��λBDCR */
            delay_ms(10);
            RCC->BDCR = 0;                              /* ������λ */
            return 1;                                   /* ��ʼ��ʧ�� */
        }
        else
        {
            RTC_ITConfig(RTC_IT_SEC, ENABLE);   /* SECF = 1, �������ж� */
            RTC_ITConfig(RTC_IT_ALR, ENABLE);   /* ALRF = 1, ���������ж� */
            RTC_WaitForLastTask();              /* �ȴ�RTC�Ĵ���������� (RTC_CRL�Ĵ����ĵ�5λ) */
        }
    }

    /* RTC�ж����� */
    NVIC_InitStruct.NVIC_IRQChannel = RTC_IRQn;              /* ѡ��RTC�ж� */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   /* ��ռ���ȼ�1 */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;          /* �����ȼ�1 */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;             /* IRQͨ��ʹ�� */
    NVIC_Init(&NVIC_InitStruct);                             /* RTC�жϳ�ʼ�� */

    RTC_Get_Time();

    return 0;
}



/**********************************************************
* @funcName ��RTC_Set_Time
* @brief    ������RTC���ꡢ�¡��ա�ʱ���֡���
* @param    ��RTC_DATES *rtc (Ҫ���õ�RTCʱ��ֵ)
* @retval   ��uint8_t  ---  (0���óɹ�  1����ʧ��)
* @details  ��
* @fn       ��
*             ��1970��1��1��Ϊ��׼, �����ۼ�ʱ��
*             �Ϸ���ݷ�ΧΪ: 1970 ~ 2105��
************************************************************/
uint8_t RTC_Set_Time(RTC_DATES rtc)
{
    uint32_t seccount = 0;

    /* �ж����������Ƿ��г�����Χ */
    if((rtc.year<1970)||(rtc.year>2099)) return 1;

    seccount = RTC_DateSec(rtc.year, rtc.month, rtc.day, rtc.hour, rtc.min, rtc.sec); /* ��������ʱ����ת������������ */

    /* ����ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);   /* ʹ�ܵ�Դʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);   /* ʹ�ܱ���ʱ�� */
    PWR_BackupAccessCmd(ENABLE);                          /* ȡ��������д���� */
    /* ���������Ǳ����! */

    RTC_EnterConfigMode();      /* �������� (RTC_CRL�Ĵ����ĵ�4λ) */
    RTC_SetCounter(seccount);   /* ����RTC��������ֵ(RTC_CNTH / RTC_CNTL�Ĵ���) */
    RTC_ExitConfigMode();       /* �˳�����ģʽ(RTC_CRL�Ĵ����ĵ�4λ) */

    RTC_WaitForLastTask();      /* �ȴ����һ�ζ�RTC�Ĵ�����д�������(RTC_CRL�Ĵ����ĵ�5λ) */

    return 0;
}



/**********************************************************
* @funcName ��RTC_DateSec
* @brief    ����������ʱ����ת����������
* @param    ��uint16_t syear (���)
* @param    ��uint8_t smon   (�·�)
* @param    ��uint8_t sday   (����)
* @param    ��uint8_t hour   (Сʱ)
* @param    ��uint8_t min    (����)
* @param    ��uint8_t sec    (����)
* @retval   ��long  ---  (ת�����������)
* @details  ��
* @fn       ��
*            ��1970��1��1��Ϊ��׼, 1970��1��1��, 0ʱ0��0��, ��ʾ��0����
*            ����ʾ��2105��, ��Ϊuint32_t����ʾ136���������(����������)!
************************************************************/
static long RTC_DateSec(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec)
{
    uint32_t Y, M, D, X, T;
    signed char monx = smon;/* ���·�ת���ɴ����ŵ�ֵ, ����������� */

    if (0 >=  (monx -= 2))  /* 1..12 -> 11,12,1..10 */
    { 
        monx += 12;         /* Puts Feb last since it has leap day */
        syear -= 1;
    }

    Y = (syear - 1) * 365 + syear / 4 - syear / 100 + syear / 400;  /* ��ԪԪ��1�����ڵ������� */
    M = 367 * monx / 12 - 30 + 59;
    D = sday - 1;
    X = Y + M + D - 719162;                         /* ��ȥ��ԪԪ�굽1970������� */
    T = ((X * 24 + hour) * 60 + min) * 60 + sec;    /* �������� */
    return T;
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
    static uint16_t daycnt = 0; /*  */
    uint32_t seccount;   /* ������RTC->CNT�Ĵ�����ֵ */
    uint32_t day;        /* ���� */
    uint32_t temp;
    const uint8_t month_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};   /* ƽ����·����ڱ� */

    /* ��ȡʱ��(��ֵ) */
    seccount = RTC_GetCounter();

    /* ����1970�����ڵ�һ������������ */
    day = seccount / 86400;  /* һ����24 * 60 * 60 = 86400�� */

    /* �����������һ�� */
    if(daycnt != day)
    {
        temp = 1970;     /* ��ʼ��� */
        daycnt = day;
        /* ������������һ��(��1970Ϊ��׼���) */
        while(day >= 365)
        {
            /* �ж��ǲ������� */
            if(Leap_Year(temp))
            {
                if(day>=366) day -= 366;
                else break;
            }
            else day -= 365;
            temp++;
        }

        rtc.year = temp;      /* �洢�� */

        temp = 0;

        /* ���������ǵڼ����� */
        while(day>=28)
        {
            /* �ж��ǲ����������ǲ��Ƕ��� */
            if(Leap_Year(rtc.year) && (temp==1))
            {
                if(temp>=29)temp-=29;//�����������
                else break; 
            }
            else
            {
                /* �ж��ǲ��Ǵ�������µ����� */
                if(day>=month_table[temp]) day -= month_table[temp];
                else break;
            }
            temp++;
        }
        /* �洢�·ݺ����� */
        rtc.month = temp+1;       /* �õ��·� */
        rtc.day = day+1;          /* �õ����� */
    }

    temp = seccount % 86400;         /* �õ���ȥ�����պ������ */
    rtc.hour = temp / 3600;          /* Сʱ */
    rtc.min = (temp%3600) / 60;      /* ���� */
    rtc.sec = (temp%3600) % 60;      /* ���� */

    rtc.week = RTC_Week(rtc.year, rtc.month, rtc.day);             /* ��ȡ��ĩʱ�� */
}



/**********************************************************
* @funcName ��Leap_Year
* @brief    ���ж����껹��ƽ��
* @param    ��uint16_t year (���)
* @retval   ��uint8_t  ---  (0-ƽ�� 1-����)
* @details  ��
* @fn       ��
************************************************************/
static uint8_t Leap_Year(uint16_t year)
{
    /* ���꣺�ܱ�400�����������ܱ�4���������ܱ�100������ */
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}



/**********************************************************
* @funcName ��RTC_Week
* @brief    ��ͨ�������ջ�ȡ��������
* @param    ��uint16_t year (���)
* @param    ��uint8_t month (�·�)
* @param    ��uint8_t day   (����)
* @retval   ��uint8_t  ---  (0, ������; 1 ~ 6: ����һ ~ ������)
* @details  ��
* @fn       ��
*            ʹ�� ��ķ����ɭ���㹫ʽ ����
************************************************************/
uint8_t RTC_Week(uint16_t year, uint8_t month, uint8_t day)
{
    uint8_t week = 0;

    if (month < 3)
    {
        month += 12;
        year--;
    }

    week = (day + 1 + 2 * month + 3 * (month + 1) / 5 + year + (year >> 2) - year / 100 + year / 400) % 7;
    return week;
}



/**********************************************************
* @funcName ��RTC_Set_Alarm
* @brief    ������RTC����
* @param    ��uint16_t syear (���)
* @param    ��uint8_t smon   (�·�)
* @param    ��uint8_t sday   (����)
* @param    ��uint8_t hour   (Сʱ)
* @param    ��uint8_t min    (����)
* @param    ��uint8_t sec    (����)
* @retval   ��uint8_t  ---  (0���óɹ�  1����ʧ��)
* @details  ��
* @fn       ��
************************************************************/
uint8_t RTC_Set_Alarm(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec)
{
    uint32_t seccount = 0;

    seccount = RTC_DateSec(syear, smon, sday, hour, min, sec); /* ��������ʱ����ת������������ */

    /* ����ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);   /* ʹ�ܵ�Դʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);   /* ʹ�ܱ���ʱ�� */
    PWR_BackupAccessCmd(ENABLE);                          /* ȡ��������д���� */
    /* ���������Ǳ����! */

    RTC_EnterConfigMode();      /* �������� (RTC_CRL�Ĵ����ĵ�4λ) */
    RTC_SetAlarm(seccount);
    RTC_ExitConfigMode();       /* �˳�����ģʽ(RTC_CRL�Ĵ����ĵ�4λ) */

    RTC_WaitForLastTask();      /* �ȴ����һ�ζ�RTC�Ĵ�����д�������(RTC_CRL�Ĵ����ĵ�5λ) */

    return 0;
}



/**********************************************************
* @funcName ��RTC_IRQHandler
* @brief    ��RTC�ж�
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
*            ����RTC_CRL�Ĵ����� SECF �� ALRF λ�������ĸ��ж�
************************************************************/
void RTC_IRQHandler(void)
{
    if(RTC_GetFlagStatus(RTC_FLAG_SEC) == SET)    /* SECF = 1, �����ж� */
    {
        RTC_Get_Time();   /* ����ʱ�� */
        RTC->CRL &= ~(1 << 0);  /* SECF = 0, �������ж� */
        printf("ʵʱʱ��:%d-%d-%d week:%d %d:%d:%d\r\n", rtc.year, rtc.month, rtc.day, rtc.week , rtc.hour, rtc.min, rtc.sec);
    }

    /* ˳���������ӱ�־ */
    if(RTC_GetFlagStatus(RTC_FLAG_ALR) == SET)     /* ALRF = 1, ���ӱ�־ */
    {
        RTC->CRL &= ~(1 << 1);  /* ALRF = 0, �����ӱ�־ */
        /* �������ʱ�� */
        printf("����:%d-%d-%d %d:%d:%d\r\n", rtc.year, rtc.month, rtc.day, rtc.hour, rtc.min, rtc.sec);
    }

    RTC->CRL &= ~(1 << 2);          /* OWF = 0, �������жϱ�־ */
    while (!(RTC->CRL & (1 << 5))); /* �ȴ�RTC�Ĵ����������, ���ȴ�RTOFF == 1 */
}


