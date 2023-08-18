#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* 2�ŷ��鷽�� 2 λ��ռ���ȼ��� 2 λ��Ӧ���ȼ� */
    SysTick_Init();                         /* ��ʱ��ʼ�� */
    USART1_Init(115200);                    /* ����1��ʼ�� 115200 */
    ADC3_In5_Init();                        /* ADC3ͨ��5��ʼ�� */

    while(1)
    {
        /* ÿ�յ�����1�����ݾ�ת��һ��ADC���� */
        USART1_Receive_Byte();
        /* ��������ͨ��ת������(ADC1->CR2�Ĵ����ĵ�30λ) */
        ADC_SoftwareStartConv(ADC3);
    }
}




