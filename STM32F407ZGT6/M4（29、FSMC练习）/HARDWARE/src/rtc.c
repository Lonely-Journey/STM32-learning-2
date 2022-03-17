#include "stm32f4xx.h" 
#include "rtc.h"
#include "stdio.h"

/*****************************************
*��������  ������RTC��ʱ���֡��롢����
*������    ��RTC_Set_Time
*��������  ��u8 hour,u8 min,u8 sec,u8 pm
*��������ֵ��void
*����      ��
*******************************************/
void RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
	RTC_TimeTypeDef RTC_TimeStruct;     //ʱ�����ýṹ��
	
	RTC_TimeStruct.RTC_Hours=hour;      //ʱ
	RTC_TimeStruct.RTC_Minutes=min;     //��
	RTC_TimeStruct.RTC_Seconds=sec;     //��
	RTC_TimeStruct.RTC_H12=ampm;        //am/pm
	
	RTC_SetTime(RTC_Format_BIN,&RTC_TimeStruct);   //����ʱ��(RTC_Format_BIN�������Ƹ�ʽд��)
}

/**********************************************
*��������  ������RTC���ꡢ���ڡ��¡���
*������    ��RTC_Set_Date
*��������  ��u8 year,u8 month,u8 day,u8 week
*��������ֵ��void
*����      ��
************************************************/
void RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
	RTC_DateTypeDef  RTC_DateStruct;   //�������ýṹ��
	
	RTC_DateStruct.RTC_Date=date;      //��
	RTC_DateStruct.RTC_Month=month;    //��
	RTC_DateStruct.RTC_WeekDay=week;   //����
	RTC_DateStruct.RTC_Year=year;      //��
	
  RTC_SetDate(RTC_Format_BIN,&RTC_DateStruct);   //��������(RTC_Format_BIN�������Ƹ�ʽд��)
}


/************************************
*��������  ����RTC���г�ʼ������
*������    ��RTC_Init
*��������  ��RTC_DATES rtc
*��������ֵ��void
*����      ��
*************************************/
void MYRTC_Init(RTC_DATES rtc)
{
	RTC_InitTypeDef   RTC_InitStruct;
	
	//ʱ��ʱ��     PWR
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//ʹ��PWRʱ��
	
	//ʹ�ܶ�RTC��д����
	PWR_BackupAccessCmd(ENABLE);	//������д������ʹ�ܺ󱸼Ĵ�������(PWR->CR�Ĵ����ĵ�8λ)
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0xff)//���ڱ��⸴λ��������ʱ��ֵ(û���ʱֻ�Ը�λ���ã��ϵ���λ�����㣬��Ȼ��������ʱ��ֵ)
	{
		/*ʱ��Դѡ��*/
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		            //ѡ���ⲿ����ʱ����Ϊʱ��Դ(RCC->BDCR�Ĵ����ĵ�8-9λ)
		RCC_LSEConfig(RCC_LSE_ON);                            //�����ⲿ��������(LSE) (RCC->BDCR�Ĵ����ĵ�0λ)
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);    //�ⲿ������������ (RCC->BDCR�Ĵ����ĵ�1λ)
		RCC_RTCCLKCmd(ENABLE);                                //RTCʱ��ʹ��(RCC->BDCR�Ĵ����ĵ�15λ)
		
		/*RTC����*/
		RTC_InitStruct.RTC_AsynchPrediv = 0x7F;              //(Ĭ��ֵ��Ҫ��)RTC�첽��Ƶϵ�����첽Ԥ��Ƶϵ��0x7F+1(RTC->PRER�Ĵ����ĵ�16-22λ)
    RTC_InitStruct.RTC_SynchPrediv  = 0xFF;              //(Ĭ��ֵ��Ҫ��)RTCͬ����Ƶϵ����ͬ��Ԥ��Ƶϵ��0xFF+1(RTC->PRER�Ĵ����ĵ�0-14λ)
    RTC_InitStruct.RTC_HourFormat   = RTC_HourFormat_24;//Сʱ��ʽ��24Сʱ��ʽ(RTC_CR�Ĵ����ĵ�6λ)
    RTC_Init(&RTC_InitStruct);                          //��ʼ��RTC����
		
		/*ʱ������*/
    //����ʱ��
    RTC_Set_Time(rtc.hour,rtc.min,rtc.sec,rtc.pm);
    //��������������
    RTC_Set_Date(rtc.year,rtc.month,rtc.day,rtc.week);
		
		//���⸴λ��������ʱ��ֵ�����ϵ�������λ���Զ����㡣
		RTC_WriteBackupRegister(RTC_BKP_DR0,0xff);  //(RTC->BKP0R�Ĵ���)
	}
}














