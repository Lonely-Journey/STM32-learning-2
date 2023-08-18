#include "sys.h"
#include "delay.h"
#include "led.h"
#include "beep.h"

int main(void)
{
    SysTick_Init();                         /* ��ʱ��ʼ�� */
    LED_Init();                             /* LED������ʼ�� */
    BEEP_Init();                            /* BEEP������ʼ�� */

    while(1)
    {
    }
}




