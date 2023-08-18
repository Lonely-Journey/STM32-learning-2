#include "sys.h"
#include "delay.h"
#include "led.h"
#include "timer.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* 2�ŷ��鷽�� 2 λ��ռ���ȼ��� 2 λ��Ӧ���ȼ� */
    SysTick_Init();               /* ��ʱ��ʼ�� */
    LED_Init();                   /* LED��ʼ�� */
    TIM6_Init(4999, 7200);        /* ��ʱ��6��ʼ��  500ms��ʱ */

    while(1)
    {
        LED0_ON;
        delay_ms(500);
        LED0_OFF;
        delay_ms(500);
    }
}


