#include "sys.h"
#include "delay.h"
#include "timer.h"
#include "pwm.h"

int main(void)
{
    SysTick_Init();                  /* ��ʱ��ʼ�� */
    TIM6_Init(500,7200);             /* 500ms ��ʱ�ж� */
    TIM3_PWM_Init(1000, 72, 500);    /* 50%ռ�ձ� */

    while(1)
    {
    }
}


