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
#include "adc.h"
#include "time.h"
#include "pwm.h"
#include "capture.h"
#include "dma.h"
#include "rtc.h"
#include "fsmc.h"
#include "iwdg.h"
#include "can.h"

/**************************************************************
*��ע���
*         1��W25Q64��0x0b0000~0x1F1F58���ڴ洢�ֿ�
*            ��ʼ���飺0b   ��11��      ������0    ��0������
*                  ҳ��0    ��0ҳ       �ֽڣ�0    ��0���ֽ�
*            �������飺1F   ��31��      ������1    ��1������
*                  ҳ��F    ��15ҳ      �ֽڣ�58   ��88���ֽ�
*         2��AT24C02��0~16���ڴ洢LCD�봥����֮���У׼ϵ��
***************************************************************/

extern ADJUST LCD_Touch;

int main(void)
{			
	u8 key;
	
	SysTick_Init();	  	//��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	USART1_Init(115200);
	LED_Init();
  AT24CXX_Init();
  Touch_Init();
  LCD_Init(); 
  AT24CXX_Read_Bytes(0,sizeof(LCD_Touch),(u8 *)&LCD_Touch);
  
  //������
  LCD_Clear_XY(120,120,60,60,RED);
  
  while(1)
  {
    key = LCD_Key_Scan(120,120,60,60);
    if(key == 1) GPIOB->ODR ^= (1<<5);
  }
}




