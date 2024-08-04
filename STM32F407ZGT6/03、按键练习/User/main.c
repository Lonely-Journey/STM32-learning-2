#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h"

int main(void)
{
    uint8_t key = 0;

    SysTick_Init();                         /* ��ʱ��ʼ�� */
    LED_Init();                             /* LED��ʼ�� */
    KEY_Init();                             /* KEY��ʼ�� */

    while(1)
    {
        key = KEY_Scan();
        switch(key)
        {
            case 1:LED0_ON;LED1_OFF;break;
            case 2:LED1_ON;LED0_OFF;break;
            case 3:LED0_ON;LED1_ON;break;
            case 4:LED0_OFF;LED1_OFF;break;
        }
    }
}




