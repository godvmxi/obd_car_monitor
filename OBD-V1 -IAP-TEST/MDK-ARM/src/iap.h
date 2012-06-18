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
//}IAP_START;	//IAP ��������
//typedef struct {
//	uint8_t 	start[4];
//
//	uint16_t	crc;//���ݰ�crc
//	uint8_t		hv;
//	uint8_t		sv;	
//		
//	uint16_t	packetNum;
//	uint16_t	crcApp;	//����APP��CRCУ��
//	uint32_t	sizeOfApp; //����APP�������С��Ϣ
//}IAP_BRIFE;	//IAP ��������
//
//typedef struct {
//	uint8_t 	start[4];
//	uint16_t	crc;
//	int16_t	index;
//	uint8_t		other[4];
//}IAP_GET;	//IAP ��������
//
//typedef struct {
//	uint8_t         start[4];
//	uint16_t		crc;	//���ݰ�+����crc
//	uint16_t		index;//��ǰ���ı��
//	uint16_t		length;	//��ǰ���ĳ���
//	uint16_t		crcData;	//���ݰ�����crc������Ϊlength
//}IAP_DATA;	//IAP ���ݷ���


void iap(void);
void usartIap(void);
void tcpIap(void);
int16_t checkIapType(void);
void iapHardwareInit(void);
int16_t checkAppConfig(void);
int16_t checkAppState(void);
/*******************************************************************************
 * ��������:checkIapType                                                                     
 * ��    ��: IAP��������ѡ��                                                               
 *                                                                               
 * ��    ��:��                                                          
 * ��    ��:��                                                                    
 * ��    ��: ��:ǿ��ʹ��USART����  ���ࣺ�������ϵͳ��ȷ���Ƿ�����                                                    
 * ��    ��:����                                                                     
 * �޸�����:2012��5��9��                                                                    
 *******************************************************************************/
int16_t tcpStart(SOCKET net,IAP_BRIFE *brife);
/*******************************************************************************
 * ��������:tcpStart                                                                     
 * ��    ��: ���½���TCP��֤����������Ӧ������                                                               
 *                                                                               
 * ��    ��:��                                                          
 * ��    ��:��                                                                    
 * ��    ��: ��:ǿ��ʹ��USART����  ���ࣺ�������ϵͳ��ȷ���Ƿ�����                                                    
 * ��    ��:����                                                                     
 * �޸�����:2012��5��9��                                                                    
 *******************************************************************************/
int16_t tcpGetData(SOCKET net,IAP_BRIFE *brife,int16_t index);
/*******************************************************************************
 * ��������:checkAppCrc                                                                     
 * ��    ��: ���½���TCP��֤����������Ӧ������                                                               
 *                                                                               
 * ��    ��:��                                                          
 * ��    ��:��                                                                    
 * ��    ��: ��:ǿ��ʹ��USART����  ���ࣺ�������ϵͳ��ȷ���Ƿ�����                                                    
 * ��    ��:����                                                                     
 * �޸�����:2012��5��9��                                                                    
 *******************************************************************************/
int16_t		checkAppCrc(IAP_BRIFE brife);
void  setDebugPara(void);





#endif


