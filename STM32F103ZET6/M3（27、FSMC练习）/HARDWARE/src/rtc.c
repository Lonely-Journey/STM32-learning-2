#include "sys.h"
#include "rtc.h"
#include "stdio.h"
#include "delay.h"

/**************************************************
*��������  ����RTC���г�ʼ������
*������    ��MyRTC_Init
*��������  ��RTC_DATES rtc
*��������ֵ��void
*����      ��
****************************************************/
void MyRTC_Init(RTC_DATES rtc)
{
	/*��Դ������*/
  //ʹ�ܵ�Դ������ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR 
	//ʹ�ܱ���ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE); //ʹ��BKP
	
	//ʹ�ܶ�RTC��д����
	PWR_BackupAccessCmd(ENABLE);	//������д������ʹ�ܺ󱸼Ĵ�������(PWR->CR�Ĵ����ĵ�8λ)
	
	if(BKP_ReadBackupRegister(BKP_DR1)!=0xff)//���ڱ��⸴λ��������ʱ��ֵ(û���ʱֻ�Ը�λ���ã��ϵ���λ�����㣬��Ȼ��������ʱ��ֵ)
	{
		//��λ��������
		BKP_DeInit();
		
		/*ʱ��Դѡ��*/
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		            //ѡ���ⲿ����ʱ����Ϊʱ��Դ(RCC->BDCR�Ĵ����ĵ�8-9λ)
		RCC_LSEConfig(RCC_LSE_ON);                            //�����ⲿ��������(LSE) (RCC->BDCR�Ĵ����ĵ�0λ)
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);   //�ⲿ������������ (RCC->BDCR�Ĵ����ĵ�1λ)
		RCC_RTCCLKCmd(ENABLE);                                //RTCʱ��ʹ��(RCC->BDCR�Ĵ����ĵ�15λ)
		
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������(RTC_CRL�Ĵ����ĵ�5λ)
		//�ȴ�ȷ��RTC_CNT�� RTC_ALR��RTC_PRL�Ѿ���ͬ���Ѿ����
		RTC_WaitForSynchro();		//�ȴ�RTC�Ĵ���ͬ�� (RTC_CRL�Ĵ����ĵ�3λ)
		
		/*RTC����*/
		RTC_EnterConfigMode();    //��������ģʽ(RTC_CRL�Ĵ����ĵ�4λ)
		RTC_SetPrescaler(0x7FFF); //����RTCԤ��Ƶ��ֵ(0x7FFF��ST�ٷ��ṩ��һ���ο�ֵ���ɻ������Ϊ1���ӵ��ź�)(RTC_PRLL�Ĵ���)
		RTC_WaitForLastTask();	  //�ȴ����һ�ζ�RTC�Ĵ�����д�������(RTC_CRL�Ĵ����ĵ�5λ)
		//����ʱ��
    RTC_Set(rtc);
		RTC_ExitConfigMode(); //�˳�����ģʽ(RTC_CRL�Ĵ����ĵ�4λ)
		//�ȴ���RTC�Ĵ�����д�����Ѿ���ɡ�
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������(RTC_CRL�Ĵ����ĵ�5λ) 
		
		//��������д�±�־����ֹ�ϵ��������γ�ʼ��
		BKP_WriteBackupRegister(BKP_DR1, 0xff);	//��ָ���ĺ󱸼Ĵ�����д���û���������(BKP_DRx�Ĵ���)
	}
}

/*****************************************************************
*��������  ���ж����껹��ƽ��
*������    ��Leap_Year
*��������  ��u16
*��������ֵ��u8 
*����      ������ 1  ����     ���� 0  ƽ��
*******************************************************************/
u8 Leap_Year(u16 year)
{
  //���꣺�ܱ�400�����������ܱ�4���������ܱ�100������
  if((year % 400 == 0)||((year % 4 == 0)&&(year % 100 != 0)))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//ƽ���·���������
u8 common_year[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

/*****************************************************************
*��������  ������RTC���ꡢ�¡��ա�ʱ���֡���
*������    ��RTC_Set
*��������  ��RTC_DATES rtc
*��������ֵ��u8 
*����      ������ 1  ��ʾ����ʧ��     ���� 0   ��ʾ���óɹ�
*******************************************************************/
u8 RTC_Set(RTC_DATES rtc)
{
  u16 t;
	u32 sec_count = 0;    //������¼������
  //�ж����������Ƿ��г�����Χ
  if((rtc.year<1970)||(rtc.year>2099)) return 1;
  //����1970�굽rtc.yearһ�������˶�����
  for(t=1970;t<rtc.year;t++)
  {
    if(Leap_Year(t))sec_count += 31622400;  //����һ����366 * 24 * 60 * 60 = 31622400 ��
    else sec_count += 31536000;             //ƽ��һ����365 * 24 * 60 * 60 = 31536000 ��
  }
  //����rtc.year���1�µ�rtc.monthһ�������˶�����
  rtc.month -= 1;
  for(t=0;t<rtc.month;t++)
  {
    sec_count += common_year[t] * 86400;           //һ����24 * 60 * 60 = 86400��
    if(Leap_Year(t) && (t==1)) sec_count += 86400; //�����ƽ���2�¶�1��
  }
  //����rtc.month���1�µ�rtc.dayһ�������˶�����
  sec_count += (rtc.day-1) * 86400;
  //����rtc.day���rtc.hourһ�������˶�����
  sec_count += rtc.hour  * 3600;
  //����rtc.day���rtc.minһ�������˶�����
  sec_count += rtc.min * 60;
  //����rtc.day���rtc.minһ�������˶�����
  sec_count += rtc.sec;
  
  //��������ģʽ
  RTC_EnterConfigMode();    //��������ģʽ(RTC_CRL�Ĵ����ĵ�4λ)
  //��ʽ����ʱ��
  RTC_SetCounter(sec_count);	//����RTC��������ֵ(RTC_CNTH / RTC_CNTL�Ĵ���)
  //�˳�����
  RTC_ExitConfigMode(); //�˳�����ģʽ(RTC_CRL�Ĵ����ĵ�4λ)
  //�ȴ���RTC�Ĵ�����д�����Ѿ���ɡ�
  RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������(RTC_CRL�Ĵ����ĵ�5λ) 
  
  return 0;
}

/*****************************************************************
*��������  ����ȡRTC���ꡢ�¡��ա����ڡ�ʱ���֡���
*������    ��RTC_Get
*��������  ��void
*��������ֵ��RTC_DATES 
*����      ��
*******************************************************************/
RTC_DATES RTC_Get(void)
{
  RTC_DATES rtc;
  u32 sec_count;   //������RTC->CNT�Ĵ�����ֵ
  u32 day;
  u32 temp;        
  
  //�ȴ�ȷ��RTC_CNT�� RTC_ALR��RTC_PRL�Ѿ���ͬ���Ѿ����
	RTC_WaitForSynchro();		//�ȴ�RTC�Ĵ���ͬ�� (RTC_CRL�Ĵ����ĵ�3λ)
  //��ȡʱ��(��ֵ)
  sec_count = RTC_GetCounter();
  
  //����1970�����ڵ�һ������������
  day = sec_count / 86400;  //һ����24 * 60 * 60 = 86400��
  //�����������һ��
  if(day != 0)
  {
    temp = 1970;     //��ʼ���
    //������������һ��(��1970Ϊ��׼���)
    while(day >= 365)
    {
      //�ж��ǲ�������
      if(Leap_Year(temp))
      {
        if(day>=366) day -= 366;
        else break;
      }
      else day -= 365;
      temp++;
    }
    //�洢��
    rtc.year = temp;
    temp = 0;
    //���������ǵڼ�����
    while(day>=28)
    {
      //�ж��ǲ����������ǲ��Ƕ���
      if(Leap_Year(rtc.year) && (temp==1))
      {
        if(temp>=29)temp-=29;//�����������
				else break; 
      }
      else
      {
        //�ж��ǲ��Ǵ�������µ�����
        if(day>common_year[temp]) day -= common_year[temp];
        else break;
      }
      temp++;
    }
    //�洢�·ݺ�����
    rtc.month = temp+1;
    rtc.day = day+1;
  }
  else
  {
    //���RTC->CNT��ֵ������86400(һ��)���������ֵ
    rtc.year = 1970;
    rtc.month = 1;
    rtc.day = 1;
  }
  
  temp = sec_count % 86400;     		    //�õ���ȥ�����պ������   	   
	rtc.hour = temp / 3600;     	        //Сʱ
	rtc.min = (temp%3600) / 60; 	        //����	
	rtc.sec = (temp%3600) % 60; 	        //����
  
  rtc.week = RTC_Week(rtc);             //��ȡ��ĩʱ��
  
  return rtc;
}

/***********************************************************************
*��������  �����㵱ǰ�ǵڼ�������
*������    ��RTC_Week
*��������  ��RTC_DATES rtc
*��������ֵ��u8
*����      ��  
*    ����(Zeller)��ʽ��
*                 w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1
*             W����7�������Ǽ��������ڼ������������0����Ϊ�����ա�
*      ���ţ�
*           w������
*           c������-1
*           y���꣨��ݺ���λ����
*           m���£�m���ڵ���3��С�ڵ���14�����ڲ��չ�ʽ�У�ĳ���1��2��
*                 Ҫ������һ���13��14�������㣬����2003��1��1��Ҫ����
*                 2002���13��1�������㣩
*           d����
************************************************************************/
u8 RTC_Week(RTC_DATES rtc)
{
  u16 w;
  u8 c = rtc.year / 100;  //����-1
  u8 y = rtc.year % 100;  //��ݺ���λ��
  u8 m = rtc.month;       //��
  u8 d = rtc.day;         //��
  
  //���¡��ꡢ���ͽ��д���
  if(m <= 2)
  {
    //���չ�ʽ�У�ĳ���1��2��
    //Ҫ������һ���13��14��������
    //������պÿ����ͣ�������Ҳ�ô���
    m += 12;
    y -= 1;
    if(rtc.year % 100 == 0) c -= 1;
  }
  
  //���չ�ʽ:w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1
  w = y + (u8)(y/4) + (u8)(c/4) - 2*c + (u8)(26*(m+1)/10) + d - 1;
  w %= 7;
  if(w != 0) return w;
  else return 7;
}


















