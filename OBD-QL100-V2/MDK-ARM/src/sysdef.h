#include <ctype.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h> 

#include "stm32f10x_it.h"
#include "stm32f10x.h"

#ifndef __SYSDEF_H
#define __SYSDEF_H	

//#include <ctype.h> 
//#include <stdlib.h>
//#include <string.h>
//#include <ctype.h>
//#include <stdio.h>
//#include <math.h> 
//
//#include "stm32f10x_it.h"
//#include "stm32f10x.h"
//#include "hard_init.h"
//#include "sim900.h"
//#include "flash.h"
//#include "gps.h"
//#include "sanyData.h"
//#include "fifo.h"
//#include "flashFifo.h"
//#include "blueTooth.h"
//#include "mainfunc.h"
//#include "obd.h"
//#include "rtc.h"
//#include "at45db161d.h"
//#include "crc.h"
//#include "iap.h"


typedef enum 
{
	sim900StartError,//sim900����ʧ��
	sim900NotInsert,// sim900δ���뿨Ƭ
	sim900RegisterFail,//sim900����ע��ʧ��,������������
	sim900NetWeak,  //sim900����̫��
	sim900SendFailed,//sim900��������ʧ�� 
	gpsStartError,//gps���ϻ���gps�������

}SYSTEM_ERROR;
typedef struct 
{
//	int8_t week;
	int16_t year;
	int8_t month;
	int8_t date;
	int8_t hour;
	int8_t minute;
	int8_t second;			
}TIME;

typedef struct {
	uint8_t 	start[2];	  //fill with  '#'
	uint16_t 	crc;
	int16_t		typeOrIndex;	
	uint8_t		hv;
	uint8_t		sv;	
	uint32_t	imeiHigh;
	uint32_t	imeiLow;
}IAP_START;	//IAP ��������
typedef struct {
	uint8_t 	start[2]; //fill with '#'
	uint16_t	crc;//���ݰ�crc

	int16_t		typeOrIndex;
	uint8_t		hv;
	uint8_t		sv;

	uint16_t	packetNum;
	uint16_t	appCrc;	//����APP��CRCУ��


	uint32_t	sizeOfApp; //����APP�������С��Ϣ
//	uint8_t		app[4];	 //fill with '*'
}IAP_BRIFE;	//IAP ��������

typedef struct {
	uint8_t 	start[4];
	uint16_t	crc;
	int16_t		index;
	uint8_t		other[4];
}IAP_GET;	//IAP ��������

typedef struct {
	uint8_t 	start[2]; //fill with '#'
	uint16_t	crc;//���ݰ�crc

	int16_t		typeOrIndex;
	uint8_t		hv;
	uint8_t		sv;

	uint16_t	packetCrc;	//���������crc
	uint16_t	packetLength;	 

	uint8_t		app[4];	 //fill with '*'
}IAP_DATA;	//IAP ���ݷ���
typedef struct {
	uint32_t crc;//������CRCУ��ֵ�����ڶ��룬ֻ�����8λ��Ч
	uint32_t cfgLength;//���������ȣ����������ýṹ�峤��
	uint32_t initFlag;//����Ϊ0
	uint32_t devId;
		  	
	int8_t iapIp[20]; //��������������
	uint32_t  iapPort; 

	int8_t appIp[20];//��������������
	uint32_t  appPort; 	

	//��������������ֱ�Ӻ�׺�����Ǳ���䶯֮ǰ�����ݷ����䶯��
}APP_CFG;
//���ݷ��ͽṹ�嶨��
typedef struct {
	uint8_t  	START[2];
	uint16_t	MSG_CRC;

	

	uint8_t		time[6];
	uint16_t 	MSG_TYPE;

	uint32_t	IMEI_H;//��һ���ֽ���power mode
	uint32_t	IMEI_L;

	uint16_t 	MSG_LENGTH;
	uint8_t		hv;
	uint8_t		sv;

	uint32_t 	DEV_ID_H;//imei ��32λ
	uint32_t 	DEV_ID_L;//imei ��32λ
}DATA_HEAD;	
typedef struct{	
	uint32_t	longitude; //����
	uint32_t	latitude; 
	uint32_t	cimi;
	uint32_t	cellid;	

	int16_t		height;
	int16_t		speed;

	uint16_t	angle;	
	uint16_t	hdop;

	uint8_t		signal;
	uint8_t		stars;
	uint8_t 	posState;// 1:��λ 0��δ��λ
	uint8_t		voltage;

	uint8_t		reserved[4];								   	

}GPS_DATA_REPORT;




//Ӧ����ƽṹ�嶨��
typedef struct {
	char imei[18]; //
	char csq[3];
	char cimi[6];
	char oldCellid[9];
	char newCellid[9];
	int  voltage;
	int	 serverType;//����Ӧ��
	uint32_t imeiHigh;
	uint32_t imeiLow;
}SIM_STRUCT;
typedef struct {
	int16_t	crc;
	int16_t	flag;
	int8_t	ipUrl[40];
	int32_t udpTcp;
	uint32_t socketNum;
	int32_t localPort;
	int32_t	desPort;
	int32_t desPort2;
	uint8_t reserved[20]; 
}SOCKET;   

/*
д��flash���������ݣ�������Ա������֤���ݶ��룬�����ȿ��Ա�4����
*/


typedef struct{
	uint16_t crc;
	uint16_t other;
	uint32_t baudrate;
	uint8_t name[20];//��������
	uint8_t password[20];
	uint8_t reserved[20];	
}BLUE_CONFIG;
typedef struct{
	uint16_t cmdIndex;//��ǰ�����������б������
	uint8_t  dataLength;//�������ص����ݳ���
	uint8_t	 timeOut;//����ִ�еĵȴ�ʱ��
	
}OBD_LIST;


typedef struct {
	uint16_t crc;
	uint16_t other;
	uint32_t baudrate;
	uint32_t deviceIdHigh;
	uint32_t deviceIdLow;
	uint32_t cmdNum;
	uint16_t initStart;
	uint16_t initEnd;
	uint16_t normalStart;
	uint16_t normalEnd;
	uint16_t fastStart;
	uint16_t fastEnd;
	OBD_LIST cmdList[200];
	uint8_t reserved[20];	
}OBD_CONFIG;
typedef struct {
	uint32_t interval;//���ͼ��
	uint32_t proportion; //fast obd and normal ����
 	uint8_t	 reserved[20];//����
}SYS_CONFIG;

typedef struct {
	uint16_t crc;
	uint16_t flag;	
	SOCKET 			netConfig;
	SOCKET			iapConfig;
	IAP_BRIFE		iapBrife;
	uint8_t			iapReserved[20];	
	BLUE_CONFIG		blueConfig;
	OBD_CONFIG		obdConfig;
	SYS_CONFIG		sysConfig;	
}FLASH_CONFIG;

typedef struct {
	TIME time;
	uint16_t itemNum;
 	uint8_t buf[400];
}OBD_MSG_BUG;
typedef struct {
	char rdtc[32];
	char edtc[32];
	char vin[32];
	char eid[32]; 
	char mil[20];
	char spwr[20];
	char ife[20];
	char ehp[20];
	char whp[20];
	char ad_mil[20];
	char ad_feh[20];
	char pids[160];
}OBD_INFO;
/*
		obdAtAndWait("BT+DATA.IFE\r\n",NULL,3000);
		obdAtAndWait("BT+DATA.WHP\r\n",NULL,3000);
		obdAtAndWait("BT+DATA.AD_Mil\r\n",NULL,3000);
		obdAtAndWait("BT+DATA.AD_Accel\r\n",NULL,3000);
		obdAtAndWait("BT+DATA.AD_FEH\r\n",NULL,3000);
*/










#define		PRINTF_DEBUG  1//printf���Կ��أ�ע���������ȥ�����еĴ�ӡ���
#define		DEBUG_SIM
#define		DEBUG_GPS
#define		DEBUG_OBD     


			


//#define		QTD2_TEST
//#define		GPS_ANT_SEL
//#define		PERSONAL	  s
#define		SOFT_VERSION  'X'
#define		HARD_VERSION  'X'

#define		REPORT_TIME		90//ƽ��ϵͳ�ϱ�ʱ��
#define		SlEEP_TIME		300

#define 	POWER_LACK	3850
#define 	POWER_EMPTY		3750
#define		POWER_FULL		4200
#define 	EMPTY_COUNTER 40000
#define 	GPS_COUNTER  3


#define	NO_ERROR	0
#define	SIM_CARD_ERROR	1
#define	SYS_PARA_ERROR	2 

/*
		KEY1			
		KEY2		

*/ 
#define		KEY1_DOWN			1
#define		KEY2_DOWN			0
#define		GET_KEY1_STATE		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	
#define		GET_KEY2_STATE		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)	


/*
		LED1				PB12							���				
		LED2				PB13							���
		LED3				PB14							���
*/
#define		LED1(X)			GPIO_WriteBit(GPIOB,GPIO_Pin_12,X)
#define		LED2(X)			GPIO_WriteBit(GPIOB,GPIO_Pin_13,X)
#define		LED3(X)			GPIO_WriteBit(GPIOB,GPIO_Pin_14,X)




/*
		SIM_EN				PB9
		SIM_RST				PB4
		SIM_DTR				PB3
*/


//GSM���߹ܽ�
#define		SIM_EN(X)	GPIO_WriteBit(GPIOB,GPIO_Pin_9,X);
#define		SIM_RST(X)	GPIO_WriteBit(GPIOB,GPIO_Pin_4,X);
#define		SIM_DTR(X)	GPIO_WriteBit(GPIOB,GPIO_Pin_3,X);
/*

		GPS_EN				PB1
*/
		

#define		GPS_EN(X)	GPIO_WriteBit(GPIOB,GPIO_Pin_1,X)

/*
		OBD_EN				PB0
*/

#define		OBD_EN(X)	GPIO_WriteBit(GPIOB,GPIO_Pin_0,X);

/*
		BLUE_EN				PA14
		BLUE_STATE			PB5	
		BLUE_CLR			PB6	

		BLUE_MS				PB8
		BLUE_HSW			PB7
*/
#define		BLUE_EN(X)		GPIO_WriteBit(GPIOA,GPIO_Pin_14,X)
#define		BLUE_STATE		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)
#define		BLUE_CLR(X)		GPIO_WriteBit(GPIOB,GPIO_Pin_6,X)

#define		BLUE_MS(X)		GPIO_WriteBit(GPIOB,GPIO_Pin_8,X)
#define		BLUE_HSW(X)		GPIO_WriteBit(GPIOB,GPIO_Pin_7,X)



/*
USB_ID  
		USB_ID				PA13
*/
#define 	USB_ID			GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_13)
/*
POW
		EN_MP3410			PA1
		PW_CHECK			PA4
		BAT_CHECK			PC0
		CHARGE_STATE		PC1
		EN_CHARGE			PC2
		EN_POW5				PC3
*/

#define		POWER_STATE		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)


	


//
//
//#define ENRECHARGE	 		GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET)
//#define DISRECHARGE	 		GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET)



typedef enum   
{
	working,   //0:δ���߻�ͣ��
	powerOff,  //1���ص�Դͣ��
	dtrSleep,	//3��DTR�ܽ�����
	unsure,
} SLEEP_TYPE;



/*���ڶ���*/
extern int ISP_DIRECTION;
extern int BLUE_DIRECTION;

#define		USART_NONE		1
#define		USART_BLUE		4	//115200
#define		USART_SIM		2 	//115200
#define		USART_OBD		3	//38400
#define		USART_GPS		5   //9600		


extern	char USART1_RX_buffer[1300];	 //����1���ջ���
extern	char USART2_RX_buffer[1300];	 //����2���ջ���
extern	char USART3_RX_buffer[300];	 //����3���ջ���
extern	char UART4_RX_buffer[300];	 //����4���ջ���
extern	char UART5_RX_buffer[300];	 //����5���ջ���


extern	unsigned int USART1_RX_buffer_count; //����1���ջ���λ�ñ���
extern	unsigned int USART2_RX_buffer_count; //����2���ջ���λ�ñ���
extern	unsigned int USART3_RX_buffer_count; //����3���ջ���λ�ñ���
extern	unsigned int UART4_RX_buffer_count; //����4���ջ���λ�ñ���
extern	unsigned int UART5_RX_buffer_count; //����5���ջ���λ�ñ���

#define ISP_BUF			USART1_RX_buffer
#define OBD_BUF			USART3_RX_buffer
#define SIM_BUF			USART2_RX_buffer
#define	BLUE_BUF		UART4_RX_buffer
//#define	GPS_BUF			UART5_RX_buffer

#define ISP_COUNT		USART1_RX_buffer_count
#define OBD_COUNT		USART3_RX_buffer_count 
#define SIM_COUNT		USART2_RX_buffer_count 
#define	BLUE_COUNT		UART4_RX_buffer_count
//#define	GPS_COUNT		UART5_RX_buffer_count

#define	ISP				USART1
#define	OBD				USART2
#define	GPS				USART3
#define	BLUE			UART4
#define	SIM				UART5

//ȫ�ֱ�������
//ȫ�ֲ�����
extern 		FLASH_CONFIG  sysCfg;
//����״̬���
extern 		int	BLUE_OBD;
//SIM״̬������
extern 		SIM_STRUCT simState;
//OBD��ͨ�ϱ�������
extern		OBD_MSG_BUG obdNormalBuf;
//OBD�����ϱ�������
extern		OBD_MSG_BUG obdFastBuf;
//RTC������־
extern 		int32_t rtcModifyFlag;
//ȫ��ʱ�ӣ���Ҫ��ȡʱ�����Ч
extern 		TIME timer;
//OBD����ģʽ 0����ͨģʽ 1:����ģʽ��ͨ��ʱ�Ӽ�⣬ÿ5����һ��
extern 		int32_t OBD_MODE;
//CAN���߼�⹦�ܼ�����
extern 		int32_t CAN_DETECTER;
extern		int32_t DEVICE_STATE;
extern		int32_t OBD_START ;
extern		int32_t CAR_POWER_FAILED;
typedef struct {
	char cmd[60];
	char buf[200];
}OBD_PID;
typedef struct {
	uint32_t pidNum;
	uint32_t cmdNum;
	OBD_PID mulCmd[9];
}MUL_PIDS;
extern MUL_PIDS pidBuf;
extern OBD_INFO obdInfo,obdInfoFlash;




/////*GPS*/
//extern stru_GPSRMC  	GPS_RMC_Data; 
//extern stru_GPSGGA  	GPS_GGA_Data; 
//extern stru_GPSGSV  	GPS_GSV_Data;
//extern stru_GPS_DATA  	GPS_DATA;
//////-------------------------------------------------------
//extern int	GPS_GGA_DINGWEI_OK;//��GPS��λ���򽫴˱�����1������Ϊ0
//extern int	GPS_RMC_DINGWEI_OK;//��GPS��λ���򽫴˱�����1������Ϊ0
//extern int	GPS_SHUJU_OK;//��GPS���������Ϣ���򽫴˱�����1������Ϊ0 









/***************************************IAP SETS************************************************\

										chip flash 
			
				*************************************************************
				*				* 						*					*
				*bootloader		*		system para		* 	system main		*
				*				*						*					*
				*************************************************************

										chip state

		*********************************************************************************
		*				* 						*					*					*
		*	type		*		start address	* 	end address		*	length			*
		*				*						*					*					*
		*********************************************************************************
		*				* 						*					*					*
		*	ROM			*		0x08000000		* 	0x00000000		*	0x40000(256K)	*
		*				*						*					*					*
		*********************************************************************************
		*				* 						*					*					*
		*	RAM			*		0x20000000		* 	0x00000000		*	0xC00048K		*
		*				*						*					*					*
		*********************************************************************************
				 


											bootloader

		*********************************************************************************
		*				* 						*					*					*
		*	type		*		start address	* 	end address		*	length			*
		*				*						*					*					*
		*********************************************************************************
		*				* 						*					*					*
		*	ROM			*		0x08000000		* 	0x08004000		*  0x5000(20k)		*
		*				*						*					*					*
		*********************************************************************************
		*				* 						*					*					*
		*	RAM			*		0x20000000		* 	0x00000000		*		4K			*
		*				*						*					*					*
		*********************************************************************************


								system parameter(ROM)

		*********************************************************************************
		*				* 						*					*					*
		*	type		*		start address	* 	end address		*					*
		*				*						*					*					*
		*********************************************************************************
		*				* 						*					*					*
		*	bootloader	*		0x08005000		* 	end address		*		2K			*
		*				*						*					*					*
		*********************************************************************************


									system main

		*********************************************************************************
		*				* 						*					*					*
		*	type		*		start address	* 	end address		*	length			*
		*				*						*					*					*
		*********************************************************************************
		*				* 						*					*					*
		*	ROM			*		0x08006400		* 	end address		*  (256-25)K		*
		*				*						*					*					*
		*********************************************************************************
		*				* 						*					*					*
		*	RAM			*		0x20000000		* 	0x00000000		*		4K			*
		*				*						*					*					*
		*********************************************************************************

\******************************************************************************/

//#define		IAP_FUNCTION	1
//#define		IAP_PARAMETER			0x08005000
//#define		SYS_PARAMETER			0x08005400

#define		IAP_PARAMETER			0x0800F000
#define		SYS_PARAMETER			0x0800F400
#define 	SYS_MAIN				0x08005C00 
#define		SYS_VECTOR				(SYS_MAIN-NVIC_VectTab_FLASH)	

#define		ApplicationAddress		0x8008000
#define		NVIC_VectTab_FLASH_ADDR		ApplicationAddress-NVIC_VectTab_FLASH

#define	FLASH_ADR	0x08007400	//Ҫд�����ݵĵ�ַ
#define	FLASH_FIFO	0x08007000




#include "mainfunc.h"
#include "hard_init.h"
#include "sim900.h"
#include "flash.h"
#include "gps.h"

#include "simfifo.h"
#include "flashFifo.h"
#include "blueTooth.h" 
#include "obd.h"
#include "rtc.h"
#include "at45db161d.h"	 
#include "iap.h"  
#endif


