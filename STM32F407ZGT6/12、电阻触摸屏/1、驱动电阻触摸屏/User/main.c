#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "touch.h"

int main(void)
{
    uint16_t x=0,y=0;

    SysTick_Init();                         /* ��ʱ��ʼ�� */
    USART1_Init(115200);                    /* ����1��ʼ�� 115200 */
    Touch_IO_Init();                        /* ���败�������ų�ʼ�� */

    while(1)
    {
        if(T_PEN == 0)
        {
            x = Touch_Data(TOUCH_X);          /* ��ȡ����x���� */
            y = Touch_Data(TOUCH_Y);          /* ��ȡ����y���� */
            printf("x:%4d   y:%4d\r\n",x,y);
        }
    }
}




