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
//typedef struct{	
//	uint32_t	longitude; //经度
//	uint32_t	latitude; 
//	uint32_t	cimi;
//	uint32_t	cellid;	
//	int16_t		height;
//	uint16_t	speed;
//	uint16_t	angle;
//	
//	uint8_t		timeYY;
//	uint8_t 	timemm;
//	uint8_t		timeDD;
//	uint8_t		timeHH;
//	uint8_t		timeMM;
//	uint8_t		timeSS;
//
//	uint8_t		signal;
//	uint8_t		stars;
//	uint8_t 	posState;// 1:定位 0：未定位
//	uint8_t		debugBytes;
//
//	uint8_t		voltage;
//	uint8_t		hardVersion;
//	uint8_t		softVersion;								   
//	uint8_t		other;	
//
//}GPS_DATA_REPORT;
//
//typedef struct{
//	u16 canNum;	//can data numbers of the package
//	u8 timeYY;
//	u8 timemm;
//	u8 timeDD;
//	u8 timeHH;
//	u8 timeMM;
//	u8 timeSS;
//} OBD_DATA_REPORT;
//
///*
//写入flash的配置数据，各个成员尽量保证数据对齐，即长度可以被4整除
//*/
//typedef struct{
//	uint16_t crc;
//	uint16_t other;
//	uint32_t baudrate;
//	uint8_t name[20];//蓝牙名称
//	uint8_t password[20];	
//}BLUE_CONFIG;
//typedef struct {
//	uint16_t crc;
//	uint16_t other;
//	uint32_t baudrate;
////	uint8_t  [20];	
//}OBD_CONFIG;
//
//typedef struct {
//	uint16_t crc;
//	uint16_t flag;
//	IAP_BRIFE iapBirfe;
//	SOCKET netMsg;
//	BLUE_CONFIG blueConfig;
//	OBD_CONFIG obdConfig;
//}FLASH_CONFIG;




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



