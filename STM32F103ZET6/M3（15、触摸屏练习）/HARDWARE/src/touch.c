#include "sys.h"
#include "touch.h"
#include "delay.h"

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
*            CS     PF11  ͨ�����
*            SCK    PB1   ͨ�����
*            PEN    PF10  ͨ������
*            MISO   PB2   ͨ������
*            MOSI   PF9   ͨ�����
********************************************************/
void Touch_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;     //GPIOx���ýṹ��
	
	//ʱ��ʹ��   GPIOB   GPIOF
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	
	/*GPIOx��ʼ������*/
  //GPIOx�˿�����
	/*���������*/
  //PB1
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;				     //PB1 �˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStruct);					       //�����趨������ʼ��PB1
	//PF11��PF9
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11;				     //PF9��PF11�˿�����
	GPIO_Init(GPIOF, &GPIO_InitStruct);					       //�����趨������ʼ��PF9��PF11
	/*���������*/
	//PB2
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;				     //PB2�˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU ;        //���������ģʽ 
	GPIO_Init(GPIOB, &GPIO_InitStruct);					       //�����趨������ʼ��PB1
	//PF10
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;				     //PF10�˿�����
	GPIO_Init(GPIOF, &GPIO_InitStruct);					       //�����趨������ʼ��PF10
	
	//��ʼ��״̬����
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
  u8 i;
  u16 data = 0;
  
  //����������
  T_MOSI_L;
  //Ƭѡ������
  T_CS_L;
  //ѭ��8�η���ָ������
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
  //ʱ��������
  T_SCK_L;
  //����������
  T_MOSI_L;
  //��ʱ120us�����ڵȴ�ADC������ת����
  delay_us(120);
  //�ճ�һ��������
  T_SCK_L;
  delay_200ns();
  T_SCK_H;
  delay_200ns();
  //ѭ��12�ζ�ȡ����
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
    if(T_MISO) data |= 0x1;
    //��ʱ200ns�����ڶ�ȡ����
    delay_200ns();
  }
  //Ƭѡ����
  T_CS_H;
  //ʱ��������
  T_SCK_L;
  
  return data;
}



