#ifndef _AT24CXX_H
#define _AT24CXX_H

  #include "stm32f4xx.h"

  #define AT24CXX_NO_ERR 0      //Ӧ����Ч   ���ݴ���ɹ�
  #define AT24CXX_ERR1   1      //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
  #define AT24CXX_ERR2   2      //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
  #define AT24CXX_ERR3   3      //Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
  #define AT24CXX_ERR4   4      //Ӧ��ʧЧ   ���ֿ�ҳ����
  #define AT24CXX_WRITE  0xa0   //������ַ   д����
  #define AT24CXX_READ   0xa1   //������ַ   ������
  
  #define AT24C01		127        //8���ֽ�Ϊһҳ
  #define AT24C02		255        //8���ֽ�Ϊһҳ
  #define AT24C04		511        //16���ֽ�Ϊһҳ
  #define AT24C08		1023       //16���ֽ�Ϊһҳ
  #define AT24C16		2047       //16���ֽ�Ϊһҳ
  #define AT24C32		4095       //32���ֽ�Ϊһҳ
  #define AT24C64	  8191       //32���ֽ�Ϊһҳ
  #define AT24C128	16383      //64���ֽ�Ϊһҳ
  #define AT24C256	32767      //64���ֽ�Ϊһҳ
  
  //������ʹ�õ���24c02�����Զ���AT24CXXΪAT24C02
  #define    AT24CXX          AT24C02
  #define    AT24CXX_PAGE     8

  void AT24CXX_Init(void);//��ʼ��
  //������
  u8 AT24CXX_Write_Byte(u16 address,u8 data);              //�����ֽ�д
  u8 AT24CXX_Read_Byte(u16 address,u8 *data);              //�����ֽڶ�
  u8 AT24CXX_Write_Bytes1(u16 address,u8 byte_num,u8 *Str);//����ֽ�д���ɿ�ҳ
  u8 AT24CXX_Write_Bytes(u16 address,u8 byte_num,u8 *Str); //����ֽ�д�ɿ�ҳ(�ֶ���ҳ)
  u8 AT24CXX_Read_Bytes(u16 address,u8 byte_num,u8 *Str);  //����ֽڶ��ɿ�ҳ(Ӳ���Զ���ҳ)
  u8 AT24CXX_Write_Addr(u16 address,u8 byte_num,u8 *Str);  //����ֽ�д�ɿ�ҳ(һ����ַһ����ַд�����迼�ǿ�ҳ)
  u8 AT24CXX_Read_Addr(u16 address,u8 byte_num,u8 *Str);   //����ֽڶ��ɿ�ҳ(һ����ַһ����ַд�����迼�ǿ�ҳ)
  //������
  u8 AT24CXXaddr_Write_Byte(u16 AT24CXXmodel,u8 AT24CXXaddr,u16 address,u8 data);
  u8 AT24CXXaddr_Read_Byte(u16 AT24CXXmodel,u8 AT24CXXaddr,u16 address,u8 *data);
  u8 AT24CXXaddr_Write_Bytes1(u16 AT24CXXmodel,u8 AT24CXXaddr,u16 address,u8 byte_num,u8 *Str);
  u8 AT24CXXaddr_Write_Bytes(u16 AT24CXXmodel,u8 AT24CXXaddr,u8 AT24CXXpage,u16 address,u8 byte_num,u8 *Str);
  u8 AT24CXXaddr_Read_Bytes(u16 AT24CXXmodel,u8 AT24CXXaddr,u16 address,u8 byte_num,u8 *Str);
  u8 AT24CXXaddr_Write_Addr(u16 AT24CXXmodel,u8 AT24CXXaddr,u16 address,u8 byte_num,u8 *Str);
  u8 AT24CXXaddr_Read_Addr(u16 AT24CXXmodel,u8 AT24CXXaddr,u16 address,u8 byte_num,u8 *Str);

#endif











