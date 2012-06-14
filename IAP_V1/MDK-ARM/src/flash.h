#include "sysdef.h"
#ifndef FLASH_H_H
#define FLASH_H_H





typedef struct
{
	uint32_t initFlag;//����Ϊ0
	char G_CODE[8];	  	
	char IP[16];
	char PORT[8];  	
	char NET_CMD[64];
	int H_MODE;
	int L_MODE;
	int WORKING_MODE;

	int IP_OR_URL;//0:IP 1:URL
	char URL[40];
	char lockType;//0��δ���� 1��һ������ 2���������� 3:һ������ʧ�ܣ����͵�ѹ 4����������ʧ�ܣ����͵�ѹ
}DATA_IN_FLASH;//���ó�4�ı����������д



void writeFlash(uint32_t address,uint32_t inData,int flag);

int crcCheck(DATA_IN_FLASH *temp,int type);//type =0;����crc��type =1У��crc


int16_t flashRollBack(uint32_t src,uint32_t des);
int writeFlashCfg(uint32_t address1,uint32_t address2,uint8_t *temp,uint32_t length,uint16_t crc);
void 	flashRead(uint32_t address,uint8_t *buf,uint16_t length);


int32_t WriteReadCheck(uint32_t address,uint8_t *point,uint16_t length,uint16_t crc);
/*******************************************************************************
 * ��������:updateFlashConfig                                                                     
 * ��    ��: �������ò���                                                              
 *                                                                               
 * ��    �룺�����������ΪNULL����ʾ�������ò���                                                          
 * ��    ��:��                                                                    
 * ��    ��: ��:�������� 1����ȷ                                                    
 * ��    ��:����                                                                     
 * �޸�����:2012��5��16��                                                                    
 *******************************************************************************/
int16_t updateFlashConfig(SOCKET *net,SOCKET *iap,IAP_BRIFE *brife,BLUE_CONFIG *blue,OBD_CONFIG *obd);
/*******************************************************************************
 * ��������:calBufCrc                                                                     
 * ��    ��: ����crc16                                                              
 *                                                                               
 * ��    �룺�����������ΪNULL����ʾ�������ò���                                                          
 * ��    ��:��                                                                    
 * ��    ��: ��:�������� 1����ȷ                                                    
 * ��    ��:����                                                                     
 * �޸�����:2012��5��16��                                                                    
 *******************************************************************************/
uint16_t calBufCrc(uint8_t *ptr, uint32_t len);
uint16_t calFlashCrc(uint32_t address,uint32_t length);


void chipFlashTest(void);
#define	FLASH_ADR	0x0800F000	//Ҫд�����ݵĵ�ַ
#define	FLASH_FIFO	0x0800EC00
#define sysStateInFlash volatile (DATA_IN_FLASH *)FLASH_ADR













#endif
