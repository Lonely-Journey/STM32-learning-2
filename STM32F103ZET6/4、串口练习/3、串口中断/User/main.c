#include "sys.h"
#include "delay.h"
#include "usart.h"



int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* 2�ŷ��鷽�� 2 λ��ռ���ȼ��� 2 λ��Ӧ���ȼ� */
    SysTick_Init();               /* ��ʱ��ʼ�� */
    USART1_Init(115200);          /* ����1��ʼ��   115200 */

    while(1)
    {
    }
}


