���ڳ����ע������汾��Ϣ���޸���Ϣ��˵��


void NVIC_SetPriorityGrouping(uint32_t PriorityGroup)
 ��������NVIC_SetPriorityGrouping
 ��������ֵ�� ��
 ��������  �� uint32_t PriorityGroup   //��Ҫ���õ���ռ���ȼ���λ��
 ��������  :  �����жϵ����ȼ����飬 ������ռ����Ӧ��λ��
 
uint32_t NVIC_EncodePriority (uint32_t PriorityGroup, uint32_t PreemptPriority, uint32_t SubPriority)
 ��������NVIC_EncodePriority
 ��  �ã������ȼ����顢��ռ���ȼ�ֵ����Ӧ���ȼ�ֵ�������һ���������ݷ���
 ��  �������ȼ�����(�Ĵ����� 8~10 λ��дֵ)
		 ��ռ���ȼ�ֵ����ռ���ȼ�ֵ��
		 ��Ӧ���ȼ�ֵ����Ӧ���ȼ�ֵ��  

void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
 ��������NVIC_SetPriority
 ��  �ã����þ���ĳ���ж�Դ�����ȼ�(����)
 ��  ����IRQn_Type IRQn ----�����жϵı�ţ�����ֱ��д���֣�     
		 priority-----------����������ж����ȼ�����
		 
void NVIC_EnableIRQ(IRQn_Type IRQn)
 ��������NVIC_EnableIRQ
 ��  �ã�NVICģ����ӦƬ�������ж�Դ
 ��  ����IRQn_Type IRQn ----�����жϵı�ţ�����ֱ��д���֣�
	

�ж���غ�������
    NVIC_SetPriorityGrouping(7-2);       //���ȼ�����                 1435
	pri = NVIC_EncodePriority (5, 1, 2); //�������ȼ�����ֵ           1592
	NVIC_SetPriority(USART1_IRQn,pri);   //������ֵд������ж�Դ      1550
	NVIC_EnableIRQ(USART1_IRQn);         //ʹ��NVIC��Ӧͨ��           1467
	
	
	


��ʱ��������
static void delay_ms(u32 ms)
{
	u32 i = 168 / 4 * 1000 * ms;   //��ϵͳʱ��Ƶ�ʾ�����
	while(i)
	{
		i--;
	}
}
��ʱ����΢��
static void delay_ms(u32 us)
{
	u32 i = 168 / 4 * us;    //��ϵͳʱ��Ƶ�ʾ�����
	while(i)
	{
		i--;
	}
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	