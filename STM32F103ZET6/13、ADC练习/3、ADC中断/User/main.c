#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  /* 2�ŷ��鷽�� 2 λ��ռ���ȼ��� 2 λ��Ӧ���ȼ� */
    SysTick_Init();               /* ��ʱ��ʼ�� */
    USART1_Init(115200);          /* ����1��ʼ�� 115200 */
    ADC3_In6_Init();              /* ADC3ͨ��6��ʼ�� */

    while(1)
    {
        /* ÿ�յ�����1�����ݾ�ת��һ��ADC���� */
        USART1_Receive_Byte();
        /* ��������ͨ��ת������(ADC_CR2�Ĵ�����22λ) */
        ADC_SoftwareStartConvCmd(ADC3, ENABLE);
    }
}


