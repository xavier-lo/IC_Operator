////**********************************////
//=================//
//������������
//����USB485-232 д����
//MCU��stc12c5a08s2
//����29.4912MHz ������0.4069us
//�ļ���STC_ISR.C
//================//
////**********************************////
#include<STC_ISR.H>
#include<STC_USER.H>
/*�ֲ���������*/
sbit LED1=P1^6;
sbit LED2=P1^7;
sbit LED7=P0^1;
sbit LED8=P0^2;
sbit LED9=P0^3;

unsigned char receive_PC_count=0;//������������PC����  
unsigned char start_receive_pc=0;
volatile unsigned char xdata receive_PC_data[50];//��������PC����  
unsigned char receive_PC_overtime_count=0;//����PC���ݳ�ʱ���� 
unsigned char receive_cardhead_overtime_count=0;//����CARDHEAD���ݳ�ʱ���� 
unsigned char t0_count=0;
volatile bit time_200ms_ready=0;
unsigned char receive_cardhead_count=0;//���տ�ͷ���ݼ���	
volatile bit delay1s_ready=0;
unsigned char delay1s_count=0;		

/**/
/*ȫ�ֱ�������*/
volatile unsigned char xdata received_PC_data[48];//����PCУ��ɹ������� ��ȥ֡ͷ ֡β   	
volatile bit receive_pc_success=0;//����PC���ݳɹ���־	
volatile unsigned char xdata receive_CARDHEAD_data[30];//�������Կ�ͷ����
extern unsigned char receive_cardhead_start;//���տ�ͷ����״̬	
extern unsigned char receive_cardhead_success;//���տ�ͷ���ݳɹ���־ 
extern unsigned char xdata received_CARDHEAD_data[28];//���տ�ͷ���ݳɹ�	
extern unsigned char receive_card_state;//���տ�����״̬	
extern unsigned char xdata send_pc_data[50];
extern unsigned char send_pc_count;
/**/


//*************************//
//name:init_time0
//discription:��ʱ��0�жϳ�ʼ��
//in:
//out:
void TIME0_ISR()interrupt 1 using 1
{
	TF0=0;
	TL0=(65535-t_20ms)%256;
	TH0=(65535-t_20ms)/256;
	delay1s_count++;
	if(delay1s_count>=50) delay1s_ready=1;
	t0_count++;
	if(t0_count>=10)
	{
		t0_count=0;
		time_200ms_ready=1;
		LED2=!LED2;
	}
	if(start_receive_pc)
	{			
		receive_PC_overtime_count++;
		if(receive_PC_overtime_count>=10)
		{
			int i=0;
			start_receive_pc=0;
			for(i=0;i<48;i++)
			{
				received_PC_data[i]=0;
				receive_pc_success=0;
			}
		}
	}
	if(receive_card_state==0)
	{
		receive_cardhead_overtime_count++;
		if(receive_cardhead_overtime_count>=4)
		{
			int i=0;
			receive_cardhead_start=0;
			receive_card_state=0x80;
			receive_cardhead_overtime_count=0;
			for(i=0;i<28;i++)
			{
				received_CARDHEAD_data[i]=0;
				receive_cardhead_success=0;
			}		
		}
	}

}

//******//

//*************************//
//name:USART1_ISR
//discription:
//in:
//out:
void USART1_ISR(void)interrupt 4 using 2
{
	int i=0;
	if(RI)
	{
		RI=0;
		receive_PC_count++;
		receive_PC_overtime_count=0;
		if((start_receive_pc==0)&&(SBUF==0xbe))
		{
			receive_PC_count=0;
			start_receive_pc=1;
			receive_pc_success=0;
			receive_PC_data[receive_PC_count]=SBUF;
		}else if(receive_PC_count==1)
		{
			receive_PC_data[receive_PC_count]=SBUF;
		}else if(receive_PC_count<(receive_PC_data[1]+2))
		{
			receive_PC_data[receive_PC_count]=SBUF;
			if((receive_PC_count==(receive_PC_data[1]+1))&&(receive_PC_data[receive_PC_count]==0xe0))
			{
				if(verr_recevie_pc()==1)
				{LED7=!LED7;
					for(i=0;i<48;i++)
					{
						received_PC_data[i]=receive_PC_data[i+1];
						receive_pc_success=1;
					}
				}else
				{
					int i=0;
					for(i=0;i<48;i++)
					{
						received_PC_data[i]=0;
						receive_pc_success=0;
					}					
				}
				start_receive_pc=0;
			}
		}else
		{
			int i=0;
			start_receive_pc=0;
			for(i=0;i<20;i++)
			{
				received_PC_data[i]=0;
				receive_pc_success=0;
			}			
		}
		
		
	}
//	else if(TI)
//	{
//		TI=0;
//		if(send_pc_count<(send_pc_data[1]+2))
//			SBUF=send_pc_data[send_pc_count++];
//		else REN=1;
//	}
}

//******//
//*************************//
//name:USART1_ISR
//discription:
//in:
//out:
void USART2_ISR(void)interrupt 8 using 3
{
			int i=0;
			if(S2CON&0x01)
			{
				
				S2CON&=0xfe;
				receive_cardhead_count++;
				receive_cardhead_overtime_count=0;
				if((S2BUF==0xaa)&&(receive_cardhead_start==0))
				{
					receive_cardhead_start=1;
					receive_cardhead_count=0;
					receive_cardhead_success=0;
					receive_CARDHEAD_data[receive_cardhead_count]=S2BUF;
				}else if(receive_cardhead_count<3)
				{
					receive_CARDHEAD_data[receive_cardhead_count]=S2BUF;
				}else if(receive_cardhead_count<(receive_CARDHEAD_data[2]+3))
				{
					receive_CARDHEAD_data[receive_cardhead_count]=S2BUF;
					if(receive_cardhead_count==(receive_CARDHEAD_data[2]+2))
					{
						if(verr_recevie_cardhead()==1)
						{
							LED9=!LED9;
							receive_card_state=0x01;
							for(i=0;i<28;i++)
							{
								received_CARDHEAD_data[i]=receive_CARDHEAD_data[i+2];
							}
							receive_cardhead_success=1;
						}else
						{
							receive_card_state=0x02;
							for(i=0;i<28;i++)
							{
								received_CARDHEAD_data[i]=0;
							}
							receive_cardhead_success=0;				
						}
					}
					receive_cardhead_start=0;
				}else
				{
					for(i=0;i<28;i++)
					{
						received_CARDHEAD_data[i]=0;
					}
					receive_card_state=0x04;
					receive_cardhead_start=0;
					receive_cardhead_success=0;	
				}
			}
}

//******//