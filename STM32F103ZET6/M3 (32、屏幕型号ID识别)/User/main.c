#include "stm32f10x.h"
#include "delay.h"
#include "key.h"
#include "TFTlcd.h"
#include "usart.h"
#include "stdio.h"




int main(void)
{
	u16 id=0;

	SysTick_Init();	    	//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	USART1_Init(115200);
	TFTLCD_Init();			//LCD��ʼ��	
	delay_us(5);
	id = TFTLCD_Id();
	
	printf("id = %d , %x",id,id);
	
	

	while(1);
}


