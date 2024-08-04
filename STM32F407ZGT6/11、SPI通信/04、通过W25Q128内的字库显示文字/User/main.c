#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "w25q128.h"
#include "show.h"

/***
* ע�⣺ʹ���������ǰ�����뱣֤�Ѿ���¼���ֿ�
*/

int main(void)
{
    SysTick_Init();                         /* ��ʱ��ʼ�� */
    USART1_Init(115200);                    /* ���ڳ�ʼ��  115200 */
    LCD_ILI9341_Init();                     /* ILI9341��LCD����ʼ�� */
    W25Q128_Init();                         /* W25Q128��ʼ�� */

    LCD_Show_HZ16(100,100,(uint8_t*)"��ð�hahaha123",RED,WHITE);
    LCD_Show_HZ32(10,200,(uint8_t*)"��ð�haha123",RED,WHITE);

    while(1)
    {
    }
}




