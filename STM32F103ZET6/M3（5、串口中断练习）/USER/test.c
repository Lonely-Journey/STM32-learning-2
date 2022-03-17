#include "sys.h"	
#include "delay.h"	
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"
#include "stdio.h"

int main(void)
{			
	SysTick_Init();	  	//��ʱ��ʼ��
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  LED_Init();
  KEY_Init();
	USART1_Init(9600);
  
	while(1)
	{
		LED0_ON;
		delay_ms(500);
		LED0_OFF;
		delay_ms(500);
	}
}




