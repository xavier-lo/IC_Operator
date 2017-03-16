////**********************************////
//=================//
//本程序适用于
//用于USB485-232 写卡器
//MCU：stc12c5a08s2
//晶振：29.4912MHz 震荡周期0.4069us
//文件：STC_INIT.C
//================//
////**********************************////
#include<STC_INIT.H>
#include<math.h>
//*************************//
//name:init_time0
//discription:定时器0中断初始化
//in:  TIME	 MOD   VALUE   定时器号 工作模式 定时值
//out:
void init_time(char TIME,TIM_MOD_Type MOD,TIM_VALUE_Type VALUE)
{
/*****
//定时器0
*****/		
	if((TIME&0x01)==TIME_0)
	{
	   if(MOD==WIDE13) TMOD=(TMOD&0xf0)|0x00; 
	   else if(MOD==WIDE16) TMOD=(TMOD&0xf0)|0x01;
	   else if(MOD==WIDE8) TMOD=(TMOD&0xf0)|0x02;
	   else if(MOD==WIDE8D) TMOD=(TMOD&0xf0)|0x03;
	   TH0=(65535-VALUE)/256;
	   TL0=(65535-VALUE)%256;
	   TR0=1;
	   ET0=1;
	}
/*****
//定时器1
*****/		
	if((TIME&0x02)==TIME_1)
	{
	   if(MOD==WIDE13) TMOD=(TMOD&0x0f)|0x00; 
	   else if(MOD==WIDE16) TMOD=(TMOD&0x0f)|0x10;
	   else if(MOD==WIDE8) TMOD=(TMOD&0x0f)|0x20;
	   else if(MOD==WIDE8D) TMOD=(TMOD&0x0f)|0x30;	
	   TH1=(65535-VALUE)/256;
	   TL1=(65535-VALUE)%256;
	   TR1=1;
	   ET1=1;
	}
}
//**//
//*************************//
//name:init_uart
//discription:串行口中断初始化
//in:  S_UART  MOD	S_BTR  S_MOD  VALUE	 串口号 工作模式 选择波特率发生器 是否加倍 波特率值
//out:
void init_uart(char S_UART,UART_MOD_Type MOD,UART_MOD_Type S_BTR,UART_MOD_Type S_MOD,TIM_VALUE_Type VALUE)
{
/*****
//串口1
*****/	
	if((S_UART&0x01)==UART_1)
	{
		switch(MOD)
		{
			case MOD0:
				SM0=0;
				SM1=0;
				break;
			case MOD1:
		  	SM0=0;
				SM1=1;
				break;
			case MOD2:
				SM0=1;
				SM1=0;
				break;
			case MOD3:
				SM0=1;
				SM1=1;
				break;
			default:
				break;		
		}
		if(S_BTR==BTR_BTR)
		{
			if(S_MOD==ISSMOD)
			{
				PCON|=0x80;
				BRT=VALUE;
			}else if(S_MOD==NOSMOD)
			{
				PCON&=0x7f;
				BRT=VALUE;			
			}
			AUXR&=0XE0;
			AUXR|=0X11;	
		}
		else if(S_BTR==BTR_TM1)
		{
			TMOD&=0X0F;
			TMOD|=0X20;
			AUXR&=0Xfe;
			if(S_MOD==ISSMOD)
			{
				PCON|=0x80;
				TH1=VALUE;
				TL1=VALUE;	 
			}else if(S_MOD==NOSMOD)
			{
				PCON&=0x7f;
				TH1=VALUE;
				TL1=VALUE;							
			}
			TR1=1;		
		}
		ES=1;
	
	}
/*****
//串口2
*****/	
	if((S_UART&0x02)==UART_2)
	{
		switch(MOD)
		{
			case MOD0:
				S2CON&=0X3F;
				break;
			case MOD1:
				S2CON&=0X3F;
				S2CON|=0X40;
				break;
			case MOD2:
				S2CON&=0X3F;
				S2CON|=0X80;
				break;
			case MOD3:
				S2CON&=0X3F;
				S2CON|=0XC0;
				break;
			default:
				break;		
		}
		if(S_BTR==BTR_BTR)
		{
			if(S_MOD==ISSMOD)
			{
				AUXR&=0XE3;
				AUXR|=0x18;
				BRT=VALUE;
			}else if(S_MOD==NOSMOD)
			{
				AUXR&=0XE3;
				AUXR|=0x10;
				BRT=VALUE;		
			}
		}
		IE2|=0X01;	
	}	 
}
//**//

//*************************//
//name:init_gpio_config
//discription:IO口模式配置
//in:  GPIO_NAME_Type GPIO_MOD_Type GPIO_PIN_Type IO口 配置模式 引脚选择
//out:
void init_gpio_config(char io,GPIO_MOD_Type io_mod,char io_pin)
{
/*****
//P0
*****/	
		if((io&0x01)==GPIO_0)
		{
			if(io_mod==GPIO_STANDERD)
			{
					P0M0&=(~io_pin);
					P0M1&=(~io_pin);
			}else if(io_mod==GPIO_PUSHPULL)
			{
					P0M0|=io_pin;
					P0M1&=(~io_pin);				
			}else if(io_mod==GPIO_HIGHIMPEDENCE)
			{
					P0M0&=(~io_pin);
					P0M1|=io_pin;				
			}else if(io_mod==GPIO_OPENDRAIN)
			{
					P0M0|=io_pin;
					P0M1|=io_pin;				
			}
		}
/*****
//P1
*****/		
		if((io&0x02)==GPIO_1)
		{
			if(io_mod==GPIO_STANDERD)
			{
					P1M0&=(~io_pin);
					P1M1&=(~io_pin);
			}else if(io_mod==GPIO_PUSHPULL)
			{
					P1M0|=io_pin;
					P1M1&=(~io_pin);				
			}else if(io_mod==GPIO_HIGHIMPEDENCE)
			{
					P1M0&=(~io_pin);
					P1M1|=io_pin;				
			}else if(io_mod==GPIO_OPENDRAIN)
			{
					P1M0|=io_pin;
					P1M1|=io_pin;				
			}			
		}
/*****
//P2
*****/		
		if((io&0x04)==GPIO_2)
		{
			if(io_mod==GPIO_STANDERD)
			{
					P2M0&=(~io_pin);
					P2M1&=(~io_pin);
			}else if(io_mod==GPIO_PUSHPULL)
			{
					P2M0|=io_pin;
					P2M1&=(~io_pin);				
			}else if(io_mod==GPIO_HIGHIMPEDENCE)
			{
					P2M0&=(~io_pin);
					P2M1|=io_pin;				
			}else if(io_mod==GPIO_OPENDRAIN)
			{
					P2M0|=io_pin;
					P2M1|=io_pin;				
			}			
		}
/*****
//P3
*****/		
		if((io&0x08)==GPIO_3)
		{
			if(io_mod==GPIO_STANDERD)
			{
					P3M0&=(~io_pin);
					P3M1&=(~io_pin);
			}else if(io_mod==GPIO_PUSHPULL)
			{
					P3M0|=io_pin;
					P3M1&=(~io_pin);				
			}else if(io_mod==GPIO_HIGHIMPEDENCE)
			{
					P3M0&=(~io_pin);
					P3M1|=io_pin;				
			}else if(io_mod==GPIO_OPENDRAIN)
			{
					P3M0|=io_pin;
					P3M1|=io_pin;				
			}			
		}
/*****
//P4
*****/		
		if((io&0x10)==GPIO_4)
		{
			if(io_mod==GPIO_STANDERD)
			{
					P4M0&=(~io_pin);
					P4M1&=(~io_pin);
			}else if(io_mod==GPIO_PUSHPULL)
			{
					P4M0|=io_pin;
					P4M1&=(~io_pin);				
			}else if(io_mod==GPIO_HIGHIMPEDENCE)
			{
					P4M0&=(~io_pin);
					P4M1|=io_pin;				
			}else if(io_mod==GPIO_OPENDRAIN)
			{
					P4M0|=io_pin;
					P4M1|=io_pin;				
			}			
		}
}
//**//