#include "stm32f4xx.h" 
#include "key.h"

static void delay_ms(u32 ms)
{
	u32 i = 168 / 4 * 1000 * ms;
	while(i)
	{
		i--;
	}
}



/*****************************************************
*��������  ����KEY��Ӧ��GPIO�ڽ��г�ʼ������
*������    ��KEY_Init
*��������  ��void
*��������ֵ��void
*����      ��
*           KEY1  PA0    �ߵ�ƽ����
*           KEY2  PE2    �͵�ƽ����
*           KEY3  PE3    �͵�ƽ����
*           KEY4  PE4    �͵�ƽ����
*                  ����ģʽ
*                  ��������
********************************************************/
void KEY_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStruc;
  
  //ʱ��ʹ��  GPIOA  GPIOE 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	//PA0
	GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_0;       //PA0
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_IN;     //ͨ������
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL; //��������
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_50MHz;  //�ٶ�Ϊ50MHz
  GPIO_Init(GPIOA,&GPIO_InitStruc);             //�����趨������ʼ��PA0
	//PE2/3/4
	//��ΪPE2/3/4��PA0����һ��������һЩ��������ı�
	GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;       //PE2/3/4
	GPIO_Init(GPIOE,&GPIO_InitStruc);              //�����趨������ʼ��PA0
}

/*****************************************************
*��������  ������ɨ�躯��
*������    ��KEY_Scan
*��������  ��void
*��������ֵ��u8
*����      ��
            KEY1  PA0    �ߵ�ƽ����
            KEY2  PE2    �͵�ƽ����
            KEY3  PE3    �͵�ƽ����
            KEY4  PE4    �͵�ƽ����
                   ����ģʽ
********************************************************/
u8 KEY_Scan(void)
{
  u8 temp = 0;
  static u8 key_flay = 1;
  if(key_flay&&(KEY1||(!KEY2)||(!KEY3)||(!KEY4)))
  {
    delay_ms(10);
    if(KEY1||(!KEY2)||(!KEY3)||(!KEY4))
    {
      key_flay = 0;
      if(KEY1) temp = 1;
      else if(!(KEY2)) temp = 2;
      else if(!(KEY3)) temp = 3;
      else if(!(KEY4)) temp = 4;
    }
  }
  
  if((!KEY1)&& KEY2 && KEY3 && KEY4)
  {
    key_flay = 1;
  }
  
  return temp;
}


/*****************************************************
*��������  ����������ͬʱ��
*������    ��KEY_Scan
*��������  ��void
*��������ֵ��u8
*����      ��
            KEY1  PA0    �ߵ�ƽ����
            KEY2  PE2    �͵�ƽ����
            KEY3  PE3    �͵�ƽ����
            KEY4  PE4    �͵�ƽ����
                   ����ģʽ
********************************************************/
u8 KEY_Scan2(void)
{
  u8 temp = 0;
  static u8 key_flay = 1;
  static u32 cont = 0 ;
  if(key_flay &&(KEY1||(!KEY2)))
  {
    cont++;
    if(KEY1 && (!KEY2) && (cont==250000))
    {
      delay_ms(10);
      if(KEY1 && !(KEY2))
      {
        key_flay = 0;
        temp=12;
      }
    }
  }
  if( cont > 250000)
  {
    cont = 255000;
  }
  if((!KEY1) && KEY2)
  {
    key_flay = 1;
    cont = 0;
  }
  
  return temp;
}




