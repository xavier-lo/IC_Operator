////**********************************////
//=================//
//本程序适用于
//用于USB485-232 写卡器
//MCU：stc12c5a08s2
//晶振：29.4912MHz 震荡周期0.4069us
//文件：STC_USER.C
//================//
////**********************************////
#include<STC_USER.H>
#include<STC_INIT.H>

sbit LED1=P1^6;
sbit LED2=P1^7;
sbit LED7=P0^1;
sbit LED8=P0^2;
sbit LED9=P0^3;

/*****定义局部字节变量*****/
unsigned char temp,temp1,temp2,verr_temp;
unsigned char xdata send_data[30];
volatile unsigned char xdata send_pc_data[50];
volatile unsigned char send_pc_count=0;
/***/

/*****定义局部位变量*****/

volatile unsigned char xdata received_CARDHEAD_data[28];//接收卡头数据成功
volatile unsigned char receive_cardhead_start=0;//接收卡头数据开始	

volatile unsigned char receive_cardhead_success=0;//接收卡头数据成功标志 	
volatile unsigned char receive_card_state=0x00;//接收卡数据状态
/***/


/*全局变量定义*/
extern unsigned char xdata received_PC_data[48];//接收来自PC数据  
extern unsigned char xdata receive_PC_data[50];//接收来自PC数据  
extern bit time_200ms_ready;
extern unsigned char xdata receive_CARDHEAD_data[30];//接受来自卡头数据	
extern bit receive_pc_success;//接收PC数据成功标志	
/**/

//******************************//
//name:action
//discription:执行函数
//in:
//out:
void action(void)
{
	if(receive_pc_success)
	{
		receive_pc_success=0;
		switch(received_PC_data[1])
		{
			case 0x00:	Seek_Card();
					break;	
			case 0x02:	Read_2block_Card_nopass(received_PC_data[2]);
					break;
			case 0x12:	Write_2block_Card_nopass(received_PC_data[2],&received_PC_data);
					break;
			case 0x05:	Change_Password(received_PC_data[2],&received_PC_data);
					break;
			case 0x22:	Change_CardHead_Password(&received_PC_data);
					break;
			case 0x03:	Read_1block_Card_havepass(received_PC_data[2],&received_PC_data);
					break;	
			case 0x04:	Write_1block_Card_havepass(received_PC_data[2],&received_PC_data);
					break;
			case 0x01:	Read_1block_Card_nopass(received_PC_data[2]);
					break;
			case 0x11:	Write_1block_Card_nopass(received_PC_data[2],&received_PC_data);
					break;
			case 0x06:
					break;
			default:	break;
		}
		/*
		if(received_PC_data[1]==0x00)
		{
			Seek_Card();
		}else if(received_PC_data[1]==0x02)
		{
			Read_2block_Card_nopass(received_PC_data[2]);
		}else if(received_PC_data[1]==0x12)
		{
			Write_2block_Card_nopass(received_PC_data[2],&received_PC_data);
		}else if(received_PC_data[1]==0x05)
		{
				Change_Password(received_PC_data[2],&received_PC_data);
		}else if(received_PC_data[1]==0x22)
		{
				Change_CardHead_Password(&received_PC_data);
		}else if(received_PC_data[1]==0x03)
		{
				Read_1block_Card_havepass(received_PC_data[2],&received_PC_data);
		}else if(received_PC_data[1]==0x04)
		{
				Write_1block_Card_havepass(received_PC_data[2],&received_PC_data);
		}else if(received_PC_data[1]==0x01)
		{
				Read_1block_Card_nopass(received_PC_data[2]);
		}else if(received_PC_data[1]==0x11)
		{
				Write_1block_Card_nopass(received_PC_data[2],&received_PC_data);
		}
*/
	}

}
//***//
//******************************//
//name:init
//discription:初始化函数
//in:
//out:
void init(void)
{
	init_uart(UART_1,MOD1,BTR_TM1,NOSMOD,BTR_19200);
	init_uart(UART_2,MOD1,BTR_BTR,NOSMOD,BTR_19200);
	init_time(TIME_0,WIDE16,t_20ms);
	
}
//***//
//***//


//******************************//
//name:PC_SeekCard
//discription:寻卡
//in:
//out:
void Seek_Card(void)
{
			cardhead_seek();
			receive_card_state=0;			
			while(receive_card_state==0x00);
			if(received_CARDHEAD_data[1]==0x20)	
			{
					char i=0;
					send_pc_data[0]=0xbe;
					send_pc_data[1]=0x08;
					send_pc_data[2]=0x00;
					send_pc_data[3]=0x00;
					while(i<4)
					{
							send_pc_data[4+i]=received_CARDHEAD_data[2+i];
							i++;
					}
					i=2;
					send_pc_data[8]=send_pc_data[1];
					while(i<8)
					send_pc_data[8]^=send_pc_data[i++];
					send_pc_data[9]=0xe0;	
//					REN=0;
//					send_pc_count=0;
//					SBUF=send_pc_data[send_pc_count++];
					LED1=1;
					send_to_pc(&send_pc_data);					
			}
				else if(received_CARDHEAD_data[1]==0xdf)	
			{
					char i=0;
					send_pc_data[0]=0xbe;
					send_pc_data[1]=0x08;
					send_pc_data[2]=0x80;
					send_pc_data[3]=0x00;
					while(i<4)
					{
							send_pc_data[4+i]=00;
							i++;
					}
					i=2;
					send_pc_data[8]=send_pc_data[1];
					while(i<8)
					send_pc_data[8]^=send_pc_data[i++];
					send_pc_data[9]=0xe0;	
//					REN=0;
//					send_pc_count=0;
//					SBUF=send_pc_data[send_pc_count++];
					LED1=0;
					send_to_pc(&send_pc_data);					
			}			
}
//***//




//******************************//
//name:Read_1block_Card_nopass
//discription:读1块数据 A密地址
//in:
//out:
void Read_1block_Card_nopass(unsigned char block)
{
			char i=0;
			cardhead_seek();
			receive_card_state=0;			
			while(receive_card_state==0x00);
			if(received_CARDHEAD_data[1]==0x20)	
			{	
				cardhead_read(block);	
				receive_card_state=0;		
				while(receive_card_state==0x00);
				if(received_CARDHEAD_data[1]==0x21) 
				{
						send_pc_data[0]=0xbe;
						send_pc_data[1]=0x14;
						send_pc_data[2]=0x01;
						send_pc_data[3]=block;
						i=0;
						while(i<16)
						{
								send_pc_data[4+i]=received_CARDHEAD_data[2+i];
								i++;
						}
						i=2;
						send_pc_data[20]=send_pc_data[1];
						while(i<20)
						send_pc_data[20]^=send_pc_data[i++];
						send_pc_data[21]=0xe0;	
//								REN=0;
//								send_pc_count=0;
//								SBUF=send_pc_data[send_pc_count++];
						LED1=1;
						send_to_pc(&send_pc_data);
						return;								

				}
					send_pc_data[0]=0xbe;
					send_pc_data[1]=0x14;
					send_pc_data[2]=0x81;
					send_pc_data[3]=block;
					i=0;
					while(i<16)
					{
							send_pc_data[4+i]=0;
							i++;
					}
					i=2;
					send_pc_data[20]=send_pc_data[1];
					while(i<20)
					send_pc_data[20]^=send_pc_data[i++];
					send_pc_data[21]=0xe0;
//					REN=0;
//					send_pc_count=0;
//					SBUF=send_pc_data[send_pc_count++];
					LED1=0;			
					send_to_pc(&send_pc_data);					
			}
				else if(received_CARDHEAD_data[1]==0xdf)	
			{					
					send_pc_data[0]=0xbe;
					send_pc_data[1]=0x14;
					send_pc_data[2]=0x81;
					send_pc_data[3]=block;
					i=0;
					while(i<16)
					{
							send_pc_data[4+i]=0;
							i++;
					}
					i=2;
					send_pc_data[20]=send_pc_data[1];
					while(i<20)
					send_pc_data[20]^=send_pc_data[i++];
					send_pc_data[21]=0xe0;
//					REN=0;
//					send_pc_count=0;
//					SBUF=send_pc_data[send_pc_count++];
					LED1=0;
					send_to_pc(&send_pc_data);
			}	
}
//***//

//******************************//
//name:Write_1block_Card_nopass
//discription:写一块数据 A密地址
//in:
//out:
void Write_1block_Card_nopass(unsigned char block,unsigned char *indata0)
{
			char i=0;
			unsigned char in[16];
			cardhead_seek();
			receive_card_state=0;			
			while(receive_card_state==0x00);
			if(received_CARDHEAD_data[1]==0x20)	
			{	
				i=0;
				while(i<16)
				{
						in[i]=indata0[3+i];
						i++;
				}					
				cardhead_write(block,&in);	
				receive_card_state=0;		
				while(receive_card_state==0x00);
				if(received_CARDHEAD_data[1]==0x22) 
				{
						cardhead_read(block);	
						receive_card_state=0;		
						while(receive_card_state==0x00);
						if(received_CARDHEAD_data[1]==0x21) 
						{
								send_pc_data[0]=0xbe;
								send_pc_data[1]=0x14;
								send_pc_data[2]=0x11;
								send_pc_data[3]=block;
								i=0;
								while(i<16)
								{
										send_pc_data[4+i]=received_CARDHEAD_data[2+i];
										i++;
								}
								i=2;
								send_pc_data[20]=send_pc_data[1];
								while(i<20)
									send_pc_data[20]^=send_pc_data[i++];
								send_pc_data[21]=0xe0;	
								i=0;
								while(i<16)
								{
										if(in[i]==send_pc_data[4+i]) i++;
										else break;
								}
								if(i==16)
								{
		//								REN=0;
		//								send_pc_count=0;
		//								SBUF=send_pc_data[send_pc_count++];
										LED1=1;
										send_to_pc(&send_pc_data);
										return;									
								}									
						}
				}
					send_pc_data[0]=0xbe;
					send_pc_data[1]=0x14;
					send_pc_data[2]=0x91;
					send_pc_data[3]=block;
					i=0;
					while(i<16)
					{
							send_pc_data[4+i]=0;
							i++;
					}
					i=2;
					send_pc_data[20]=send_pc_data[1];
					while(i<20)
					send_pc_data[20]^=send_pc_data[i++];
					send_pc_data[21]=0xe0;
//					REN=0;
//					send_pc_count=0;
//					SBUF=send_pc_data[send_pc_count++];
					LED1=0;			
					send_to_pc(&send_pc_data);					
			}
				else if(received_CARDHEAD_data[1]==0xdf)	
			{					
					send_pc_data[0]=0xbe;
					send_pc_data[1]=0x14;
					send_pc_data[2]=0x91;
					send_pc_data[3]=block;
					i=0;
					while(i<16)
					{
							send_pc_data[4+i]=0;
							i++;
					}
					i=2;
					send_pc_data[20]=send_pc_data[1];
					while(i<20)
					send_pc_data[20]^=send_pc_data[i++];
					send_pc_data[21]=0xe0;
//					REN=0;
//					send_pc_count=0;
//					SBUF=send_pc_data[send_pc_count++];
					LED1=0;
					send_to_pc(&send_pc_data);
			}	
}
//***//


//******************************//
//name:Read_2block_Card_nopass
//discription:读卡 使用A密地址
//in:
//out:
void Read_2block_Card_nopass(unsigned char block)
{
			char i=0;
			cardhead_seek();
			receive_card_state=0;			
			while(receive_card_state==0x00);
			if(received_CARDHEAD_data[1]==0x20)	
			{				
				cardhead_read(block);	
				receive_card_state=0;		
				while(receive_card_state==0x00);
				if(received_CARDHEAD_data[1]==0x21) 
				{
						send_pc_data[0]=0xbe;
						send_pc_data[1]=0x24;
						send_pc_data[2]=0x02;
						send_pc_data[3]=block;
						i=0;
						while(i<16)
						{
								send_pc_data[4+i]=received_CARDHEAD_data[2+i];
								i++;
						}
						cardhead_read(block+1);	
						receive_card_state=0;		
						while(receive_card_state==0x00);
						if(received_CARDHEAD_data[1]==0x21) 
						{
								i=0;
								while(i<16)
								{
										send_pc_data[20+i]=received_CARDHEAD_data[2+i];
										i++;
								}	
								i=2;
								send_pc_data[36]=send_pc_data[1];
								while(i<36)
								send_pc_data[36]^=send_pc_data[i++];
								send_pc_data[37]=0xe0;	
//								REN=0;
//								send_pc_count=0;
//								SBUF=send_pc_data[send_pc_count++];
								LED1=1;
								send_to_pc(&send_pc_data);
								return;								
						}	
				}
					send_pc_data[0]=0xbe;
					send_pc_data[1]=0x24;
					send_pc_data[2]=0x82;
					send_pc_data[3]=block;
					i=0;
					while(i<32)
					{
							send_pc_data[4+i]=00;
							i++;
					}
					i=2;
					send_pc_data[36]=send_pc_data[1];
					while(i<36)
					send_pc_data[36]^=send_pc_data[i++];
					send_pc_data[37]=0xe0;	
//					REN=0;
//					send_pc_count=0;
//					SBUF=send_pc_data[send_pc_count++];
					LED1=0;			
					send_to_pc(&send_pc_data);					
			}
				else if(received_CARDHEAD_data[1]==0xdf)	
			{					
					send_pc_data[0]=0xbe;
					send_pc_data[1]=0x24;
					send_pc_data[2]=0x82;
					send_pc_data[3]=block;
					i=0;
					while(i<32)
					{
							send_pc_data[4+i]=00;
							i++;
					}
					i=2;
					send_pc_data[36]=send_pc_data[1];
					while(i<36)
					send_pc_data[36]^=send_pc_data[i++];
					send_pc_data[37]=0xe0;	
//					REN=0;
//					send_pc_count=0;
//					SBUF=send_pc_data[send_pc_count++];
					LED1=0;
					send_to_pc(&send_pc_data);
			}			
}
//***//


//******************************//
//name:Write_2block_Card_nopass
//discription:写卡 使用A密地址
//in:
//out:
void Write_2block_Card_nopass(unsigned char block,unsigned char *indata)
{
			char i=0;
			unsigned char in[16];
			cardhead_seek();
			receive_card_state=0;			
			while(receive_card_state==0x00);
			if(received_CARDHEAD_data[1]==0x20)	
			{	
				i=0;
				while(i<16)
				{
					in[i]=indata[3+i];
					i++;
				}
				cardhead_write(block,&in);	
				receive_card_state=0;		
				while(receive_card_state==0x00);
				if(received_CARDHEAD_data[1]==0x22) 
				{
						i=0;
						while(i<16)
						{
							in[i]=indata[19+i];
							i++;
						}
						cardhead_write(block+1,&in);	
						receive_card_state=0;		
						while(receive_card_state==0x00);
						if(received_CARDHEAD_data[1]==0x22) 
						{
								cardhead_read(block);	
								receive_card_state=0;		
								while(receive_card_state==0x00);
								if(received_CARDHEAD_data[1]==0x21) 
								{
										send_pc_data[0]=0xbe;
										send_pc_data[1]=0x24;
										send_pc_data[2]=0x12;
										send_pc_data[3]=block;
										i=0;
										while(i<16)
										{
												send_pc_data[4+i]=received_CARDHEAD_data[2+i];
												i++;
										}
										cardhead_read(block+1);	
										receive_card_state=0;		
										while(receive_card_state==0x00);
										if(received_CARDHEAD_data[1]==0x21) 
										{
												i=0;
												while(i<16)
												{
														send_pc_data[20+i]=received_CARDHEAD_data[2+i];
														i++;
												}	
												i=2;
												send_pc_data[36]=send_pc_data[1];
												while(i<36)
												send_pc_data[36]^=send_pc_data[i++];
												send_pc_data[37]=0xe0;	
												i=0;
												while(i<32)
												{
														if(send_pc_data[4+i]==indata[3+i]) i++;
														else break;															
												}
												if(i==32)
												{
//														REN=0;
//														send_pc_count=0;
//														SBUF=send_pc_data[send_pc_count++];
														LED1=1;
														send_to_pc(&send_pc_data);
														return;														
												}							
										}	
								}							
						}

				}
					send_pc_data[0]=0xbe;
					send_pc_data[1]=0x24;
					send_pc_data[2]=0x92;
					send_pc_data[3]=block;
					i=0;
					while(i<32)
					{
							send_pc_data[4+i]=00;
							i++;
					}
					i=2;
					send_pc_data[36]=send_pc_data[1];
					while(i<36)
					send_pc_data[36]^=send_pc_data[i++];
					send_pc_data[37]=0xe0;	
//					REN=0;
//					send_pc_count=0;
//					SBUF=send_pc_data[send_pc_count++];
					LED1=0;
					send_to_pc(&send_pc_data);					
			}
				else if(received_CARDHEAD_data[1]==0xdf)	
			{					
					send_pc_data[0]=0xbe;
					send_pc_data[1]=0x24;
					send_pc_data[2]=0x92;
					send_pc_data[3]=block;
					i=0;
					while(i<32)
					{
							send_pc_data[4+i]=00;
							i++;
					}
					i=2;
					send_pc_data[36]=send_pc_data[1];
					while(i<36)
					send_pc_data[36]^=send_pc_data[i++];
					send_pc_data[37]=0xe0;	
//					REN=0;
//					send_pc_count=0;
//					SBUF=send_pc_data[send_pc_count++];
					LED1=0;
					send_to_pc(&send_pc_data);
			}			
}
//***//

//******************************//
//name:Change_Password
//discription:更改密码 A密
//in:
//out:
void Change_Password(unsigned char block,unsigned char *inpass)
{
			char i=0;
			unsigned char oldpass[6];
			unsigned char newpass[6];
			cardhead_seek();
			receive_card_state=0;			
			while(receive_card_state==0x00);
			if(received_CARDHEAD_data[1]==0x20)	
			{	
				i=0;
				while(i<6)
				{
						oldpass[i]=inpass[3+i];
						i++;
				}
				i=0;
				while(i<6)
				{
						newpass[i]=inpass[9+i];
						i++;
				}				
				cardhead_writepass(block,&oldpass,&newpass);	
				receive_card_state=0;		
				while(receive_card_state==0x00);
				if(received_CARDHEAD_data[1]==0x22) 
				{
						send_pc_data[0]=0xbe;
						send_pc_data[1]=0x04;
						send_pc_data[2]=0x05;
						send_pc_data[3]=block;
						i=2;
						send_pc_data[4]=send_pc_data[1];
						while(i<4)
						send_pc_data[4]^=send_pc_data[i++];
						send_pc_data[5]=0xe0;	
//								REN=0;
//								send_pc_count=0;
//								SBUF=send_pc_data[send_pc_count++];
						LED1=1;
						send_to_pc(&send_pc_data);
						return;								

				}
					send_pc_data[0]=0xbe;
					send_pc_data[1]=0x04;
					send_pc_data[2]=0x85;
					send_pc_data[3]=block;
					i=2;
					send_pc_data[4]=send_pc_data[1];
					while(i<4)
					send_pc_data[4]^=send_pc_data[i++];
					send_pc_data[5]=0xe0;	
//					REN=0;
//					send_pc_count=0;
//					SBUF=send_pc_data[send_pc_count++];
					LED1=0;			
					send_to_pc(&send_pc_data);					
			}
				else if(received_CARDHEAD_data[1]==0xdf)	
			{					
					send_pc_data[0]=0xbe;
					send_pc_data[1]=0x04;
					send_pc_data[2]=0x85;
					send_pc_data[3]=block;
					i=2;
					send_pc_data[4]=send_pc_data[1];
					while(i<4)
					send_pc_data[4]^=send_pc_data[i++];
					send_pc_data[5]=0xe0;	
//					REN=0;
//					send_pc_count=0;
//					SBUF=send_pc_data[send_pc_count++];
					LED1=0;
					send_to_pc(&send_pc_data);
			}			
}
//***//

//******************************//
//name:Change_CardHead_Password
//discription:更改卡头密码 A密
//in:
//out:
void Change_CardHead_Password(unsigned char *inpass)
{
				char i=0;
				unsigned char newpass[6];
				i=0;
				while(i<6)
				{
						newpass[i]=inpass[4+i];
						i++;
				}
				cardhead_writecardheadpass(inpass[2],inpass[3],&newpass);	
				receive_card_state=0;		
				while(receive_card_state==0x00);
				if(received_CARDHEAD_data[1]==0x2B) 
				{
						send_pc_data[0]=0xbe;
						send_pc_data[1]=0x0b;
						send_pc_data[2]=0x22;
						send_pc_data[3]=inpass[2];
						send_pc_data[4]=inpass[3];
						i=0;
						while(i<6)
						{
								send_pc_data[5+i]=inpass[4+i];
								i++;
						}
						i=2;
						send_pc_data[11]=send_pc_data[1];
						while(i<11)
						send_pc_data[11]^=send_pc_data[i++];
						send_pc_data[12]=0xe0;	
//								REN=0;
//								send_pc_count=0;
//								SBUF=send_pc_data[send_pc_count++];
						LED1=1;
						send_to_pc(&send_pc_data);
						return;								

				}
					send_pc_data[0]=0xbe;
					send_pc_data[1]=0x0b;
					send_pc_data[2]=0xa2;
					send_pc_data[3]=inpass[2];
					send_pc_data[4]=inpass[3];
					i=0;
					while(i<6)
					{
							send_pc_data[5+i]=inpass[4+i];
							i++;
					}
					i=2;
					send_pc_data[11]=send_pc_data[1];
					while(i<11)
					send_pc_data[11]^=send_pc_data[i++];
					send_pc_data[12]=0xe0;	
//					REN=0;
//					send_pc_count=0;
//					SBUF=send_pc_data[send_pc_count++];
					LED1=0;			
					send_to_pc(&send_pc_data);					

}
//***//

//******************************//
//name:Read_1block_Card_havepass
//discription:读1块数据 A密
//in:
//out:
void Read_1block_Card_havepass(unsigned char block,unsigned char *inpass)
{
			char i=0;
			unsigned char pass[6];
			cardhead_seek();
			receive_card_state=0;			
			while(receive_card_state==0x00);
			if(received_CARDHEAD_data[1]==0x20)	
			{	
				i=0;
				while(i<6)
				{
						pass[i]=inpass[3+i];
						i++;
				}				
				cardhead_read_havepass(block,&pass);	
				receive_card_state=0;		
				while(receive_card_state==0x00);
				if(received_CARDHEAD_data[1]==0x21) 
				{
						send_pc_data[0]=0xbe;
						send_pc_data[1]=0x14;
						send_pc_data[2]=0x03;
						send_pc_data[3]=block;
						i=0;
						while(i<16)
						{
								send_pc_data[4+i]=received_CARDHEAD_data[2+i];
								i++;
						}
						i=2;
						send_pc_data[20]=send_pc_data[1];
						while(i<20)
						send_pc_data[20]^=send_pc_data[i++];
						send_pc_data[21]=0xe0;	
//								REN=0;
//								send_pc_count=0;
//								SBUF=send_pc_data[send_pc_count++];
						LED1=1;
						send_to_pc(&send_pc_data);
						return;								

				}
					send_pc_data[0]=0xbe;
					send_pc_data[1]=0x14;
					send_pc_data[2]=0x83;
					send_pc_data[3]=block;
					i=0;
					while(i<16)
					{
							send_pc_data[4+i]=0;
							i++;
					}
					i=2;
					send_pc_data[20]=send_pc_data[1];
					while(i<20)
					send_pc_data[20]^=send_pc_data[i++];
					send_pc_data[21]=0xe0;
//					REN=0;
//					send_pc_count=0;
//					SBUF=send_pc_data[send_pc_count++];
					LED1=0;			
					send_to_pc(&send_pc_data);					
			}
				else if(received_CARDHEAD_data[1]==0xdf)	
			{					
					send_pc_data[0]=0xbe;
					send_pc_data[1]=0x14;
					send_pc_data[2]=0x83;
					send_pc_data[3]=block;
					i=0;
					while(i<16)
					{
							send_pc_data[4+i]=0;
							i++;
					}
					i=2;
					send_pc_data[20]=send_pc_data[1];
					while(i<20)
					send_pc_data[20]^=send_pc_data[i++];
					send_pc_data[21]=0xe0;
//					REN=0;
//					send_pc_count=0;
//					SBUF=send_pc_data[send_pc_count++];
					LED1=0;
					send_to_pc(&send_pc_data);
			}	
}
//***//

//******************************//
//name:Write_1block_Card_havepass
//discription:写一块数据 A密
//in:
//out:
void Write_1block_Card_havepass(unsigned char block,unsigned char *indata0)
{
			char i=0;
			unsigned char pass[6];
			unsigned char in[16];
			cardhead_seek();
			receive_card_state=0;			
			while(receive_card_state==0x00);
			if(received_CARDHEAD_data[1]==0x20)	
			{	
				i=0;
				while(i<6)
				{
						pass[i]=indata0[3+i];
						i++;
				}	
				i=0;
				while(i<16)
				{
						in[i]=indata0[9+i];
						i++;
				}					
				cardhead_write_havepass(block,&pass,&in);	
				receive_card_state=0;		
				while(receive_card_state==0x00);
				if(received_CARDHEAD_data[1]==0x22) 
				{
						cardhead_read_havepass(block,&pass);	
						receive_card_state=0;		
						while(receive_card_state==0x00);
						if(received_CARDHEAD_data[1]==0x21) 
						{
								send_pc_data[0]=0xbe;
								send_pc_data[1]=0x14;
								send_pc_data[2]=0x04;
								send_pc_data[3]=block;
								i=0;
								while(i<16)
								{
										send_pc_data[4+i]=received_CARDHEAD_data[2+i];
										i++;
								}
								i=2;
								send_pc_data[20]=send_pc_data[1];
								while(i<20)
									send_pc_data[20]^=send_pc_data[i++];
								send_pc_data[21]=0xe0;	
								i=0;
								while(i<16)
								{
										if(in[i]==send_pc_data[4+i]) i++;
										else break;
								}
								if(i==16)
								{
		//								REN=0;
		//								send_pc_count=0;
		//								SBUF=send_pc_data[send_pc_count++];
										LED1=1;
										send_to_pc(&send_pc_data);
										return;									
								}									
						}
				}
					send_pc_data[0]=0xbe;
					send_pc_data[1]=0x14;
					send_pc_data[2]=0x84;
					send_pc_data[3]=block;
					i=0;
					while(i<16)
					{
							send_pc_data[4+i]=0;
							i++;
					}
					i=2;
					send_pc_data[20]=send_pc_data[1];
					while(i<20)
					send_pc_data[20]^=send_pc_data[i++];
					send_pc_data[21]=0xe0;
//					REN=0;
//					send_pc_count=0;
//					SBUF=send_pc_data[send_pc_count++];
					LED1=0;			
					send_to_pc(&send_pc_data);					
			}
				else if(received_CARDHEAD_data[1]==0xdf)	
			{					
					send_pc_data[0]=0xbe;
					send_pc_data[1]=0x14;
					send_pc_data[2]=0x84;
					send_pc_data[3]=block;
					i=0;
					while(i<16)
					{
							send_pc_data[4+i]=0;
							i++;
					}
					i=2;
					send_pc_data[20]=send_pc_data[1];
					while(i<20)
					send_pc_data[20]^=send_pc_data[i++];
					send_pc_data[21]=0xe0;
//					REN=0;
//					send_pc_count=0;
//					SBUF=send_pc_data[send_pc_count++];
					LED1=0;
					send_to_pc(&send_pc_data);
			}	
}
//***//

//******************************//
//name:send_to_pc
//discription:发送至pc数据 uart1
//in:
//out:
void send_to_pc(unsigned char *in)
{
		char i=in[1]+2;
		TI=0;
		REN=0;
		while(i--)
		{
				SBUF=*in++;
				while(!TI);
				TI=0;
		}
		REN=1;	
}
//***//

//******************************//
//name:verr
//discription:校验函数
//in:
//out:
bit verr_recevie_pc(void)
{
	unsigned char i=2;
	temp=receive_PC_data[1];
	while(i<receive_PC_data[1])
	{
		temp^=receive_PC_data[i];
		i++;
	}
	if(temp==receive_PC_data[receive_PC_data[1]])	return 1;
	else return 0;
}
//***//

//******************************//
//name:verr
//discription:校验函数
//in:
//out:
bit verr_recevie_cardhead(void)
{
	unsigned char i=1;
	temp1=receive_CARDHEAD_data[0];
	while(i<(receive_CARDHEAD_data[2]+2))
	{
		temp1^=receive_CARDHEAD_data[i];
		i++;
	}
	if(temp1==(~receive_CARDHEAD_data[receive_CARDHEAD_data[2]+2]))	return 1;
	else return 0;
}
//***//

//******************************//
//name:cardhead_seek
//discription:发送指令到卡头，寻卡
//in:
//out:
void cardhead_seek(void)
{
//	unsigned char send_data[6];
	send_data[0]=0xaa;
	send_data[1]=0x55;
	send_data[2]=0x03;
	send_data[3]=0x20;
	send_data[4]=0x00;
	send_data[5]=0x23;
	send_cmd_to_cardhead(&send_data);
}
//***//

//******************************//
//name:cardhead_read
//discription:发送指令到卡头，读卡
//in:
//out:
void cardhead_read(unsigned char block)
{
	//unsigned char send_data[9];
	unsigned char i=0;
	send_data[0]=0xaa;
	send_data[1]=0x55;
	send_data[2]=0x06;
	send_data[3]=0x21;
	send_data[4]=0x02;
	send_data[5]=block;
	send_data[6]=0x80;
	send_data[7]=0;
	i=3;
	send_data[8]=send_data[2];
	while(i<8)
	send_data[8]^=	send_data[i++];
	send_cmd_to_cardhead(&send_data);
}
//***//

//******************************//
//name:cardhead_read_havepass
//discription:发送指令到卡头，读卡
//in:
//out:
void cardhead_read_havepass(unsigned char block,unsigned char *pass)
{
	//unsigned char send_data[9];
	unsigned char i=0;
	send_data[0]=0xaa;
	send_data[1]=0x55;
	send_data[2]=0x0a;
	send_data[3]=0x21;
	send_data[4]=0x00;
	send_data[5]=block;
	i=0;
	while(i<6)
			send_data[6+(i++)]=*pass++;
	i=3;
	send_data[12]=send_data[2];
	while(i<12)
	send_data[12]^=	send_data[i++];
	send_cmd_to_cardhead(&send_data);
}
//***//

//******************************//
//name:cardhead_write
//discription:发送指令到卡头，写卡
//in:
//out:
void cardhead_write(unsigned char block,unsigned char *indata11)
{
	//unsigned char send_data[25];
	unsigned char i=0;
	send_data[0]=0xaa;
	send_data[1]=0x55;
	send_data[2]=0x16;
	send_data[3]=0x22;
	send_data[4]=0x02;
	send_data[5]=block;
	send_data[6]=0x80;
	send_data[7]=0;	
	i=0;
	while(i<16)
	{
			send_data[8+i]=indata11[i];
			i++;
	}
	i=3;
	send_data[24]=send_data[2];
	while(i<24)
		send_data[24]^=	send_data[i++];	
	send_cmd_to_cardhead(&send_data);
}
//***//

//******************************//
//name:cardhead_write_havepass
//discription:发送指令到卡头，写卡
//in:
//out:
void cardhead_write_havepass(unsigned char block,unsigned char *pass,unsigned char *indata)
{
	//unsigned char send_data[25];
	unsigned char i=0;
	send_data[0]=0xaa;
	send_data[1]=0x55;
	send_data[2]=0x1a;
	send_data[3]=0x22;
	send_data[4]=0x00;
	send_data[5]=block;
	i=0;
	while(i<6)
			send_data[6+(i++)]=*pass++;

	i=0;
	while(i<16)
			send_data[12+(i++)]=*indata++;
	i=3;
	send_data[28]=send_data[2];
	while(i<28)
	send_data[28]^=	send_data[i++];
	send_cmd_to_cardhead(&send_data);
}
//***//

//******************************//
//name:cardhead_writepass
//discription:发送指令到卡头，写卡密码
//in:
//out:
void cardhead_writepass(unsigned char block,unsigned char *oldpass,unsigned char *newpass)
{
	//unsigned char send_data[29];
	unsigned char i=0;
	send_data[0]=0xaa;
	send_data[1]=0x55;
	send_data[2]=0x1a;
	send_data[3]=0x22;
	send_data[4]=0x00;
	send_data[5]=block;
	i=0;
	while(i<6)
		send_data[6+(i++)]=	*oldpass++;
	i=0;
	while(i<6)
		send_data[12+(i++)]= *newpass++;
		
	send_data[18]=0xff;
	send_data[19]=0x07;
	send_data[20]=0x80;
	send_data[21]=0x69;
	
	send_data[22]=0xff;
	send_data[23]=0xff;
	send_data[24]=0xff;
	send_data[25]=0xff;
	send_data[26]=0xff;
	send_data[27]=0xff;

	i=3;
	send_data[28]=send_data[2];
	while(i<28)
	send_data[28]^=	send_data[i++];
	send_cmd_to_cardhead(&send_data);
}
//***//

//******************************//
//name:cardhead_write_havepass
//discription:发送指令到卡头，写卡
//in:
//out:
void cardhead_writecardheadpass(unsigned char addrl,unsigned char addrh,unsigned char *pass)
{
	//unsigned char send_data[25];
	unsigned char i=0;
	send_data[0]=0xaa;
	send_data[1]=0x55;
	send_data[2]=0x0a;
	send_data[3]=0x2b;
	send_data[4]=addrl;//0x80
	send_data[5]=addrh;//0x00
	i=0;
	while(i<6)
			send_data[6+(i++)]=*pass++;
	i=3;
	send_data[12]=send_data[2];
	while(i<12)
	send_data[12]^=	send_data[i++];
	send_cmd_to_cardhead(&send_data);
}
//***//

//******************************//
//name:cardhead_OPENorCLOSE
//discription:发送指令到卡头
//in:
//out:
void cardhead_OPENorCLOSE(unsigned char oc)
{
	//unsigned char send_data[6];
	unsigned char i=0;
	send_data[0]=0xaa;
	send_data[1]=0x55;
	send_data[2]=0x03;
	send_data[3]=0x11;
	send_data[4]=oc;
	i=3;
	send_data[5]=send_data[2];
	while(i<5)
	send_data[5]^=	send_data[i++];
//	send_data[8]=~send_data[8];
	send_cmd_to_cardhead(&send_data);
}
//***//

//******************************//
//name:send_cmd_to_cardhead
//discription:发送指令到卡头
//in:
//out:
void send_cmd_to_cardhead(unsigned char *in)
{
	unsigned char i=0;
	S2CON&=0xef;
	S2CON&=0xfd;
	while(i<(in[2]+3))
	{		
		S2BUF=in[i++];
		while((S2CON&0x02)!=0x02);
		S2CON&=0xfd;
		LED8=!LED8;
	}
	S2CON&=0xfc;
	S2CON|=0x10;
}
//***//


//******************************//
//name:verr
//discription:校验函数
//in:
//out:
//bit SAMPLE(void)
//{

//}
//***//



