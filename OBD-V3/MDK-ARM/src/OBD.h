#include "sysdef.h"

#ifndef OBD_H_H

#define OBD_H_H



typedef struct
{
   void (*formula)(int raw_data, char *buf);  //fromula handler
   char label[32];							  //label
   char screen_buf[64];						  //not nessory
   char pid[16];								  //cmd type
   int enabled;								  //whether to cmd
   int bytes; 								  // number of data bytes expected from vehicle
} SENSOR;

typedef struct
{
	void (*formula)(int raw_data, char *buf);  //fromula handler
	char label[32];							  //label
	
//	char ackData[32];						  //not nessory
	char ackBuf[32];
	char pid[32];								  //cmd type
	int enabled;								  //whether to cmd
	int bytes; 								  // number of data bytes expected from vehicle
	int timeOut;							//命令超时
	int data1;
	int data2;
} OBD_SENSOR;
//typedef struct{
//
//
//}OBD_IRQ_BUF;



/*OBD cmd
baud 38400

0x0D结束命令
0x0A结束数据

AT sets




ST sets


*/
/*
AT@S 获取芯片编号
	CodeNumb:98764323
ATZ	Reset Device


*/

#define OBD_MAX_CMD_NUM 18*4


int Obd_Send_AT_And_Wait(char *cmd,char *right_return,unsigned int delay_time);
int	obdReset(int state);
int obdPower(int state);
void obdTest(void);
int	initObdMode(int type);
int16_t obdExec(int16_t cmdNum);
void execObd(int index);
void obdBlue(void);
void obdIrq(void);
int checkObdReturn(char *buf ,uint16_t type);
uint16_t getBufIndex(uint16_t index);
int16_t fillObdBuf(uint16_t index,char *buf,int value);




#endif

