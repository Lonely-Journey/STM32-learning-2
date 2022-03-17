#include "stm32f4xx.h"  
#include "led.h"
#include "beep.h"

static void delay_ms(u32 ms)
{
	u32 i = 168 / 4 * 1000 * ms;
	while(i)
	{
		i--;
	}
}

/*****************************************************
*��������  ����LED��Ӧ��GPIO�ڽ��г�ʼ������
*������    ��LED_Init
*��������  ��void
*��������ֵ��void
*����      ��
*            LED1     PF6
*            LED2     PF9
*            LED3     PF10
*            LED4     PC0
*            �ߵ�ƽ�𣬵͵�ƽ��-----���ģʽ
********************************************************/
void LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruc;      //GPIOx�ṹ������
  
  //ʱ��ʹ��  GPIOF  GPIOC
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
  /*GPIOx�˿�����*/
  //PF6��9��10
  GPIO_InitStruc.GPIO_Pin   = (GPIO_Pin_6 | GPIO_Pin_9 | GPIO_Pin_10);   //ѡ��PF6��9��10
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_OUT;                             //ͨ�����
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;                          //��������
  GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;                             //�������
  GPIO_InitStruc.GPIO_Speed = GPIO_Speed_50MHz;                          //�ٶ�Ϊ50MHz
  GPIO_Init(GPIOF,&GPIO_InitStruc);                                      //�����趨������ʼ��PF6��9��10
  
  //PC0
  GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_0;       //PC0
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_OUT;    //ͨ�����
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL; //��������
  GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;    //�������
  GPIO_InitStruc.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�Ϊ50MHz
  GPIO_Init(GPIOC,&GPIO_InitStruc);             //�����趨������ʼ��PC0
  
  /*����������ݼĴ���*/
  //�Ƚ�LED���ر�
  GPIO_SetBits(GPIOF,(GPIO_Pin_6 | GPIO_Pin_9 | GPIO_Pin_10));      //�ߵ�ƽ��
  GPIO_SetBits(GPIOC,GPIO_Pin_0);                                   //�ߵ�ƽ��
}

/************************************************************
*��������  ��LED��ȫ��
*������    ��LED_All_ON
*��������  ��void
*��������ֵ��void
*����      ��
***************************************************************/
void LED_All_ON(void)
{
  GPIO_ResetBits(GPIOF,(GPIO_Pin_6 | GPIO_Pin_9 | GPIO_Pin_10));
  GPIO_ResetBits(GPIOC,GPIO_Pin_0);
}

/************************************************************
*��������  ������LEDȫ��
*������    ��LED_All_OFF
*��������  ��void
*��������ֵ��void
*����      ��
***************************************************************/
void LED_All_OFF(void)
{
  GPIO_SetBits(GPIOF,(GPIO_Pin_6 | GPIO_Pin_9 | GPIO_Pin_10));
  GPIO_SetBits(GPIOC,GPIO_Pin_0);
}

/************************************************************
*��������  ��ѡ�����
*������    ��LED_All_ON
*��������  ��u8
*��������ֵ��void
*����      ��
***************************************************************/
void LED_X_ON(u8 n)
{
  switch(n)
  {
    case 0: LED0_ON;break;
    case 1: LED1_ON;break;
    case 2: LED2_ON;break;
    case 3: LED4_ON;break;
  }
}

/************************************************************
*��������  ��ѡ�����
*������    ��LED_All_OFF
*��������  ��u8
*��������ֵ��void
*����      ��
***************************************************************/
void LED_X_OFF(u8 n)
{
  switch(n)
  {
    case 0: LED0_OFF;break;
    case 1: LED1_OFF;break;
    case 2: LED2_OFF;break;
    case 3: LED4_OFF;break;
  }
}

/************************************************************
*��������  ����ˮ��
*������    ��LED_Flash1
*��������  ��void
*��������ֵ��void
*����      ��
***************************************************************/
void LED_Flash1(void)
{
  static int i = 0;
  LED_X_ON(i);
  delay_ms(300);
  LED_X_OFF(i);
  i++;
  if(i>3)
  {
    i=0;
  }
}

/************************************************************
*��������  ����ˮ��+������
*������    ��LED_Flash2
*��������  ��void
*��������ֵ��void
*����      ��
***************************************************************/
void LED_Flash2(int n)
{
  static int i = 0;
  static int cont = 0;
  static int c = 1;
  LED_X_ON(i);
  cont++;
  if(cont >= 1000000/2*n)
  {
    BEEP_OFF;
  }
  if(cont >= 1000000*n)
  {
    LED_X_OFF(i);
    i++;
    if(i>3)
    {
      i=0;
    }
    BEEP_ON;
    c=!c;
    cont = 0;
  }
}

/************************************************************
*��������  ����ˮ��
*������    ��LED_Flash3
*��������  ��int n
*��������ֵ��void
*����      ��
***************************************************************/
void LED_Flash3(int n)
{
  static int i = 0;
  static int cont = 0;
  
  LED_X_ON(i);
  cont++;
  
  if(cont >= 30000*n)
  {
    LED_X_OFF(i);
    i++;
    if(i>3)
    {
      i=0;
    }
    cont = 0;
  }
}

