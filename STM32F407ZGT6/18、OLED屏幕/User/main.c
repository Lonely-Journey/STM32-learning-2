#include "sys.h"
#include "delay.h"
#include "iic.h"
#include "oled.h"

int main(void)
{
    SysTick_Init();                         /* ��ʱ��ʼ�� */
    OLED_Init();                            /* OLED����ʼ�� */

    OLED_ShowString16(0, 0, "123asdf");

    while(1)
    {
    }
}




