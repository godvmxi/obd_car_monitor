#include "iap.h"


/*******************************************************************************
 * ��������:iap                                                                     
 * ��    ��: IAP��������                                                               
 *                                                                               
 * ��    ��:��                                                          
 * ��    ��:��                                                                    
 * ��    ��: ��                                                    
 * ��    ��:����                                                                     
 * �޸�����:2012��5��9��                                                                    
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
	__disable_irq();//�����ж�
	__disable_fault_irq();
	delay_ms(2000);
	//jump to obd app
	JumpAddress = *(__IO uint32_t*) (ApplicationAddress+4);
	Jump_To_Application = (pFunction) JumpAddress;
	__set_MSP(*(__IO uint32_t*) ApplicationAddress);
	Jump_To_Application();		
}
/*******************************************************************************
 * ��������:iap                                                                     
 * ��    ��: IAP��������                                                               
 *                                                                               
 * ��    ��:��                                                          
 * ��    ��:��                                                                    
 * ��    ��: ��                                                    
 * ��    ��:����                                                                     
 * �޸�����:2012��5��9��                                                                    
 *******************************************************************************/
void usartIap(void){


}
/*******************************************************************************
 * ��������:iap                                                                     
 * ��    ��: IAP��������                                                               
 *                                                                               
 * ��    ��:��                                                          
 * ��    ��:��                                                                    
 * ��    ��: ��                                                    
 * ��    ��:����                                                                     
 * �޸�����:2012��5��9��                                                                    
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
					__disable_irq();//�����ж�
					__disable_fault_irq();
					delay_ms(2000);
					NVIC_SystemReset();
				}


			}
		
		}
		else{
			for(i = 0 ;i<500;i++){
				printf("Net Error,reConnect\r\n");
	
				while(establishConnect(net,0) <= 0){
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
 * ��������:tcpStart                                                                     
 * ��    ��: ���½���TCP��֤����������Ӧ������                                                               
 *                                                                               
 * ��    ��:��                                                          
 * ��    ��:��                                                                    
 * ��    ��: ��:ǿ��ʹ��USART����  ���ࣺ�������ϵͳ��ȷ���Ƿ�����                                                    
 * ��    ��:����                                                                     
 * �޸�����:2012��5��9��                                                                    
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

	dataSend(iapGet.start , sizeof(IAP_GET),0,0,0,net,0); //�������ݣ��������ڷ��������½������ӣ������˳������»�ȡ������Ϣ
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
 * ��������:tcpStart                                                                     
 * ��    ��: ���½���TCP��֤����������Ӧ������                                                               
 *                                                                               
 * ��    ��:��                                                          
 * ��    ��:��                                                                    
 * ��    ��: ��:ǿ��ʹ��USART����  ���ࣺ�������ϵͳ��ȷ���Ƿ�����                                                    
 * ��    ��:����                                                                     
 * �޸�����:2012��5��9��                                                                    
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
 * ��������:checkIapType                                                                     
 * ��    ��: IAP��������ѡ��                                                               
 *                                                                               
 * ��    ��:��                                                          
 * ��    ��:��                                                                    
 * ��    ��: ��:ǿ��ʹ��USART����  ���ࣺ�������ϵͳ��ȷ���Ƿ�����                                                    
 * ��    ��:����                                                                     
 * �޸�����:2012��5��9��                                                                    
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
 * ��������:iap                                                                     
 * ��    ��: IAP��������                                                               
 *                                                                               
 * ��    ��:��                                                          
 * ��    ��:��                                                                    
 * ��    ��: ��                                                    
 * ��    ��:����                                                                     
 * �޸�����:2012��5��9��                                                                    
 *******************************************************************************/
void iapHardwareInit(void){
	return;
}

/*******************************************************************************
 * ��������:checkAppConfig                                                                    
 * ��    ��: ������ò���������                                                              
 *                                                                               
 * ��    ��:��                                                          
 * ��    ��:��                                                                    
 * ��    ��: 0������ ���㣺У�����                                                    
 * ��    ��:����                                                                     
 * �޸�����:2012��5��9��                                                                    
 *******************************************************************************/
int16_t checkAppConfig(void){
	return 0;

}
/*******************************************************************************
 * ��������:checkAppState                                                                    
 * ��    ��: ���OBD����������                                                              
 *                                                                               
 * ��    ��:��                                                          
 * ��    ��:��                                                                    
 * ��    ��:0������ ���㣺У�����                                                   
 * ��    ��:����                                                                     
 * �޸�����:2012��5��9��                                                                    
 *******************************************************************************/
int16_t checkAppState(void){
	return 0;

}

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
int16_t		checkAppCrc(IAP_BRIFE brife){

	return 1;
}

void  setDebugPara(void){
	int i = 0;
	const OBD_LIST temp[] = {
		{0		,0	,5	},	//ATZ
		{1		,0	,5	},
		{2		,0	,5	},
		{3		,0	,5	},
		{4		,0	,5	},
		{5		,0	,5	}, //5
		{6		,0	,5	},
		{7		,0	,5	},
		{8		,0	,5	},
		{9		,0	,5	},
		{10		,0	,5	},//10
		{11		,0	,5	},
		{12		,0	,5	},
		{13		,0	,5	},
		{14		,0	,5	},
		{15		,0	,5	},//15
		{16		,0	,5	},
		{17		,0	,5	},
		{18		,0	,5	},
		{19		,0	,5	},
		{20		,0	,5	}, //20
		{21		,0	,5	},
		{22		,0	,5	},
		{23		,0	,5	},
		{24		,0	,5	},
		{25		,0	,5	},
		{26		,0	,5	},
		{27		,0	,5	},
		{28		,0	,5	},
		{29		,0	,5	},
		{30		,0	,5	},
		{31		,0	,5	},
		{32		,0	,5	},
		{33		,0	,5	},
		{34		,0	,5	},
		{35		,0	,5	},
		{36		,0	,5	},
		{37		,0	,5	},
		{38		,0	,5	},
		{39		,0	,5	},
		{40		,0	,5	},
		{41		,0	,5	},
		{42		,0	,5	},
		{43		,0	,5	},
		{44		,0	,5	},
		{45		,0	,5	},
		{46		,0	,5	},
		{47		,0	,5	},
		{48		,0	,5	},
		{49		,0	,5	},
		{50		,8	,5	}, //50
		{51		,8	,5	},
		{52	,	2	,5	},
		{53	,	4	,5	},
		{54	,	2	,5	},
		{55	,	2	,5	},//55
		{56	,	4	,5	},
		{57	,	4	,5	},
		{58	,	4	,5	},
		{59	,	4	,5	},
		{60	,	4	,5	},	//60
		{61	,	4	,5	},
		{62	,	4	,5	},
		{63	,	2	,5	},
		{64	,	2	,5	},
		{65	,	1	,5	}, //65
		{66	,	4	,5	},
		{67	,	2	,5	},
		{68	,	4	,5	},
		{69	,	4	,5	},
		{70	,	4	,5	}, //70
		{71	,	4	,5	},
		{72	,	4	,5	},
		{73	,	4	,5	},
		{74	,	4	,5	},
		{75	,	4	,5	}, //75
		{76	,	4	,5	},
		{77	,	4	,5	},
		{78	,	1	,5	},
		{79	,	4	,5	},
		{80	,	4	,5	}, //80
		{81	,	4	,5	},
		{82	,	8	,5	},
		{83	,	4	,5	},
		{84	,	4	,5	},
		{85	,	4	,5	},//85
		{86	,	4	,5	},
		{87	,	4	,5	},
		{88	,	4	,5	},
		{89	,	4	,5	},
		{90	,	4	,5	},	//90
		{91	,	4	,5	},
		{92	,	4	,5	},
		{93	,	4	,5	},
		{94	,	4	,5	},
		{95	,	4	,5	}, //95
		{96	,	4	,5	},
		{97	,	4	,5	},
		{98	,	4	,5	},
		{99	,	4	,5	},
		{100	,4	,5	},//100
		{101	,4	,5	},
		{102	,4	,5	},
		{103	,4	,5	},
		{104	,4	,5	},
		{105	,4	,5	},//105
		{106	,4	,5	},
		{107	,4	,5	},
		{108	,4	,5	},
		{109	,4	,5	},
		{110	,4	,5	},//110
		{111	,4	,5	},
		{112	,4	,5	},
		{113	,4	,5	},
		{114	,8	,5	},
		{115	,4	,5	},//115
		{116	,4	,5	},
		{117	,4	,5	},
		{118	,4	,5	},
		{119	,4	,5	},
		{120	,4	,5	},//120
		{121	,4	,5	},
		{122	,4	,5	},
		{123	,4	,5	},
		{124	,4	,5	},
		{125	,4	,5	},//125
		{126	,4	,5	},
		{127	,4	,5	},
		{128	,4	,5	},
		{129	,4	,5	},
		{130	,4	,5	},//130
		{131	,4	,5	},
		{132	,4	,5	},
		{133	,4	,5	},
		{134	,4	,5	},
		{135	,4	,5	},//135
		{136	,4	,5	},
		{137	,4	,5	},
		{138	,4	,5	},
		{139	,4	,5	},
		{140	,4	,5	},//140
		{141	,4	,5	},
		{142	,4	,5	},
		{143	,4	,5	},
		{144	,4	,5	},
		{145	,4	,5	},//145
		{146	,4	,5	},
		{147	,4	,5	},
		{148	,4	,5	},
		{149	,4	,5	},
		{150	,4	,5	},//150

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
	sysCfg.obdConfig.fastStart = 50;
	sysCfg.obdConfig.fastEnd = 71;

	sysCfg.obdConfig.normalStart = 50;
	sysCfg.obdConfig.normalEnd = 79;

	sysCfg.obdConfig.initStart = 0;
	sysCfg.obdConfig.initEnd = 20;
	
 	printf("\r\ncmd num :%d\r\n",sysCfg.obdConfig.cmdNum);

	sysCfg.obdConfig.crc = calBufCrc(sysCfg.obdConfig.reserved-6,sizeof(OBD_CONFIG)-2);

	sysCfg.flag = 0x5555;




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












