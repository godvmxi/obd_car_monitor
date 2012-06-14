#include "flash.h"
//chip flash 

uint16_t calBufCrc(uint8_t *ptr, uint32_t len) {
	uint16_t crc;
	uint8_t da;
	
	const uint16_t crc_ta[256]={ /* CRC余式表 */
		0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
		0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
		0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
		0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
		0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
		0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
		0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
		0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
		0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
		0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
		0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
		0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
		0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
		0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
		0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
		0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
		0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
		0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
		0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
		0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
		0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
		0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
		0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
		0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
		0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
		0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
		0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
		0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
		0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
		0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
		0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
		0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
	};
	crc=0;
//	printf("\r\nCAL BUF CRC IN PROCESS :CRC LENGTH :%d  \r\n\r\n",len);
	while(len--!=0) {
		da=(uint8_t) (crc/256); /* 以8位二进制数的形式暂存CRC的高8位 */
		crc<<=8; /* 左移8位,相当于CRC的低8位乘以 */
		crc^=crc_ta[da^*ptr]; /* 高8位和当前字节相加后再查表求CRC ,再加上以前的CRC */
//		printf("%3X",*ptr);
		ptr++;
	}
//	printf("\r\n\r\n-->%3X\r\n",crc);
	return(crc);
}

uint16_t calFlashCrc(uint32_t address,uint32_t length){
	uint16_t crc;
	uint8_t da;
	uint32_t i = 0;
	uint8_t buf;
	const uint16_t crc_ta[256]={ /* CRC余式表 */
		0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
		0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
		0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
		0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
		0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
		0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
		0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
		0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
		0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
		0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
		0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
		0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
		0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
		0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
		0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
		0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
		0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
		0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
		0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
		0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
		0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
		0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
		0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
		0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
		0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
		0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
		0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
		0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
		0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
		0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
		0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
		0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
	};
	crc=0;

//	printf("\r\nCAL FLASH CRC IN PROCESS :CRC LENGTH :%d  \r\n\r\n",length);
	for(i = 0;i <length ;i++)
	{
		buf = *((vu32 *)(address + i));
//		if(i %16 == 0){
//			printf("\r\n");
//		}
//		if(i % 1024 == 0){
//			printf("\r\n\r\n+++++++++++++++++++++++++++0x%X  ---->   %d++++++++++++++++++++++++++++++++\r\n\r\n",address+i ,(i)/1024);
//		}
//		printf("%3X",buf);
		
		da=(uint8_t) (crc/256); /* 以8位二进制数的形式暂存CRC的高8位 */
		crc<<=8; /* 左移8位,相当于CRC的低8位乘以 */
		crc^=crc_ta[da^buf]; /* 高8位和当前字节相加后再查表求CRC ,再加上以前的CRC */
//		if(i %16 == 0){
//			printf("\r\n");
//		}
//		if(i % 512 == 0){
//			printf("\r\n\r\n+++++++++++++++++++++++++++0x%X  ----> %d  -->%X++++++++++++++++++++++++++++++++\r\n\r\n",address+i ,(i)/512,crc);
//		}
//		printf("%3X",buf);
		
	}
//	printf("\r\n\r\n-->%3X\r\n",crc);
	return(crc);
}
//void initFlashData(DATA_IN_FLASH *inData)
//{
//	memset(inData,'\0',sizeof(DATA_IN_FLASH));
//}


/*******************************************************************************
 * 函数名称:flashWrite                                                                     
 * 描    述:lflash 写入功能                                                                    
 *                                                                               
 * 输    入:Address: 写入地址 	point：待写入的缓冲区数据,需要   len:写入数据长度                                                                 
 * 输    出:无                                                                    
 * 返    回:无                                                                     
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月1日                                                                    
 *******************************************************************************/
void	flashWrite(uint32_t Address,uint8_t *point, uint16_t len,int flag)
{
	uint32_t Data;
	uint16_t i;
//	printf("\r\n????????????????????????????????????????????????????????\r\n");
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	if(flag != 0){
		FLASH_ErasePage(Address);
		printf("clear current page :0x%X",Address);
	}

//	printf("\r\n????????????????????????????????????????????????????????\r\n");
	if( (len % 4) != 0)//空白区填充0xFF
	{
		for(i = 0; i < (4 - (len % 4)); i++)
		{
			*(point + i + len) = 0xff;
		}
		len +=(4 - (len % 4)); 
	}
	
	for(i = 0; i < len; i+=4)
	{
		Data = 0;
		Data |= *(point + i + 3);
		Data = Data << 8;
		Data |= *(point + i + 2);
		Data = Data << 8;
		Data |= *(point + i + 1);
		Data = Data << 8;
		Data |= *(point + i);
		FLASH_ProgramWord( (Address + i),Data);

	}
	FLASH_Lock();
//	printf("\r\n??????????????????FLASH WRITE OK :0x%X?????????????????????\r\n",Address);
}
/*******************************************************************************
 * 函数名称:flashRead                                                                     
 * 描    述:读出flash数据功能                                                                    
 *                                                                               
 * 输    入:Address: 读写地址 	point：待读出的缓冲区数据   len:写入数据长度                                                                 
 * 输    出:无                                                                    
 * 返    回:无                                                                     
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月1日                                                                    
 *******************************************************************************/
void 	flashRead(uint32_t address,uint8_t *buf,uint16_t length){
	uint16_t i = 0;
//	printf("\r\n\r\n+++++++++++++++++++++++++++++++++++++++++++++--->0x%X\r\n",address);
	for(i = 0;i <length ;i++)
	{
		buf[i] = *((vu32 *)(address + i));
	}
//	printf("\r\n\r\n+++++++++READ DATA OK :0x%X++++++++++++++++\r\n",address);
//	delay_ms(2000);

	
}
/*******************************************************************************
 * 函数名称:flashRead                                                                     
 * 描    述: 该函数通过传入的crc校验数据是否正确，然后写入flash，并从flash中回读数据计算crc，确保写入正确                                                                   
 *                                                                               
 * 输    入:Address: 读写地址 	point：待写入的缓冲区数据   len:写入数据长度  crc：写入数据的crc校验值                                                                
 * 输    出:无                                                                    
 * 返    回: -1 :输出数据crc校验失败 ，-2：写入数据crc校验失败  1：cr写入读出校验成功                                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月1日                                                                    
 *******************************************************************************/
int32_t WriteReadCheck(uint32_t address,uint8_t *point,uint16_t length,uint16_t crc){
//	uint8_t buf[1300];
//	int i = 0;
//
//		printf("\r\n\r\n\r\nWRITE INTO FLASH DATA::::\r\n");
//		for(i = 0 ;i < length;i++)
//		{
//			if(i %16 == 0){
//				printf("\r\n");
//			}
//			printf("%3X",point[i]);
//		}
//		printf("\r\n\r\n\r\n");
	
	
	if(crc != calBufCrc(point,length)){
//		printf("\r\nbuf data crc error ,current data abort,please retry!!!\r\n");
		return -1;
	}

	if((address - FLASH_START)%2048 == 0){
//		printf("\r\ncurent need be clear:0x%X  ---> %X\r\n",address,(address - FLASH_START)%2);
		flashWrite(address,point,length,1);
	}
	else
	{
		flashWrite(address,point,length,0);	
	}

	
//	delay_ms(1000);
//	memset(buf,0xFF,1300);
	if(crc != calFlashCrc(address,length)){
//		printf("\r\ncal flash crc error :%X\r\n",calFlashCrc(address,length));
		printf("\r\ncal flash crc error :%X\r\n",crc);
		printf("\r\ncal flash crc error\r\n");
		printf("\r\ncal flash crc error\r\n");
		printf("\r\ncal flash crc error\r\n");
	   	return -2;
	}
//	flashRead(address,buf,length);


//	printf("\r\n\r\nLOCAL CRC :%6X\r\n",crc);


//	if(crc != calBufCrc(buf,length)) {
//		printf("\r\nread flash data error:\r\n");
//
//		return -2;
//	}
//	printf("\r\nwrite read check flash ok+++++\r\n");

	return 1;
}

void chipFlashTest(void){
	uint8_t in[1024],out[1024];
	uint32_t address = ApplicationAddress;
	uint32_t i = 0,j=0,flag = 0;
	for(i=0;i<5000;i++){
		memset(in,i,1024);
		flashWrite(address,in,1024,1);
		flashRead(address,out,1024);
		for(j=0;j<1000;j++){
			if(in[j]  !=  out[j]) {
				flag = 1;
			}
		}
		if(flag == 1){
			printf("\r\naddress error :%X\r\n",address);
		}
		else{
			printf("\r\naddress ok :%X\r\n",address);
		}
		flag = 0;		
		address += 0x400;
		delay_ms(1000);
	}
}

//int readFlashCfg(uint32_t address,uint8_t *temp,uint32_t length);
//int writeFlashCfg(uint32_t address,uint8_t *temp,uint32_t length);
/*******************************************************************************
 * 函数名称:writeFlashCfg                                                                     
 * 描    述: 该函数通过传入的crc校验数据是否正确，然后写入flash，并从flash中回读数据计算crc，确保写入正确                                                                   
 *                                                                               
 * 输    入:Address: 读写地址 	point：待写入的缓冲区数据   len:写入数据长度  crc：写入数据的crc校验值                                                                
 * 输    出:无                                                                    
 * 返    回: -1 :输出数据crc校验失败 ，-2：写入数据crc校验失败  1：cr写入读出校验成功                                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月1日                                                                    
 *******************************************************************************/
int writeFlashCfg(uint32_t address1,uint32_t address2,uint8_t *temp,uint32_t length,uint16_t crc)
{ 
	uint16_t  i = 0,j=0;
	printf("\r\nflash address :%10X     %10X\r\n",address1,address2);
	while(1){
		if(WriteReadCheck(address1,temp,length,crc) > 0){
			printf("\r\nconfig write ok : %X\r\n",address1);
			break;
		}
		
		if(crc == calBufCrc(temp,length) && i < 4){
			continue;
		}
		else
		{
			if(flashRollBack(address2,address1) < 0){
				for(j = 0;j< 50;j++){
					printf("\r\nsystem fault error ，roll back error ,from %X to %X\r\n",address2,address1);
					delay_ms(5000);
				}
				//system reset
			}
			else{
				printf("\r\nupdate config fail ,roll back success!!\r\n");
				printf("system reboot");
				delay_ms(1000);
				__disable_irq();//关总中断
				__disable_fault_irq();
				delay_ms(2000);
				NVIC_SystemReset();
			}
		}
	}

	while(1){
		if(WriteReadCheck(address2,temp,length,crc) > 0){
			printf("\r\nconfig write ok : %X\r\n",address2);
			break;
		}
		
		if(crc == calBufCrc(temp,length) && i < 4){
			continue;
		}
		else
		{
			if(flashRollBack(address1,address2) < 0){
				for(j = 0;j< 50;j++){
					printf("\r\nsystem fault error ，roll back error ,from %X to %X\r\n",address1,address2);
					delay_ms(5000);
				}
				//system reset
			}
			else{
				printf("\r\nupdate config fail ,roll back success!!\r\n");
				printf("system reboot");
				delay_ms(1000);
				__disable_irq();//关总中断
				__disable_fault_irq();
				delay_ms(2000);
				NVIC_SystemReset();
			}
		}
	}
	printf("\r\nconfig renew ok\r\n");
	return 1;

}
/*******************************************************************************
 * 函数名称:flashRead                                                                     
 * 描    述: 该函数通过传入的crc校验数据是否正确，然后写入flash，并从flash中回读数据计算crc，确保写入正确                                                                   
 *                                                                               
 * 输    入:Address: 读写地址 	point：待写入的缓冲区数据   len:写入数据长度  crc：写入数据的crc校验值                                                                
 * 输    出:无                                                                    
 * 返    回: -1 :输出数据crc校验失败 ，-2：写入数据crc校验失败  1：cr写入读出校验成功                                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月1日                                                                    
 *******************************************************************************/

int16_t flashRollBack(uint32_t srx,uint32_t des){
	return 1;

}
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
//typedef struct {
//	uint16_t crc;
//	uint16_t flag;
//	SOCKET netConfig;
//	SOCKET iapConfig;
//	IAP_BRIFE iapBirfe;	
//	BLUE_CONFIG blueConfig;
//	OBD_CONFIG obdConfig;
//}FLASH_CONFIG;
int16_t updateFlashConfig(SOCKET *net,SOCKET *iap,IAP_BRIFE *brife,BLUE_CONFIG *blue,OBD_CONFIG *obd){
	FLASH_CONFIG updateFlash;
	int i = 0;
	int8_t *p = (int8_t *)(&updateFlash);
	flashRead(IAP_PARAMETER,(uint8_t *)&updateFlash,sizeof(FLASH_CONFIG));
	if(net != NULL){
		updateFlash.netConfig = *net;
	}
	if(iap != NULL){
		updateFlash.iapConfig = *iap; 
	}
	if(brife != NULL){
		updateFlash.iapBrife = *brife; 
	}
	if(blue != NULL){
		updateFlash.blueConfig = *blue;	
	}
	if(obd != NULL){
		updateFlash.obdConfig = *obd;  
	}
	updateFlash.flag = 0x5555;//标记为
	updateFlash.crc = calBufCrc(p + 2,sizeof(FLASH_CONFIG)-2);
//	printf("\r\nflash should be\r\n");
//	showConfig(&updateFlash);
//	printf("\r\nin crc :  %6X\r\n",calBufCrc(p,sizeof(FLASH_CONFIG)));
//	for(i = 0;i<sizeof(FLASH_CONFIG);i++){
//		printf("%3X",p[i]);
//	}
	if(writeFlashCfg(IAP_PARAMETER,SYS_PARAMETER,p,sizeof(FLASH_CONFIG),calBufCrc(p,sizeof(FLASH_CONFIG))) < 0){
		printf("\r\n parameter update error,do nothing\r\n");
		return -1;
	}
	else
	{
		printf("\r\nparameter update success\r\n");
		memset(&updateFlash,0,sizeof(FLASH_CONFIG));
		printf("\r\nflash now be\r\n");
		flashRead(IAP_PARAMETER,p,sizeof(FLASH_CONFIG));
//		readFlashCfg(IAP_PARAMETER,(uint8_t *)&updateFlash,sizeof(FLASH_CONFIG));
		showConfig(&updateFlash);
		return 1;
	}
}
	



