#include "sys.h"
#include "delay.h"
#include "led.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  /* 2�ŷ��鷽�� 2 λ��ռ���ȼ��� 2 λ��Ӧ���ȼ� */
    SysTick1_Init(1000);          /* ��ʱ��ʼ��  1s��ʱ�ж�*/
    LED_Init();                   /* LED��ʼ�� */

    while(1)
    {
    }
}


