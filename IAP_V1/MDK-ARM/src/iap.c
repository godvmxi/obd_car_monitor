#include "iap.h"
extern SIM_STRUCT simState;


/*******************************************************************************
 * 函数名称:iap                                                                     
 * 描    述: IAP升级工程                                                               
 *                                                                               
 * 输    入:无                                                          
 * 输    出:无                                                                    
 * 返    回: 无                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年5月9日                                                                    
 *******************************************************************************/
typedef  void (*pFunction)(void);
pFunction Jump_To_Application;

void iapfunc(void){
	
	uint32_t JumpAddress;
	iapHardwareInit();
	
	printf("\r\nIAP KEY TEST\r\n");
	if(checkIapType() < -10){
		LED1(Bit_SET);
		LED3(Bit_RESET);
		LED2(Bit_RESET);
		printf("\r\nbegin pc iap upgrade\r\n");
//		delay_ms(2000);
		usartIap();

	}
	else
	{
		if((checkAppState() < 0) || (BKP_ReadBackupRegister(BKP_DR4) == 0x55)){
			LED1(Bit_RESET);
			LED2(Bit_RESET);
			LED3(Bit_SET);
			printf("\r\nbegin net iap upgrade\r\n");
			delay_ms(2000);
			ISP_DIRECTION=USART_SIM; 
			tcpIap();
			if(updateFlashConfig(NULL,NULL,&sysCfg.iapBrife,NULL,NULL) <0)
				printf("\r\nupdate iapBrife ok\r\n");
			else
				printf("\r\nupdate iapBrife error\r\n");
			BKP_WriteBackupRegister(BKP_DR4, 0);
		}		
	}

	
	if(checkAppCrc(&(sysCfg.iapBrife)) < 0){
		printf("\r\nFLASH CRC CHECK ERROR\r\n");
		__disable_irq();//关总中断
		__disable_fault_irq();
		delay_ms(2000);
		NVIC_SystemReset();
	}
	//goto obd app

	
	printf("\r\n++++++++++++++++++++++++++++++go to obd app++++++++++++++++++++++++++++++++++\r\n");
	printf("\r\n++++++++++++++++++++++++++++++go to obd app++++++++++++++++++++++++++++++++++\r\n");
	printf("\r\n++++++++++++++++++++++++++++++go to obd app++++++++++++++++++++++++++++++++++\r\n");
	printf("\r\n++++++++++++++++++++++++++++++go to obd app++++++++++++++++++++++++++++++++++\r\n");
	printf("\r\n++++++++++++++++++++++++++++++go to obd app++++++++++++++++++++++++++++++++++\r\n");
	
	__disable_irq();//关总中断
	__disable_fault_irq();
	delay_ms(2000);
	//jump to obd app
	JumpAddress = *(__IO uint32_t*) (ApplicationAddress+4);
	Jump_To_Application = (pFunction) JumpAddress;
	__set_MSP(*(__IO uint32_t*) ApplicationAddress);
	Jump_To_Application();	
	printf("\r\ngo to obd app++++++++++++++++++++++++++++++++++\r\n");
	__disable_irq();//关总中断
	__disable_fault_irq();
	delay_ms(2000);
	//jump to obd app
	JumpAddress = *(__IO uint32_t*) (ApplicationAddress+4);
	Jump_To_Application = (pFunction) JumpAddress;
	__set_MSP(*(__IO uint32_t*) ApplicationAddress);
	Jump_To_Application();		
}
/*******************************************************************************
 * 函数名称:iap                                                                     
 * 描    述: IAP升级工程                                                               
 *                                                                               
 * 输    入:无                                                          
 * 输    出:无                                                                    
 * 返    回: 无                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年5月9日                                                                    
 *******************************************************************************/
int16_t usartGetBrife(IAP_BRIFE *brife){

	IAP_START iapStart;
	int i = 0;

	int8_t *p = NULL,*q=(int8_t *)brife; 
	uint8_t *pointer = (uint8_t *)(&iapStart);

	LED1(Bit_SET);
	LED2(Bit_SET);
	LED3(Bit_SET);
	iapStart.start[0] = '#';
	iapStart.start[1] = '#';
//	
//	iapStart.imeiHigh = simState.imeiHigh;
//	iapStart.imeiLow  = simState.imeiLow;
	iapStart.imeiHigh = 0;
	iapStart.imeiLow  = 0;
	iapStart.hv		= 0;
	iapStart.sv		= 0;
	iapStart.typeOrIndex = 0;

	iapStart.crc = calBufCrc(iapStart.start+4,sizeof(IAP_START)-4);

	memset(ISP_BUF,'\0',1300);
	ISP_COUNT = 0;
//	printf("\r\nDATA :\r\n");
//	for(i = 0;i<sizeof(IAP_START);i++)
//	{
//	 	printf("%3X",pointer[i]);
//	}
//	printf("\r\n\r\n");
	for(i= 0;i<sizeof(IAP_START);i++){
		USART_SendData(ISP,pointer[i]);	
		while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);

	}
	delay_ms(3000);

	p = strstr(ISP_BUF,"##");


	if(p != NULL)
	{
		for(i = 0;i < sizeof(IAP_BRIFE);i++){
			q[i] = p[i];

		}
		printf("\r\nSIM BUFFER :\r\n");
		for(i = 0;i< 200;i++){
			printf("%3X",SIM_BUF[i]);
		}
		
		printf("\r\ncrc         :%X\r\n",brife->crc);
		printf("\r\ntypeOrIndex :%X\r\n",brife->typeOrIndex);
		printf("\r\nhv          :%X\r\n",brife->hv);
		printf("\r\nsv          :%X\r\n",brife->sv);
		printf("\r\npacketNum   :%X\r\n",brife->packetNum);
		printf("\r\nappCrc      :%X\r\n",brife->appCrc);
		printf("\r\nsize        :%X\r\n",brife->sizeOfApp);



		printf("\r\nready for crc data :\r\n");
		for(i=0;i<sizeof(IAP_BRIFE);i++)
		{
		 	printf("%3X",brife->start[i]);
		}
		printf("\r\nlocal crc :%3X  ->desCrc :%3X\r\n",calBufCrc(brife->start + 4,sizeof(IAP_BRIFE)-4),brife->crc);
		if(brife->crc == calBufCrc(brife->start + 4,sizeof(IAP_BRIFE)-4)){
			printf("\r\nget iap brife data ok\r\n");
			return 1;				
		}
		else{
			printf("\r\ncrc error\r\n");
			delay_ms(2000);
			return -1;
		}
	}
	else{
		return -1;
	}
}
int16_t usartGetData(IAP_BRIFE *brife,int index){

	IAP_START iapStart;
	IAP_DATA  *iapData;
	uint32_t address ; 
	int i = 0;
	static uint8_t appBuf[1024];
	uint8_t   *pointer = (uint8_t *)&iapStart;
	uint16_t errorCounter  = 0;
	uint8_t *temp; 



	iapStart.start[0] = '#';
	iapStart.start[1] = '#';
	
	iapStart.typeOrIndex = index;

	iapStart.imeiHigh = 0;
	iapStart.imeiLow  = 0;
	iapStart.hv		= 1;
	iapStart.sv		= 1;
	iapStart.crc = calBufCrc(iapStart.start+4,sizeof(IAP_START)-4);
	
	memset(ISP_BUF,'\0',1300);
	ISP_COUNT = 0;
	for(i= 0;i<sizeof(IAP_START);i++){
		USART_SendData(ISP,pointer[i]);	
		while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);

	}
	delay_ms(2000);
	//wait for data
	pointer = strstr(ISP_BUF,"##");
	if(pointer != NULL)
	{
		//reset sim
		iapData = (IAP_DATA *)pointer;
//		printf("\r\ncrc			:%X\r\n",iapData->crc);
//		printf("\r\ntypeOrIndex	:%d\r\n",iapData->typeOrIndex);
//		printf("\r\nhv			:%X\r\n",iapData->hv);
//		printf("\r\nsv			:%X\r\n",iapData->sv);
//		printf("\r\npCrc	:%X\r\n",iapData->packetCrc);
//		printf("\r\npacketLength:%d\r\n",iapData->packetLength);

//		printf("\r\nCAL :%X -->SRC CRC %4X\r\n",calBufCrc(iapData->start + 4,sizeof(IAP_BRIFE)-4),iapData->crc);
		temp = iapData->app + 4;
//		for(i = 0;i<512;i++)
//		{
//			if(i % 16 ==0)
//					printf("\r\n");
//			printf("%3X",temp[i]);
//
//		}

//		delay_ms(3000);	
				
		
		if(iapData->crc == calBufCrc(iapData->start + 4,sizeof(IAP_DATA)-4))
		{			
			printf("\r\nget iap app data ok :%d\r\n",index);			
			pointer = iapData->app + 4;
//			for(i = 0;i<100;i++){
//				printf("%3X",pointer[i]);
//			}

			if(iapData->packetCrc != calBufCrc(pointer,iapData->packetLength)){
				printf("\r\napp crc error\r\n");
				return -1;
			}
			if(index %2 != 0)
			{//奇数包
				memset(appBuf,0,1024);
				memcpy(appBuf,pointer,iapData->packetLength);
				if(iapData->packetCrc != calBufCrc(appBuf,iapData->packetLength)){
					printf("\r\n\r\ncopy data error:%d\r\n",index);
					return -1;
				}
				else
				{
					// 注意最后一个包
					if(index < brife->packetNum)
						return 1;
					else{//最后一个包
//						printf("\r\nWRITE LAST FLASH : %d\r\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n\r\n",index);
						address  = ApplicationAddress + 1024*(index/2);
						if(WriteReadCheck(address,appBuf,iapData->packetLength,iapData->packetCrc) < 0 ){				
						   	printf("\r\nwrite flash error\r\n");
							delay_ms(1000);
							return -1;	 
						}
						else
						{
							printf("\r\nWRITE IN BUF :%d\r\n",index);
							return 1;
						}
					}
				}	
			}
			else
			{
//			 	printf("\r\nbuf data first 512 :++++++++++++++++++\r\n\r\n");
//				for(i = 0;i<512;i++)
//				{
//					if(i % 16 ==0)
//							printf("\r\n");
//					printf("%3X",appBuf[i]);
//		
//				}
				

				memcpy(appBuf+512,pointer,iapData->packetLength);

//				printf("\r\nbuf data first 512 :++++++++++++++++++\r\n\r\n");
//				for(i = 0;i<512;i++)
//				{
//					if(i % 16 ==0)
//							printf("\r\n");
//					printf("%3X",appBuf[i]);
//		
//				}

//				printf("\r\nbuf data second 512 :++++++++++++++++++\r\n\r\n");
//				for(i = 0;i<512;i++)
//				{
//					if(i % 16 ==0)
//							printf("\r\n");
//					printf("%3X",pointer[i+512]);
//		
//				}
//				printf("\r\nWRITE FLASH : %d\r\n",index);
				if(iapData->packetCrc != calBufCrc(appBuf+512,iapData->packetLength))
				{
					printf("\r\n\r\ncopy data error:%d\r\n",index);
					return -1;
				}
				address  = ApplicationAddress + 1024*(index/2 - 1);
				if(WriteReadCheck(address,appBuf,iapData->packetLength +512,calBufCrc(appBuf,iapData->packetLength +512)) < 0 )
				{				
				   	printf("\r\nwrite flash error\r\n");
					delay_ms(1000);
					return -1;	 
				}
				else{

//					printf("\r\n\r\nwrite flash ok\r\n");
					return 1;

				}
			}				
		}
		else{
			printf("\r\ncrc error\r\n");
			delay_ms(2000);
			return -1;
		}
	}
	else
	{
		return -1;
	
	}	
			
	
	

}
void usartIap(void){
//	IAP_BRIFE iapBrife;
	IAP_DATA  *iapDate;
	int16_t currentIndex = 1;
	int16_t reTryCounter = 0;
	uint32_t JumpAddress;
	openUsart1();
//	errorStart:
//	currentIndex = 1;
	while(1){
		if(usartGetBrife(&sysCfg.iapBrife) > 0){
//			printf("\r\nGET BRIFE DATA!\r\n");
			if(updateFlashConfig(NULL,NULL,&sysCfg.iapBrife,NULL,NULL) >0)
				printf("\r\nupdate iapBrife ok\r\n");
			else
			{
				printf("\r\nupdate iapBrife error\r\n");
				continue;
			}
			break;
		}
		else
		{
			LED3(Bit_RESET);
			delay_ms(300);
			LED3(Bit_SET);
			delay_ms(300);

		}	
	};
	while(1){
		
		if(usartGetData(&sysCfg.iapBrife,currentIndex) < 0)
		{
//			printf("\r\nINDEX ERROR ,RETRY :%d\r\n",currentIndex);
			reTryCounter++;
//			if(reTryCounter > 5){
//				reTryCounter = 0;
//				goto errorStart;
//			}
			LED1(Bit_RESET);
			delay_ms(300);
			LED1(Bit_SET);
			delay_ms(300);
//			LED1(Bit_RESET);
//			delay_ms(300);
//			LED1(Bit_SET);
//			delay_ms(300);
//			LED1(Bit_RESET);
//			delay_ms(300);
//			LED1(Bit_SET);
//			LED1(Bit_RESET);
//			delay_ms(300);
//			LED1(Bit_SET);
//			delay_ms(300);
//			LED1(Bit_RESET);
//			delay_ms(300);
//			LED1(Bit_SET);
//			delay_ms(300);
//			LED1(Bit_RESET);
//			delay_ms(300);
//			LED1(Bit_SET);
		}
		else
		{
			currentIndex++;
			reTryCounter = 0;

//			printf("\r\nall packet number is :%d++++++++++++++++\r\n",sysCfg.iapBrife.packetNum);
//			printf("\r\nNEXT PACKET NUM :%d++++++++++++++++\r\n",currentIndex);
//			printf("\r\n app crc :%X\r\n",sysCfg.iapBrife.appCrc);
//			printf("\r\n app size :%X\r\n",sysCfg.iapBrife.sizeOfApp);
			if(currentIndex > sysCfg.iapBrife.packetNum)
			{
//				currentIndex = 1;
				//write ok
				//check whether ok
				//break and goto new main
				if(checkAppCrc(&(sysCfg.iapBrife)) < 0){
					printf("\r\nFLASH CRC CHECK ERROR\r\n");
					__disable_irq();//关总中断
					__disable_fault_irq();
					delay_ms(2000);
					NVIC_SystemReset();
				}
				printf("\r\ngo to obd app++++++++++++++++++++++++++++++++++\r\n");
				
				__disable_irq();//关总中断
				__disable_fault_irq();
				delay_ms(2000);
				//jump to obd app
				JumpAddress = *(__IO uint32_t*) (ApplicationAddress+4);
				Jump_To_Application = (pFunction) JumpAddress;
				__set_MSP(*(__IO uint32_t*) ApplicationAddress);
				Jump_To_Application();
			}
		}

	}

}
/*******************************************************************************
 * 函数名称:iap                                                                     
 * 描    述: IAP升级工程                                                               
 *                                                                               
 * 输    入:无                                                          
 * 输    出:无                                                                    
 * 返    回: 无                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年5月9日                                                                    
 *******************************************************************************/
void tcpIap(void){
	int16_t currentIndex = 1;
	uint32_t failCounter = 0;
//	IAP_START iapStart;	
//	IAP_GET   iapGet;
	uint32_t JumpAddress;
	int i=0;
	

	IAP_BRIFE iapBrife;
	IAP_DATA  *iapDate;

	printf("\r\nplease set usart baudratet to 230400\r\n");
	delay_ms(2000);
	setUsart1(230400,0,1);


	failCounter++;
	ISP_DIRECTION = USART_SIM;	
	ERROR_ENTRY:	
	memset(SIM_BUF,'\0',1300);
	sim900_power_on(); 
	delay_ms(3000);
	if(strstr(SIM_BUF,"NOT INSERTED") || strstr(SIM_BUF,"REMOVE"))// || GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) == 1)
	{
		printf("\r\nsim card not insert\r\n");
		sysErrorHandle(sim900NotInsert);
		goto ERROR_ENTRY;
	}
	Send_AT_And_Wait("AT\r","OK",500);
	Send_AT_And_Wait("AT\r","OK",500);
//	Send_AT_And_Wait("ATE0\r","OK",500);
	readImei();



//	memset(iapGet.start,'#',2) ;

	while(1){

		if( checkConnectState(sysCfg.iapConfig.socketNum) > 0)
		{
			printf("\r\nCURRENT DATA INDEX :%d\r\n",currentIndex);
//			showConfig(&sysCfg);
			if(tcpGetData(&(sysCfg.iapConfig),&(sysCfg.iapBrife),currentIndex) < 0){
				continue;	
			}
			else{
				currentIndex++;
				printf("\r\nall packet number is :%d++++++++++++++++\r\n",sysCfg.iapBrife.packetNum);
				printf("\r\nNEXT PACKET NUM :%d++++++++++++++++\r\n",currentIndex);
				printf("\r\n app crc :%X\r\n",sysCfg.iapBrife.appCrc);
				printf("\r\n app size :%X\r\n",sysCfg.iapBrife.sizeOfApp);
//				showConfig(&sysCfg);
//				checkAppCrc(&(sysCfg.iapBrife));
				if(currentIndex > sysCfg.iapBrife.packetNum)
				{
					currentIndex = 1;
					//write ok
					//check whether ok
					//break and goto new main
					if(checkAppCrc(&(sysCfg.iapBrife)) < 0){
						printf("\r\nFLASH CRC CHECK ERROR\r\n");
						__disable_irq();//关总中断
						__disable_fault_irq();
						delay_ms(2000);
						NVIC_SystemReset();
					}
					printf("\r\ngo to obd app++++++++++++++++++++++++++++++++++\r\n");
					printf("\r\nnet iap update ok++++++++++++++++++++++++++++++++++\r\n");
					return ;
				}
			}  		
		}
		else{
			for(i = 0 ;i<500;i++){
				printf("Net Error,reConnect\r\n"); 	
				while(establishConnect(sysCfg.iapConfig) <= 0){
					printf("\r\nestablish network fail :%d\r\n",failCounter);
					delay_ms(1000);
					
				}
				if(tcpStart(&sysCfg.iapConfig,&sysCfg.iapBrife) < 0){
					printf("\r\nsleep 2 s\r\n");
					delay_ms(2000);
					i = 0;
				}
				else
				{
					
					i = 1000;//break;
				}
			}
		}	
	} 
}

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

//typedef struct {
//	uint8_t 	start[2];	  //fill with  '#'
//	uint16_t 	crc;
//
//	int16_t		typeOrIndex;	
//	uint8_t		hv;
//	uint8_t		sv;
//
//	uint32_t	imeiHigh;
//	uint32_t	imeiLow;
//}IAP_START;	//IAP 参数请求

//typedef struct {
//	uint8_t 	start[2]; //fill with '#'
//	uint16_t	crc;//数据包crc
//
//	int16_t		typeOrIndex;
//	uint8_t		hv;
//	uint8_t		sv;
//
//	uint16_t	packetCrc;	//附带软件包crc
//	uint16_t	packetLength;	 
//
//	uint8_t		app[4];	 //fill with '*'
//}IAP_DATA;	//IAP 数据返回

int16_t tcpGetData(SOCKET *net,IAP_BRIFE *brife,int16_t index){

	IAP_START iapStart;
	IAP_DATA *iapData;
	static uint8_t  appBuf[1024];
	uint8_t	*pointer = NULL;
	uint8_t *temp = NULL;
	int16_t i=0;
	uint32_t address ;// = ApplicationAddress + 1024*(index/2); 

	memset(&iapStart,'\0',sizeof(IAP_START));

	memset(iapStart.start,'#',2);
	iapStart.typeOrIndex = index;
	iapStart.hv = sysCfg.iapBrife.hv;
	iapStart.sv = sysCfg.iapBrife.sv;

	iapStart.imeiHigh	=	simState.imeiHigh;	
	iapStart.imeiLow 	=	simState.imeiLow;


	iapStart.crc = calBufCrc(iapStart.start + 4 , sizeof(IAP_START)-4);

//	printf("\r\nCRC data:");
//	for(i = 0;i<sizeof(IAP_START);i++) {
//		printf("%3X",iapStart.start[i]);
//	}
	
	dataSend(iapStart.start , sizeof(IAP_START),0,0,0,*net,0); //发送数据，不尝试在发送中重新建立连接，错误即退出，重新获取数据信息
	
	delay_ms(4000);
	pointer = strstr(SIM_BUF,"##");
//	printf("\r\n\r\n\r\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
//	printf("\r\n\r\n\r\nSIM BUF: \r\n");			
//	for(i = 0;i<600;i++){
//		printf("%3X",SIM_BUF[i]);
//	}
//	printf("\r\n\r\n\r\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
//
//	printf("\r\n\r\n\r\nGET_DATA: \r\n");			
//	for(i = 0;i<600;i++){
//		printf("%3X",pointer[i]);
//	}
//	printf("\r\n\r\n\r\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n\r\n\r\n\r\n");
//	return 1;
	if(pointer != NULL){
		//reset sim
		iapData = (IAP_DATA *)pointer;
		printf("\r\ncrc			:%X\r\n",iapData->crc);
		printf("\r\ntypeOrIndex	:%d\r\n",iapData->typeOrIndex);
		printf("\r\nhv			:%X\r\n",iapData->hv);
		printf("\r\nsv			:%X\r\n",iapData->sv);
		printf("\r\npacketCrc	:%X\r\n",iapData->packetCrc);
		printf("\r\npacketLength:%X\r\n",iapData->packetLength);

		printf("\r\n\r\n\r\nCAL CRC %4X\r\n",calBufCrc(iapData->start + 4,sizeof(IAP_BRIFE)-4));
		temp = iapData->app + 4;
//		for(i = 0;i<512;i++)
//		{
//			if(i % 16 ==0)
//					printf("\r\n");
//			printf("%3X",temp[i]);
//
//		}

//		delay_ms(3000);	
				
		
		if(iapData->crc == calBufCrc(iapData->start + 4,sizeof(IAP_DATA)-4))
		{			
			printf("\r\nget iap app data ok :\r\n");			
			pointer = iapData->app + 4;
//			for(i = 0;i<100;i++){
//				printf("%3X",pointer[i]);
//			}

			if(iapData->packetCrc != calBufCrc(pointer,iapData->packetLength)){
				printf("\r\napp crc error\r\n");
				return -1;
			}
			if(index %2 != 0)
			{//奇数包
				memset(appBuf,0,1024);
				memcpy(appBuf,pointer,iapData->packetLength);
				if(iapData->packetCrc != calBufCrc(appBuf,iapData->packetLength)){
					printf("\r\n\r\ncopy data error:%d\r\n",index);
					return -1;
				}
				else
				{
					// 注意最后一个包
					if(index < brife->packetNum)
						return 1;
					else{//最后一个包
						printf("\r\nWRITE LAST FLASH : %d\r\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n\r\n",index);
						address  = ApplicationAddress + 1024*(index/2);
						if(WriteReadCheck(address,appBuf,iapData->packetLength,iapData->packetCrc) < 0 ){				
						   	printf("\r\nwrite flash error\r\n");
							delay_ms(1000);
							return -1;	 
						}
						else
						{
							printf("\r\nWRITE IN BUF :%d\r\n",index);
							return 1;
						}
					}
				}	
			}
			else
			{
//			 	printf("\r\nbuf data first 512 :++++++++++++++++++\r\n\r\n");
//				for(i = 0;i<512;i++)
//				{
//					if(i % 16 ==0)
//							printf("\r\n");
//					printf("%3X",appBuf[i]);
//		
//				}
				

				memcpy(appBuf+512,pointer,iapData->packetLength);

//				printf("\r\nbuf data first 512 :++++++++++++++++++\r\n\r\n");
//				for(i = 0;i<512;i++)
//				{
//					if(i % 16 ==0)
//							printf("\r\n");
//					printf("%3X",appBuf[i]);
//		
//				}

//				printf("\r\nbuf data second 512 :++++++++++++++++++\r\n\r\n");
//				for(i = 0;i<512;i++)
//				{
//					if(i % 16 ==0)
//							printf("\r\n");
//					printf("%3X",pointer[i+512]);
//		
//				}
				printf("\r\nWRITE FLASH : %d\r\n",index);
				if(iapData->packetCrc != calBufCrc(appBuf+512,iapData->packetLength))
				{
					printf("\r\n\r\ncopy data error:%d\r\n",index);
					return -1;
				}
				address  = ApplicationAddress + 1024*(index/2 - 1);
				if(WriteReadCheck(address,appBuf,iapData->packetLength +512,calBufCrc(appBuf,iapData->packetLength +512)) < 0 )
				{				
				   	printf("\r\nwrite flash error\r\n");
					delay_ms(1000);
					return -1;	 
				}
				else{

					printf("\r\n\r\nwrite flash ok\r\n");
					return 1;

				}
			}				
		}
		else{
			printf("\r\ncrc error\r\n");
			delay_ms(2000);
			return -1;
		}
	}
	else
	{


		printf("\r\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\r\n");  
		printf("\r\n\r\ncan't find data,whether to reConnect\r\n\r\n\r\n"); 
		printf("\r\nc$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\r\n");	 
		delay_ms(2000);
	
	}	

}

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
 
//typedef struct {
//	uint8_t 	start[2];	  //fill with  '#'
//	uint16_t 	crc;
//	int16_t		typeOrIndex;	
//	uint8_t		hv;
//	uint8_t		sv;	
//	uint32_t	imeiHigh;
//	uint32_t	imeiLow;
//}IAP_START;	//IAP 参数请求
int16_t tcpStart(SOCKET *net,IAP_BRIFE *brife){
	IAP_START iapStart;
	IAP_BRIFE test;

	int16_t errorCounter = 0;
	int8_t *p = NULL,*q=(int8_t *)brife; 

	int i = 0;
	printf("\r\nTCP START\r\n");
	printf("\r\nsend header ok\r\n");
	ERROR_START: 
	if(errorCounter > 3){
		printf("\r\ncan,t get iap brife\r\n");
		return -1;
	}
	errorCounter++;

	memset(&iapStart,'\0',sizeof(IAP_START));
	memset(iapStart.start,'#',2);
	printf("\r\n\r\n");
	iapStart.typeOrIndex = 0;
	iapStart.imeiHigh	=	simState.imeiHigh;	
	iapStart.imeiLow 	=	simState.imeiLow;
	iapStart.hv = 1;
	iapStart.sv = 1;
	
	iapStart.crc = calBufCrc(iapStart.start+4,sizeof(IAP_START)-4);

	printf("\r\nCRC data:");
	for(i = 0;i<sizeof(IAP_START);i++) {
		printf("%3X",iapStart.start[i]);
	}

	dataSend(iapStart.start,sizeof(IAP_START),0,0,0,*net,1);
	delay_ms(5000);
	
//	p+=2;
//	p = strstr(SIM_BUF,"##");
	printf("\r\nSIM BUFFER :\r\n");
	for(i = 0;i< 200;i++){
		printf("%3X",SIM_BUF[i]);
	}

	printf("\r\nDATA :\r\n");
	p = strstr(SIM_BUF,"##");
	for(i = 0;i< 20;i++){
		printf("%3X",p[i]);
	}
	printf("\r\n");

	if(p != NULL){
		for(i = 0;i < sizeof(IAP_BRIFE);i++){
			q[i] = p[i];

		}

		printf("\r\ncrc         :%X\r\n",brife->crc);
		printf("\r\ntypeOrIndex :%X\r\n",brife->typeOrIndex);
		printf("\r\nhv          :%X\r\n",brife->hv);
		printf("\r\nsv          :%X\r\n",brife->sv);
		printf("\r\npacketNum   :%X\r\n",brife->packetNum);
		printf("\r\nappCrc      :%X\r\n",brife->appCrc);
		printf("\r\nsize        :%X\r\n",brife->sizeOfApp);


//		return 1;
		printf("\r\nready for crc data :\r\n");
		for(i=0;i<sizeof(IAP_BRIFE);i++)
		{
		 	printf("%3X",brife->start[i]);
		}
		printf("\r\nlocal crc :%3X  ->desCrc :%3X\r\n",calBufCrc(brife->start + 4,sizeof(IAP_BRIFE)-4),brife->crc);
		if(brife->crc == calBufCrc(brife->start + 4,sizeof(IAP_BRIFE)-4)){
			printf("\r\nget iap brife data ok\r\n");
			return 1;				
		}
		else{
			printf("\r\ncrc error\r\n");
			delay_ms(2000);
			goto ERROR_START;
		}
	}
	else{
		printf("\r\nno brife data return ,error\r\n");
		delay_ms(2000);
		goto ERROR_START;
	}
}
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
int16_t checkIapType(void){
	uint32_t i=0;
	int32_t keyCounter=0;
	
//	delay_ms(2000);
	
	for(i=0;i<15;i++){
		if(GET_KEY2_STATE == KEY2_DOWN){
			keyCounter--;
		}
		else{
			keyCounter++;
		}
		LED1(Bit_SET);
		LED2(Bit_SET);
		LED3(Bit_SET);
		delay_ms(100);
		if(GET_KEY2_STATE == KEY2_DOWN){
			keyCounter--;
		}
		else{
			keyCounter++;
		}
		LED1(Bit_RESET);
		LED2(Bit_RESET);
		LED3(Bit_RESET);
		delay_ms(100);
	}
	printf("\r\nkey :%d\r\n",keyCounter);
	return keyCounter;
}
/*******************************************************************************
 * 函数名称:iap                                                                     
 * 描    述: IAP升级工程                                                               
 *                                                                               
 * 输    入:无                                                          
 * 输    出:无                                                                    
 * 返    回: 无                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年5月9日                                                                    
 *******************************************************************************/
void iapHardwareInit(void){
	return;
}

/*******************************************************************************
 * 函数名称:checkAppConfig                                                                    
 * 描    述: 检查配置参数完整性                                                              
 *                                                                               
 * 输    入:无                                                          
 * 输    出:无                                                                    
 * 返    回: 0：正常 非零：校验出错                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年5月20日                                                                    
 *******************************************************************************/
int16_t checkAppConfig(void){
	uint16_t crc = calBufCrc(((uint8_t *)&sysCfg) +2 ,sizeof(FLASH_CONFIG)-2);
	printf("\r\ncheck app config\r\n");
	if(sysCfg.crc != crc ){
		printf("config crc :%10X cal crc :%10X",sysCfg.crc,crc);
		return -1;
	}
	else
		return 1;

}
/*******************************************************************************
 * 函数名称:checkAppState                                                                    
 * 描    述: 检查OBD程序完整性                                                              
 *                                                                               
 * 输    入:无                                                          
 * 输    出:无                                                                    
 * 返    回:0：正常 非零：校验出错                                                   
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年5月20日                                                                    
 *******************************************************************************/
int16_t checkAppState(void){
	printf("\r\ncheck app state\r\n");
	if(checkAppCrc(&sysCfg.iapBrife) < 0 ){
		printf("\r\napp crc error\r\n");
		return -1;
	}
	else
	{
		printf("\r\napp crc ok\r\n");	
	}
	if(checkAppConfig() < 0){
		printf("\r\napp config crc error\r\n");
		return -1;
	}
	else
	{
		printf("\r\napp config crc ok\r\n");
	}
	
	return 1;	

}

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
int16_t		checkAppCrc(IAP_BRIFE *brife){

	uint16_t crc =  0;
	printf("\r\nCAL FALSH CRC :0x%X --->  %d \r\n",SYS_MAIN,brife->sizeOfApp);
	crc =  calFlashCrc(SYS_MAIN,brife->sizeOfApp);
	if(brife->appCrc != crc){
		printf("\r\nmain app crc error :%X   %X\r\n",brife->appCrc,crc);
		return -1;
	}
	else
	{
		printf("\r\nmain app crc ok\r\n");
		return 1;
	}
}

void  setDebugPara(void){
	
	if(BKP_ReadBackupRegister(BKP_DR7) == 0x55){
	 	printf("\r\nnot first time\r\n");
		delay_ms(2000);
		return ;
	}

	sysCfg.iapConfig.flag = 0x5555;
	strcat(sysCfg.iapConfig.ipUrl,"in.phonewingstech.com");
	sysCfg.iapConfig.udpTcp = 0;
	sysCfg.iapConfig.socketNum = 1;
	sysCfg.iapConfig.localPort = 5000;
	sysCfg.iapConfig.desPort = 9000;
	sysCfg.iapConfig.crc = calBufCrc(sysCfg.iapConfig.ipUrl-2,sizeof(SOCKET)-2);

	sysCfg.netConfig.flag = 0x5555;
	strcat(sysCfg.netConfig.ipUrl,"in.phonewingstech.com");
	sysCfg.netConfig.udpTcp = 1;
	sysCfg.netConfig.socketNum = 1;
	sysCfg.netConfig.localPort = 5000;
	sysCfg.netConfig.desPort = 9001;
	sysCfg.netConfig.crc = calBufCrc(sysCfg.netConfig.ipUrl-2,sizeof(SOCKET)-2);


	sysCfg.blueConfig.baudrate = 38400;		
	strcat(sysCfg.blueConfig.name,"OBD_2");
	strcat(sysCfg.blueConfig.password,"0000"); 
	sysCfg.netConfig.crc = calBufCrc(sysCfg.blueConfig.name-6,sizeof(BLUE_CONFIG)-2);


	sysCfg.obdConfig.baudrate = 38400;	
	sysCfg.obdConfig.crc = calBufCrc(sysCfg.obdConfig.reserved-6,sizeof(OBD_CONFIG)-2);

	sysCfg.crc = 0x5555;




	sysCfg.iapBrife.hv = 1;
	sysCfg.iapBrife.sv = 1;
	sysCfg.iapBrife.packetNum = 0xff;
	sysCfg.iapBrife.appCrc = 0x5555;
	sysCfg.iapBrife.sizeOfApp = 12345;
	sysCfg.iapBrife.start[0] = '*';
	sysCfg.iapBrife.start[1] = '*';



	sysCfg.iapBrife.crc = calBufCrc(sysCfg.iapBrife.start+4,sizeof(IAP_BRIFE)-4);



	updateFlashConfig(&sysCfg.iapConfig,&sysCfg.iapConfig,&sysCfg.iapBrife,&sysCfg.blueConfig,&sysCfg.obdConfig);
	BKP_WriteBackupRegister(BKP_DR7, 0x55);


	


}











