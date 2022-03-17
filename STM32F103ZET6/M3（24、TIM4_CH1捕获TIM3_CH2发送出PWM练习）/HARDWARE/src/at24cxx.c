#include "sys.h" 
#include "at24cxx.h"
#include "delay.h"
#include "iic.h"
#include "stdio.h"

/*****************************************************
*��������  ����at24cxx����Ҫ��GPIO�ڽ��г�ʼ������
*������    ��AT24C04_Init
*��������  ��void
*��������ֵ��void
*����      ��
*          ʱ����  IIC_SCL  PB8   �������
*          ������  IIC_SDA  PB9   ��©���
********************************************************/
void AT24CXX_Init(void)
{
  IIC_Init();
}

/*****************************************************************
*��������  ��дһ���ֽڵ����ݵ�AT24CXX��ĳ���ڲ���ַ��
*������    ��AT24CXX_Write_Byte
*��������  ��u16 address   u8 data
*��������ֵ��u8
*����      ��
*     0      AT24CXX_NO_ERR    //Ӧ����Ч   ���ݴ���ɹ�
*     1      AT24CXX_ERR1      //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*     2      AT24CXX_ERR2      //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*     3      AT24CXX_ERR3      //Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*     0xa0   AT24CXX_WRITE     //������ַ   д����
*     0xa1   AT24CXX_READ      //������ַ   ������
*******************************************************************/
u8 AT24CXX_Write_Byte(u16 address,u8 data)
{
  u8 ack;
  
  //����������ʼ�ź�
  IIC_Start();
  /*�ж�AT24CXX�ͺ�*/
  //�����AT24C16���ϵ��ͺ�(������AT24C16)
  if(AT24CXX > AT24C16)
  {
    //��������������ַ(д)
    IIC_Send_Byte(AT24CXX_WRITE);
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
    //���͸߰�λ�洢��ַ(�ֵ�ַ)
    IIC_Send_Byte(address>>8);
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR2;
    }
  }
  else 
  {
    //��������������ַ(д)
    IIC_Send_Byte(AT24CXX_WRITE+((address/256)<<1));
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
  }
  //���͵Ͱ�λ�洢��ַ(�ֵ�ַ)
  IIC_Send_Byte(address%256);
  //���մӻ�Ӧ���ź�
  ack = IIC_Receive_Ack();
  //�ж�Ӧ��
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }
  //������������
  IIC_Send_Byte(data);
  //���մӻ�Ӧ���ź�
  ack = IIC_Receive_Ack();
  //�ж�Ӧ��
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }
  //��������ֹͣ�ź�
  IIC_Stop();
  //��ʱ6ms
  delay_ms(6);
  
  return AT24CXX_NO_ERR;
}

/*****************************************************************
*��������  ����AT24CXX�����ȡ1���ֽڵ�����
*������    ��AT24CXX_Read_Byte
*��������  ��u8 address   u8 *data
*��������ֵ��u8
*����      ��
*     0      AT24CXX_NO_ERR    //Ӧ����Ч   ���ݴ���ɹ�
*     1      AT24CXX_ERR1      //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*     2      AT24CXX_ERR2      //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*     3      AT24CXX_ERR3      //Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*     0xa0   AT24CXX_WRITE     //������ַ   д����
*     0xa1   AT24CXX_READ      //������ַ   ������
*******************************************************************/
u8 AT24CXX_Read_Byte(u16 address,u8 *data)
{
  u8 ack;
  
  //����������ʼ�ź�
  IIC_Start();
  /*�ж�AT24CXX�ͺ�*/
  //�����AT24C16���ϵ��ͺ�(������AT24C16)
  if(AT24CXX > AT24C16)
  {
    //��������������ַ(д)
    IIC_Send_Byte(AT24CXX_WRITE);
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
    //���͸߰�λ�洢��ַ(�ֵ�ַ)
    IIC_Send_Byte(address>>8);
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR2;
    }
  }
  else 
  {
    //��������������ַ(д)
    IIC_Send_Byte(AT24CXX_WRITE+((address/256)<<1));
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
  }
  //���͵Ͱ�λ�洢��ַ(�ֵ�ַ)
  IIC_Send_Byte(address%256);
  //���մӻ�Ӧ���ź�
  ack = IIC_Receive_Ack();
  //�ж�Ӧ��
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }  
  //����������ʼ�ź�
  IIC_Start();
  //��������������ַ(��)
  IIC_Send_Byte(AT24CXX_READ);
  //��������Ӧ��
  ack = IIC_Receive_Ack();
  //�ж�Ӧ��
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }
  //������ʼ��������
  *data = IIC_Receive_Byte();
  //��������Ӧ���ź�    //��Ӧ���ź�
  IIC_Send_Ack(1);
  //��������ֹͣ�ź�
  IIC_Stop();
  
  return ack;
}

/*****************************************************************
*��������  ��д����ֽڵ����ݵ�AT24CXX����ܿ�ҳ��
*������    ��AT24CXX_Write_Bytes1
*��������  ��u16 address u8 byte_num u8 *Str
*��������ֵ��u8 
*����      ��
*     0      AT24CXX_NO_ERR    //Ӧ����Ч   ���ݴ���ɹ�
*     1      AT24CXX_ERR1      //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*     2      AT24CXX_ERR2      //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*     3      AT24CXX_ERR3      //Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*     4      AT24CXX_ERR4      //Ӧ��ʧЧ   ���ֿ�ҳ����
*     0xa0   AT24CXX_WRITE     //������ַ   д����
*     0xa1   AT24CXX_READ      //������ַ   ������
*******************************************************************/
u8 AT24CXX_Write_Bytes1(u16 address,u8 byte_num,u8 *Str)
{
  u8 i;
  u8 ack;
  
  //����������ʼ�ź�
  IIC_Start();
  /*�ж�AT24CXX�ͺ�*/
  //�����AT24C16���ϵ��ͺ�(������AT24C16)
  if(AT24CXX > AT24C16)
  {
    //��������������ַ(д)
    IIC_Send_Byte(AT24CXX_WRITE);
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
    //���͸߰�λ�洢��ַ(�ֵ�ַ)
    IIC_Send_Byte(address>>8);
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR2;
    }
  }
  else 
  {
    //��������������ַ(д)
    IIC_Send_Byte(AT24CXX_WRITE+((address/256)<<1));
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
  }
  //���͵Ͱ�λ�洢��ַ(�ֵ�ַ)
  IIC_Send_Byte(address%256);
  //���մӻ�Ӧ���ź�
  ack = IIC_Receive_Ack();
  //�ж�Ӧ��
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }
  for(i=0;i<byte_num;i++)
  {
    //������������
    IIC_Send_Byte(*Str);
    //��������Ӧ��
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      delay_ms(6);
      return AT24CXX_ERR3;
    }
    Str++;
  }
  //��������ֹͣ�ź�
  IIC_Stop();
  //��ʱ6ms
  delay_ms(6);
  
  return AT24CXX_NO_ERR;
}

/*****************************************************************
*��������  ��д����ֽڵ����ݵ�AT24CXX��ɿ�ҳ��
*������    ��AT24CXX_Write_Bytes
*��������  ��u16 address u8 byte_num u8 *Str
*��������ֵ��u8 
*����      ��
*     0      AT24CXX_NO_ERR    //Ӧ����Ч   ���ݴ���ɹ�
*     1      AT24CXX_ERR1      //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*     2      AT24CXX_ERR2      //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*     3      AT24CXX_ERR3      //Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*     4      AT24CXX_ERR4      //Ӧ��ʧЧ   ���ֿ�ҳ����
*     0xa0   AT24CXX_WRITE     //������ַ   д����
*     0xa1   AT24CXX_READ      //������ַ   ������
*******************************************************************/
u8 AT24CXX_Write_Bytes(u16 address,u8 byte_num,u8 *Str)
{
  u8 sck = 0;
  u8 remain;
  
  while(1)
  {
    //���㵱ǰҪ�洢��ҳ��ʣ�����ֽڿ���д
    remain =AT24CXX_PAGE - address%AT24CXX_PAGE;
    if(remain >= byte_num)
    {
      sck = AT24CXX_Write_Bytes1(address,byte_num,Str);
      break;
    }
    else
    {
      sck = AT24CXX_Write_Bytes1(address,remain,Str);
      byte_num -= remain;   //��ȥд�������
      address += remain;    //����洢�µ�ַ
      Str += remain;        //����������µ�ַ
    } 
  }
  return sck;
}


/*****************************************************************
*��������  ����AT24CXX�����ȡ����ֽڵ�����
*������    ��AT24CXX_Read_Bytes
*��������  ��u16 address   u8 byte_num    u8 *Str
*��������ֵ��u8
*����      ��
*     0      AT24CXX_NO_ERR    //Ӧ����Ч   ���ݴ���ɹ�
*     1      AT24CXX_ERR1      //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*     2      AT24CXX_ERR2      //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*     3      AT24CXX_ERR3      //Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*     0xa0   AT24CXX_WRITE     //������ַ   д����
*     0xa1   AT24CXX_READ      //������ַ   ������
*******************************************************************/
u8 AT24CXX_Read_Bytes(u16 address,u8 byte_num,u8 *Str)
{
  u8 ack;
  
  //����������ʼ�ź�
  IIC_Start();
  /*�ж�AT24CXX�ͺ�*/
  //�����AT24C16���ϵ��ͺ�(������AT24C16)
  if(AT24CXX > AT24C16)
  {
    //��������������ַ(д)
    IIC_Send_Byte(AT24CXX_WRITE);
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
    //���͸߰�λ�洢��ַ(�ֵ�ַ)
    IIC_Send_Byte(address>>8);
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR2;
    }
  }
  else 
  {
    //��������������ַ(д)
    IIC_Send_Byte(AT24CXX_WRITE+((address/256)<<1));
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
  }
  //���͵Ͱ�λ�洢��ַ(�ֵ�ַ)
  IIC_Send_Byte(address%256);
  //���մӻ�Ӧ���ź�
  ack = IIC_Receive_Ack();
  //�ж�Ӧ��
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }  
  //����������ʼ�ź�
  IIC_Start();
  //��������������ַ(��)
  IIC_Send_Byte(AT24CXX_READ);
  //��������Ӧ��
  ack = IIC_Receive_Ack();
  //�ж�Ӧ��
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }
  while(1)
  {
   //������ʼ��������
   *Str = IIC_Receive_Byte();
   byte_num--;
   if(byte_num == 0)
   {
     IIC_Send_Ack(1);   //����Ӧ��
     break;
   }
   IIC_Send_Ack(0);
   Str++;
  }
  //��������ֹͣ�ź�
  IIC_Stop();

  return ack;
}

/*******************************************************************************
*��������  ��д����ֽڵ����ݵ�AT24CXX��(ֱ��һ��һ����ַд�����ÿ��ǿ粻��ҳ)
*������    ��AT24CXX_Write_Addr
*��������  ��u16 address u8 byte_num u8 *Str
*��������ֵ��u8 
*����      ��
*     0      AT24CXX_NO_ERR    //Ӧ����Ч   ���ݴ���ɹ�
*     1      AT24CXX_ERR1      //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*     2      AT24CXX_ERR2      //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*     3      AT24CXX_ERR3      //Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*     4      AT24CXX_ERR4      //Ӧ��ʧЧ   ���ֿ�ҳ����
*     0xa0   AT24CXX_WRITE     //������ַ   д����
*     0xa1   AT24CXX_READ      //������ַ   ������
*********************************************************************************/
u8 AT24CXX_Write_Addr(u16 address,u8 byte_num,u8 *Str)
{
  u8 i;
  u8 ack;
  for(i=0;i<byte_num;i++)
  {
    ack = AT24CXX_Write_Byte(address,*Str);
    address++;
    Str++;
  }
  return ack;
}

/*******************************************************************************
*��������  ����AT24CXX�����ȡ����ֽڵ�����(ֱ��һ��һ����ַд�����ÿ��ǿ粻��ҳ)
*������    ��AT24CXX_Read_Addr
*��������  ��u16 address   u8 byte_num    u8 *Str
*��������ֵ��u8
*����      ��
*     0      AT24CXX_NO_ERR    //Ӧ����Ч   ���ݴ���ɹ�
*     1      AT24CXX_ERR1      //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*     2      AT24CXX_ERR2      //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*     3      AT24CXX_ERR3      //Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*     0xa0   AT24CXX_WRITE     //������ַ   д����
*     0xa1   AT24CXX_READ      //������ַ   ������
*********************************************************************************/
u8 AT24CXX_Read_Addr(u16 address,u8 byte_num,u8 *Str)
{
  u8 i;
  u8 ack;
  for(i=0;i<byte_num;i++)
  {
    ack = AT24CXX_Read_Byte(address,Str);
    address++;
    Str++;
  }
  return ack;
}

/******************************************************************************************
*��������  ��(���AT24CXX����ʱ)дһ���ֽڵ����ݵ�ĳ��AT24CXX��ĳ���ڲ���ַ��
*������    ��AT24CXXaddr_Write_Byte
*��������  ��u16 AT24CXXmodel(�����ͺ�)  u8 AT24CXXaddr(������ַ(д))   u16 address   u8 data   
*��������ֵ��u8
*����      ��
*     0      AT24CXX_NO_ERR    //Ӧ����Ч   ���ݴ���ɹ�
*     1      AT24CXX_ERR1      //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*     2      AT24CXX_ERR2      //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*     3      AT24CXX_ERR3      //Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*     0xa0   AT24CXX_WRITE     //������ַ   д����
*     0xa1   AT24CXX_READ      //������ַ   ������
*********************************************************************************************/
u8 AT24CXXaddr_Write_Byte(u16 AT24CXXmodel,u8 AT24CXXaddr,u16 address,u8 data)
{
  u8 ack;
  
  //����������ʼ�ź�
  IIC_Start();
  /*�ж�AT24CXX�ͺ�*/
  //�����AT24C16���ϵ��ͺ�(������AT24C16)
  if(AT24CXXmodel > AT24C16)
  {
    //��������������ַ(д)
    IIC_Send_Byte(AT24CXXaddr);
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
    //���͸߰�λ�洢��ַ(�ֵ�ַ)
    IIC_Send_Byte(address>>8);
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR2;
    }
  }
  else 
  {
    //��������������ַ(д)
    IIC_Send_Byte(AT24CXXaddr+((address/256)<<1));
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
  }
  //���͵Ͱ�λ�洢��ַ(�ֵ�ַ)
  IIC_Send_Byte(address%256);
  //���մӻ�Ӧ���ź�
  ack = IIC_Receive_Ack();
  //�ж�Ӧ��
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }
  //������������
  IIC_Send_Byte(data);
  //���մӻ�Ӧ���ź�
  ack = IIC_Receive_Ack();
  //�ж�Ӧ��
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }
  //��������ֹͣ�ź�
  IIC_Stop();
  //��ʱ6ms
  delay_ms(6);
  
  return AT24CXX_NO_ERR;
}

/*****************************************************************************************
*��������  ��(���AT24CXX����ʱ)��ĳ��AT24CXX��ĳ���洢��ַ���ȡ1���ֽڵ�����
*������    ��AT24CXXaddr_Read_Byte
*��������  ��u16 AT24CXXmodel(�����ͺ�)  u8 AT24CXXaddr(������ַ(д))   u16 address   u8 data   
*��������ֵ��u8
*����      ��
*     0      AT24CXX_NO_ERR    //Ӧ����Ч   ���ݴ���ɹ�
*     1      AT24CXX_ERR1      //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*     2      AT24CXX_ERR2      //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*     3      AT24CXX_ERR3      //Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*     0xa0   AT24CXX_WRITE     //������ַ   д����
*     0xa1   AT24CXX_READ      //������ַ   ������
******************************************************************************************/
u8 AT24CXXaddr_Read_Byte(u16 AT24CXXmodel,u8 AT24CXXaddr,u16 address,u8 *data)
{
  u8 ack;
  
  //����������ʼ�ź�
  IIC_Start();
  /*�ж�AT24CXX�ͺ�*/
  //�����AT24C16���ϵ��ͺ�(������AT24C16)
  if(AT24CXXmodel > AT24C16)
  {
    //��������������ַ(д)
    IIC_Send_Byte(AT24CXXaddr);
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
    //���͸߰�λ�洢��ַ(�ֵ�ַ)
    IIC_Send_Byte(address>>8);
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR2;
    }
  }
  else 
  {
    //��������������ַ(д)
    IIC_Send_Byte(AT24CXXaddr+((address/256)<<1));
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
  }
  //���͵Ͱ�λ�洢��ַ(�ֵ�ַ)
  IIC_Send_Byte(address%256);
  //���մӻ�Ӧ���ź�
  ack = IIC_Receive_Ack();
  //�ж�Ӧ��
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }  
  //����������ʼ�ź�
  IIC_Start();
  //��������������ַ(��)
  IIC_Send_Byte(AT24CXXaddr|0x1);
  //��������Ӧ��
  ack = IIC_Receive_Ack();
  //�ж�Ӧ��
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }
  //������ʼ��������
  *data = IIC_Receive_Byte();
  //��������Ӧ���ź�    //��Ӧ���ź�
  IIC_Send_Ack(1);
  //��������ֹͣ�ź�
  IIC_Stop();
  
  return ack;
}

/*******************************************************************************************************
*��������  ��(���AT24CXX)��ĳ��AT24CXX��ĳ���洢��ַ��ʼд����ֽڵ����ݵ������ܿ�ҳ��
*������    ��AT24CXXaddr_Write_Bytes1
*��������  ��u16 AT24CXXmodel(�����ͺ�)  u8 AT24CXXaddr(������ַ(д))  u16 address   u8 byte_num  u8 *Str
*��������ֵ��u8 
*����      ��
*     0      AT24CXX_NO_ERR    //Ӧ����Ч   ���ݴ���ɹ�
*     1      AT24CXX_ERR1      //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*     2      AT24CXX_ERR2      //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*     3      AT24CXX_ERR3      //Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*     4      AT24CXX_ERR4      //Ӧ��ʧЧ   ���ֿ�ҳ����
*     0xa0   AT24CXX_WRITE     //������ַ   д����
*     0xa1   AT24CXX_READ      //������ַ   ������
**********************************************************************************************************/
u8 AT24CXXaddr_Write_Bytes1(u16 AT24CXXmodel,u8 AT24CXXaddr,u16 address,u8 byte_num,u8 *Str)
{
  u8 i;
  u8 ack;
  
  //����������ʼ�ź�
  IIC_Start();
  /*�ж�AT24CXX�ͺ�*/
  //�����AT24C16���ϵ��ͺ�(������AT24C16)
  if(AT24CXXmodel > AT24C16)
  {
    //��������������ַ(д)
    IIC_Send_Byte(AT24CXXaddr);
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
    //���͸߰�λ�洢��ַ(�ֵ�ַ)
    IIC_Send_Byte(address>>8);
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR2;
    }
  }
  else 
  {
    //��������������ַ(д)
    IIC_Send_Byte(AT24CXXaddr+((address/256)<<1));
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
  }
  //���͵Ͱ�λ�洢��ַ(�ֵ�ַ)
  IIC_Send_Byte(address%256);
  //���մӻ�Ӧ���ź�
  ack = IIC_Receive_Ack();
  //�ж�Ӧ��
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }
  for(i=0;i<byte_num;i++)
  {
    //������������
    IIC_Send_Byte(*Str);
    //��������Ӧ��
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      delay_ms(6);
      return AT24CXX_ERR3;
    }
    Str++;
  }
  //��������ֹͣ�ź�
  IIC_Stop();
  //��ʱ6ms
  delay_ms(6);
  
  return AT24CXX_NO_ERR;
}

/*******************************************************************************************************
*��������  ��(���AT24CXX)��ĳ��AT24CXX��ĳ���洢��ַ��ʼд����ֽڵ����ݵ����ɿ�ҳ��
*������    ��AT24CXXaddr_Write_Bytes
*��������  ��u16 AT24CXXmodel(�����ͺ�)  u8 AT24CXXaddr(������ַ(д))  u16 address u8 byte_num u8 *Str
*��������ֵ��u8 
*����      ��
*     0      AT24CXX_NO_ERR    //Ӧ����Ч   ���ݴ���ɹ�
*     1      AT24CXX_ERR1      //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*     2      AT24CXX_ERR2      //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*     3      AT24CXX_ERR3      //Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*     4      AT24CXX_ERR4      //Ӧ��ʧЧ   ���ֿ�ҳ����
*     0xa0   AT24CXX_WRITE     //������ַ   д����
*     0xa1   AT24CXX_READ      //������ַ   ������
**********************************************************************************************************/
u8 AT24CXXaddr_Write_Bytes(u16 AT24CXXmodel,u8 AT24CXXaddr,u8 AT24CXXpage,u16 address,u8 byte_num,u8 *Str)
{
  u8 sck = 0;
  u8 remain;
  
  while(1)
  {
    //���㵱ǰҪ�洢��ҳ��ʣ�����ֽڿ���д
    remain =AT24CXXpage - address%AT24CXXpage;
    if(remain >= byte_num)
    {
      sck = AT24CXXaddr_Write_Bytes1(AT24CXXmodel,AT24CXXaddr,address,byte_num,Str);
      break;
    }
    else
    {
      sck = AT24CXXaddr_Write_Bytes1(AT24CXXmodel,AT24CXXaddr,address,remain,Str);
      byte_num -= remain;   //��ȥд�������
      address += remain;    //����洢�µ�ַ
      Str += remain;        //����������µ�ַ
    } 
  }
  return sck;
}

/**********************************************************************************************************
*��������  ��(���AT24CXX)��ĳ��AT24CXX��ĳ����ַ��ʼ��ȡ����ֽڵ�����
*������    ��AT24CXX_Read_Bytes
*��������  ��u16 AT24CXXmodel(�����ͺ�)  u8 AT24CXXaddr(������ַ(д))   u16 address   u8 byte_num    u8 *Str
*��������ֵ��u8
*����      ��
*     0      AT24CXX_NO_ERR    //Ӧ����Ч   ���ݴ���ɹ�
*     1      AT24CXX_ERR1      //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*     2      AT24CXX_ERR2      //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*     3      AT24CXX_ERR3      //Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*     0xa0   AT24CXX_WRITE     //������ַ   д����
*     0xa1   AT24CXX_READ      //������ַ   ������
**********************************************************************************************************/
u8 AT24CXXaddr_Read_Bytes(u16 AT24CXXmodel,u8 AT24CXXaddr,u16 address,u8 byte_num,u8 *Str)
{
  u8 ack;
  
  //����������ʼ�ź�
  IIC_Start();
  /*�ж�AT24CXX�ͺ�*/
  //�����AT24C16���ϵ��ͺ�(������AT24C16)
  if(AT24CXXmodel > AT24C16)
  {
    //��������������ַ(д)
    IIC_Send_Byte(AT24CXXaddr);
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
    //���͸߰�λ�洢��ַ(�ֵ�ַ)
    IIC_Send_Byte(address>>8);
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR2;
    }
  }
  else 
  {
    //��������������ַ(д)
    IIC_Send_Byte(AT24CXXaddr+((address/256)<<1));
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
  }
  //���͵Ͱ�λ�洢��ַ(�ֵ�ַ)
  IIC_Send_Byte(address%256);
  //���մӻ�Ӧ���ź�
  ack = IIC_Receive_Ack();
  //�ж�Ӧ��
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }  
  //����������ʼ�ź�
  IIC_Start();
  //��������������ַ(��)
  IIC_Send_Byte(AT24CXXaddr|1);
  //��������Ӧ��
  ack = IIC_Receive_Ack();
  //�ж�Ӧ��
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }
  while(1)
  {
   //������ʼ��������
   *Str = IIC_Receive_Byte();
   byte_num--;
   if(byte_num == 0)
   {
     IIC_Send_Ack(1);   //����Ӧ��
     break;
   }
   IIC_Send_Ack(0);
   Str++;
  }
  //��������ֹͣ�ź�
  IIC_Stop();

  return ack;
}

/*******************************************************************************
*��������  ��(���AT24CXX)д����ֽڵ����ݵ�AT24CXX��(ֱ��һ��һ����ַд�����ÿ��ǿ粻��ҳ)
*������    ��AT24CXXaddr_Write_Addr
*��������  ��u16 address u8 byte_num u8 *Str
*��������ֵ��u8 
*����      ��
*     0      AT24CXX_NO_ERR    //Ӧ����Ч   ���ݴ���ɹ�
*     1      AT24CXX_ERR1      //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*     2      AT24CXX_ERR2      //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*     3      AT24CXX_ERR3      //Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*     4      AT24CXX_ERR4      //Ӧ��ʧЧ   ���ֿ�ҳ����
*     0xa0   AT24CXX_WRITE     //������ַ   д����
*     0xa1   AT24CXX_READ      //������ַ   ������
*********************************************************************************/
u8 AT24CXXaddr_Write_Addr(u16 AT24CXXmodel,u8 AT24CXXaddr,u16 address,u8 byte_num,u8 *Str)
{
  u8 i;
  u8 ack;
  for(i=0;i<byte_num;i++)
  {
    ack = AT24CXXaddr_Write_Byte(AT24CXXmodel,AT24CXXaddr,address,*Str);
    address++;
    Str++;
  }
  return ack;
}

/*******************************************************************************
*��������  ����AT24CXX�����ȡ����ֽڵ�����(ֱ��һ��һ����ַд�����ÿ��ǿ粻��ҳ)
*������    ��AT24CXX_Read_Addr
*��������  ��u16 address   u8 byte_num    u8 *Str
*��������ֵ��u8
*����      ��
*     0      AT24CXX_NO_ERR    //Ӧ����Ч   ���ݴ���ɹ�
*     1      AT24CXX_ERR1      //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*     2      AT24CXX_ERR2      //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*     3      AT24CXX_ERR3      //Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*     0xa0   AT24CXX_WRITE     //������ַ   д����
*     0xa1   AT24CXX_READ      //������ַ   ������
*********************************************************************************/
u8 AT24CXXaddr_Read_Addr(u16 AT24CXXmodel,u8 AT24CXXaddr,u16 address,u8 byte_num,u8 *Str)
{
  u8 i;
  u8 ack;
  for(i=0;i<byte_num;i++)
  {
    ack = AT24CXXaddr_Read_Byte(AT24CXXmodel,AT24CXXaddr,address,Str);
    address++;
    Str++;
  }
  return ack;
}




















