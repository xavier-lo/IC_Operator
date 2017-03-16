////**********************************////
//=================//
//������������
//����USB485-232 д����
//MCU��stc12c5a08s2
//����29.4912MHz ������0.4069us
//�ļ���STC_INIT.H
//================//
////**********************************////
#ifndef __STC_INIT_H__
#define __STC_INIT_H__

#include<STC_MCU.H>
void init_time(char TIME,TIM_MOD_Type MOD,TIM_VALUE_Type VALUE);
void init_uart(char UART,UART_MOD_Type MOD,UART_MOD_Type S_BTR,UART_MOD_Type S_MOD,TIM_VALUE_Type VALUE);
void init_gpio_config(char io,GPIO_MOD_Type io_mod,char io_pin);
#endif