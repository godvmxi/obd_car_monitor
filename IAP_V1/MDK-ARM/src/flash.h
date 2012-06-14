#include "sysdef.h"
#ifndef FLASH_H_H
#define FLASH_H_H





typedef struct
{
	uint32_t initFlag;//设置为0
	char G_CODE[8];	  	
	char IP[16];
	char PORT[8];  	
	char NET_CMD[64];
	int H_MODE;
	int L_MODE;
	int WORKING_MODE;

	int IP_OR_URL;//0:IP 1:URL
	char URL[40];
	char lockType;//0：未锁机 1：一级锁机 2：二级锁机 3:一级锁机失败，降低电压 4：二级锁机失败，降低电压
}DATA_IN_FLASH;//设置成4的倍数，方便读写



void writeFlash(uint32_t address,uint32_t inData,int flag);

int crcCheck(DATA_IN_FLASH *temp,int type);//type =0;计算crc，type =1校验crc


int16_t flashRollBack(uint32_t src,uint32_t des);
int writeFlashCfg(uint32_t address1,uint32_t address2,uint8_t *temp,uint32_t length,uint16_t crc);
void 	flashRead(uint32_t address,uint8_t *buf,uint16_t length);


int32_t WriteReadCheck(uint32_t address,uint8_t *point,uint16_t length,uint16_t crc);
/*******************************************************************************
 * 函数名称:updateFlashConfig                                                                     
 * 描    述: 更新配置参数                                                              
 *                                                                               
 * 输    入：任意参数设置为NULL，表示不升级该参数                                                          
 * 输    出:无                                                                    
 * 返    回: 负:升级错误 1：正确                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年5月16日                                                                    
 *******************************************************************************/
int16_t updateFlashConfig(SOCKET *net,SOCKET *iap,IAP_BRIFE *brife,BLUE_CONFIG *blue,OBD_CONFIG *obd);
/*******************************************************************************
 * 函数名称:calBufCrc                                                                     
 * 描    述: 计算crc16                                                              
 *                                                                               
 * 输    入：任意参数设置为NULL，表示不升级该参数                                                          
 * 输    出:无                                                                    
 * 返    回: 负:升级错误 1：正确                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年5月16日                                                                    
 *******************************************************************************/
uint16_t calBufCrc(uint8_t *ptr, uint32_t len);
uint16_t calFlashCrc(uint32_t address,uint32_t length);


void chipFlashTest(void);
#define	FLASH_ADR	0x0800F000	//要写入数据的地址
#define	FLASH_FIFO	0x0800EC00
#define sysStateInFlash volatile (DATA_IN_FLASH *)FLASH_ADR













#endif
