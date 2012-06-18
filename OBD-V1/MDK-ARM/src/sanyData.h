#include "sysdef.h"
#ifndef DANY_DATA_H

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
#define CAN_BUFFER_NUM		15

#define IPD_BUFFER			100
#define REAL_BUFFER_NUM		7
#define NORMAL_BUFFER_NUM	8

//receive flag
#define SEND_OK_F 		1
#define SEND_FAIL_F		2
#define IPD_F	 		4
#define ERROR_F	  		8
#define CLOSE_F	  		16
//#define 
//#define 
//#define 
#define LOCK_VOLTAGE_25 128
#define LOCK_VOLTAGE_0	0


typedef struct {
	u8	MSG_TAG[3];
	u8	MSG_CRC;
	u16 MSG_TYPE;
	u16 MSG_LENGTH;
	u32 MSG_ID;
	u8	MDT_ID[8]; 
}SANY_DATA;

typedef struct{
	
	
	uint32_t	longitude; //经度
	uint32_t	latitude; 
	uint32_t	cimi;
	uint32_t	cellid;	
	int16_t		height;
	uint16_t	speed;
	uint16_t	angle;
	
	uint8_t		timeYY;
	uint8_t 	timemm;
	uint8_t		timeDD;
	uint8_t		timeHH;
	uint8_t		timeMM;
	uint8_t		timeSS;

	uint8_t		signal;
	uint8_t		stars;
	uint8_t 	posState;// 1:定位 0：未定位
	uint8_t		debugBytes;

	uint8_t		voltage;
	uint8_t		hardVersion;
	uint8_t		softVersion;
	uint8_t		others;			
	
}SANY_POS_REPORT;


typedef struct{
	u16 canNum;	//can data numbers of the package
	u8 timeYY;
	u8 timemm;
	u8 timeDD;
	u8 timeHH;
	u8 timeMM;
	u8 timeSS;
	CanRxMsg txBuffer[CAN_BUFFER_NUM];
} CAN_TX_BUFFER;
typedef struct {
	u16 realBufferIndex; //实时数据缓存区索引
	u16 normalBufferIndex; //非实时数据缓冲区索引
	CAN_TX_BUFFER realRxBuffer[2];	//0/1作为实时动态数据，2作为非实时数据
//	CAN_TX_BUFFER normalRxBuffer[2];	//0/1作为实时动态数据，2作为非实时数据
} CAN_RX_BUFFER;
enum {
	REGISTER_MSG	=	0X0000,
	//SINGAL MESSAGE  OBD->>HOST
	HEART_MSG = 0X0010,
	ONLINE_MSG = 0X0011,
	OFFLINE_MSG	=	0X0012,
	//NORMAL MESSAGE  OBD->>HOST
	REPORT_GPS_MSG	=	0X0020,
	REPORT_CAN_MSG	=	0X0030,
	REPORT_OBD_MSG	=	0X0040,

	//POWER STATE REPORT
	REPORT_POWER_ON_MSG = 0X1150,
	REPORT_POWER_OFF_MSG = 0X1151,

		//CONTROL MESSAGE HOST->>OBD
	SOFT_UPDATE_MSG	=	0X0110,
	SET_NET_MSG		=	0X0111,
	SET_MODE_MSG	=	0X0112,
	SET_BLUE_MSG	=	0X0113,

	//CMD
	RUN_CAN_MSG		=	0X0120,
	RUN_OBD_MSG		=	0X0130,

	//TELE
	SET_PHONE_MSG	=	0X0140,


	//POWER STATE REPORT
	ACK_POWER_ON_MSG = 0X9130,
	ACK_POWER_OFF_MSG = 0X9131,
	//NORMAL MESSAGE ACK 	HOST ->>BEE
	ACK_HEART_MSG	=	0X9100,
	ACK_ONLINE_MSG  =   0X9101,
	ACK_OFFLINE_MSG	=	0X9102,

	ACK_GPS_MSG		=	0X9110,
	ACK_CAN_MSG		=	0X9120,





	//CONTROL MESSAGE BEE->>HOST
	ACK_SOFT_UPDATE_MSG	=	0X90F1,
	ACK_SET_NET_MSG		=	0X90F2,
	ACK_SET_REPORT_MSG	=	0X90F3,
	//CONTROL CAN MES  HOST->>BEE
	
	SET_CAN_LOCK_1_MSG	=	0X1121,
	SET_CAN_LOCK_2_MSG	=	0X1122,
	SET_CAN_UNLOCK_MSG	=	0X1123,
	//				   BEE->>HOST
	ACK_CAN_LUCK_MSG	=	0x9121,
	ACK_CAN_UNLOCK_MSG	=	0x9122
};
typedef struct {
	u8 year;
	u8 month;
	u8 day;
	u8 hour;
	u8 minute;
	u8 second;
}RTC_STRUCT;

//void reportPos(void);

u8   getCrc(char *pointer,int length);
u8   checkCrc(char *pointer,int length);
void getDataHead(u16 type,char *pointer,int length);
void reportheartHeat(void);
void sendData(char *pointer,int length,int head,int reSend,int checkAck);//type 0:不自动发送数据包头 1：自动发送  gpsFlag :发送gps数据包
char *stringSearch(char *sourse,char *des,int length);
void checkAckMsg(void);
u16  getNextMsgType(void);
void answerMsg(u16 type,u16 msgID);
void SendCan(u16 dat);
void CAN_Interrupt(void);
void fillCanBuffer(CanRxMsg* RxMessage);
void initCanBuffer(int bufferNum);
CAN_TX_BUFFER *getCanBufferPointer(int type);//0：获取实时数据指针 1：获取非实时数据指针
u32 reConnect(void);
void checkIpdMsg(char temp);
void rtc(void);

void dealAckMsg(char *charP0);
void showCanData(CAN_TX_BUFFER can);
int  lockCan(int type);
int  unlockCan(void);
void setCan(int enable);
void storeToFlash(char *pointer,int length,int head);
u8	reSendFlashMsg(void);
void checkSysState(void);
void checkLockState(void);
void	checkBlueToothState(void);
void	reportPowerState(void);
void	setNetArgument(char * pointer,int length);
void	checkOnlineTimeOut(void);





#define DANY_DATA_H
#endif

