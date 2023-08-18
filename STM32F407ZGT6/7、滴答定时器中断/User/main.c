#include "sys.h"
#include "delay.h"
#include "led.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* 2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    SysTick1_Init(1000);                    /* 延时初始化  1s定时中断*/
    LED_Init();                             /* LED初始化 */

    while(1)
    {
    }
}




