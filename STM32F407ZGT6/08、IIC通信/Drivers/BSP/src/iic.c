#include "iic.h"



/**********************************************************
* @funcName ��IIC_Init
* @brief    ����ģ��IIC����Ҫ��GPIO�ڽ��г�ʼ������
* @param    ��void
* @retval   ��void
* @details  ��
*            ʱ����  IIC_SCL  PB8   �������
*            ������  IIC_SDA  PB9   ��©���
*                 �ߵ�ƽ���ڿ���״̬
* @fn       ��
************************************************************/
void IIC_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;       /* GPIOx���ýṹ�� */

    /* ʱ��ʹ��  GPIOB */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* GPIOx�˿����� */
    /* PB8 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8;           /* ѡ��PB8 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;        /* ��ͨ��� */
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;        /* ������� */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;    /* �ٶ�Ϊ100MHz */
    GPIO_Init(GPIOB, &GPIO_InitStruct);                /* �����趨������ʼ��PB8 */
    /* PB9 */
    /* ��Ϊ����һ��������һЩ��������ı� */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;           /* ѡ��PB9 */
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;        /* ��©��� */
    GPIO_Init(GPIOB, &GPIO_InitStruct);                /* �����趨������ʼ��PB9 */

    /* �Ƚ�ʱ���ߺ����������ÿ��� */
    GPIO_SetBits(GPIOB,GPIO_Pin_8);                    /* ʱ�������� */
    GPIO_SetBits(GPIOB,GPIO_Pin_9);                    /* ���������� */
}



/**********************************************************
* @funcName ��IIC_Start
* @brief    ��ģ��IIC����ʼ�ź�
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
*            SCL_H         ʱ��������
*            SCL_L         ʱ��������
*            SDA_OUT_H     ���ģʽ������������
*            SDA_OUT_L     ���ģʽ������������
************************************************************/
void IIC_Start(void)
{
    SCL_L;         /* ʱ�������� */
    SDA_OUT_H;     /* ���������� */
    SCL_H;         /* ʱ�������� */
    delay_us(5);   /* ��ʱ5us */
    SDA_OUT_L;     /* ���������� */
    delay_us(5);   /* ��ʱ5us */
    SCL_L;         /* ʱ��������0 */
}



/**********************************************************
* @funcName ��IIC_Stop
* @brief    ��ģ��IIC��ֹͣ�ź�
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
*            SCL_H         ʱ��������
*            SCL_L         ʱ��������
*            SDA_OUT_H     ���ģʽ������������
*            SDA_OUT_L     ���ģʽ������������
************************************************************/
void IIC_Stop(void)
{
    SCL_L;        /* ʱ�������� */
    SDA_OUT_L;    /* ���������� */
    SCL_H;        /* ʱ�������� */
    delay_us(5);  /* ��ʱ5us */
    SDA_OUT_H;    /* ���������� */
    delay_us(5);  /* ��ʱ5us */
}



/**********************************************************
* @funcName ��IIC_Send_Ack
* @brief    ��ģ��IIC����������Ӧ���ź�
* @param    ��uint8_t ack
*   @arg    ��0 --- Ӧ��   1 --- ��Ӧ��
* @retval   ��void
* @details  ��
* @fn       ��
*            SCL_H         ʱ��������
*            SCL_L         ʱ��������
*            SDA_OUT_H     ���ģʽ������������
*            SDA_OUT_L     ���ģʽ������������
************************************************************/
void IIC_Send_Ack(uint8_t ack)
{
    SCL_L;                /* ʱ�������� */
    delay_us(5);          /* ��ʱ5us */
    /*�ж��Ƿ�Ҫ����Ӧ���ź�*/
    if(ack == 1)          /* ���Ͳ�Ӧ���ź� */
    {
        SDA_OUT_H;        /* ���������� */
        delay_us(1);      /* ��ʱ1us */
        SCL_H;            /* ʱ�������� */
    }
    else if(ack == 0)     /* ����Ӧ���ź� */
    {
        SDA_OUT_L;        /* ���������� */
        delay_us(1);      /* ��ʱ1us */
        SCL_H;            /* ʱ�������� */
    }

    delay_us(5);          /* ��ʱ5us */
    SCL_L;                /* ʱ�������� */
}



/**********************************************************
* @funcName ��IIC_Receive_Ack
* @brief    ��ģ��IIC����������Ӧ���ź�
* @param    ��void
* @retval   ��uint8_t  ---  (0-Ӧ��   1-��Ӧ��)
* @details  ��
* @fn       ��
*            SCL_H         ʱ��������
*            SCL_L         ʱ��������
*            SDA_OUT_H     ���ģʽ������������
*            SDA_OUT_L     ���ģʽ������������
*            SDA_IN        ����ģʽ
*            SDA_INT       ���յ���ֵ
************************************************************/
uint8_t IIC_Receive_Ack(void)
{
    SCL_L;         /* ʱ�������� */
    SDA_IN;        /* ��������ģʽ */
    delay_us(5);   /* ��ʱ5us */
    SCL_H;         /* ʱ�������� */

    /* �жϴӻ����͵�Ӧ���ź� */
    /* �ӻ���Ӧ�� */
    if(SDA_INT)
    {
        SCL_L;      /* ʱ�������� */
        return 1;
    }
    /* �ӻ�Ӧ�� */
    /* ʱ�������� */
    SCL_L;
    return 0;
}



/**********************************************************
* @funcName ��IIC_Send_Byte
* @brief    ��ģ��IIC����������һ���ֽڵ�����
* @param    ��uint8_t data
* @retval   ��void
* @details  ��
* @fn       ��
*            SCL_H         ʱ��������
*            SCL_L         ʱ��������
*            SDA_OUT_H     ���ģʽ������������
*            SDA_OUT_L     ���ģʽ������������
************************************************************/
void IIC_Send_Byte(uint8_t data)
{
    uint8_t i;

    for(i=0;i<8;i++)
    {
        SCL_L;             /* ʱ�������� */
        delay_us(5);       /* ��ʱ5us */

        /*�жϷ�����0����1*/
        if(data & 0x80)    /* ����1 */
        {
            SDA_OUT_H;     /* ���������� */
        }
        else               /* ����0 */
        {
            SDA_OUT_L;     /* ���������� */
        }

        delay_us(1);       /* ��ʱ1us */
        SCL_H;             /* ʱ�������� */
        delay_us(5);       /* ��ʱ5us */
        
        data <<= 1;        /* data����һλ */
    }

    SCL_L;                 /* ʱ�������� */
}



/**********************************************************
* @funcName ��IIC_Receive_Byte
* @brief    ��ģ��IIC����������һ���ֽڵ�����
* @param    ��void
* @retval   ��uint8_t
* @details  ��
* @fn       ��
*            SCL_H         ʱ��������
*            SCL_L         ʱ��������
*            SDA_OUT_H     ���ģʽ������������
*            SDA_OUT_L     ���ģʽ������������
*            SDA_IN        ����ģʽ
*            SDA_INT       ���յ���ֵ
************************************************************/
uint8_t IIC_Receive_Byte(void)
{
    uint8_t i;
    uint8_t data = 0;

    SCL_L;               /* ����ʱ���� */
    SDA_IN;              /* �������ģʽ */

    for(i=0;i<8;i++)
    {
        SCL_L;           /* ʱ�������� */

        data <<= 1;      /* data����һλ */
        delay_us(5);     /* ��ʱ5us */
        SCL_H;           /* ʱ�������� */

        /* �жϽ��յ���1����0 */ 
        /* ���յ���1 */
        if(SDA_INT)
        {
            data |= 0x1;
        }
        /* ���յ���0 �������� */
    }

    SCL_L;                /* ʱ�������� */

    return data;
}



