#include "sys.h"
#include "delay.h"
#include "lcd.h"
#include "usart.h"

int main(void)
{
    uint32_t rgb=0;

    SysTick_Init();           /* ��ʱ��ʼ�� */
    USART1_Init(115200);      /* ����1��ʼ��   115200 */
    LCD_ILI9341_Init();       /* ILI9341��LCD����ʼ�� */

    delay_ms(50);

    LCD_ILI9341_ClearXY(100, 100, 100, 100, RED);    /* ��һ����ɫ���� */
    rgb = LCD_ReadPoint(101, 101);                   /* ȡ��ɫ�������һ�����ص����ɫ���� */
    printf("rgb��%d    %x", rgb, rgb);

    while(1)
    {
    }
}


