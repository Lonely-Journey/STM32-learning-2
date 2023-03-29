#include "stm32f10x.h"
#include "touch.h"
#include "delay.h"
#include "lcd.h"
#include "stdio.h"

//200ns��ʱ����
static void delay_200ns(void)
{
  u8 i = 20;
  while(i) i--;
}

ADJUST LCD_Touch;//���ڴ��LCD�봥������֮��ı���ϵ����ƫ����(Ҳ����У׼ϵ��)

/*****************************************************
*��������  ���Դ�����оƬ��Ӧ��GPIO�ڽ��г�ʼ������
*������    ��Touch_Init
*��������  ��void
*��������ֵ��void
*����      ��
*            CS     PF11  ͨ�����
*            SCK    PB1   ͨ�����
*            PEN    PF10  ͨ������
*            MISO   PB2   ͨ������
*            MOSI   PF9   ͨ�����
********************************************************/
void Touch_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;     //GPIOx���ýṹ��
	
	//ʱ��ʹ��   GPIOB   GPIOF
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	
	/*GPIOx��ʼ������*/
  //GPIOx�˿�����
	/*���������*/
  //PB1
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;				     //PB1 �˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStruct);					       //�����趨������ʼ��PB1
	//PF11��PF9
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11;				     //PF9��PF11�˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOF, &GPIO_InitStruct);					       //�����趨������ʼ��PF9��PF11
	/*���������*/
	//PB2
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;				     //PB2�˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU ;        //���������ģʽ 
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStruct);					       //�����趨������ʼ��PB1
	//PF10
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;				     //PF10�˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU ;        //���������ģʽ 
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOF, &GPIO_InitStruct);					       //�����趨������ʼ��PF10
	
	//��ʼ��״̬����
  T_CS_H;
  T_SCK_L;
  T_MOSI_L;
}



/*****************************************************
*��������  ��ͨ��������оƬ��ȡ��������������
*������    ��Touch_Data
*��������  ��u16
*��������ֵ��u8 command 
*����      ��
*       Ƭѡ��        T_CS_H       T_CS_L  
*       ʱ����        T_SCK_H      T_SCK_L  
*       ���������    T_MOSI_H     T_MOSI_L
*       ����������    T_MISO 
*       �ʽӴ���      T_PEN    
********************************************************/
u16 Touch_Data(u8 command)
{
  u8 i;
  u16 data = 0;
  
  //����������
  T_MOSI_L;
  //Ƭѡ������
  T_CS_L;
  //ѭ��8�η���ָ������
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
  //ʱ��������
  T_SCK_L;
  //����������
  T_MOSI_L;
  //��ʱ120us�����ڵȴ�ADC������ת����
  delay_us(120);
  //�ճ�һ��������
  T_SCK_L;
  delay_200ns();
  T_SCK_H;
  delay_200ns();
  //ѭ��12�ζ�ȡ����
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
    if(T_MISO) data |= 0x1;
    //��ʱ200ns�����ڶ�ȡ����
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
  u16 touch_x[10];
  u16 touch_y[10];
  u16 temp;
  u16 sum_x = 0,sum_y = 0;
  TOUCH_XY mean;
  
  //��ȡʮ����������
  for(i=0;i<10;i++)
  {
    touch_x[i] = Touch_Data(TOUCH_X);
    touch_y[i] = Touch_Data(TOUCH_Y);
  }
  
  //�����ݽ�������
  for(i=1;i<10;i++)
  {
    for(j=0;j<10-i;j++)
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
  for(i=1;i<9;i++)
  {
    sum_x += touch_x[i];
    sum_y += touch_y[i];
  }
  mean.x = sum_x / 8;
  mean.y = sum_y / 8;
  
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
*��������  ��LCD�봥����У׼
*������    ��LCD_Touch_Calibration
*��������  ��void
*��������ֵ��void
*����      ��
********************************************************/
void LCD_Touch_Calibration(void)
{
  u8 i;
  TOUCH_XY touch_xy[4];
  TOUCH_XY lcd_xy[4] = {{20,20},{220,20},{20,300},{220,300}};//����У׼��LCD���������
  double temp1,temp2;

THIS:  
  /***************��ȡУ׼��������***************/
  //ͨ���ض���LCD���������ȡ������Ĵ���������
  //LCD��0��У׼���������(20,20)
  //LCD��1��У׼���������(220,20)
  //LCD��2��У׼���������(20,300)
  //LCD��3��У׼���������(220,300)
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
    delay_ms(150);
  }
  
  /****************�жϵ����Ч��****************/
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
  
  /***************��ʼ����LCD�ʹ������Ĺ�ϵ��������***************/
  //lcd�ʹ�������xy�����ƫ����
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
  LCD_Touch.a_x = (220.0 - 20.0) / (touch_xy[3].x - touch_xy[0].x);
  LCD_Touch.a_x += (20.0 - 220.0)/(touch_xy[2].x - touch_xy[1].x);
  LCD_Touch.a_x /= 2;
  LCD_Touch.c_y = (300.0 - 20.0) / (touch_xy[3].y - touch_xy[0].y);
  LCD_Touch.c_y += (300.0 - 20.0) / (touch_xy[2].y - touch_xy[1].y);
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
  LCD_Touch.b_x += 220 - LCD_Touch.a_x * touch_xy[3].x;
  LCD_Touch.b_x /= 2;
  LCD_Touch.d_y = 20 - LCD_Touch.c_y * touch_xy[0].y;
  LCD_Touch.d_y += 300 - LCD_Touch.c_y * touch_xy[3].y;
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
  TOUCH_XY touch_xy;
  
  //��ȡ����������
  touch_xy = Touch_Smoothing();
  //������������ת��ΪLCD������
  touch_xy.x = (u16)(LCD_Touch.a_x * touch_xy.x + LCD_Touch.b_x);
  touch_xy.y = (u16)(LCD_Touch.c_y * touch_xy.y + LCD_Touch.d_y);
  
  printf("x:%d    y:%d\r\n",touch_xy.x ,touch_xy.y);
  
  return touch_xy;
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


















