#include "sys.h"
#include "delay.h"
#include "led.h"

int main(void)
{
    SysTick_Init();                         /* ��ʱ��ʼ�� */
    LED_Init();                             /* LED��ʼ�� */

    while(1)
    {
    }
}




