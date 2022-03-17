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
	u8 key_flay;
	
	SysTick_Init();	  	//��ʱ��ʼ��
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  LED_Init();
  KEY_Init();
  EXTI5_Swier_Init();
  
	while(1)
	{
    key_flay = KEY_Scan();
    if(key_flay == 2)
    {
      EXTI->SWIER |= (1<<5);
    }
  }
}




