////**********************************////
//=================//
//������������
//����USB485-232 д����
//MCU��stc12c5a08s2
//����29.4912MHz ������0.4069us
//�ļ���STC_USER.H
//================//
////**********************************////
#ifndef __STC_USER_H__
#define __STC_USER_H__

#include<STC_MCU.H>
void action(void);
void init(void);
bit verr_recevie_pc(void);
bit verr_recevie_cardhead(void);
void cardhead_seek(void);
void send_cmd_to_cardhead(unsigned char *in);
void cardhead_read(unsigned char block);
void cardhead_read_havepass(unsigned char block,unsigned char *pass);
void cardhead_OPENorCLOSE(unsigned char oc);
void cardhead_write(unsigned char block,unsigned char *indata11);
void cardhead_write_havepass(unsigned char block,unsigned char *pass,unsigned char *indata);
void cardhead_writepass(unsigned char block,unsigned char *oldpass,unsigned char *newpass);
void cardhead_writecardheadpass(unsigned char addrl,unsigned char addrh,unsigned char *pass);
void send_to_pc(unsigned char *in);
void Seek_Card(void);
void Read_2block_Card_nopass(unsigned char block);
void Write_2block_Card_nopass(unsigned char block,unsigned char *indata);
void Change_Password(unsigned char block,unsigned char *inpass);
void Change_CardHead_Password(unsigned char *inpass);
void Read_1block_Card_havepass(unsigned char block,unsigned char *inpass);
void Write_1block_Card_havepass(unsigned char block,unsigned char *indata0);
void Read_1block_Card_nopass(unsigned char block);
void Write_1block_Card_nopass(unsigned char block,unsigned char *indata0);
void Batch_Write_Card(unsigned char block,unsigned char *indata1,unsigned char *last_serial);

////******IC�������***********//
//typedef enum IC_CMD
//{
//	IC_SEEK=
//} IC_CMD_Type;
////*********//

//static unsigned char code tab_seek_ic_cmd[]={
//	0xaa,0x55,0x03,0x02,0x00,0x23//Ѱ��
//};
//static unsigned char code tab_read_ic_cmd[]={
//	0xaa,0x55,0x03,0x02,0x00,0x23//����
//};

#endif