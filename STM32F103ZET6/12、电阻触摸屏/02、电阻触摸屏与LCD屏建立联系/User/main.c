#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "touch.h"

int main(void)
{
    TOUCH_XY tmp;

    SysTick_Init();           /* ��ʱ��ʼ�� */
    USART1_Init(115200);      /* ����1��ʼ�� 115200 */
    LCD_ILI9341_Init();       /* ILI9341��LCD����ʼ�� */
    tp_dev.init();            /* ��������ʼ�� */
    tp_dev.adjust();          /* ��ĻУ׼ */

    while(1)
    {
        if(T_PEN == 0)
        {
            tmp = tp_dev.scan(1);
            printf("x:%d   y:%d\r\n",tmp.x, tmp.y);
        }
    }
}


