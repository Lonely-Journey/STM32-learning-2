#include "sys.h"  
#include "stdio.h"

/*****************************************************
*��������  ����CAM��Ӧ��GPIO�ںͼĴ�����������
*������    ��CAN1_Init
*��������  ��void
*��������ֵ��void
*����      ��
********************************************************/
void CAN1_Init(void)
{
	CAN_InitTypeDef         CAN_InitStruct;           //CAN������״̬���ýṹ��
	CAN_FilterInitTypeDef   CAN_FilterInitStruct;     //CANɸѡ�����ýṹ��
	
	//ʱ��ʱ��  CAN1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);  //ʹ��CAN1ʱ��	
	
	/*GPIOx����*/
	//�ػ�ģʽֱ���þ��У�����GPIO
	
	/*CAN�Ĵ�������*/
	//���ؼĴ���
	CAN_InitStruct.CAN_TTCM = DISABLE;	                 //��ֹʱ�䴥��ͨ��ģʽ     (CAN->MCR�Ĵ����ĵ�7λ)   
	CAN_InitStruct.CAN_ABOM = DISABLE;	                 //�����Զ������߹رչ���   (CAN->MCR�Ĵ����ĵ�6λ)	  
	CAN_InitStruct.CAN_AWUM = DISABLE;                   //�����Զ�����ģʽ         (CAN->MCR�Ĵ����ĵ�5λ)	
	CAN_InitStruct.CAN_NART = DISABLE;	                 //���������Զ�����         (CAN->MCR�Ĵ����ĵ�4λ)	
	CAN_InitStruct.CAN_RFLM = DISABLE;	                 //���Ĳ�����,�µĸ��Ǿɵ�  (CAN->MCR�Ĵ����ĵ�3λ)
	CAN_InitStruct.CAN_TXFP = DISABLE;	                 //���ȼ��ɱ��ı�ʶ������   (CAN->MCR�Ĵ����ĵ�2λ)
	CAN_InitStruct.CAN_Mode = CAN_Mode_Silent_LoopBack;	 //��Ĭ��ػ�ģʽ           (CAN_BTR�Ĵ����ĵ�30-31λ)
	CAN_InitStruct.CAN_SJW  = CAN_SJW_2tq;	             //��������ʱ��Ƭ�����     (CAN_BTR�Ĵ����ĵ�24-25λ)
	//500K ��һ������ͨ������õ����ο��ο��ֲ�622
	CAN_InitStruct.CAN_BS1  = CAN_BS1_11tq;              //Tbs1��Χ1 ~ 16           (CAN_BTR�Ĵ����ĵ�20-22λ)
	CAN_InitStruct.CAN_BS2  = CAN_BS2_7tq;               //Tbs2��Χ1 ~ 8            (CAN_BTR�Ĵ����ĵ�16-19λ)
	CAN_InitStruct.CAN_Prescaler = 0x03;                 //��Ƶϵ��(Fdiv)Ϊbrp+1	    (CAN_BTR�Ĵ����ĵ�0-9λ)
	CAN_Init(CAN1, &CAN_InitStruct);                     // ��ʼ��CAN1������״̬�Ĵ���  
	
	//ɸѡ�����Ĵ���
	CAN_FilterInitStruct.CAN_FilterNumber         = 0;	                     //����ѡ�������0
	CAN_FilterInitStruct.CAN_FilterMode           = CAN_FilterMode_IdList;   //����0�������ɸѡ����ģʽΪ��ʶ���б�ģʽ(CAN->FM1R�Ĵ���)
	CAN_FilterInitStruct.CAN_FilterScale          = CAN_FilterScale_32bit;   //0�������ɸѡ���߶�����Ϊ��32λ�߶� (CAN->FS1R�Ĵ���)
	
	/*************************************************************
	*ע��1��
	*     CAN_FiRx�Ĵ�����Ϊ����ɸѡ�Ĵ����ֱ���FR1��FR2
	*     ��ɸѡ��(FR1)���ɸѡ��(FR2)�����ʹ�õ�,
	*     FR1����������CANx�����ID��FR2����������CANx���������
	*     FR2��ĳλ����1�Ļ���ID(���͹����������)��Ӧλ�ϵ���ֵ
	*     �ͱ����FR1��λ�ϵ�һ��������һ�£���֮���ǡ������ġ���
	*
	*ע��2��
	*     ����21λ����Ϊ��CAN ���� FIFO �����ʶ���Ĵ��� ��λ���룬
	*     (���� FIFO �����ʶ���Ĵ�����21λ��ʼ���Ǳ�ʶ��ID)
	***************************************************************/
	CAN_FilterInitStruct.CAN_FilterIdHigh         = (123<<5);        //��16λ������ID(��׼��ʶ��):123
	CAN_FilterInitStruct.CAN_FilterIdLow          = 0x0000;          //��16λ                    ��ɸѡ��(FR1)    
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh     = 0x0000;          //��16λ������ID����:0
	CAN_FilterInitStruct.CAN_FilterMaskIdLow      = 0x0000;          //��16λ                    ��ɸѡ��(FR2)     
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//ɸѡ��0������FIFO0  (CAN->FFA1R�Ĵ���)
	CAN_FilterInitStruct.CAN_FilterActivation     = ENABLE;          //����ɸѡ��0         (CAN->FA1R�Ĵ���)
	CAN_FilterInit(&CAN_FilterInitStruct);                           //��ʼ��ɸCAN1ѡ���Ĵ���
}

/******************************************
*��������  ��CAN1���߷���һ���ֽ�����
*������    ��CAN1_Send_Byte
*��������  ��u8 data
*��������ֵ��void
*��������  ��
*********************************************/
void CAN1_Send_Byte(u8 data)
{
	u8 mbox;
  CanTxMsg TxMessage;   //�����������ýṹ��
	
	//׼������
  TxMessage.StdId = 123;	 //��׼��ʶ��(�����ID) ��123                (CAN_TIxR�Ĵ�����21-31λ)
  TxMessage.ExtId = 0;	   //������չ��ʾ�� ��0                        (CAN_TIxR�Ĵ�����3-20λ)
  TxMessage.IDE = 0;		   //ʹ����չ��ʶ��                            (CAN_TIxR�Ĵ�����2λ)
  TxMessage.RTR = 0;		   //��Ϣ����Ϊ����֡                          (CAN_TIxR�Ĵ�����1λ)
  TxMessage.DLC = 1;		   //һ�η������ݳ���(��Χ��1~8���ֽ�)��1���ֽ�(CAN_TDTxR�Ĵ�����0-3λ)
  TxMessage.Data[0]= data; //Ҫ���͵�����                              (CAN_TDLxR�Ĵ���)    
  mbox= CAN_Transmit(CAN1, &TxMessage);     //��ʼ���� ������ֵ�ǵ�ǰ���õĿ�����(ÿ��CAN����3�����䣺0���䡢1���䡢2����)   
	
	//�ȴ����ͽ���
	//CAN_TxStatus_Ok      :������ɡ����ͳɹ�������x��
	//CAN_TxStatus_Pending :�޿�����
	//CAN_TxStatus_Failed  :������ɡ�����x��
  while(!(CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Ok));	
}

/******************************************
*��������  ��CAN1���߽���һ���ֽ�����
*������    ��CAN1_Receive_Byte
*��������  ��void
*��������ֵ��void
*��������  ��
*********************************************/
void CAN1_Receive_Byte(void)
{
	CanRxMsg RxMessage;
	u8 data;

	//�����ݵ����������Ϊ0��������������(CAN1->RF0R�Ĵ����ĵ�0-2λ)
	while(CAN_MessagePending(CAN1,CAN_FIFO0) != 0)
	{
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����
		data = RxMessage.Data[0];                //(CAN_RDLxR�Ĵ�����
		
		printf("���յ���������:%d\r\n",data);
	}
}
















