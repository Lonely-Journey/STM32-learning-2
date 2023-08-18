#include "sys.h"
#include "delay.h"
#include "usart.h"



int main(void)
{
    uint8_t data;

    SysTick_Init();           /* ��ʱ��ʼ�� */
    USART1_Init(115200);      /* ����1��ʼ��  115200 */

    while(1)
    {
        data = USART1_Receive_Byte();
        USART1_Send_Byte(data);
    }
}


