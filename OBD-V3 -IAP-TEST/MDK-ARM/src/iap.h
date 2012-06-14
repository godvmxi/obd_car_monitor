#include "sysdef.h"
#ifndef IAP_H_H
#define IAP_H_H

//typedef struct {
//	uint8_t 	start[4];
//	uint16_t 	crc;	
//	uint8_t		hv;
//	uint8_t		sv;
//	uint32_t	imeiHigh;
//	uint32_t	imeiLow;
//}IAP_START;	//IAP 参数请求
//typedef struct {
//	uint8_t 	start[4];
//
//	uint16_t	crc;//数据包crc
//	uint8_t		hv;
//	uint8_t		sv;	
//		
//	uint16_t	packetNum;
//	uint16_t	crcApp;	//整个APP的CRC校验
//	uint32_t	sizeOfApp; //整个APP的体积大小信息
//}IAP_BRIFE;	//IAP 参数返回
//
//typedef struct {
//	uint8_t 	start[4];
//	uint16_t	crc;
//	int16_t	index;
//	uint8_t		other[4];
//}IAP_GET;	//IAP 数据请求
//
//typedef struct {
//	uint8_t         start[4];
//	uint16_t		crc;	//数据包+数据crc
//	uint16_t		index;//当前包的编号
//	uint16_t		length;	//当前包的长度
//	uint16_t		crcData;	//数据包内容crc，长度为length
//}IAP_DATA;	//IAP 数据返回


void iap(void);
void usartIap(void);
void tcpIap(void);
int16_t checkIapType(void);
void iapHardwareInit(void);
int16_t checkAppConfig(void);
int16_t checkAppState(void);
/*******************************************************************************
 * 函数名称:checkIapType                                                                     
 * 描    述: IAP升级方法选择                                                               
 *                                                                               
 * 输    入:无                                                          
 * 输    出:无                                                                    
 * 返    回: 负:强制使用USART升级  其余：正常检查系统并确定是否升级                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年5月9日                                                                    
 *******************************************************************************/
int16_t tcpStart(SOCKET net,IAP_BRIFE *brife);
/*******************************************************************************
 * 函数名称:tcpStart                                                                     
 * 描    述: 重新进行TCP认证，并返回相应的数据                                                               
 *                                                                               
 * 输    入:无                                                          
 * 输    出:无                                                                    
 * 返    回: 负:强制使用USART升级  其余：正常检查系统并确定是否升级                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年5月9日                                                                    
 *******************************************************************************/
int16_t tcpGetData(SOCKET net,IAP_BRIFE *brife,int16_t index);
/*******************************************************************************
 * 函数名称:checkAppCrc                                                                     
 * 描    述: 重新进行TCP认证，并返回相应的数据                                                               
 *                                                                               
 * 输    入:无                                                          
 * 输    出:无                                                                    
 * 返    回: 负:强制使用USART升级  其余：正常检查系统并确定是否升级                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年5月9日                                                                    
 *******************************************************************************/
int16_t		checkAppCrc(IAP_BRIFE brife);
void  setDebugPara(void);





#endif


