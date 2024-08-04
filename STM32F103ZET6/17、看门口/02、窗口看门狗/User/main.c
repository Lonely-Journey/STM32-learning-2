#include "sys.h"
#include "delay.h"
#include "led.h"
#include "wwdg.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* 2�ŷ��鷽�� 2 λ��ռ���ȼ��� 2 λ��Ӧ���ȼ� */
    SysTick_Init();               /* ��ʱ��ʼ�� */
    LED_Init();                   /* LED��ʼ�� */
    LED0_ON;                      /* LED0�� */
    delay_ms(300);                /* ����300ms */
    WWDG_Init(0x7f, 0x5f, 3);     /* ������ֵΪ7f,���ڼĴ���Ϊ5f,��Ƶ��Ϊ8 */

    while(1)
    {
        LED0_OFF;                 /* LED0�� */
    }
}


