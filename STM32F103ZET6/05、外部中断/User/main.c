#include "sys.h"
#include "delay.h"
#include "led.h"
#include "exti.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  /* 2�ŷ��鷽�� 2 λ��ռ���ȼ��� 2 λ��Ӧ���ȼ� */
    SysTick_Init();        /* ��ʱ��ʼ�� */
    LED_Init();            /* LED��ʼ�� */
    EXTI4_Init();          /* �ⲿ�ж�4��ʼ�� */

    while(1)
    {
    }
}


