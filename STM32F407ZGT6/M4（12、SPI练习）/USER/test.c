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
	u8 str[] = "momoû����";
  u8 str2[30]; 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	SysTick_Init();
	USART1_Init(9600);
	
	W25Q64_Init();
  W25Q64_Sector_Erase(0x5);
  W25Q64_Page_Write(0x1,sizeof(str),str);
  W25Q64_Read_Bytes(0x1,sizeof(str),str2);
  
	while(1)
	{
		printf("str2 : %s\r\n",str2);
    delay1_ms(500);
	}
}


