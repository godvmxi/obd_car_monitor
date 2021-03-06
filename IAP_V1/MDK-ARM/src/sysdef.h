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


typedef struct {
	uint8_t 	start[2];	  //fill with  '#'
	uint16_t 	crc;
	int16_t		typeOrIndex;	
	uint8_t		hv;
	uint8_t		sv;	
	uint32_t	imeiHigh;
	uint32_t	imeiLow;
}IAP_START;	//IAP 参数请求
typedef struct {
	uint8_t 	start[2]; //fill with '#'
	uint16_t	crc;//数据包crc

	int16_t		typeOrIndex;
	uint8_t		hv;
	uint8_t		sv;

	uint16_t	packetNum;
	uint16_t	appCrc;	//整个APP的CRC校验


	uint32_t	sizeOfApp; //整个APP的体积大小信息
//	uint8_t		app[4];	 //fill with '*'
}IAP_BRIFE;	//IAP 参数返回

typedef struct {
	uint8_t 	start[4];
	uint16_t	crc;
	int16_t		index;
	uint8_t		other[4];
}IAP_GET;	//IAP 数据请求

typedef struct {
	uint8_t 	start[2]; //fill with '#'
	uint16_t	crc;//数据包crc

	int16_t		typeOrIndex;
	uint8_t		hv;
	uint8_t		sv;

	uint16_t	packetCrc;	//附带软件包crc
	uint16_t	packetLength;	 

	uint8_t		app[4];	 //fill with '*'
}IAP_DATA;	//IAP 数据返回

typedef struct {
	uint32_t crc;//参数区CRC校验值，便于对齐，只是最低8位有效
	uint32_t cfgLength;//配置区长度，及整个配置结构体长度
	uint32_t initFlag;//设置为0
	uint32_t devId;
		  	
	int8_t iapIp[20]; //升级参数配置区
	uint32_t  iapPort; 

	int8_t appIp[20];//正常工作配置区
	uint32_t  appPort; 	

	//有其他参数可以直接后缀，但是避免变动之前的数据发生变动，
}APP_CFG;
//数据发送结构体定义
typedef struct {
	uint8_t		MSG_TAG[3];
	uint8_t	 	MSG_CRC;
	uint32_t 	DEV_ID_H;//imei 高32位
	uint32_t 	DEV_ID_L;//imei 低32位
	uint16_t 	MSG_TYPE;
	uint16_t 	MSG_LENGTH;
	uint32_t	MSG_ID;
}DATA_HEAD;	
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
	uint8_t		other;	

}GPS_DATA_REPORT;

typedef struct{
	u16 canNum;	//can data numbers of the package
	u8 timeYY;
	u8 timemm;
	u8 timeDD;
	u8 timeHH;
	u8 timeMM;
	u8 timeSS;
} OBD_DATA_REPORT;
//应用设计结构体定义
typedef struct {
	char imei[18]; //
	char csq[3];
	char cimi[6];
	char oldCellid[9];
	char newCellid[9];
	int  voltage;
	int	 serverType;//服务供应商
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
	uint8_t reserved[20]; 
}SOCKET;  

/*
写入flash的配置数据，各个成员尽量保证数据对齐，即长度可以被4整除
*/


typedef struct{
	uint16_t crc;
	uint16_t other;
	uint32_t baudrate;
	uint8_t name[20];//蓝牙名称
	uint8_t password[20];
	uint8_t reserved[20];	
}BLUE_CONFIG;


typedef struct {
	uint16_t crc;
	uint16_t other;
	uint32_t baudrate;
	uint8_t  reserved[20];	
}OBD_CONFIG;

typedef struct {
	uint16_t crc;
	uint16_t flag;	  //初始化为0x5555
	SOCKET 			netConfig;
	SOCKET			iapConfig;
	IAP_BRIFE		iapBrife;
	uint8_t			iapReserved[20];	
	BLUE_CONFIG		blueConfig;
	OBD_CONFIG		obdConfig;	
}FLASH_CONFIG;

extern FLASH_CONFIG  sysCfg;






#define		PRINTF_DEBUG //printf调试开关，注销掉便可以去掉所有的打印语句
#define		DEBUF_SIM
#define		DEBUF_GPS
#define		DEBUF_OBD


			


//#define		QTD2_TEST
//#define		GPS_ANT_SEL
//#define		PERSONAL	  s
#define		SOFT_VERSION  'X'
#define		HARD_VERSION  'X'

#define		REPORT_TIME		90//平均系统上报时间
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
  		LED1				PB3
		LED2				PB4
		LED3				PB5
*/
#define		LED1(X)			GPIO_WriteBit(GPIOB,GPIO_Pin_3,X)
#define		LED2(X)			GPIO_WriteBit(GPIOB,GPIO_Pin_4,X)
#define		LED3(X)			GPIO_WriteBit(GPIOB,GPIO_Pin_5,X)




/*
		SIM_EN				PB13
		SIM_RST				PA4
		SIM_DTR				PA6

*/


//GSM休眠管脚
#define		SIM_EN(X)	GPIO_WriteBit(GPIOB,GPIO_Pin_13,X);
#define		SIM_RST(X)	GPIO_WriteBit(GPIOA,GPIO_Pin_4,X);
#define		SIM_DTR(X)	GPIO_WriteBit(GPIOA,GPIO_Pin_6,X);


/*

		GPS_EN				PC3
*/
		

#define		GPS_EN(X)	GPIO_WriteBit(GPIOC,GPIO_Pin_3,X)

/*
		OBD_EN				PB12
*/

#define		OBD_EN(X)	GPIO_WriteBit(GPIOB,GPIO_Pin_12,X)
#define		OBD_RST(X)	GPIO_WriteBit(GPIOB,GPIO_Pin_15,X)
#define		OBD_NVM(X)	GPIO_WriteBit(GPIOB,GPIO_Pin_14,X)


/*
		BLUE_EN				PA1
		BLUE_STATE			PB7	
		BLUE_CLR			PB6


		BLUE_MS				PB8	
		BLUE_HSW			PB9	
*/
#define		BLUE_EN(X)		GPIO_WriteBit(GPIOA,GPIO_Pin_1,X)
#define		BLUE_STATE		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7,X)
#define		BLUE_CLR(X)		GPIO_WriteBit(GPIOB,GPIO_Pin_7,X)

#define		BLUE_MS(X)		GPIO_WriteBit(GPIOB,GPIO_Pin_8,X)
#define		BLUE_HSW(X)		GPIO_WriteBit(GPIOB,GPIO_Pin_9,X)


	


//
//
//#define ENRECHARGE	 		GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET)
//#define DISRECHARGE	 		GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET)



typedef enum   
{
	working,   //0:未休眠获停机
	powerOff,  //1：关电源停机
	dtrSleep,	//3：DTR管脚休眠
	unsure,
} SLEEP_TYPE;



/*串口定义*/
extern int ISP_DIRECTION;
extern int BLUE_DIRECTION;

#define		USART_NONE		1
#define		USART_BLUE		4	//115200
#define		USART_SIM		2 	//115200
#define		USART_OBD		3	//38400
#define		USART_GPS		5   //9600		


extern	char USART1_RX_buffer[1300];	 //串口1接收缓存
extern	char USART2_RX_buffer[1300];	 //串口2接收缓存
extern	char USART3_RX_buffer[300];	 //串口3接收缓存
extern	char UART4_RX_buffer[300];	 //串口4接收缓存
extern	char UART5_RX_buffer[300];	 //串口5接收缓存


extern	unsigned int USART1_RX_buffer_count; //串口1接收缓存位置变量
extern	unsigned int USART2_RX_buffer_count; //串口2接收缓存位置变量
extern	unsigned int USART3_RX_buffer_count; //串口3接收缓存位置变量
extern	unsigned int UART4_RX_buffer_count; //串口4接收缓存位置变量
extern	unsigned int UART5_RX_buffer_count; //串口5接收缓存位置变量

#define ISP_BUF			USART1_RX_buffer
#define OBD_BUF			USART3_RX_buffer
#define SIM_BUF			USART2_RX_buffer
#define	BLUE_BUF		UART4_RX_buffer
#define	GPS_BUF			UART5_RX_buffer

#define ISP_COUNT		USART1_RX_buffer_count
#define OBD_COUNT		USART3_RX_buffer_count 
#define SIM_COUNT		USART2_RX_buffer_count 
#define	BLUE_COUNT		UART4_RX_buffer_count
#define	GPS_COUNT		UART5_RX_buffer_count

#define	ISP				USART1
#define	OBD				USART3
#define	SIM				USART2
#define	BLUE			UART4
#define	GPS				UART5

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

#define		IAP_PARAMETER			0x0803E800 //250
#define		SYS_PARAMETER			0x0803F000 //251

#define 	FLASH_START				0x08000000

#define 	SYS_MAIN				0x0800A000 //
#define		SYS_VECTOR				(SYS_MAIN-NVIC_VectTab_FLASH)	

#define		ApplicationAddress			0x0800A000
#define		NVIC_VectTab_FLASH_ADDR		ApplicationAddress-NVIC_VectTab_FLASH




#include "hard_init.h"
#include "sim900.h"
#include "flash.h"
//#include "gps.h"
//#include "sanyData.h"
#include "fifo.h"
//#include "flashFifo.h"
//#include "blueTooth.h"
#include "mainfunc.h"
//#include "obd.h"
#include "rtc.h"
//#include "at45db161d.h"
#include "crc.h"
#include "iap.h"


#endif


