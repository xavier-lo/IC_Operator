////**********************************////
//=================//
//������������
//����USB485-232 д����
//MCU��stc12c5a08s2
//����29.4912MHz ������0.4069us
//�ļ���CARD_WRITER_usb485_232.C
//================//
////**********************************////
#include<STC_INIT.H>
#include<STC_ISR.H>
#include<STC_USER.H>
extern bit delay1s_ready;
void main(void)
{	
	//int i=60000;
	init();
	REN=1;
	EA=1;
	//while(i--);	
	while(!delay1s_ready);
	//cardhead_OPENorCLOSE(0x01);
	while(1)
	{
		action();
	}
}