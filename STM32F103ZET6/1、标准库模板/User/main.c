#include "sys.h"
#include "delay.h"
#include "led.h"

int main(void)
{
    SysTick_Init();       /* ��ʱ��ʼ�� */
    LED_Init();           /* LED��ʼ�� */

    while(1)
    {
        LED0_ON;
        LED1_ON;
        delay_ms(300);
        LED0_OFF;
        LED1_OFF;
        delay_ms(300);
    }
}


