#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "stmflash.h"

/* ����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С + 0X08000000) */
#define FLASH_SAVE_ADDR 0X08070000

int main(void)
{
    int i;
    uint16_t buf1[10]={0,1,2,3,4,5,6,7,8,9};
    uint16_t buf2[10]={0,0,0,0,0,0,0,0,0,0};

    SysTick_Init();       /* ��ʱ��ʼ�� */
    USART1_Init(115200);  /* ����1��ʼ�� 115200 */
    LED_Init();           /* LED��ʼ�� */

    STMFLASH_Write(FLASH_SAVE_ADDR, buf1, 10);
    delay_ms(100);
    STMFLASH_Read(FLASH_SAVE_ADDR, buf2, 10);

    for(i=0; i<10; i++) printf("%d\r\n", buf2[i]);

    while(1)
    {
        LED0_ON;
        LED1_ON;
        delay_ms(300);
        LED0_OFF;
        LED1_OFF;
        delay_ms(300);
    }
}


