#include "mainfunc.h"

DATA_HEAD obdHeader;
GPS_DATA_REPORT gpsDataReport;
SOCKET socket;
//OBD_DATA obdData;
///*GPS*/
extern stru_GPSRMC  	GPS_RMC_Data; 
extern stru_GPSGGA  	GPS_GGA_Data; 
extern stru_GPSGSV  	GPS_GSV_Data;
extern stru_GPS_DATA  	GPS_DATA;
////-------------------------------------------------------
extern int	GPS_GGA_DINGWEI_OK;//若GPS定位，则将此变量置1，否则为0
extern int	GPS_RMC_DINGWEI_OK;//若GPS定位，则将此变量置1，否则为0
extern int	GPS_SHUJU_OK;//若GPS输出坐标信息，则将此变量置1，否则为0 
extern SIM_STRUCT simState;

/*******************************************************************************
 * 函数名称:checkAckMsg                                                                     
 * 描    述:检查下发的数据                                                                     
 *                                                                               
 * 输    入:处理的最大的数据量 0：全部处理 -1：不处理                                                                    
 * 输    出:无                                                                    
 * 返    回:实际处理的消息量                                                                     
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月9日                                                                    
 *******************************************************************************/
int checkAckMsg(int num)
{
	return dealFifoMsg(&fifoHead,num);	
}

/*******************************************************************************
 * 函数名称:collectAndSend                                                                     
 * 描    述:发送数据函数，负责各种信息手机和发送                                                                     
 *                                                                               
 * 输    入:无                                                                     
 * 输    出:无                                                                    
 * 返    回:无                                                                     
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月9日                                                                    
 *******************************************************************************/
int16_t	collectAndSend(void){
	int i = 0;
	int8_t temp[20];
	uint32_t timeCounter = 0; 	
	uint32_t obdType = 0;
	int failCounter = 0; //错误计数器
	BLUE_OBD = 0;
	ERROR_ENTRY:
	printf("\r\n system begin\r\n");

	initData();

	ISP_DIRECTION=USART_OBD;
	obdPower(0);
	delay_ms(1000);
	obdPower(1);

	gpsPowerOn();
	ISP_DIRECTION=USART_SIM;

	failCounter++;
	ISP_DIRECTION = USART_SIM;
	printf("\r\nBEGIN SEND MSG\r\n");
	
	
	ISP_DIRECTION = USART_OBD;
	TIM_Cmd(TIM3, DISABLE);
	OBD_START = 0;
//	BLUE_OBD = 1;
	TIM_Cmd(TIM3, ENABLE);


	
	ISP_DIRECTION=USART_SIM;
	sim900_power_on();


	Send_AT_And_Wait("AT\r","OK",500);
	Send_AT_And_Wait("AT\r","OK",500);
	readImei();

	blueToothPower(0);
	blueToothPower(1);
	initBlueTooth(0);

	ISP_DIRECTION=USART_OBD;
	obdAtAndWait("BT+RDTC\r\n",NULL,5000);
//	obdAtAndWait("BT+EDTC\r\n",NULL,5000);
	obdAtAndWait("BT+MIL\r\n",NULL,5000);
//	obdAtAndWait("BT+EDTC\r\n",NULL,5000);



	OBD_START = getObdPids() > 0 ?1:0;
	ISP_DIRECTION=USART_SIM;
	readImei();
	read_cellid();
	read_cimi();
	read_voltage();
	if(establishConnect(sysCfg.netConfig,1) <= 0)
	{
		goto  ERROR_ENTRY;
	}



	

	if(strstr(SIM_BUF,"NOT INSERTED") || strstr(SIM_BUF,"REMOVE"))// || GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) == 1)
	{
		printf("\r\nsim card not insert\r\n");
		sysErrorHandle(sim900NotInsert);
		goto ERROR_ENTRY;
	}
	
	printf("\r\nprepare gps\r\n");
//	gpsDataInit();//初始化gps数据
//test

	TIM_Cmd(TIM3, DISABLE);



	while(1){
	   	timeCounter = RTC_GetCounter();
		printf("\r\nprepare gps\r\n");

		if(POWER_STATE != 0){
			printf("\r\nPOWER BY CAR\r\n");
			if(CAR_POWER_FAILED == 0){
				printf("\r\nrecover from power failed ,power on blue and obd \r\n");
				CAR_POWER_FAILED = 1;
				goto  ERROR_ENTRY;
			}

			if(DEVICE_STATE != 0)//running
			{		
				printf("\r\ncar running ---------------------\r\n");
				if(OBD_START == 0){	//如果设备没有成功获取PID列表，则尝试重新获取，只有获取成功后才可以发送OBD信息
					OBD_START = getObdPids() > 0 ?1:0;
				}
				else
				{
					reportQL100(&sysCfg.netConfig,0,1,5);
					delay_ms(5000);
				}
				reportGps(&sysCfg.netConfig,0,1);
				
				printf("\r\nprepare gps\r\n");
//				gpsDataInit();//初始化gps数据
				delay_ms(5000);
			}
			else
			{
				printf("\r\ncat stopped++++++++++++++++++++++\r\n");
				reportGps(&sysCfg.netConfig,0,1);
				printf("\r\nprepare gps\r\n");
//				gpsDataInit();//初始化gps数据
				for(i = 0; i < 10 ;i++){
					if(DEVICE_STATE != 0){
						break;
						printf("\r\ncar start\r\n");
					}
					delay_ms(1000);
				}
			}
		}

		else
		{
			printf("\r\nPOWER BY BATTERY\r\n");
			printf("\r\ncat emergency !!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
			reportGps(&sysCfg.netConfig,0,1);
			if(CAR_POWER_FAILED == 1)
			{	 //just for alert
				printf("\r\ncar power failed ,shut down blue and obd for power saving\r\n");
				CAR_POWER_FAILED = 0;//power fail ,shut down blue and obd for power saving
				blueToothPower(0);
				obdPower(0);

			}
			for(i = 0; i < 30 ;i++){
				if(DEVICE_STATE != 0){
					break;
					printf("\r\ncar start\r\n");
				}
				delay_ms(1000);
			}

		}
		printf("\r\nTIME USED :%d->%d-->%d\r\n",RTC_GetCounter(),timeCounter,RTC_GetCounter() - timeCounter);
	}
}
/*******************************************************************************
 * 函数名称:checkConnectState                                                                     
 * 描    述:检查指定的socket号的连接状态 ，字符串格式                                                                
 *                                                                               
 * 输    入:待检查的socket号                                                                    
 * 输    出:无                                                                    
 * 返    回:-1：发生错误 0：不存在指定的socket 1：找到指定socket                                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月9日                                                                    
 *******************************************************************************/
int		checkConnectState(uint32_t socketNum){
	char num[3];
	if(socketNum > 4 || socketNum <1){
		printf("query socket ID error\r\n");
		return -1;
	}
	sprintf(num,"%d",socketNum);
	printf("\r\ncheck socket %s state+++++++++++++\r\n",num);
	if(Send_AT_And_Wait("AT+MIPOPEN?\r","+MIPOPEN",400) == 0){
		printf("query socket cmd error\r\n");
		return -1;
	}
	if(strstr(SIM_BUF,num)){
		printf("can not find the socket:%s\r\n",num);
		return 0;
	}
	else{
		printf("find the socket:%s\r\n",num);
		return 1;
	}
}

/*******************************************************************************
 * 函数名称:establishConnect                                                                     
 * 描    述:建立TCP连接                                                               
 *                                                                               
 * 输    入:建立连接的类型type 0：TCP 1：UDP 2:SSL     sokcet :socket号支持1-4                                                                   
 * 输    出:无                                                                    
 * 返    回:-1：发生错误 0：不存在指定的socket 1：找到指定socket   2:新建了socker连接，socket号为                                                                 
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月9日                                                                    
 *******************************************************************************/
int		establishConnect(SOCKET socket,int close){

	int	regieterCounter = 0;
	int i,j;

	static uint8_t socketFlag=0,socketCmd[80],socketState[40];
	char closeCmd[50];
		
	sprintf(closeCmd,"AT+MIPCLOSE=1\r");
	printf("\r\nclose socket\r\n");
	if(close != 0){
		Send_AT_And_Wait("AT+MIPCLOSE?\r","NULL",1000);
		Send_AT_And_Wait("AT+MIPCLOSE=1\r","NULL",1000);
	} 

	
	if(socketFlag == 0){
		delay_ms(1000);
		memset(socketCmd,'\0',80);
		memset(socketState,'\0',40);

		sprintf(socketCmd,"AT+MIPOPEN=%d,%d,\"%s\",%d,%d\r",socket.socketNum,socket.localPort,socket.ipUrl,socket.desPort,socket.udpTcp);
		//Send_AT_And_Wait("AT+MIPOPEN=1,5000,\"114.91.112.239\",80,1\r","+MIPOPEN: 1,1",2000);
		sprintf(socketState,"+MIPOPEN: %d,%d",socket.socketNum,1);

		printf("\r\nsocket cmd :%s\r\n",socketCmd);
		printf("\r\nsocket state:%s\r\n",socketState);

		socketFlag = 1;
	}
	if( checkConnectState(socket.socketNum) > 0){
		printf("network ok\r\n");
		return 1;		
	}
	else{
		printf("Net Error,reConnect\r\n");
		printf("\r\nestablish network\r\n");
	}	

	checkSimState:
	if(regieterCounter > 3)
	{
		#ifdef PRINTF_DEBUG
		printf("\r\n网络注册失败次数过多,设备故障，请检查\r\n");
		#endif
		sysErrorHandle(sim900NotInsert);
		sim900_power_on();
		regieterCounter = 0;
		return -1;	
	}
	regieterCounter++;
	if(power_sim900())//成功启动SIM900
	{
//		Send_AT_And_Wait("ATE0\r","OK",300);
//		Send_AT_And_Wait("AT+CREG=0\r","OK",200);
//		Send_AT_And_Wait("AT+CIPCLOSE\r","OK",300);	  
//		Send_AT_And_Wait("AT+CIPSHUT\r","OK",300);		

		
		for(i=0;i<30;i++)
		{
			if(Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,1",1000))
			break;	
			if(Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,5",1000))
			break;
		}
	
		if(i>=29)//网络注册失败						
		{
			#ifdef PRINTF_DEBUG
			printf("\r\nSIM900网络注册失败\r\n");
			#endif
			sysErrorHandle(sim900StartError);
			goto checkSimState;

		}
		else  //网络注册成功
		{
			read_cellid();
			read_csq();
//			getSimTime();
		
			if(setSimApn(0) < 0)
				goto checkSimState;
			
			for(i=0;i<10;i++)
			{
				//if(Send_AT_And_Wait(sysStateNow.NET_CMD,"OK",1500))
				Send_AT_And_Wait("AT\r","OK",400);
				Send_AT_And_Wait(socketCmd,socketState,2000);
				printf("\r\nwaiting net\r\n");
				while(1){								
					
					if(strstr(SIM_BUF,"+MIPSTAT") != NULL || strstr(SIM_BUF,"+MIPOPEN") != NULL)
					{	 	
						printf("\r\nopen cmd over,begin check state\r\n");
						if(1 == checkConnectState(socket.socketNum))
						{
							
							#ifdef PRINTF_DEBUG
							printf("\r\nscoket ok\r\n");
							printf("\r\nSIM900 reconnect OK\r\n");
							printf("\r\nSEND ONLINE MESSAGE\r\n");
							#endif

							return 1;
						}
						else
						{
							delay_ms(1000);	
							break;
						}
					}
					else
					{
						delay_ms(1000);
						printf("+++");
						j++;
						if(j > 5){
							j = 0;
							break;
						}
					}
				}
			}
			if(i>=9)//网络注册失败						
			{
				#ifdef PRINTF_DEBUG
				printf("\r\nSIM网络建立失败\r\n");
				#endif
				sysErrorHandle(sim900StartError);
				goto checkSimState;

			}	
		}
	}
	else			//SIM900启动失败++
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nSIM900启动失败\r\n");
		#endif
		sysErrorHandle(sim900StartError);
		goto checkSimState;
	
	}
	

	return 1;
}
/*******************************************************************************
 * 函数名称:reportGps                                                                    
 * 描    述:发送GPS数据                                                               
 *                                                                               
 * 输    入:SOCKET,网络参数 ，timeout : GPS未定位是的等待时间，  flag :未定位信息是否上报                                                                  
 * 输    出:无                                                                    
 * 返    回:-1：发生错误 0：不存在指定的socket 1：找到指定socket                                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月9日                                                                    
 *******************************************************************************/

void reportGps(SOCKET *soc,int timeout,int flag)
{
	int  i;
	char *temp;
	static uint32_t timeCounter = 0;
	static int32_t rtcModifyCounter = 0;
	i = 0;
	printf("\r\nREPORT GPS MESSAGE++++\r\n");
	printf("\r\nmodify clock : %d --> %d\r\n",rtcModifyFlag,rtcModifyCounter);
	USART_Cmd(GPS, DISABLE);

	delay_ms(100);//wait for gps irq exit //maybe useless
	if(gpsGpgga.state[0] == '1' && gpsGprmc.state[0] == 'A')
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nBegin GPS Message\r\n");
		#endif
		rtcModifyCounter++;
	}
	else
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nGPS not ready\r\n");
		#endif
		rtcModifyCounter = 0;
	}
	//连续定位三次（大约3分钟）并且rtc没有被配置过，重新配置rtc
	//或者rtc每连续定位30次，重新修正一下
	if((rtcModifyFlag == 0 && rtcModifyCounter > 2) || rtcModifyCounter % 5 == 0 && rtcModifyCounter > 3){
		
		rtcModifyFlag = 1;
		printf("\r\nmodify rtc timer-------\r\n");
		
		timer.year = 	(gpsGprmc.utcDate[4]-'0')*10+(gpsGprmc.utcDate[5]-'0') + 2000;
		timer.month = 	(gpsGprmc.utcDate[2]-'0')*10+(gpsGprmc.utcDate[3]-'0');
		timer.date = 	(gpsGprmc.utcDate[0]-'0')*10+(gpsGprmc.utcDate[1]-'0');
		timer.hour = 	(gpsGprmc.utc[0]-'0')*10+(gpsGprmc.utc[1]-'0');
		timer.minute = 	(gpsGprmc.utc[2]-'0')*10+(gpsGprmc.utc[3]-'0');
		timer.second = 	(gpsGprmc.utc[4]-'0')*10+(gpsGprmc.utc[5]-'0');
		showTime(&timer);
		RTC_Set(timer.year,timer.month,timer.date,timer.hour,timer.second,timer.second);
		RTC_Get();
		showTime(NULL);
		printf("set rtc ok");
//		delay_ms(5000);
//		while(1);
	}
	else{
		printf("\r\nnot raady for modify clock : %d  --> %d\r\n",rtcModifyFlag,rtcModifyCounter);
	}
	rtcModifyCounter++;


	memset(&gpsDataReport,0,sizeof(GPS_DATA_REPORT));

	gpsDataReport.stars 	= atoi(gpsGpgga.stars);	
	//get height
	gpsDataReport.height 	= atoi(gpsGpgga.height); 
	//get speed
	gpsDataReport.speed 	= atoi(gpsGprmc.speed);
	//get angle
	gpsDataReport.angle 	= atoi(gpsGprmc.angle);
	//get stars
	gpsDataReport.stars 	= atoi(gpsGpgga.stars);

	gpsDataReport.voltage 	= simState.voltage;//may be battery voltage

	gpsDataReport.signal	= (simState.csq[0]-'0')*10+(simState.csq[1]-'0');


	gpsDataReport.cimi 		= 	((u32)(simState.cimi[0]-'0'))*10000+\
								((u32)(simState.cimi[1]-'0'))*1000+\
								((u32)(simState.cimi[2]-'0'))*100+\
								((u32)(simState.cimi[3]-'0'))*10+\
								((u32)(simState.cimi[4]-'0'));
	
				
	#ifdef PRINTF_DEBUG	
	printf("\r\nCIMI：%d\r\n",gpsDataReport.cimi);
	#endif
	//get cellid
	gpsDataReport.cellid = strtol(simState.newCellid,&temp,16);
	#ifdef PRINTF_DEBUG	  
	printf("\r\nCELLID：%s ---> %d\r\n",simState.newCellid,gpsDataReport.cellid);
	#endif

	showGpsData();	
	if(gpsGpgga.state[0] == '1' || gpsGprmc.state[0] == 'A')
	{
		gpsDataReport.posState = 1;
		printf("\r\nGPS ready\r\n");	
	}
	else if(gpsDataReport.stars >= 4)
	{
		gpsDataReport.posState = 2;
		printf("\r\nGPS weak\r\n");
	}
	else
	{
		gpsDataReport.posState = 0;
		printf("\r\nGPS error\r\n");
	} 
	//get longitude经度	//10
	//get latitude 纬度	  //9
	gpsDataReport.longitude = 	((u32)(gpsGpgga.longitude[0]-'0'))*100000000 +\
								((u32)(gpsGpgga.longitude[1]-'0'))*10000000 +\
								((u32)(gpsGpgga.longitude[2]-'0'))*1000000 +\
								((u32)(gpsGpgga.longitude[3]-'0'))*100000 +\
								((u32)(gpsGpgga.longitude[4]-'0'))*10000 +\
								((u32)(gpsGpgga.longitude[6]-'0'))*1000 +\
								((u32)(gpsGpgga.longitude[7]-'0'))*100 +\
								((u32)(gpsGpgga.longitude[8]-'0'))*10 +\
								((u32)(gpsGpgga.longitude[9]-'0')) ;
	
	//get NS
	if(gpsGpgga.ns[0] == 'S' || gpsGpgga.ns[0] == 's')   //
	{
		gpsDataReport.latitude  |= 0x8000;
	}

	//get longitude经度	//10
	gpsDataReport.latitude = 	((u32)(gpsGpgga.latitude[0]-'0'))*10000000 +\
								((u32)(gpsGpgga.latitude[1]-'0'))*1000000 +\
								((u32)(gpsGpgga.latitude[2]-'0'))*100000 +\
								((u32)(gpsGpgga.latitude[3]-'0'))*10000 +\
								((u32)(gpsGpgga.latitude[5]-'0'))*1000 +\
								((u32)(gpsGpgga.latitude[6]-'0'))*100 +\
								((u32)(gpsGpgga.latitude[7]-'0'))*10 +\
								((u32)(gpsGpgga.latitude[8]-'0')) ;
	//get WE
	if(gpsGpgga.ew[0] == 'W' || gpsGpgga.ew[0] == 'w')
	{
		gpsDataReport.longitude |= 0x8000;
	}
	
		  

	#ifdef PRINTF_DEBUG
	printf("\r\nGPS DATA---->\r\n");
	printf("\r\nlongitude	:%10d",gpsDataReport.longitude);
	printf("\r\nlatitude	:%10d",gpsDataReport.latitude);
	printf("\r\ncimi		:%10d",gpsDataReport.cimi);
	printf("\r\ncellid		:%10d",gpsDataReport.cellid);
	printf("\r\nheight		:%10d",gpsDataReport.height);
	printf("\r\nspeed		:%10d",gpsDataReport.speed);
	printf("\r\nangle		:%10d",gpsDataReport.angle);
	printf("\r\nhdop		:%10d",gpsDataReport.hdop);
	printf("\r\nsignal		:%10d",gpsDataReport.signal);
	printf("\r\nstars		:%10d",gpsDataReport.stars);
	printf("\r\nstate		:%10d",gpsDataReport.posState);
	printf("\r\nvoltage		:%10d",gpsDataReport.voltage);

	#endif	 
	USART_Cmd(GPS, ENABLE);

	setDataHeader(DEV_POS_REPORT,(char *)(&gpsDataReport),sizeof(DEV_POS_REPORT));	
	dataSend((char *)(&gpsDataReport),sizeof(GPS_DATA_REPORT),1,1,1,*soc,1);

}	 



/*******************************************************************************
 * 函数名称:reportPos                                                                     
 * 描    述:建立TCP连接                                                               
 *                                                                               
 * 输    入:SOCKET,网络参数 ，timeout : GPS未定位是的等待时间，  flag :未定位信息是否上报                                                                  
 * 输    出:无                                                                    
 * 返    回:-1：发生错误 0：不存在指定的socket 1：找到指定socket                                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月9日                                                                    
 *******************************************************************************/

void reportPos(SOCKET *soc,int timeout,int flag)
{
	int  i;
	char *temp;
	TIME time;
	static uint32_t timeCounter = 0;
	static int32_t rtcModifyCounter = 0;
	i = 0;
	printf("\r\nREPORT GPS MESSAGE++++\r\n");
	if(GPS_RMC_DINGWEI_OK && GPS_GGA_DINGWEI_OK)
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nBegin GPS Message\r\n");
		#endif
		rtcModifyCounter++;
	}
	else
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nGPS not ready\r\n");
		#endif
		rtcModifyCounter = 0;
	}
	//连续定位三次（大约3分钟）并且rtc没有被配置过，重新配置rtc
	//或者rtc每连续定位30次，重新修正一下
	if((rtcModifyFlag == 0 && rtcModifyCounter > 3) || rtcModifyCounter % 30 == 0 && rtcModifyCounter > 3){
		
		rtcModifyFlag = 1;
		printf("\r\nmodify rtc timer-------\r\n");
		timer.year = (GPS_RMC_Data.UTCDate[4]-'0')*10+(GPS_RMC_Data.UTCDate[5]-'0');
		timer.month = (GPS_RMC_Data.UTCDate[2]-'0')*10+(GPS_RMC_Data.UTCDate[3]-'0');
		timer.date = (GPS_RMC_Data.UTCDate[0]-'0')*10+(GPS_RMC_Data.UTCDate[1]-'0');
		timer.hour = (GPS_RMC_Data.UTCTime[0]-'0')*10+(GPS_RMC_Data.UTCTime[1]-'0');
		timer.minute = (GPS_RMC_Data.UTCTime[2]-'0')*10+(GPS_RMC_Data.UTCTime[3]-'0');
		timer.second = (GPS_RMC_Data.UTCTime[4]-'0')*10+(GPS_RMC_Data.UTCTime[5]-'0');
		RTC_Set(timer.year,timer.month,timer.date,timer.hour,timer.second,timer.second);
	}
	else{
		printf("\r\nnot raady for modify clock :%d %d\r\n",rtcModifyFlag,rtcModifyCounter);
	}

//	if(rtcModifyFlag != 0){
//		RTC_Get();
//		gpsDataReport.time = timer;	
//	}
//	else{
//		memset((char *)&gpsDataReport.time ,sizeof(TIME),0);
//	}
//	showTime(&gpsDataReport.time);

	
		
	gpsDataReport.voltage = simState.voltage;
	
	#ifdef PRINTF_DEBUG
	printf("\r\nVoltage：%d\r\n",gpsDataReport.voltage);
	#endif

	gpsDataReport.signal= (simState.csq[0]-'0')*10+(simState.csq[1]-'0');
	#ifdef PRINTF_DEBUG
	printf("\r\nCSQ：%d\r\n",gpsDataReport.signal);
	#endif

	gpsDataReport.cimi = 	((u32)(simState.cimi[0]-'0'))*10000+\
							((u32)(simState.cimi[1]-'0'))*1000+\
							((u32)(simState.cimi[2]-'0'))*100+\
							((u32)(simState.cimi[3]-'0'))*10+\
							((u32)(simState.cimi[4]-'0'));
	
				
	#ifdef PRINTF_DEBUG	
	printf("\r\nCIMI：%d\r\n",gpsDataReport.cimi);
	#endif
	//get cellid
	gpsDataReport.cellid = strtol(simState.newCellid,&temp,16);
	#ifdef PRINTF_DEBUG	  
	printf("\r\nCELLID：%d\r\n",gpsDataReport.cellid);
	#endif


	

	gpsDataReport.posState = 0;
	if(GPS_RMC_DINGWEI_OK && GPS_GGA_DINGWEI_OK)
	{
		printf("\r\nGPS DATA READY\r\n");
		printf("\r\nGPS RMC UTC    :%s\r\n",GPS_RMC_Data.UTCTime);
		printf("\r\nGPS RMC STA    :%s\r\n",GPS_RMC_Data.Status);
		printf("\r\nGPS RMC Lat    :%s\r\n",GPS_RMC_Data.Latitude);
		printf("\r\nGPS RMC NS     :%s\r\n",GPS_RMC_Data.NS);
		printf("\r\nGPS RMC LON    :%s\r\n",GPS_RMC_Data.Longitude);
		printf("\r\nGPS RMC EW     :%s\r\n",GPS_RMC_Data.EW);
		printf("\r\nGPS RMC SPEED  :%s\r\n",GPS_RMC_Data.Speed);
		printf("\r\nGPS RMC Course :%s\r\n\r\n\r\n",GPS_RMC_Data.Course);
		printf("\r\nGPS GGA Course :%s\r\n",GPS_GGA_Data.Latitude);
		printf("\r\nGPS GGA Course :%s\r\n",GPS_GGA_Data.NS);
		printf("\r\nGPS GGA Course :%s\r\n",GPS_GGA_Data.Longitude);
		printf("\r\nGPS GGA Course :%s\r\n",GPS_GGA_Data.EW);
		printf("\r\nGPS GGA Course :%s\r\n",GPS_GGA_Data.PositionFix);
		printf("\r\nGPS GGA Course :%s\r\n",GPS_GGA_Data.SateUsed);
		printf("\r\nGPS GGA Course :%s\r\n",GPS_GGA_Data.HDOP);
		printf("\r\nGPS GGA Course :%s\r\n",GPS_GGA_Data.HEIGHT);
		printf("\r\nGPS GGA Course :%s\r\n",GPS_GGA_Data.hdop_int);

		printf("\r\nGPS GGA Course :%s\r\n",GPS_DATA.A_Latitude);
		printf("\r\nGPS GGA Course :%s\r\n",GPS_DATA.A_NS);
		printf("\r\nGPS GGA Course :%s\r\n",GPS_DATA.A_Longitude);
		printf("\r\nGPS GGA Course :%s\r\n",GPS_DATA.A_EW);
		printf("\r\nGPS GGA Course :%d\r\n",GPS_DATA.HDOP);
		printf("\r\nGPS GGA Course :%s\r\n",GPS_DATA.Speed);
		printf("\r\nGPS GGA Course :%s\r\n",GPS_DATA.Course);

		

		gpsDataReport.posState = 1;


		


		//get height
		gpsDataReport.height = atoi(GPS_GGA_Data.HEIGHT);
		#ifdef PRINTF_DEBUG
		printf("\r\nGPS high：%d\r\n",gpsDataReport.height);
		#endif
		//	delay_ms(2000);
		//get state
		gpsDataReport.posState = 1;
		//get longitude经度	//10
		gpsDataReport.longitude = 	((u32)(GPS_DATA.A_Longitude[0]-'0'))*100000000 +\
									((u32)(GPS_DATA.A_Longitude[1]-'0'))*10000000 +\
									((u32)(GPS_DATA.A_Longitude[2]-'0'))*1000000 +\
									((u32)(GPS_DATA.A_Longitude[3]-'0'))*100000 +\
									((u32)(GPS_DATA.A_Longitude[4]-'0'))*10000 +\
									((u32)(GPS_DATA.A_Longitude[6]-'0'))*1000 +\
									((u32)(GPS_DATA.A_Longitude[7]-'0'))*100 +\
									((u32)(GPS_DATA.A_Longitude[8]-'0'))*10 +\
									((u32)(GPS_DATA.A_Longitude[9]-'0')) ;
		//get latitude 纬度	  //9
		gpsDataReport.latitude = 		((u32)(GPS_DATA.A_Latitude[0]-'0'))*10000000 +\
									((u32)(GPS_DATA.A_Latitude[1]-'0'))*1000000 +\
									((u32)(GPS_DATA.A_Latitude[2]-'0'))*100000 +\
									((u32)(GPS_DATA.A_Latitude[3]-'0'))*10000 +\
									((u32)(GPS_DATA.A_Latitude[5]-'0'))*1000 +\
									((u32)(GPS_DATA.A_Latitude[6]-'0'))*100 +\
									((u32)(GPS_DATA.A_Latitude[7]-'0'))*10 +\
									((u32)(GPS_DATA.A_Latitude[8]-'0')) ;
		//get WE
		if(GPS_DATA.A_EW == 'W' || GPS_DATA.A_EW == 'w')
		{
			gpsDataReport.longitude |= 0x8000;
		}
		//get NS
		if(GPS_DATA.A_NS == 'S' || GPS_DATA.A_NS == 's')   //
		{
			gpsDataReport.latitude  |= 0x8000;
		}
		//get height
		gpsDataReport.height = 0;

		//get speed
		gpsDataReport.speed = 0;
		if((temp = strstr(GPS_DATA.Speed,".")) != NULL)
		{
			while(1)
			{	
				gpsDataReport.speed = *(--temp) - '0';//取个位
				if(GPS_DATA.Speed == temp)
				{
					break;
				}
				if(GPS_DATA.Speed == --temp )
				{
					gpsDataReport.speed += (*(temp) - '0')*10;
					break;
				}
				gpsDataReport.speed = (*(temp)-'0')*10 + ((u16)(*(--temp)-'0'))*100;
				break;
			}
		}
		else
		{
		gpsDataReport.speed =	0;	
		}
		//get angle
		gpsDataReport.angle = 0;
		if(strstr(GPS_DATA.Course,"."))
		{
			i=(strstr(GPS_DATA.Course,".")-GPS_DATA.Course);
			
			if(i==3)
			{
				gpsDataReport.angle = ((u16)(GPS_DATA.Course[0]-'0' ))*100 + \
									((u16)(GPS_DATA.Course[1]-'0' ))*10 + \
									((u16)(GPS_DATA.Course[2]-'0' ));
			}
			else if(i==2)
			{
				gpsDataReport.angle = ((u16)(GPS_DATA.Course[0]-'0' ))*10 + \
									((u16)(GPS_DATA.Course[1]-'0' ));
			
			}
			else if(i==1)
			{
				gpsDataReport.angle = ((u16)(GPS_DATA.Course[0]-'0' ));				
			}
			else
			{
				gpsDataReport.angle = 0;	
			}	
		 }
		else
		{
			gpsDataReport.angle = 0;
		}
		//get stars
		gpsDataReport.stars = 0; 
		gpsDataReport.stars = ((GPS_DATA.A_SateUsed[0]-'0')*10+(GPS_DATA.A_SateUsed[1]-'0'));	
		
//$GPGGA,060057.291,3130.1393,N,12021.7751,E,1,04,4.6,25.5,M,6.8,M,,0000*55	   
	}
	else if(GPS_SHUJU_OK)			  //如果GPS有数据但没定位
	{
		printf("\r\nGPS DATA WEAK\r\n");
		gpsDataReport.posState = 2;
		gpsDataReport.longitude = 	((u32)(GPS_DATA.V_Longitude[0]-'0'))*10000000 +\
									((u32)(GPS_DATA.V_Longitude[1]-'0'))*1000000 +\
									((u32)(GPS_DATA.V_Longitude[2]-'0'))*100000 +\
									((u32)(GPS_DATA.V_Longitude[3]-'0'))*10000 +\
									((u32)(GPS_DATA.V_Longitude[4]-'0'))*1000 +\
									((u32)(GPS_DATA.V_Longitude[6]-'0'))*100 +\
									((u32)(GPS_DATA.V_Longitude[7]-'0'))*10 +\
									((u32)(GPS_DATA.V_Longitude[8]-'0')) ;
		//get latitude
		gpsDataReport.latitude = 		((u32)(GPS_DATA.V_Latitude[0]-'0'))*10000000 +\
									((u32)(GPS_DATA.V_Latitude[1]-'0'))*1000000 +\
									((u32)(GPS_DATA.V_Latitude[2]-'0'))*100000 +\
									((u32)(GPS_DATA.V_Latitude[3]-'0'))*10000 +\
									((u32)(GPS_DATA.V_Latitude[4]-'0'))*1000 +\
									((u32)(GPS_DATA.V_Latitude[6]-'0'))*100 +\
									((u32)(GPS_DATA.V_Latitude[7]-'0'))*10 +\
									((u32)(GPS_DATA.V_Latitude[8]-'0')) ;

				//get WE
		if(GPS_DATA.V_EW == 'W' || GPS_DATA.V_EW == 'w')
		{
			gpsDataReport.longitude |= 0x8000;
		}
		//get NS
		if(GPS_DATA.V_NS == 'S' || GPS_DATA.V_NS == 's')   //
		{
			gpsDataReport.latitude  |= 0x8000;
		}
		//get height
		gpsDataReport.height = 0;
		//get speed
		gpsDataReport.speed = 0;
		//get angle
		gpsDataReport.angle = 0;

		//get stars
		gpsDataReport.stars = 0; 
		gpsDataReport.stars = ((GPS_DATA.V_SateUsed[0]-'0')*10+(GPS_DATA.V_SateUsed[1]-'0'));
	}
	else
	{
		gpsDataReport.posState = 0;
		gpsDataReport.longitude = 0;
		gpsDataReport.latitude = 0;
		//get height
		gpsDataReport.height = 0;
		//get speed
		gpsDataReport.speed = 0;
		//get angle
		gpsDataReport.angle = 0;
		//get stars
		gpsDataReport.stars = 0;
		#ifdef PRINTF_DEBUG
		printf("\r\nNO GPS DATA：\r\n");
		#endif	
//		return ; 
	} 
	#ifdef PRINTF_DEBUG
	printf("\r\nGPS DATA：\r\n");
	printf("\r\nlongitude：%10d",gpsDataReport.longitude);
	printf("\r\nlatitude ：%10d",gpsDataReport.latitude);
	printf("\r\nheight   ：%10d",gpsDataReport.height);
	printf("\r\nspeed    ：%10d",gpsDataReport.speed);
	printf("\r\nangle    ：%10d",gpsDataReport.angle);
	printf("\r\nstars    ：%10d",gpsDataReport.stars);
	printf("\r\nstate    ：%10d",gpsDataReport.posState);
//	printf("\r\nTime: %2d%2d%2d%2d%2d%2d\r\n",gpsDataReport.timeYY,gpsDataReport.timemm,gpsDataReport.timeDD,gpsDataReport.timeHH,gpsDataReport.timeMM,gpsDataReport.timeSS);
	#endif	 


	setDataHeader(DEV_POS_REPORT,(char *)(&gpsDataReport),sizeof(DEV_POS_REPORT));	
	dataSend((char *)(&gpsDataReport),sizeof(GPS_DATA_REPORT),1,1,1,*soc,1);

}	 

/*******************************************************************************
 * 函数名称:establishConnect                                                                     
 * 描    述:建立TCP连接                                                               
 *                                                                               
 * 输    入:建立连接的类型0：TCP 1：UDP                                                                    
 * 输    出:无                                                                    
 * 返    回:-1：发生错误 0：不存在指定的socket 1：找到指定socket                                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月9日                                                                    
 *******************************************************************************/
int closeConnect(SOCKET *soc){
	int8_t	cmd[20];
	sprintf(cmd,"AT+ MIPCLOSE=%d\r",soc->socketNum);
	Send_AT_And_Wait(cmd,"OK",500);
	return 1;
}
/*******************************************************************************
 * 函数名称:str2hex                                                                     
 * 描    述:将指定长度的字符串转化成HEX串                                                               
 *                                                                               
 * 输    入:src 待转换字符串 des：目标字符串,传入前需初始化完成 length：待转化字符串长度                                                                  
 * 输    出:无                                                                    
 * 返    回:-1：发生错误 正偶数：转化后的字符串长度                                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月9日                                                                    
 *******************************************************************************/
int16_t str2hex(int8_t *src,uint16_t length,int8_t *des){
	int16_t i = 0,j=0;
	int8_t tmp[2],temp = '0';
//	int8_t useless = 0;
	if(length ==0 ){
		return 0;
	}
	if(src == NULL  || des == NULL)	 {
		printf("\r\nstring init error!\r\n");
		return -1;
	}
//	printf("\r\n");
//	for(i=0;i<length;i++){
//		printf("%4d ",(uint8_t)src[i]);
//	}
//	printf("\r\n");
	
	for(i=0;i<length;i++){
//		tmp[0] = '\0' ;
//		tmp[1] = '\0' ;
//		tmp[2] = '\0' ;
		sprintf(tmp,"%2X",(uint8_t)(src[i]));
//		sprintf(tmp,"%2X",(uint8_t)src[i]);
		if(src[i] <= 0x0F && src[i] >= 0){
			tmp[0] = '0';
		}
//		temp = src[i] >> 4;
//		tmp[0] = (src[i]>>4) > 0x09 ? ((src[i]>>4)+55) : (src[i] + 48);
//		tmp[1] = (src[i] & 0x0F) > 0x09 ? (src[i]+55) : (src[i] + 48);

//		printf("%4X-%s",src[i],tmp); 		
		strcat(des,tmp);
		j+=2;
	}
//	#ifdef PRINTF_DEBUG
//	printf("\r\nsrc string : %s\r\n",src);
//	printf("\r\ndes string : %s\r\n",des);
//	printf("\r\nsrc len :%d\r\n",length);
//	printf("\r\ndes len :%d\r\n",j);
//	#endif
	return j;
}

/*******************************************************************************
 * 函数名称:setDataHeader                                                                    
 * 描    述:设置数据头                                                            
 *                                                                               
 * 输    入:无                                                          
 * 输    出:无                                                                    
 * 返    回:0：正常 非零：校验出错                                                   
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年5月9日                                                                    
 *******************************************************************************/
void setDataHeader(uint16_t type,int8_t *pointer,int16_t length)
{

	uint16_t i = 0;
	int8_t buf[300];

	if(rtcModifyFlag != 0){
		RTC_Get();
		obdHeader.time[0] = timer.year % 100;
		obdHeader.time[1] = timer.month;
		obdHeader.time[2] = timer.date;
		obdHeader.time[3] = timer.hour;
		obdHeader.time[4] = timer.minute;
		obdHeader.time[5] = timer.second;	
	}
	else{
		memset((obdHeader.time),0,6);
	}
	showTime(&obdHeader.time);
	obdHeader.START[0] = '*';
	obdHeader.START[1] = '*';
		
	obdHeader.MSG_TYPE = type;

	obdHeader.IMEI_H = simState.imeiHigh;
	obdHeader.IMEI_L = simState.imeiLow;

	obdHeader.DEV_ID_H = 0;
	obdHeader.DEV_ID_L = 0;

//	printf("\r\nIMEI :%d-%d\r\n",simState.imeiHigh,simState.imeiLow);
	printf("\r\nIMEI :%d-%d\r\n",obdHeader.IMEI_H,obdHeader.IMEI_L);
//	printf("\r\nVID :%d-%d\r\n",obdHeader.DEV_ID_H,obdHeader.DEV_ID_L);

	if(POWER_STATE != 0){
		obdHeader.IMEI_H = obdHeader.IMEI_H | 0xFF000000;//最高位置一
	}

	//not set data length 
	obdHeader.MSG_LENGTH = length;

	obdHeader.MSG_CRC = calBufsCrc((char *)(&obdHeader) +2,sizeof(DATA_HEAD)-2,pointer,length,NULL,0);

//	printf("\r\nCRC :%X  ->DATA HEADER :\r\n",obdHeader.MSG_CRC);
//	for(i = 0;i < sizeof(DATA_HEAD);i++){
//   		printf("%3X",((char *)(&obdHeader))[i]);
//	}
//	printf("\r\n");

}
																																		  
int16_t dataSend(char *pointer,int length,int head,int reSend,int checkAck,SOCKET socket,int reConnect)//type 0:不自动发送数据包头 1：自动发送  gpsFlag :发送gps数据包
{
	static int8_t sendBuf[1000],sendCmd[20],setSocket[20];
	int16_t	dataLength = 0;
	char netSend[20];
	int i=0;
	int reTryCounter = 0;
	int reStr2HexCounter = 0;



	memset(sendBuf,'\0',1000);
	
//------------------------------建立连接-----------------------------------	 	
		
	memset(netSend,'\0',20);
	reTryCounter = 0;
	reTrySend://发送重试3次后保存数据
	
	if(reTryCounter > 2)
	{
		//存储数据
		if(reSend == 1)
		{
			#ifdef PRINTF_DEBUG
			printf("\r\ndata resend error ,not store\r\n");
			#endif			
			return -1;			
		}
		#ifdef PRINTF_DEBUG
		printf("\r\ndata resend error store into Flash\r\n");
		#endif
//		RED_ON;
		reTryCounter = 0;
//		storeToFlash(pointer,length,head);
		delay_ms(1000);
//		RED_OFF;
		return 0;
	}
	reTryCounter++;
	if(reConnect){
		printf("\r\ncheck net state\r\n");
		if(establishConnect(socket,1) <= 0)
		{
			#ifdef PRINTF_DEBUG
			printf("\r\nreconnect net error\r\n");
			#endif
			goto reTrySend;	
		}
	}
 		
//-------------------------------发送数据报文---------------------------------------------- 
	reStr2HexCounter = 0;
	reStr2Hex :
	reStr2HexCounter++;
	if(reStr2HexCounter > 2){
		printf("\r\nstr2Hex error\r\n");
		return -1;

	}
	memset(sendBuf,'\0',1000);
	dataLength = 0;
	printf("\r\ndata length : %d\r\n",dataLength);
	if(head){
		dataLength = str2hex((int8_t *)(&obdHeader),sizeof(DATA_HEAD),sendBuf);

		if( (dataLength%2) != 0){
			printf("\r\nchar tralate header error\r\n");
			goto reStr2Hex ;

		} 
		dataLength = str2hex((int8_t *)pointer,length,sendBuf+dataLength) + dataLength;

		if((dataLength%2) != 0){
			printf("\r\ntranslate data error\r\n");
			goto reStr2Hex ;

		}

	}
	else{
	 	dataLength = str2hex((int8_t *)pointer,length,sendBuf+strlen(sendBuf)) + dataLength;
		if((dataLength%2) != 0){
			printf("\r\ntranslate data error\r\n");
			return -1;
		}
		

	}

	sprintf(setSocket,"AT+MIPSETS=1,%d,500\r",dataLength/2);
//	sprintf(setSocket,"AT+MIPSETS=1,%d\r",42);
	printf("\r\nset socket cmd:%s\r\n",setSocket);	
	Send_AT_And_Wait(setSocket,"OK",500); 


	memset(SIM_BUF,'\0',1300);	 //串口2接收缓存清零 
	SIM_COUNT = 0;	


	memset(sendCmd,'\0',20);
	sprintf(sendCmd,"AT+MIPSEND=%d,\"",socket.socketNum);
//	printf("\r\ndata  :%s\r\n",sendBuf);
//	printf("\r\nsend cmd :%s%s",sendCmd,sendBuf);
	
//	USART_SendData(ISP,'0');	
//	while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);
//	USART_SendData(ISP,'D');	
//	while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);
//	USART_SendData(ISP,'"');	
//	while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);
//	USART_SendData(ISP,'\r');	
//	while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);
//	USART_SendData(ISP,'\n');	
//	while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);

//	printf("\r\ncmd:%d  buf:%d  data:%d\r\n",strlen(sendCmd),strlen(sendBuf),dataLength); 

	delay_ms(1000);

//	printf("\r\ncmd size :%d\r\n",sizeof(sendCmd));
//	printf("\r\nbuf size :%d\r\n",dataLength);
	for(i=0;i<strlen(sendCmd);i++)//cmd
	{
		USART_SendData(SIM,sendCmd[i]);	
		while(USART_GetFlagStatus(SIM,USART_FLAG_TC)==RESET);					
	}
	
	for(i=0;i<dataLength;i++)//data
	{
		USART_SendData(SIM,sendBuf[i]);	
		while(USART_GetFlagStatus(SIM,USART_FLAG_TC)==RESET);

		USART_SendData(ISP,sendBuf[i]);	
		while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);
						
	}
//	USART_SendData(SIM,'0');	
//	while(USART_GetFlagStatus(SIM,USART_FLAG_TC)==RESET);
//	USART_SendData(SIM,'D');	
	while(USART_GetFlagStatus(SIM,USART_FLAG_TC)==RESET);
	USART_SendData(SIM,'"');	
	while(USART_GetFlagStatus(SIM,USART_FLAG_TC)==RESET);
	USART_SendData(SIM,'\r');	
	while(USART_GetFlagStatus(SIM,USART_FLAG_TC)==RESET);

	printf("\r\n");	 

	while(1)
	{
		delay_ms(100);
		if(strstr(USART2_RX_buffer,"OK"))
		{
			#ifdef PRINTF_DEBUG	
			printf("\r\nFIND SEND OK,CONTINUE\r\n"); 
			#endif
			break;	
		}
		if(strstr(USART2_RX_buffer,"ERROR"))
		{
			#ifdef PRINTF_DEBUG
			printf("\r\nFIND ERROR.GOTO STORE AND ERROR\r\n"); 	
			#endif 
			goto  reTrySend;
			//stroe message
		}
		i++;
		if(i>50)
		{
			#ifdef PRINTF_DEBUG	
			printf("\r\nTIME OUT ,GOON\r\n");
			#endif
			break;

		}
	}
	printf("\r\nWait for udp data\r\n");
	delay_ms(5000);
	if(checkAck)//检查返回数据
	{
		checkAckMsg(0);//检查返回信息
	} 
	return 1;

}
/*******************************************************************************
 * 函数名称:reportObd                                                                     
 * 描    述:建立TCP连接                                                               
 *                                                                               
 * 输    入:建立连接的类型0：TCP 1：UDP                                                                    
 * 输    出:无                                                                    
 * 返    回:-1：发生错误 0：不存在指定的socket 1：找到指定socket                                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月9日                                                                    
 *******************************************************************************/
//extern OBD_MSG_BUG obdBuf;
//typedef struct 
//{
//	int8_t week;
//	int16_t year;
//
//	int8_t month;
//	int8_t date;
//	int8_t hour;
//	int8_t minute;
//	int8_t second;
//		
//}TIME;
char ql100Buf[1024];
void reportQL100(SOCKET *soc,int timeout,int flag,int mode){
	int32_t len = 0,i = 0,msgLength = 0;
	static uint32_t obdType = 0; 

	printf("\r\nreport ql100 data_+_+_+_+_+_+_+_+_\r\n");
	TIM_Cmd(TIM3, DISABLE);
	delay_ms(100);
	if(rtcModifyFlag != 0){
	   		RTC_Get();
			obdFastBuf.time = timer;
	}
	else{
		memset((uint8_t *)&obdFastBuf.time ,sizeof(TIME),0);
	}
	showTime(&obdNormalBuf.time);
	memset(ql100Buf,0,1024);
	if(obdType++ % mode != 0){//normal obd data
		printf("\r\nreport FAST obd data\r\n");
		strcat(ql100Buf,"&");
		strcat(ql100Buf,obdInfo.vin);
		strcat(ql100Buf,"&");
		strcat(ql100Buf,obdInfo.eid);
		for(i = 0 ;i<pidBuf.cmdNum;i++){ //accel data and pids data
			printf("pid buf %d -->%s",i,pidBuf.mulCmd[i].buf);
			strcat(ql100Buf,pidBuf.mulCmd[i].buf);
		}
		msgLength = strlen(ql100Buf);
		printf("\r\n\r\n\r\n\r\nobd fast --> %s\r\n",ql100Buf);
		
		setDataHeader(OBD_FAST_REPORT,(int8_t *)(ql100Buf),msgLength);
	}
	else{
		printf("\r\nreport NORMAL obd data\r\n");
		strcat(ql100Buf,"&");
		strcat(ql100Buf,obdInfo.vin);
		strcat(ql100Buf,"&");
		strcat(ql100Buf,obdInfo.eid);
		strcat(ql100Buf,"&");
		strcat(ql100Buf,obdInfo.rdtc);
		strcat(ql100Buf,"&");
		strcat(ql100Buf,obdInfo.edtc);
		strcat(ql100Buf,"&");
		strcat(ql100Buf,obdInfo.mil);
		strcat(ql100Buf,"&");
		strcat(ql100Buf,obdInfo.spwr);
		strcat(ql100Buf,"&");
		strcat(ql100Buf,obdInfo.ife);
		strcat(ql100Buf,"&");
		strcat(ql100Buf,obdInfo.whp);
		strcat(ql100Buf,"&");
		strcat(ql100Buf,obdInfo.ehp);
		strcat(ql100Buf,"&");
		strcat(ql100Buf,obdInfo.ad_mil);
		strcat(ql100Buf,"&");
		strcat(ql100Buf,obdInfo.ad_feh);
		msgLength = strlen(ql100Buf);
		printf("\r\n\r\n\r\n\r\nobd normal --> %s\r\n",ql100Buf);
		setDataHeader(OBD_COMMON_REPORT,(int8_t *)(ql100Buf),msgLength);
	}	
	TIM_Cmd(TIM3, ENABLE);
	dataSend((int8_t *)ql100Buf,msgLength,1,0,0,*soc,1);
}



void reportObd(SOCKET *soc,int timeout,int flag,int mode){
	uint32_t msgLength = 0;
	printf("\r\nREPORT OBD MESSAGE++++\r\n");
	TIM_Cmd(TIM3, DISABLE);
	if(mode ==0){
		if(rtcModifyFlag != 0){
	   		RTC_Get();
			obdFastBuf.time = timer;
		}
		else{
			memset((uint8_t *)&obdFastBuf.time ,sizeof(TIME),0);
		}
		obdNormalBuf.itemNum = sysCfg.obdConfig.normalEnd - sysCfg.obdConfig.normalEnd;
		showTime(&obdNormalBuf.time);
		msgLength =  getBufIndex(sysCfg.obdConfig.normalEnd,0) + sysCfg.obdConfig.cmdList[sysCfg.obdConfig.normalEnd].dataLength  + 8;// 最后一条数据的位置+最后一条数据的长度+obd头的长度
		printf("\r\nOBD normal data begin length --> %d\r\n",msgLength);
		
		setDataHeader(OBD_COMMON_REPORT,(int8_t *)(&obdNormalBuf),msgLength);
		dataSend((int8_t *)(&obdNormalBuf),msgLength,1,0,0,*soc,1);
	}
	if(mode == 1){
		if(rtcModifyFlag != 0){
	   		RTC_Get();
			obdFastBuf.time = timer;
		}
		else{
			memset((uint8_t *)&obdFastBuf.time ,sizeof(TIME),0);
		}
		obdFastBuf.itemNum = sysCfg.obdConfig.fastEnd - sysCfg.obdConfig.fastStart;
		showTime(&obdFastBuf.time);
		msgLength =  getBufIndex(sysCfg.obdConfig.fastEnd,0) + sysCfg.obdConfig.cmdList[sysCfg.obdConfig.fastEnd].dataLength  + 8;// 最后一条数据的位置+最后一条数据的长度+obd头的长度
		printf("\r\nOBD normal data begin length --> %d\r\n",msgLength);
		
		setDataHeader(OBD_FAST_REPORT,(int8_t *)(&obdFastBuf),msgLength);
		dataSend((int8_t *)(&obdFastBuf),msgLength,1,0,0,*soc,1);
	}
	TIM_Cmd(TIM3, ENABLE);
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
void reportHeart(SOCKET *soc,int timeout,int flag){
	printf("\r\nHEART BEAT\r\n");
//	setDataHeader(HEARTHEAT_MSG,NULL,0);
	dataSend(NULL,0,1,0,0,*soc,1); 
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
void initSimSms(int type)
{


}

























