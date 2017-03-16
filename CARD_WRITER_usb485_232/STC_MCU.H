////**********************************////
//=================//
//本程序适用于
//用于USB485-232 写卡器
//MCU：stc12c5a08s2
//晶振：11.0592MHz 震荡周期1.8304us
//文件：STC_MCU.H
//================//
////**********************************////


#ifndef __STC_MCU_H__
#define __STC_MCU_H__

#include<STC12C5A60S2.h>
//******中断定义***********//
typedef enum IRQn
{
/***适用于STC12C60S2**/
	EXTI0	=	0,//外部中断0
	TIME0	=	1,//定时器中断0
	EXTI1	=	2,//外部中断1
	TIME1	=	3,//定时器中断1
	UART	=	4,//串行中断1
	ADC		=	5,//模数转换中断
	LVD		=	6,
	PCA		=	7,
	UART2	=	8,//串行中断2
	SPI		=	9 //SPI中断
} IRQn_Type;
//**********//
//******定时器定义***********//
typedef enum TIM_MOD
{
/***适用于STC12C60S2**/
	WIDE13	=	0,//13位定时计数
	WIDE16	=	1,//16位定时计数
	WIDE8	=	2,//8位定时计数
	WIDE8D	=	3,//双8位定时计数 定时器0
} TIM_MOD_Type;
//*********//

//******串口定义***********//
typedef enum UART_MOD
{
/***适用于STC12C60S2**/	  
	MOD0	=	0,//同步移位寄存方式
	MOD1	=	1,//串行8位，波特率可变
	MOD2	=	2,//串行9位
	MOD3	=	3,//串行9位，波特率可变	   
	BTR_BTR	=	4,//波特率使用独立波特率发生器
	BTR_TM1	=	5,//波特率使用TIME1为波特率发生器
	ISSMOD	=	6,//波特率加倍
	NOSMOD	=	7 //波特率不加倍
} UART_MOD_Type;
//*********//
//******串口定义***********//
typedef enum UART_NAME
{
/***适用于STC12C60S2**/	  
	UART_1	=	1,
	UART_2	=	2
} UART_NAME_Type;
//*********//
//******定时器定义***********//
typedef enum TIME_NAME
{
/***适用于STC12C60S2**/	  
	TIME_0	=	1,
	TIME_1	=	2
} TIME_NAME_Type;
//*********//
//******定时器定义***********//
typedef enum TIM_VALUE
{
/***适用于STC12C60S2**/
	t_1ms 	=	2457,
	t_2ms		= 2457*2,
	t_20ms	=	49152,
	btr_38400	=	 256-2,
	BTR_9600	=	256-3,
	BTR_19200	=	256-4,
	BTR_10k		=	256-13			//29.4912MHz/13/(32*12)=0.005008mbit/s =5.008kbit/s
} TIM_VALUE_Type;
//*********//

//*****IO模式定义***********//
typedef enum GPIO_MOD
{
	GPIO_STANDERD				=	0,//标准双向口
	GPIO_PUSHPULL				=	1,//推挽输出 强上拉输出
	GPIO_HIGHIMPEDENCE	=	2,//高阻输入
	GPIO_OPENDRAIN			=	3//开漏输出 内部上拉断开 需外加			
}GPIO_MOD_Type;
//*******//
//*****IO引脚定义***********//
typedef enum GPIO_PIN
{
	GPIO_PIN_0				=	1,
	GPIO_PIN_1				=	2,
	GPIO_PIN_2				=	4,
	GPIO_PIN_3				=	8,
	GPIO_PIN_4				=	16,
	GPIO_PIN_5				=	32,
	GPIO_PIN_6				=	64,
	GPIO_PIN_7				=	128,
	GPIO_PIN_all			=	255	
}GPIO_PIN_Type;
//*******//
//*****IO名定义***********//
typedef enum GPIO_NAME
{
	GPIO_0				=	1,//P0
	GPIO_1				=	2,//P1
	GPIO_2				=	4,//P2
	GPIO_3				=	8,//P3
	GPIO_4				=	16//P4
}GPIO_NAME_Type;
//*******//

////******定时器定义***********//
//struct TIME_InitTypeDef
//{
//	unsigned char MOD;
//	unsigned int VALUE;	
//};
////*****//	



#endif