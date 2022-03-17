#include "stm32f4xx.h"  
#include "touch.h"
#include "delay.h"
#include "stdio.h"

//200ns��ʱ����
static void delay_200ns(void)
{
  u8 i = 20;
  while(i) i--;
}

/*****************************************************
*��������  ���Դ�����оƬ��Ӧ��GPIO�ڽ��г�ʼ������
*������    ��Touch_Init
*��������  ��void
*��������ֵ��void
*����      ��
*            CS     PC13 ͨ�����
*            SCK    PB0  ͨ�����
*            PEN    PB1  ͨ������
*            MISO   PB2  ͨ������
*            MOSI   PF11 ͨ�����
********************************************************/
void Touch_Init(void)
{
	 GPIO_InitTypeDef GPIO_InitStruc;        //GPIOx���ýṹ��
  
  //ʱ��ʹ��  GPIOB  GPIOC  GPIOF
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
  /*GPIOx�˿�����*/
	/*���*/
  //PB0
  GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_0;         //ѡ��PB0
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_OUT;      //ͨ�����
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;   //��������
  GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;      //�������
  GPIO_InitStruc.GPIO_Speed = GPIO_Speed_100MHz;  //�ٶ�Ϊ100MHz
  GPIO_Init(GPIOB,&GPIO_InitStruc);               //�����趨������ʼ��PB0
	//PC13
	GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_13;        //ѡ��PC13
	GPIO_Init(GPIOC,&GPIO_InitStruc);               //�����趨������ʼ��PC13
	//PF11
	GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_11;        //ѡ��PF11
	GPIO_Init(GPIOF,&GPIO_InitStruc);               //�����趨������ʼ��PF11
	/*����*/
	//PB1��2
	GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_1|GPIO_Pin_2;         //ѡ��PB1��2
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_IN;                  //ͨ������
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;              //��������
  GPIO_InitStruc.GPIO_Speed = GPIO_Speed_100MHz;             //�ٶ�Ϊ100MHz
  GPIO_Init(GPIOB,&GPIO_InitStruc);                          //�����趨������ʼ��PB1��2
	
	//��ʼ״̬����
  T_CS_H;
  T_SCK_L;
  T_MOSI_L;
}

/*****************************************************
*��������  ��ͨ��������оƬ��ȡ��������������
*������    ��Touch_Data
*��������  ��u16
*��������ֵ��u8 command 
*����      ��
*       Ƭѡ��        T_CS_H       T_CS_L  
*       ʱ����        T_SCK_H      T_SCK_L  
*       ���������    T_MOSI_H     T_MOSI_L
*       ����������    T_MISO 
*       �ʽӴ���      T_PEN    
********************************************************/
u16 Touch_Data(u8 command)
{
  u16 data = 0;
  u8 i;
  
  //�������������
  T_MOSI_L;
  //����Ƭѡ
  T_CS_L;
  //ѭ��8�η���ָ��
  for(i=0;i<8;i++)
  {
    //ʱ��������
    T_SCK_L;
    //�ж�ָ��λ��ƽ
    if(command & 0x80)T_MOSI_H;
    else T_MOSI_L;
    //��ʱ200ns���������ݽ���
    delay_200ns();
    //ʱ��������
    T_SCK_H;
    //��ʱ200ns���������ݶ�ȡ
    delay_200ns();
    //ָ����������
    command <<= 1;
  }
  //�������������
  T_MOSI_L;
  //����ʱ����
  T_SCK_L;
  //��ʱ120us�����ڵȴ�ADCת������
  delay_us(120);
  //�ճ�һ��������
  T_SCK_L;
  delay_200ns();
  T_SCK_H;
  delay_200ns();
  //ѭ��12�ν�������
  for(i=0;i<12;i++)
  {
    //ָ����������
    data <<= 1;
    //ʱ��������
    T_SCK_L;
    //��ʱ200ns���������ݽ���
    delay_200ns();
    //ʱ��������
    T_SCK_H;
    //��ȡ����λ��ƽ
    if(T_MISO) 
    {
      data |= 0x1;
    }
    //��ʱ200ns���������ݶ�ȡ
    delay_200ns();
  }
  //Ƭѡ����
  T_CS_H;
  //ʱ��������
  T_SCK_L;
  
  return data;
}















