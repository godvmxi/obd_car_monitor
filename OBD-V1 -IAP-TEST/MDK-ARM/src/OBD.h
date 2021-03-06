#include "sysdef.h"

#ifndef OBD_H_H

#define OBD_H_H



typedef struct
{
   void (*formula)(int raw_data, char *buf);  //fromula handler
   char label[32];							  //label
   char screen_buf[64];						  //not nessory
   char pid[3];								  //cmd type
   int enabled;								  //whether to cmd
   int bytes; 								  // number of data bytes expected from vehicle
} SENSOR;
//typedef struct{
//
//
//}OBD_IRQ_BUF;



/*OBD cmd
baud 38400

0x0D��������
0x0A��������

AT sets




ST sets


*/
/*
AT@S ��ȡоƬ���
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




#endif

