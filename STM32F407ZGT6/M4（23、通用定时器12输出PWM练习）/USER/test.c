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
#include "touch.h"
#include "adc.h"
#include "time.h"
#include "pwm.h"

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

extern ADJUST LCD_Touch;

int main(void)
{ 
	u8 key;
	u16 pwm = 500;
	
  SysTick_Init();
  USART1_Init(115200);
  KEY_Init();
  LCD_Init();
  TIM12_PWM_Init(1000,84,pwm);
  
  while(1)
  {
    key = KEY_Scan();
    if(key == 3)
    {
      if(pwm == 0) pwm = 0;
      else pwm -= 100;
			TIM_SetCompare2(TIM12,pwm);
    }
    else if(key == 4)
    {
      if(pwm == 1000) pwm = 1000;
      else pwm += 100;
			TIM_SetCompare2(TIM12,pwm);
    }
  }
}


