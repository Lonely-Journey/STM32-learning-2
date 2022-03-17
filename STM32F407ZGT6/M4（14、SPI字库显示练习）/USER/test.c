#include "stm32f4xx.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"
#include "stdio.h"
#include "exit.h"
#include "iic.h"
#include "at24cxx.h"
#include "delay.h"
#include "lcd.h"
#include "w25q64.h"

//static void delay_ms(u32 ms)
//{
//	u32 i = 168 / 4 * 1000 * ms;
//	while(i)
//	{
//		i--;
//	}
//}

/**************************************************************
*��ע���
*         1��W25Q64��0x0b0000~0x1F1F58���ڴ洢�ֿ�
*            ��ʼ���飺0b   ��11��      ������0    ��0������
*                  ҳ��0    ��0ҳ       �ֽڣ�0    ��0���ֽ�
*            �������飺1F   ��31��      ������1    ��1������
*                  ҳ��F    ��15ҳ      �ֽڣ�58   ��88���ֽ�
*         2��AT24C02��0~16���ڴ洢LCD�봥����֮���У׼ϵ��
***************************************************************/

int main(void)
{ 
	u8 data;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	SysTick_Init();
	USART1_Init(115200);
	W25Q64_Init();
  LCD_Init();
  
	while(1)
	{
		data = 'a';
    while(data != 123)
    {
      LCD_Show_HZ16(100,100,&data,RED,WHITE);
      LCD_Show_HZ32(100,200,&data,RED,WHITE);
      data++;
      delay_ms(300);
    }
    data = 'A';
    while(data != 91)
    {
      LCD_Show_HZ16(100,100,&data,RED,WHITE);
      LCD_Show_HZ32(100,200,&data,RED,WHITE);
      data++;
      delay_ms(300);
    }
	}
}


