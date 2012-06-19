#include "iap.h"


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

void iap(void){
	
	uint32_t JumpAddress;
	iapHardwareInit();
	tcpIap();
//	readFlashConfig();
	if(checkIapType() < -5){
		LED1(Bit_SET);
		delay_ms(2000);
		usartIap();
	}
	else
	{
		if((checkAppState() < 0) || (BKP_ReadBackupRegister(BKP_DR4) == 0x11) || checkAppConfig()){
			LED3(Bit_SET);
			tcpIap();
			BKP_WriteBackupRegister(BKP_DR8, 0);
		}		
	}
	//goto obd app
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
void usartIap(void){


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
	uint16_t currentIndex = 0;
	uint32_t failCounter = 0;
//	IAP_START iapStart;	
	IAP_GET   iapGet;
	int i=0;

	IAP_BRIFE iapBrife;
	IAP_DATA  *iapDate;
	SOCKET net;
	memset(net.ipUrl,'\0',40);
	strcat(net.ipUrl,"in.phonewingstech.com");
	net.localPort=5000;
	net.desPort=9000;	
	net.udpTcp = 0;	//TCP
	net.socketNum = 1;	 


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
//	readImei(simState.imei);
//	printf("\r\nestablish network\r\n");
//	if(establishConnect(net) <= 0){
//		printf("\r\nestablish network fail :%d\r\n",failCounter);
//		goto ERROR_ENTRY;
//	}


	memset(iapGet.start,'*',4) ;

	while(1){
//		if(establishConnect(net) <= 0){
//			printf("\r\nestablish network fail :%d\r\n",failCounter);
//			goto ERROR_ENTRY;
//		}
		if( checkConnectState(net.socketNum) > 0)
		{
			printf("network ok\r\n");
			if(tcpGetData(net,&iapBrife,currentIndex) < 0){
				
			}
			else{
				//write ok
				//check whether ok
				//break and goto new main
				if(checkAppCrc(iapBrife) < 0){
					printf("\r\napp crc error.reset\r\n");
					__disable_irq();//关总中断
					__disable_fault_irq();
					delay_ms(2000);
					NVIC_SystemReset();
				}


			}
		
		}
		else{
			for(i = 0 ;i<500;i++){
				printf("Net Error,reConnect\r\n");
	
				while(establishConnect(net) <= 0){
					printf("\r\nestablish network fail :%d\r\n",failCounter);
					delay_ms(1000);
					
				}
				if(tcpStart(net,&iapBrife) < 0){
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
int16_t tcpGetData(SOCKET net,IAP_BRIFE *brife,int16_t index){
	IAP_GET iapGet;
	IAP_DATA *iapData;
	int8_t	*pointer = NULL;
	int16_t i=0;
	uint32_t address = ApplicationAddress + 1024*(index-1); 

	memset(&iapGet,'\0',sizeof(IAP_GET));

	memset(iapGet.start,'\0',4);
	iapGet.index = index ;
	memset(iapGet.other,0xFF,4);
	iapGet.crc = calBufCrc(iapGet.start + 6 , sizeof(IAP_GET)-6);

	dataSend(iapGet.start , sizeof(IAP_GET),0,0,0,net,0); //发送数据，不尝试在发送中重新建立连接，错误即退出，重新获取数据信息
	delay_ms(2000);
	pointer = strstr(SIM_BUF,"****");
	if(pointer == NULL){
		//reset sim
		iapData = (IAP_DATA *)pointer;
		printf("\r\ncrc       :%d\r\n",iapData->crc);
		printf("\r\nindex     :%d\r\n",iapData->typeOrIndex);
		printf("\r\nlength    :%d\r\n",iapData->packetLength);
		printf("\r\ncrcData   :%d\r\n",iapData->packetCrc);
		if(iapData->crc == calBufCrc(iapData->start + 6,sizeof(IAP_BRIFE)-6)){			
			printf("\r\nget iap brife data ok\r\n");			
			pointer = (uint8_t *)(&(iapData->packetCrc)) +2;
			if(WriteReadCheck(address,pointer,iapData->packetLength,iapData->packetCrc) < 0 ){				
			   	printf("\r\nwrite flash error\r\n");
				return -1;	 
			}
			for(i = 0;i < iapData->packetLength;i++){
				printf("%3X",pointer[i]);
				if(i % 10 == 0){
					printf("\r\n");
				}
			}
			return 1;				
		}
		else{
			printf("\r\ncrc error\r\n");
			delay_ms(2000);
			return -1;
		}


	}
	 
	printf("\r\nget data error,whether to reConnect\r\n"); 	 
	delay_ms(1000);	

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
int16_t tcpStart(SOCKET net,IAP_BRIFE *brife){
	IAP_START iapStart;
	IAP_BRIFE *iapBrife;
	int16_t errorCounter = 0;
	char *p = NULL; 
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
	memset(iapStart.start,'*',4);
	iapStart.imeiHigh	=	0xFFFFFFFF;	
	iapStart.imeiLow 	=	0xFFFFFFFF;
	iapStart.hv = 0;
	iapStart.sv = 0;
	iapStart.crc = calBufCrc((uint8_t *)(iapStart.crc+1),sizeof(IAP_START)-6);
	//
	printf("\r\nbuffer data:");
	for(i = 0;i<sizeof(IAP_START);i++) {
		printf("%3X",iapStart.start +i);
	}
	dataSend(iapStart.start,sizeof(IAP_START),0,0,0,net,1);
	delay_ms(1000);
	p = strstr(SIM_BUF,"****");
	if(p != NULL){
		iapBrife = (IAP_BRIFE *)p;
		printf("\r\ncrc   :%d\r\n",iapBrife->crc);
		printf("\r\nhv    :%d\r\n",iapBrife->hv);
		printf("\r\nsv    :%d\r\n",iapBrife->sv);
		printf("\r\npacket:%d\r\n",iapBrife->packetNum);
		printf("\r\nother :%d\r\n",iapBrife->appCrc);
		printf("\r\nsize  :%d\r\n",iapBrife->sizeOfApp);
		if(((IAP_BRIFE *)p)->crc == calBufCrc((uint8_t *)(&(iapBrife->crc)) + 2,sizeof(IAP_BRIFE)-6)){
			*brife = *iapBrife;
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
	
	delay_ms(2000);
	
	for(i=0;i<30;i++){
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
		if(GET_KEY1_STATE == KEY1_DOWN){
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
 * 修改日期:2012年5月9日                                                                    
 *******************************************************************************/
int16_t checkAppConfig(void){
	return 0;

}
/*******************************************************************************
 * 函数名称:checkAppState                                                                    
 * 描    述: 检查OBD程序完整性                                                              
 *                                                                               
 * 输    入:无                                                          
 * 输    出:无                                                                    
 * 返    回:0：正常 非零：校验出错                                                   
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年5月9日                                                                    
 *******************************************************************************/
int16_t checkAppState(void){
	return 0;

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
int16_t		checkAppCrc(IAP_BRIFE brife){

	return 1;
}

void  setDebugPara(void){
	int i = 0;
	const OBD_LIST temp[] = {
		{0	,0	,5	},
		{1	,0	,5	},
		{2	,0	,5	},
		{3	,0	,5	},
		{4	,0	,5	},
		{5	,0	,5	},
		{6	,0	,5	},
		{7	,0	,5	},
		{8	,0	,5	},
		{9	,0	,5	},
		{10	,0	,5	},
		{11	,0	,5	},
		{12	,0	,5	},
		{13	,0	,5	},
		{14	,0	,5	},
		{15	,0	,5	},
		{16	,0	,5	},
		{17	,0	,5	},
		{18	,0	,5	},
		{19	,0	,5	},
		{20	,0	,5	},
		{21	,0	,5	},
		{22	,0	,5	},
		{23	,0	,5	},
		{24	,0	,5	},
		{50	,0	,5	},
		{51	,0	,5	},
		{52	,0	,5	},
		{53	,0	,5	},
		{54	,0	,5	},
		{55	,0	,5	},
		{56	,0	,5	},
		{57	,0	,5	},
		{58	,0	,5	},
		{59	,0	,5	},
		{60	,0	,5	},
		{61	,0	,5	},
		{62	,0	,5	},
		{63	,0	,5	},
		{64	,0	,5	},
		{65	,0	,5	},
		{66	,0	,5	},
		{67	,0	,5	},
		{68	,0	,5	},
		{69	,0	,5	},
		{70	,0	,5	},
		{71	,0	,5	},
		{72	,0	,5	},
		{73	,0	,5	},
		{74	,0	,5	},
		{75	,0	,5	},
		{76	,0	,5	},
		{77	,0	,5	},
		{78	,0	,5	},
		{79	,0	,5	},
		{80	,0	,5	},
		{81	,0	,5	},
		{82	,0	,5	},
		{83	,0	,5	},
		{84	,0	,5	},
		{85	,0	,5	},
		{86	,0	,5	},
		{87	,0	,5	},
		{88	,0	,5	},
		{89	,0	,5	},
		{90	,0	,5	},
		{91	,0	,5	},
		{92	,0	,5	},
		{93	,0	,5	},
		{94	,0	,5	},
		{95	,0	,5	},
		{96	,0	,5	},
		{97	,0	,5	},
		{98	,0	,5	},
		{99	,0	,5	},
		{100	,0	,5	},
		{101	,0	,5	},
		{102	,0	,5	},
		{103	,0	,5	},
		{104	,0	,5	},
		{105	,0	,5	},

	};
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
	sysCfg.obdConfig.cmdNum =  sizeof(temp)/sizeof(OBD_LIST);
	for(i =0;i<sysCfg.obdConfig.cmdNum;i++)
	{
		sysCfg.obdConfig.cmdList[i] = temp[i];
	}
	
 	printf("\r\ncmd num :%d\r\n",sysCfg.obdConfig.cmdNum);

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



	updateFlashConfig(&sysCfg.netConfig,&sysCfg.iapConfig,&sysCfg.iapBrife,&sysCfg.blueConfig,&sysCfg.obdConfig);
}













