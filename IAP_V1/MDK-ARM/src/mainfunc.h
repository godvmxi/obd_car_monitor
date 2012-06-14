#include "sysdef.h"
#ifndef		MAIN_FUNC_H_H
#define		MAIN_FUNC_H_H



//Message type
////Treminal --> host
//#define HEARTHEAT_MSG  		0x0101
//#define	TERMINAL_ONLINE		0x0102
//#define	TERMINAL_OFFLINE 	0x0103
//#define	TERMINAL_POS_REPORT	0x1104
//#define	CAN_REPORT			0x1105
//
////Host --> terminal	
//#define	SOFT_UPDATE			0x1001
//#define	SET_NET_ARGUMENT 	0x1002
//#define	SET_REPORT_ARGUMENT 0x1003

//typedef struct {
//	uint8_t		MSG_TAG[3];
//	uint8_t	 	MSG_CRC;
//	uint32_t 	DEV_ID_H;//imei 高32位
//	uint32_t 	DEV_ID_L;//imei 低32位
//	uint16_t 	MSG_TYPE;
//	uint16_t 	MSG_LENGTH;
//	uint32_t	MSG_ID;
//}DATA_HEAD;
//




int16_t	collectAndSend(void);
int		checkConnectState(uint32_t socketNum);
int		establishConnect(SOCKET socket);

void reportPos(SOCKET soc);
void reportObd(SOCKET soc);
int closeConnect(int socket);
int16_t str2hex(int8_t *src,uint16_t length,int8_t *des);
void setDataHeader(uint16_t type,int8_t *pointer,int16_t length);
int16_t dataSend(char *pointer,int length,int head,int reSend,int checkAck,SOCKET socket,int reConnect);//type 0:不自动发送数据包头 1：自动发送  gpsFlag :发送gps数据包









#endif



