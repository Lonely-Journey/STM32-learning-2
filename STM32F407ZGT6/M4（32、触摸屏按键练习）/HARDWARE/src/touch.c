#include "stm32f4xx.h"  
#include "touch.h"
#include "delay.h"
#include "stdio.h"
#include "lcd.h"
#include "stdlib.h"

ADJUST LCD_Touch;//���ڴ��LCD�봥������֮��ı���ϵ����ƫ����(Ҳ����У׼ϵ��)

//200ns��ʱ����
static void delay_200ns(void)
{
  u8 i = 20;
  while(i) i--;
}

/*****************************************************
*��������  ���Դ�����оƬ��Ӧ��GPIO�ڽ��г�ʼ������
*������    ��Touch_Init
*��������  ��void
*��������ֵ��void
*����      ��
*            CS     PC13 ͨ�����
*            SCK    PB0  ͨ�����
*            PEN    PB1  ͨ������
*            MISO   PB2  ͨ������
*            MOSI   PF11 ͨ�����
********************************************************/
void Touch_Init(void)
{
	 GPIO_InitTypeDef GPIO_InitStruc;        //GPIOx���ýṹ��
  
  //ʱ��ʹ��  GPIOB  GPIOC  GPIOF
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
  /*GPIOx�˿�����*/
	/*���*/
  //PB0
  GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_0;         //ѡ��PB0
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_OUT;      //ͨ�����
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;   //��������
  GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;      //�������
  GPIO_InitStruc.GPIO_Speed = GPIO_Speed_100MHz;  //�ٶ�Ϊ100MHz
  GPIO_Init(GPIOB,&GPIO_InitStruc);               //�����趨������ʼ��PB0
	//PC13
	GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_13;        //ѡ��PC13
	GPIO_Init(GPIOC,&GPIO_InitStruc);               //�����趨������ʼ��PC13
	//PF11
	GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_11;        //ѡ��PF11
	GPIO_Init(GPIOF,&GPIO_InitStruc);               //�����趨������ʼ��PF11
	/*����*/
	//PB1��2
	GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_1|GPIO_Pin_2;         //ѡ��PB1��2
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_IN;                  //ͨ������
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;              //��������
  GPIO_InitStruc.GPIO_Speed = GPIO_Speed_100MHz;             //�ٶ�Ϊ100MHz
  GPIO_Init(GPIOB,&GPIO_InitStruc);                          //�����趨������ʼ��PB1��2
	
	//��ʼ״̬����
  T_CS_H;
  T_SCK_L;
  T_MOSI_L;
}

/*****************************************************
*��������  ��ͨ��������оƬ��ȡ��������������
*������    ��Touch_Data
*��������  ��u8 command
*��������ֵ��u16 
*����      ��
*       Ƭѡ��        T_CS_H       T_CS_L  
        ʱ����        T_SCK_H      T_SCK_L  
        ���������    T_MOSI_H     T_MOSI_L
        ����������    T_MISO 
        �ʽӴ���      T_PEN    
********************************************************/
u16 Touch_Data(u8 command)
{
  u16 data = 0;
  u8 i;
  
  //�������������
  T_MOSI_L;
  //����Ƭѡ
  T_CS_L;
  //ѭ��8�η���ָ��
  for(i=0;i<8;i++)
  {
    //ʱ��������
    T_SCK_L;
    //�ж�ָ��λ��ƽ
    if(command & 0x80)T_MOSI_H;
    else T_MOSI_L;
    //��ʱ200ns���������ݽ���
    delay_200ns();
    //ʱ��������
    T_SCK_H;
    //��ʱ200ns���������ݶ�ȡ
    delay_200ns();
    //ָ����������
    command <<= 1;
  }
  //�������������
  T_MOSI_L;
  //����ʱ����
  T_SCK_L;
  //��ʱ120us�����ڵȴ�ADCת������
  delay_us(120);
  //�ճ�һ��������
  T_SCK_L;
  delay_200ns();
  T_SCK_H;
  delay_200ns();
  //ѭ��12�ν�������
  for(i=0;i<12;i++)
  {
    //ָ����������
    data <<= 1;
    //ʱ��������
    T_SCK_L;
    //��ʱ200ns���������ݽ���
    delay_200ns();
    //ʱ��������
    T_SCK_H;
    //��ȡ����λ��ƽ
    if(T_MISO) 
    {
      data |= 0x1;
    }
    //��ʱ200ns���������ݶ�ȡ
    delay_200ns();
  }
  //Ƭѡ����
  T_CS_H;
  //ʱ��������
  T_SCK_L;
  
  return data;
}

/*****************************************************
*��������  �������������˲�����
*������    ��Touch_Smoothing
*��������  ��void
*��������ֵ��TOUCH_XY
*����      ��
********************************************************/
TOUCH_XY Touch_Smoothing(void)
{
  u8 i,j;
  u16 touch_x[5];
  u16 touch_y[5];
  u16 temp;
  u16 sum_x = 0,sum_y = 0;
  TOUCH_XY mean;
  
  //��ȡʮ����������
  for(i=0;i<5;i++)
  {
    touch_x[i] = Touch_Data(TOUCH_X);
    touch_y[i] = Touch_Data(TOUCH_Y);
  }
  
  //�����ݽ�������
  for(i=1;i<5;i++)
  {
    for(j=0;j<5-i;j++)
    {
      if(touch_x[j]>touch_x[j+1])
      {
        temp = touch_x[j];
        touch_x[j] = touch_x[j+1];
        touch_x[j+1] = temp;
      }
      if(touch_y[j]>touch_y[j+1])
      {
        temp = touch_y[j];
        touch_y[j] = touch_y[j+1];
        touch_y[j+1] = temp;
      }
    }
  }
  
  //ȥ��������С��ȡƽ��ֵ
  for(i=1;i<4;i++)
  {
    sum_x += touch_x[i];
    sum_y += touch_y[i];
  }
  mean.x = sum_x / 3;
  mean.y = sum_y / 3;
  
  return mean;
}

/*****************************************************
*��������  ������У׼��ʮ����ʾ����
*������    ��LCD_Calibration_Cross
*��������  ��u16 x  u16 y  u16 colour
*��������ֵ��void
*����      ��
********************************************************/
void LCD_Calibration_Cross(u16 x,u16 y,u16 colour)
{
  u16 i;
  for(i=x-10;i<x+10;i++)
  {
    LCD_Point(i,y,colour);
  }
  for(i=y-10;i<y+10;i++)
  {
    LCD_Point(x,i,colour);
  }
}

/*****************************************************
*��������  ��LCD�봥����У׼����ϵ����ƫ����
*������    ��LCD_Touch_Calibration
*��������  ��void
*��������ֵ��void
*����      ��
********************************************************/
void LCD_Touch_Calibration(void)
{
  u8 i;
  TOUCH_XY touch_xy[4];//���ڴ��LCDУ׼����ȡ�ĵĴ���������
  TOUCH_XY lcd_xy[4] = {{20,20},{300,20},{20,460},{300,460}};//����У׼��LCD���������
  double temp1,temp2;

THIS:  
  /***************��ȡУ׼��������***************/
  //ͨ���ض���LCD���������ȡ������Ĵ���������
  //LCD��0��У׼���������(20,20)
  //LCD��1��У׼���������(300,20)
  //LCD��2��У׼���������(20,460)
  //LCD��3��У׼���������(300,460)
  for(i=0;i<4;i++)
  {
    //����i������ʮ��
    LCD_Calibration_Cross(lcd_xy[i].x,lcd_xy[i].y,RED);
    //�ȴ�����������
    while(T_PEN);
    //��ʱ10ms�������¶���
    delay_ms(10);
    //��ȡ����������
    touch_xy[i] = Touch_Smoothing(); 
    //�ȴ�̧��
    while(!T_PEN);
    //��������ʮ��
    LCD_Calibration_Cross(lcd_xy[i].x,lcd_xy[i].y,WHITE);
    //��ʱ100ms������̧�𶶶�
    delay_ms(100);
  }

  /***************�жϵ����Ч��***************/
  //���0����1�ľ���ƽ��temp1
  temp1 = (touch_xy[0].x - touch_xy[1].x)*(touch_xy[0].x - touch_xy[1].x) + (touch_xy[0].y - touch_xy[1].y)*(touch_xy[0].y - touch_xy[1].y);
  //���2����3�ľ���ƽ��temp2
  temp2 = (touch_xy[2].x - touch_xy[3].x)*(touch_xy[2].x - touch_xy[3].x) + (touch_xy[2].y - touch_xy[3].y)*(touch_xy[2].y - touch_xy[3].y);
  //�ж��Ƿ�����Χ��
  if(((temp1/temp2)<0.97) || ((temp1/temp2)>1.03))
  {
    //����ȡ������
    goto THIS;
  }
  
  //���0����2�ľ���ƽ��temp1
  temp1 = (touch_xy[0].x - touch_xy[2].x)*(touch_xy[0].x - touch_xy[2].x) + (touch_xy[0].y - touch_xy[2].y)*(touch_xy[0].y - touch_xy[2].y);
  //���1����3�ľ���ƽ��temp2
  temp2 = (touch_xy[1].x - touch_xy[3].x)*(touch_xy[1].x - touch_xy[3].x) + (touch_xy[1].y - touch_xy[3].y)*(touch_xy[1].y - touch_xy[3].y);
  //�ж��Ƿ�����Χ��
  if(((temp1/temp2)<0.97) || ((temp1/temp2)>1.03))
  {
    //����ȡ������
    goto THIS;
  }
  
  //���0����3�ľ���ƽ��temp1
  temp1 = (touch_xy[0].x - touch_xy[3].x)*(touch_xy[0].x - touch_xy[3].x) + (touch_xy[0].y - touch_xy[3].y)*(touch_xy[0].y - touch_xy[3].y);
  //���1����2�ľ���ƽ��temp2
  temp2 = (touch_xy[1].x - touch_xy[2].x)*(touch_xy[1].x - touch_xy[2].x) + (touch_xy[1].y - touch_xy[2].y)*(touch_xy[1].y - touch_xy[2].y);
  //�ж��Ƿ�����Χ��
  if(((temp1/temp2)<0.97) || ((temp1/temp2)>1.03))
  {
    //����ȡ������
    goto THIS;
  }
  
  /**********��ʼ����LCD�ʹ������Ĺ�ϵ��������**********/
  //lcd�ʹ�������xy����ı���ϵ��
  //ȡ������������(LCD�봥����)�����ϵ��(ȡ���Σ���ƽ��ֵ����С���)
  //ע��:ֻ��ԶԽ��ߵĵ�
  //��ʽ: a_x = (LCD_x1 - LED_x2) / (��x1 - ��x2)
  //      c_y = (LCD_y1 - LED_y2) / (��y1 - ��y2)
  //��ע: a_x��c_y������ϵ��
  //      LCD_x1��LCD_y1��LCD��һ������������
  //      LCD_x2��LCD_y2��LCD�ڶ�������������
  //      ��x1����y1����������һ������������
  //      ��x2����y2���������ڶ�������������
  LCD_Touch.a_x = (300.0 - 20.0) / (touch_xy[3].x - touch_xy[0].x);
  LCD_Touch.a_x += (20.0 - 300.0)/(touch_xy[2].x - touch_xy[1].x);
  LCD_Touch.a_x /= 2;
  LCD_Touch.c_y = (460.0 - 20.0) / (touch_xy[3].y - touch_xy[0].y);
  LCD_Touch.c_y += (460.0 - 20.0) / (touch_xy[2].y - touch_xy[1].y);
  LCD_Touch.c_y /= 2;
  //��ȡlcd�ʹ�������xy�����ƫ����
  //�Ѿ��������ϵ����ֻ��ȡһ�����������(LCD�ʹ�����)�Ϳ�����ƫ����(ȡ���Σ���ƽ��ֵ����С���)
  //��ʽ: b_x = LCD_x - a_x * ��x
  //      d_y = LCD_y - c_y * ��y
  //��ע: b_x��d_y��x��y��ƫ����
  //      a_x��c_y������ϵ��
  //      LCD_x��LCD_y��LCDһ��У׼��������������
  //      ��x����y��������һ��У׼��������������
  LCD_Touch.b_x = 20 - LCD_Touch.a_x * touch_xy[0].x;
  LCD_Touch.b_x += 300 - LCD_Touch.a_x * touch_xy[3].x;
  LCD_Touch.b_x /= 2;
  LCD_Touch.d_y = 20 - LCD_Touch.c_y * touch_xy[0].y;
  LCD_Touch.d_y += 460 - LCD_Touch.c_y * touch_xy[3].y;
  LCD_Touch.d_y /= 2; 
}

/*****************************************************
*��������  ��������������ֵת��ΪLCD����ֵ
*������    ��LCD_Touch_Transition
*��������  ��void
*��������ֵ��void
*����      ��
*        ��ʽ��LCD_x = a_x * ��x + b_x
*              LCD_y = c_y * ��y + d_x
*        ��ע��b_x��d_y��x��y��ƫ����
*              a_x��c_y������ϵ��
********************************************************/
TOUCH_XY LCD_Touch_Transition(void)
{
  TOUCH_XY touch_xy[2];
  TOUCH_XY xy;
  
  //��ȡ����������
  touch_xy[0] = Touch_Smoothing();
  touch_xy[1] = Touch_Smoothing();
  //������������ת��ΪLCD������
  touch_xy[0].x = (u16)(LCD_Touch.a_x * touch_xy[0].x + LCD_Touch.b_x);
  touch_xy[0].y = (u16)(LCD_Touch.c_y * touch_xy[0].y + LCD_Touch.d_y);
  touch_xy[1].x = (u16)(LCD_Touch.a_x * touch_xy[1].x + LCD_Touch.b_x);
  touch_xy[1].y = (u16)(LCD_Touch.c_y * touch_xy[1].y + LCD_Touch.d_y);
  
  //�����˲���ȥը��
  if((abs(touch_xy[0].x - touch_xy[1].x)<8)&&(abs(touch_xy[0].y - touch_xy[1].y)<8))
  {
    xy.x = (touch_xy[0].x + touch_xy[1].x)/2;
    xy.y = (touch_xy[0].y + touch_xy[1].y)/2;
  }
  else
  {
    xy.x = 0xfff;
    xy.y = 0xfff;
  }
  
  return xy;
}


/*****************************************************
*��������  ������������ɨ��
*������    ��LCD_Key_Scan
*��������  ��void
*��������ֵ��TOUCH_XY
*����      ��
********************************************************/
u8 LCD_Key_Scan(u16 x,u16 y,u16 Long,u16 Wide)
{
  TOUCH_XY xy;
  u8 temp = 0;
  static u8 key_flay = 1;
  
  //�ж��Ƿ���
  if((!T_PEN) && key_flay)
  {
    //��ȡ����ֵ��ת��ΪLCDֵ
    xy = LCD_Touch_Transition();
    //����־λ
    key_flay = 0;
    //�ж��Ƿ��ڰ���ͼ�η�Χ��
    if(((x<xy.x) && (xy.x<(x+Long))) && ((y<xy.y) && (xy.y<(y+Wide))))
    {
      temp = 1;
    }
  }
  //̧�������־λ
  if(T_PEN)
  {
    key_flay = 1;
  }
  return temp;
}










