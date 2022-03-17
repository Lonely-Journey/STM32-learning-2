#include "stm32f4xx.h"     
#include "beep.h"

/*****************************************************
*��������  ����BEEP��Ӧ��GPIO�ڽ��г�ʼ������
*������    ��BEEP_Init
*��������  ��void
*��������ֵ��void
*����      ��
             BEEP  PF8
             �ߵ��죬�͵粻��-----���ģʽ
********************************************************/
void BEEP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruc;
  
  //ʱ��ʹ��  GPIOF 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
	GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_8;       //PF8
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_OUT;    //ͨ�����
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL; //��������
  GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;    //�������
  GPIO_InitStruc.GPIO_Speed = GPIO_Speed_50MHz;  //�ٶ�Ϊ50MHz
  GPIO_Init(GPIOF,&GPIO_InitStruc);             //�����趨������ʼ��PF8
	
	/*����������ݼĴ���*/
  //�Ƚ����������ر�
  GPIO_SetBits(GPIOF,GPIO_Pin_8);               //�͵粻��  
}
















