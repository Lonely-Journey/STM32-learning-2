#include "sys.h"	
#include "delay.h"	
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"
#include "stdio.h"
#include "exti.h"

int main(void)
{			
	
	SysTick_Init();	  	//��ʱ��ʼ��
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  LED_Init();
  
	while(1)
	{
    LED_ON();
    delay_ms(10000);
    LED_OFF();
    delay_ms(10000);
  }
}




