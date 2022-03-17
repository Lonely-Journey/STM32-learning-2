#include "sys.h"

/*****************************************************
*函数功能  ：对通用定时器3进行初始化设置并输出PWM
*函数名    ：TIM3_PWM_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc,u16 ccr2
*描述      ：通过PWM来控制LED0屏亮度
*            PWM输出口 ： PB5   TIM3_CH2  输出通道2
********************************************************/
void TIM3_PWM_Init(u16 arr,u16 psc,u16 ccr2)
{
	GPIO_InitTypeDef   GPIO_InitStruct;               //GPIOx配置结构体
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStruct; //TIMx配置结构体
	TIM_OCInitTypeDef  TIM_OCInitStruct;              //输出通道配置结构体
	
	
	/*时钟使能*/   //GPIOB  TIM3
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	//复用重映射寄存器
	//AFIO时钟使能
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//部分映像,使PB5变成PWM通道2
  GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
	
	/*GPIOx配置*/
  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_5;               //PB5 端口配置
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;		      //复用推挽输出
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;        //IO口速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStruct);                   //根据设定参数初始化PB5
	
	/*TIMx初始化设置*/
  TIM_TimeBaseInitStruct.TIM_Period        = arr;                 //重载值(TIMx_ARR寄存器)
  TIM_TimeBaseInitStruct.TIM_Prescaler     = psc;                 //分频(TIMx_PSC寄存器)
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        //TDTS = Tck_tim(TIMx_CR1寄存器的第9-8位)(基本定时器没有)
  TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;  //向上计数(TIMx_CR1寄存器的第4位)(基本定时器只递增)
  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);                 //初始化TIM3
	
	/*PWM 通道初始化配置*/
  //通道2 输出配置
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;               //配置为PWM模式1(TIMx_CCMR1的OC1M位)
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;   //通道模式：选择为输出模式(TIMx_CCER的CC1E)
  TIM_OCInitStruct.TIM_Pulse = ccr2;	                         //设置比较寄存器(TIMx->CCR2)
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;        //设置有效电平为低电平(TIMx_CCER的CC1P位)
  TIM_OC2Init(TIM3, &TIM_OCInitStruct);	                       //通道2初始化配置
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);            //通道2比较寄存器影子寄存器使能
  
  //使能重装载影子寄存器函数
	TIM_ARRPreloadConfig(TIM3, ENABLE);			                     // 使能TIM3重载寄存器ARR
  
  /*使能定时器3函数 */
	TIM_Cmd(TIM3, ENABLE);                                       //使能定时器3
}	









