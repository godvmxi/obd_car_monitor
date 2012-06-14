#include "sanyData.h"

int CAN_LOCK_FLAG = 0;
int CAN_UN_LOCK_FLAG = 0;

SANY_DATA sanyData;
SANY_POS_REPORT sanyPosData;
extern FIFO_NODE fifoNode[FIFO_MAX_SIZE];
extern FIFO_HEAD fifoHead;
extern DATA_IN_FLASH sysStateNow;
extern int voltage;


extern char cimi[5],csq[2],new_cellid[8];//,old_cellid[8];
extern stru_GPSRMC  	GPS_RMC_Data; 
extern stru_GPSGGA  	GPS_GGA_Data; 
extern stru_GPSGSV  	GPS_GSV_Data;
extern stru_GPS_DATA  	GPS_DATA;
extern int		GPS_GGA_DINGWEI_OK;//若GPS定位，则将此变量置1，否则为0
extern int		GPS_SHUJU_OK;//若GPS输出坐标信息，则将此变量置1，否则为0 
extern int		GPS_RMC_DINGWEI_OK;
CAN_RX_BUFFER	sanyCanBuffer;
CAN_TX_BUFFER	sanyTxBuffer;
RTC_STRUCT rtcData;
int RTC_MODIFY_FLAG = 0;
u16 IPD_FLAG_1 = 0;
u16 IPD_FLAG_2 = 0;
u16 rxMsgFlag = 0;
u16 rxBufferFlag = 0;
char rxBuffer1[IPD_BUFFER];
char rxBuffer2[IPD_BUFFER];
char buffer1[IPD_BUFFER];
char buffer2[IPD_BUFFER];
extern int ERROR_FLAG;
extern volatile int	blueToothCounter;
int blueToothState = 0;//0:未连接 1：已连接

int systemStopState = 0;//0:运动 1：静止
volatile int canCounter = 0;
extern volatile int onlineTimeOutFlag;

volatile int canMsgLockType	= 0;//can data lock type
                           