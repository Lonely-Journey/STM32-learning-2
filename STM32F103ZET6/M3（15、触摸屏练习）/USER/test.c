#include "sys.h"
#include "delay.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"
#include "stdio.h" 
#include "nvic.h"
#include "string.h"
#include "exti.h"
#include "at24cxx.h"
#include "stdio.h"
#include "lcd.h"
#include "spi.h"
#include "w25q128.h"
#include "touch.h"

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
	u16 x,y;
	
	SysTick_Init();	  	//��ʱ��ʼ��
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  LED_Init();
	USART1_Init(115200);
	Touch_Init();
  
	while(1)
	{
    if(!T_PEN)
    {
      x = Touch_Data(TOUCH_X);
      y = Touch_Data(TOUCH_Y);
      printf("x:%4d   y:%4d\r\n",x,y);
    }
  }
}




