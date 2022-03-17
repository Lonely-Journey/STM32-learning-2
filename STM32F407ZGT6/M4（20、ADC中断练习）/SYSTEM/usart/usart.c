#include "stm32f4xx.h"
#include "stdio.h"

/******************************************************************
*��������  ���Դ���1��Ӧ��GPIO�ڽ��г�ʼ���ͶԴ���1�ļĴ�����������
*������    ��USART1_Init
*��������  ��u32 baud
*��������ֵ��void
*����      ��
*           PA9   TX    ���
*           PA10  RX    ����
*******************************************************************/
void USART1_Init(u32 bps)
{
	GPIO_InitTypeDef   GPIO_InitStruc;    //GPIOx���ýṹ��
	USART_InitTypeDef  USART_InitStruct;  //USART���ýṹ��
	NVIC_InitTypeDef   NVIC_InitStruct;   //NVIC(�ж�)���ýṹ��
	
  //ʱ��ʹ��   GPIOA   USART1
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
	
	/*GPIO������*/
	//PA9��PA10
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //ѡ��PA9��PA10
	GPIO_InitStruc.GPIO_Mode = GPIO_Mode_AF;            //���ù���
	GPIO_InitStruc.GPIO_PuPd = GPIO_PuPd_UP;            //����
	GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;          //�������
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_50MHz;     	//�ٶ�50MHz
	GPIO_Init(GPIOA,&GPIO_InitStruc);                   //�����趨������ʼ��PA9��PA10
	
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);  //PA9 ����ΪUSART1�����(TX)
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //PA10����ΪUSART1������(RX)
	
	/*���ڳ�ʼ��*/
	//USART1
	USART_InitStruct.USART_BaudRate = bps;                    //����������(USART->BRR�Ĵ���)
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;  //�ֳ�Ϊ8λ���ݸ�ʽ(USART->CR1�Ĵ����ĵ�12λ)
	USART_InitStruct.USART_StopBits = USART_StopBits_1;       //һ��ֹͣλ(USART->CR2�Ĵ����ĵ�12��13λ)
	USART_InitStruct.USART_Parity = USART_Parity_No;          //����żУ��λ(USART->CR1�Ĵ����ĵ�10λ)
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������(USART->CR3�Ĵ����ĵ�9λ)
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ(USART->CR1�Ĵ����ĵ�2��3λ)
  USART_Init(USART1, &USART_InitStruct);                        //��ʼ������1
	
	/*�ж�����*/
	//ѡ���жϷ�ʽ��ʹ�ܸ÷�ʽ�ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);     //��ȡ���ݼĴ�����Ϊ��(�������)�ж� (USART->CR1�ĵ�5λ)  
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);     //��⵽������·(����)�ж� (USART->CR1�ĵ�4λ)
	
	//����1�ж�����-----NVIC ����
  NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;      //����1�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =3;		  //�����ȼ�3
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			  //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStruct);	                      //����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//����ʹ��
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1
}


/******************************************************************
*��������  ������1����һ���ֽ����ݣ�8λ��
*������    ��USART1_Send_Byte
*��������  ��u8 data
*��������ֵ��void
*����      ��
            PA9   TX    ���
            PA10  RX    ����
*******************************************************************/
void USART1_Send_Byte(u8 data)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);   //�ȴ����ͽ���(USART1->SR�ĵ�6λ) 
	USART_SendData(USART1, data);                              //����һ���ֽڵ�����(USART1->DR)
}

/******************************************************************
*��������  ������1����һ���ֽ����ݣ�8λ��
*������    ��USART1_Receive_Byte
*��������  ��void
*��������ֵ��u8 
*����      ��
            PA9   TX    ���
            PA10  RX    ����
*******************************************************************/
u8 USART1_Receive_Byte(void)
{
  u8 data;
  while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) != SET);    //�ȴ����ͽ���(USART1->SR�ĵ�5λ)
  data = USART_ReceiveData(USART1);                              //��ȡһ���ֽڵ�����(USART1->DR)
  
  return data;
}

/******************************************************************
*��������  ������1����һ���ַ���
*������    ��USART1_Send_Str
*��������  ��u8 *str
*��������ֵ��void
*����      ��
            PA9   TX    ���
            PA10  RX    ����
*******************************************************************/
void USART1_Send_Str(u8 *str)
{
  while(*str != '\0')
  {
    USART1_Send_Byte(*str);
    str++;
  }
}

/******************************************************************
*��������  ������1����һ���ַ���
*������    ��USART1_Receive_Str
*��������  ��u8 *str
*��������ֵ��void 
*����      ��
            PA9   TX    ���
            PA10  RX    ����
*******************************************************************/
void USART1_Receive_Str(u8 *str)
{
  u8 data;
  while(1)
  {
    data = USART1_Receive_Byte();
    if(data == '#')   //�� # Ϊ������־
    {
      break;
    }
    *str = data;
    str++;
  }
  *str = '\0'; 
}














































































/**********************************printf����*****************************************/
#if 1
	#pragma import(__use_no_semihosting)             
	//��׼����Ҫ��֧�ֺ���                 
	struct __FILE 
	{ 
		int handle; 
		/* Whatever you require here. If the only file you are using is */ 
		/* standard output using printf() for debugging, no file handling */ 
		/* is required. */ 
	}; 
	/* FILE is typedef�� d in stdio.h. */ 
	FILE __stdout;       
	//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
  _sys_exit(int x) 
	{ 
		x = x; 
	} 
	//�ض���fputc���� 
	int fputc(int ch, FILE *f)
	{      
		while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
		USART1->DR = (u8) ch;      
		return ch;
	}
	#endif 
/***********************************************************************************/



