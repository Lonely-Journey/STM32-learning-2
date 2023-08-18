#include "sys.h"
#include "delay.h"
#include "rtc.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* 2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    SysTick_Init();                         /* 延时初始化 */
    USART1_Init(115200);                    /* 串口1初始化 115200 */
    RTC_Set_Alarm(1, 12, 0, 1);             /* 设置闹钟 */
    RTC_Set_WakeUp(4, 0);                   /* 设置周期中断 */
    MyRTC_Init();                           /* 初始化RTC */

    while(1)
    {
    }
}




