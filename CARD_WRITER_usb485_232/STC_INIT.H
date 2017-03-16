////**********************************////
//=================//
//本程序适用于
//用于USB485-232 写卡器
//MCU：stc12c5a08s2
//晶振：29.4912MHz 震荡周期0.4069us
//文件：STC_INIT.H
//================//
////**********************************////
#ifndef __STC_INIT_H__
#define __STC_INIT_H__

#include<STC_MCU.H>
void init_time(char TIME,TIM_MOD_Type MOD,TIM_VALUE_Type VALUE);
void init_uart(char UART,UART_MOD_Type MOD,UART_MOD_Type S_BTR,UART_MOD_Type S_MOD,TIM_VALUE_Type VALUE);
void init_gpio_config(char io,GPIO_MOD_Type io_mod,char io_pin);
#endif