#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "norflash.h"

int main(void)
{
    SysTick_Init();                         /* ��ʱ��ʼ�� */
    USART1_Init(115200);                    /* ���ڳ�ʼ��  115200 */
    NorFlash_Init();                        /* W25Q128��ʼ�� */

    NorFlash_Read_ID();

    while(1)
    {
    }
}




