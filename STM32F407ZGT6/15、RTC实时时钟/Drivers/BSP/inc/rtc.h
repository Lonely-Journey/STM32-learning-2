#ifndef __RTC_H
#define __RTC_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"
    #include "delay.h"
    #include "usart.h"



    /****************     �ṹ�嶨��     ****************/
    /* RTC�ṹ�� */
    typedef struct
    {
        uint16_t year;  /* �� */
        uint8_t month;  /* �� */
        uint8_t day;    /* �� */
        uint8_t week;   /* ���� 1-7 ����һ�������� */
        uint8_t hour;   /* ʱ */
        uint8_t min;    /* �� */
        uint8_t sec;    /* �� */
        uint8_t ampm;   /* AM/PM, 0=AM/24H; 1=PM/12H; */
    } RTC_DATES;


    /****************    �����ⲿ����   *****************/
    extern RTC_DATES rtc;


    /****************    �����ⲿ����   *****************/
    uint16_t RTC_Read_Bkr(uint8_t bkrx);                          /* RTC��ȡ������SRAM */
    void RTC_Write_Bkr(uint8_t bkrx, uint16_t data);              /* RTCд�������SRAM */
    static uint8_t RTC_Init_Mode(void);                           /* RTC�����ʼ��ģʽ */
    static uint8_t RTC_Wait_Synchro(void);                        /* �ȴ�RSFͬ�� */
    static uint8_t RTC_DecDcd(uint8_t val);                       /* ʮ����ת��ΪBCD�� */
    static uint8_t RTC_DcdDec(uint8_t val);                       /* BCD��ת��Ϊʮ�������� */
    uint8_t RTC_Week(uint16_t year, uint8_t month, uint8_t day);  /* ͨ�������ջ�ȡ�������� */

    uint8_t MyRTC_Init(void);                                     /* ��RTC���г�ʼ������ */
    uint8_t RTC_Set_Time(RTC_DATES rtc);                          /* ����RTCʱ�� */
    void RTC_Get_Time(void);                                      /* ��ȡRTCʱ�� */
    void RTC_Set_Alarm(uint8_t week, uint8_t hour, uint8_t min, uint8_t sec);    /* ����RTC���� */
    void RTC_Set_WakeUp(uint8_t wksel, uint16_t cnt);                            /* ����RTC�����ж� */

#endif
