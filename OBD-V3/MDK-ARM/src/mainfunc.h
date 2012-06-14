#include "sysdef.h"
#ifndef		MAIN_FUNC_H_H
#define		MAIN_FUNC_H_H



//Message type
////Treminal --> host
#define   	REGISTER_MSG  		0x0000
#define   	HEARTHEAT_MSG  		0x0010
#define		DEV_ONLINE			0x0011
#define		DEV_OFFLINE			0x0012
#define		DEV_POS_REPORT		0x0020
#define		CAN_REPORT			0x0030
#define		OBD_REPORT			0x0040
//
//Host --> terminal	
#define		SET_IAP_PARA		0x0110
#define		SET_NET_PARA	 	0x0111
#define		SET_WORK_PARA		0x0112
#define		SET_BLUE_PARA		0x0113
#define		SET_OBD_PARA		0x0114

#define		EXEC_CAN			0x0120

#define		EXEC_OBD			0x0130

#define		SET_PHONE			0x0140

int16_t	collectAndSend(void);
int		checkConnectState(uint32_t socketNum);
int		establishConnect(SOCKET socket);


int closeConnect(SOCKET *soc);
int16_t str2hex(int8_t *src,uint16_t length,int8_t *des);
void setDataHeader(uint16_t type,int8_t *pointer,int16_t length);
int16_t dataSend(char *pointer,int length,int head,int reSend,int checkAck,SOCKET socket,int reConnect);//type 0:不自动发送数据包头 1：自动发送  gpsFlag :发送gps数据包


void reportPos(SOCKET *soc,int timeout,int flag);
void reportObd(SOCKET *soc,int timeout,int flag);
void reportCan(SOCKET *soc,int timeout,int flag);
void reportHeart(SOCKET *soc,int timeout,int flag);

#endif



