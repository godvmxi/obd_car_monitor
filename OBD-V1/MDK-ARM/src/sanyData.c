#include "sanyData.h"

int CAN_LOCK_FLAG = 0;
int CAN_UN_LOCK_FLAG = 0;

SANY_DATA sanyData;
SANY_POS_REPORT sanyPosData;
extern FIFO_NODE fifoNode[FIFO_MAX_SIZE];
extern FIFO_HEAD fifoHead;
extern DATA_IN_FLASH sysStateNow;
extern int voltage;


extern char cimi[5],csq[2],new_cellid[8];//,old_cellid[8];
extern stru_GPSRMC  	GPS_RMC_Data; 
extern stru_GPSGGA  	GPS_GGA_Data; 
extern stru_GPSGSV  	GPS_GSV_Data;
extern stru_GPS_DATA  	GPS_DATA;
extern int		GPS_GGA_DINGWEI_OK;//若GPS定位，则将此变量置1，否则为0
extern int		GPS_SHUJU_OK;//若GPS输出坐标信息，则将此变量置1，否则为0 
extern int		GPS_RMC_DINGWEI_OK;
CAN_RX_BUFFER	sanyCanBuffer;
CAN_TX_BUFFER	sanyTxBuffer;
RTC_STRUCT rtcData;
int RTC_MODIFY_FLAG = 0;
u16 IPD_FLAG_1 = 0;
u16 IPD_FLAG_2 = 0;
u16 rxMsgFlag = 0;
u16 rxBufferFlag = 0;
char rxBuffer1[IPD_BUFFER];
char rxBuffer2[IPD_BUFFER];
char buffer1[IPD_BUFFER];
char buffer2[IPD_BUFFER];
extern int ERROR_FLAG;
extern volatile int	blueToothCounter;
int blueToothState = 0;//0:未连接 1：已连接

int systemStopState = 0;//0:运动 1：静止
volatile int canCounter = 0;
extern volatile int onlineTimeOutFlag;

volatile int canMsgLockType	= 0;//can data lock type

void reportheartHeat(void)
{
	#ifdef PRINTF_DEBUG
	printf("\r\nBegin HeartBeat Message\r\n");
	#endif
	delay_ms(2000);
	getDataHead(HEART_MSG,NULL,0);
	sendData(NULL,0,1,0,1);
}
void reportOnline(void)
{

	#ifdef PRINTF_DEBUG
	printf("\r\nBegin Online Message\r\n");
	#endif
	getDataHead(ONLINE_MSG,NULL,0);
	sendData(NULL,0,1,0,1);
}
void reportCan(void)
{
	
	CAN_TX_BUFFER *canData;

	#ifdef PRINTF_DEBUG
	printf("\r\nBegin Can Message\r\n");
	#endif

		canData = getCanBufferPointer(0);



		canData->timeYY = 0;
		canData->timemm = 0;
		canData->timeDD = 0;
		canData->timeHH = 0;
		canData->timeMM = 0;
		canData->timeSS = 0;	

	getDataHead(REPORT_CAN_MSG,(char *)canData,sizeof(CAN_TX_BUFFER));
	showCanData(*canData);
	sendData((char *)canData,sizeof(CAN_TX_BUFFER),1,0,1);
	// printf("\r\n Can ERROR_FLAG:%d\r\n\r\n",ERROR_FLAG);
}

void reportOffline(void)
{
	#ifdef PRINTF_DEBUG
	printf("\r\nBegin offline Message\r\n");
	#endif
	getDataHead(OFFLINE_MSG,NULL,0);
	sendData(NULL,0,1,0,1);
}
u8  getCrc(char *pointer,int length)
{
	int i = 0;
	u8 crc = 0;
	char *head = (char *)(&sanyData) + 4;//skip head 4 bytes
	for(i=0;i<sizeof(SANY_DATA)-4;i++)//cal head
	{
		crc ^= head[i]; 
	}
	if(length != 0)
	{
		for(i=0;i<length;i++) //cal data
		{
			crc ^= pointer[i];
		}
	}
//	#ifdef PRINTF_DEBUG
//	printf("\r\nCRC 校验值为：%d --> %X\r\n",crc,crc);
//	#endif
	return crc;
}
u8   checkCrc(char *pointer,int length)
{
	u8 crc = 0;
	int i;
	for(i=0;i<length;i++)
	{
		crc ^= pointer[i];
	}
	return crc;
}
void getDataHead(u16 type,char *pointer,int length)
{

	int i = 0;
//	#ifdef PRINTF_DEBUG
//	printf("\r\n获取数据头：%X\r\n",type);
//	#endif
	sanyData.MSG_TAG[0] = '*';
	sanyData.MSG_TAG[1] = '*';
	sanyData.MSG_TAG[2] = '*';

	if(type == 0x0102) //first send
		sanyData.MSG_ID = 0;
	else
		sanyData.MSG_ID++;//
	sanyData.MSG_TYPE = type;	
	for(i= 0;i<8;i++) //get MDT_ID
		sanyData.MDT_ID[i] = sysStateNow.G_CODE[i];
	//not set data length 
	sanyData.MSG_LENGTH = length;
//	#ifdef PRINTF_DEBUG
//	printf("\r\n获取数据长度：%d\r\n",length);
//	#endif

	sanyData.MSG_CRC = getCrc(pointer,length);
}

void sendData(char *pointer,int length,int head,int reSend,int checkAck)//type 0:不自动发送数据包头 1：自动发送  gpsFlag :发送gps数据包
{

//------------------------------建立连接-----------------------------------	 		
	int i=0;
	static int reTryCounter = 0;
	
	char netSend[20];	
	memset(netSend,'\0',20);
	reTryCounter = 0;
	reTrySend://发送重试3次后保存数据
	
	if(reTryCounter > 1)
	{
		//存储数据
		if(reSend == 1)
		{
			#ifdef PRINTF_DEBUG
			printf("\r\n数据重发失败状态，不补发数据\r\n");
			#endif			
			return ;			
		}
		#ifdef PRINTF_DEBUG
		printf("\r\n数据重发失败，存储到数据Flash中\r\n");
		#endif
//		RED_ON;
		reTryCounter = 0;
//		storeToFlash(pointer,length,head);
		delay_ms(1000);
//		RED_OFF;
		return;
	}
	reTryCounter++;

	if(reConnect()== 0)
	{
		#ifdef PRINTF_DEBUG
		printf("\r\n网络重连失败\r\n");
		#endif
//		RED_ON;
//		delay_ms(10000);
//		RED_OFF;
		goto reTrySend;	
	}
 	if(head)
	{
		sprintf(netSend,"AT+CIPSEND=%d\r",length+20);
	}
	else
	{
		sprintf(netSend,"AT+CIPSEND=%d\r",length);
	}

	i=0;
	if(!Send_AT_And_Wait(netSend,">",1000))//命令执行成功
	{
		delay_ms(1000);
		while(1)
		{  										
			if(strstr(USART2_RX_buffer,">"))
				break;//退出循环，发送数据					    
			if(strstr(USART2_RX_buffer,"ERROR"))
				Send_AT_And_Wait(netSend,">",1);
			//错误重发
			delay_ms(1000);
			i++;
			if(i>10)
			{
				#ifdef PRINTF_DEBUG
				printf("\r\nSIM900数据发送失败,尝试重新发送\r\n");
				#endif
				goto  reTrySend;
//				sysErrorHandle(sim900NetWeak);
//				return;
			}
		}
	} 		
//-------------------------------发送数据报文---------------------------------------------- 

	if(head)
	{
		for(i=0;i<sizeof(SANY_DATA);i++)//head
		{
			USART_SendData(USART2,((char *)&sanyData)[i]);	
			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);						
		}
	}
	for(i=0;i<length;i++)//data
	{
		USART_SendData(USART2,pointer[i]);	
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);						
	}
	i = 0;
	while(1)
	{
		delay_ms(100);
		if(strstr(USART2_RX_buffer,"SEND OK"))
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
	if(checkAck)//检查返回数据
	{
		checkAckMsg();//检查返回信息
	}


}


u16 getNextMsgType(void)
{
	static int msgId = 0;
	if(msgId == 0)
	{
		reportOnline();
	}
	return 0;  	
}



void answerMsg(u16 type,u16 msgID)
{
	
}

void SendCan(u16 dat)
{
	CanTxMsg TxMessage;

	TxMessage.StdId=0;
	TxMessage.ExtId=0x12345678;
	TxMessage.IDE=CAN_ID_EXT;
	TxMessage.RTR=CAN_RTR_DATA;
	TxMessage.DLC=8;
	TxMessage.Data[0]=dat;
	TxMessage.Data[1]=dat;
	TxMessage.Data[2]=dat;
	TxMessage.Data[3]=dat;
	TxMessage.Data[4]=dat;
	TxMessage.Data[5]=dat;
	TxMessage.Data[6]=dat;
	TxMessage.Data[7]=dat;	 	
	CAN_Transmit(CAN1,&TxMessage);

}
void CAN_Interrupt(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
//	CanTxMsg TxMessage;
	
	/* CAN register init */
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	
	/* CAN cell init */
	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM=ENABLE;		//时间触发
	CAN_InitStructure.CAN_ABOM=ENABLE;		//自动离线管理
	CAN_InitStructure.CAN_AWUM=ENABLE;		//自动唤醒
	CAN_InitStructure.CAN_NART=ENABLE;		//ENABLE:错误不自动重传 DISABLE:重传
	CAN_InitStructure.CAN_RFLM=ENABLE;
	CAN_InitStructure.CAN_TXFP=ENABLE;
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;		//正常传输模式
	CAN_InitStructure.CAN_SJW=CAN_SJW_4tq;			//1-4
	CAN_InitStructure.CAN_BS1=CAN_BS1_16tq;			//1-16
	CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;			//1-8
	CAN_InitStructure.CAN_Prescaler=6;				//波特率为 8M/(4*(1+1+2))=500k
	CAN_Init(CAN1,&CAN_InitStructure);
	
	/* CAN 过滤器设置 */
	CAN_FilterInitStructure.CAN_FilterNumber=0;
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	/* 允许FMP0中断   FIFO消息挂号*/ 
	CAN_ITConfig(CAN1,CAN_IT_FMP0|CAN_IT_FOV0|CAN_IT_FMP1|CAN_IT_FOV1, ENABLE);
}

void fillCanBuffer(CanRxMsg* RxMessage)
{

	#ifndef PRINTF_DEBUG  //串口转发数据
	int i = 0;
	char *pointer;	
		
	pointer = (char *)RxMessage;
	{
		for(i=0;i<20;i++)
		{
			USART_SendData(USART1,pointer[i]);	
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
		}
	}
	#endif
	// printf("\r\ncan ready\r\n");
	canCounter++;
	switch(RxMessage->ExtId)
	{
		case 0x120:	//
			sanyCanBuffer.realRxBuffer[sanyCanBuffer.realBufferIndex].txBuffer[0] = *RxMessage;  
			break;
		case 0x130:
			sanyCanBuffer.realRxBuffer[sanyCanBuffer.realBufferIndex].txBuffer[1] = *RxMessage; 
			break;
		case 0x140:
			sanyCanBuffer.realRxBuffer[sanyCanBuffer.realBufferIndex].txBuffer[2] = *RxMessage; 
			break;
		case 0x191:
			sanyCanBuffer.realRxBuffer[sanyCanBuffer.realBufferIndex].txBuffer[3] = *RxMessage;  	
			break;
		case 0x1A0:
			sanyCanBuffer.realRxBuffer[sanyCanBuffer.realBufferIndex].txBuffer[4] = *RxMessage;
			break; 
		case 0x1A4:
			sanyCanBuffer.realRxBuffer[sanyCanBuffer.realBufferIndex].txBuffer[5] = *RxMessage; 
			break;
		case 0x1A8:
			sanyCanBuffer.realRxBuffer[sanyCanBuffer.realBufferIndex].txBuffer[6] = *RxMessage; 
			break;
		case 0x1AC:
			sanyCanBuffer.realRxBuffer[sanyCanBuffer.realBufferIndex].txBuffer[7] = *RxMessage;  
			break;

		//以下数据为高速上报数据

		case 0x215://realtime message
			sanyCanBuffer.realRxBuffer[sanyCanBuffer.realBufferIndex].txBuffer[8] = *RxMessage; 
			break;
		case 0x225:
			sanyCanBuffer.realRxBuffer[sanyCanBuffer.realBufferIndex].txBuffer[9] = *RxMessage;
			break;
		case 0x235:
			sanyCanBuffer.realRxBuffer[sanyCanBuffer.realBufferIndex].txBuffer[10] = *RxMessage;
			break;
		case 0x245:
			sanyCanBuffer.realRxBuffer[sanyCanBuffer.realBufferIndex].txBuffer[11] = *RxMessage;
			break;
		case 0x256:
			sanyCanBuffer.realRxBuffer[sanyCanBuffer.realBufferIndex].txBuffer[12] = *RxMessage;
			break;
		case 0x266:
			sanyCanBuffer.realRxBuffer[sanyCanBuffer.realBufferIndex].txBuffer[13] = *RxMessage;
			break;
		case 0x276:
			sanyCanBuffer.realRxBuffer[sanyCanBuffer.realBufferIndex].txBuffer[14] = *RxMessage;
			switch(RxMessage->Data[7])
			{
				case 0://normal
					canMsgLockType = 0;//ok
					break;
				case 1:
					canMsgLockType = 1;//lock 1
					break;
				case 2:
					canMsgLockType = 2;//lock 2
					break;
				case 3:
					canMsgLockType = 3;//gps error
					break;
				default:
					canMsgLockType = 0;//ok
					break;
			}
			break;

		//----------------------------------------------------
		case 0xC9://发送出去的锁机帧
			
			break;
		case 0x12D:
			#ifdef PRINTF_DEBUG
			printf("\r\n确认指令返回\r\n");
			#endif
			if(RxMessage->Data[0] == 0x31 && RxMessage->Data[1] == 0x46 &&RxMessage->Data[2] == 0x68 &&RxMessage->Data[3] ==0x07)
			{
				CAN_LOCK_FLAG = 1;
				#ifdef PRINTF_DEBUG	
				printf("\r\n锁机确认返回\r\n");	
				#endif
				break;
			}
	
			if(RxMessage->Data[0] == 0x31 && RxMessage->Data[1] == 0x47 &&RxMessage->Data[2] == 0x68 &&RxMessage->Data[3] == 0x07)
			{
								
				CAN_UN_LOCK_FLAG = 1;
				#ifdef PRINTF_DEBUG		
				printf("\r\n解锁确认返回\r\n");
				#endif
				break;
			}
			
			#ifdef PRINTF_DEBUG		
			printf("\r\n确认指令错误\r\n");
			#endif
			
			break;
		default :
			return;
		
	}

}

CAN_TX_BUFFER *getCanBufferPointer(int type)
{
	if(type == 0) //获取实时数据缓冲区
	{
		if(sanyCanBuffer.realBufferIndex == 0)
		{
//	   		#ifdef PRINTF_DEBUG
//			printf("\r\nChange Can buffer to 1\r\n");
//			#endif
			// memset(&(sanyCanBuffer.realRxBuffer[1]),0,sizeof(CAN_TX_BUFFER));//format buffer 1
			sanyCanBuffer.realBufferIndex = 1; 
			sanyCanBuffer.realRxBuffer[0].canNum = 15;
			return (&sanyCanBuffer.realRxBuffer[0]);
		}
		else
		{
//			#ifdef PRINTF_DEBUG
//			printf("\r\nChange Can buffer to 0\r\n");
//			#endif
			
			// memset(&(sanyCanBuffer.realRxBuffer[0]),0,sizeof(CAN_TX_BUFFER)); //format buffer 0
			sanyCanBuffer.realBufferIndex = 0;
			sanyCanBuffer.realRxBuffer[1].canNum = 15;
			return (&sanyCanBuffer.realRxBuffer[1]);
		}
	}
	return 0;
//	else	//获取非实时数据缓冲区
//	{
//		if(sanyCanBuffer.normalBufferIndex == 0)
//		{
//			sanyCanBuffer.normalBufferIndex = 1;
//			sanyCanBuffer.normalRxBuffer[0].canNum =NORMAL_BUFFER_NUM;//normalRxBuffer
//			return (&sanyCanBuffer.normalRxBuffer[0]);
//		}
//		else
//		{
//			
//			sanyCanBuffer.normalBufferIndex = 0;
//			sanyCanBuffer.normalRxBuffer[1].canNum = NORMAL_BUFFER_NUM;
//			return (&sanyCanBuffer.normalRxBuffer[1]);
//		}	
//	}
}

u32 reConnect(void)
{
	
	int i;
	static char netSend[20] = "AT+CIPSEND=20\r";
	static int regieterCounter = 0;
	SANY_DATA online;
	


	if(Send_AT_And_Wait("AT+CIPSTATUS\r","CONNECT OK",500) )
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nTCP NET OK\r\n");
		#endif
		return 1;
	}
	#ifdef PRINTF_DEBUG
	printf("\r\nSIM900 TRY TO RECONNECT\r\n");
	#endif
	regieterCounter = 0;
	checkSim900:
	if(regieterCounter > 1)
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nREGEIGISTER ERROR EXCEETS MAX\r\n");
		#endif
		sysErrorHandle(sim900NotInsert);
		regieterCounter = 0;
		return 0;	
	}
	regieterCounter++;
	if(power_sim900())//成功启动SIM900
	{
//		Send_AT_And_Wait("ATE0\r","OK",300);
//		Send_AT_And_Wait("AT+CREG=0\r","OK",200);
		Send_AT_And_Wait("AT+CIPCLOSE\r","OK",300);	  
		Send_AT_And_Wait("AT+CIPSHUT\r","OK",300); 
		
		for(i=0;i<15;i++)
		{
			if(Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,1",300))
			break;	
			if(Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,5",300))
			break;
			delay_ms(500);
		}
	
		if(i>=14)//网络注册失败						
		{
			#ifdef PRINTF_DEBUG
			printf("\r\nSIM900网络注册失败\r\n");
			#endif
			sysErrorHandle(sim900StartError);
			goto checkSim900;

		}
		else  //网络注册成功
		{
			read_cellid();
			read_cimi();
			read_csq();
		
			for(i=0;i<10;i++)
			{
				if(Send_AT_And_Wait(sysStateNow.NET_CMD,"OK",1500))
				{	
					break;
				}						

			}
			if(i>=9)//网络注册失败						
			{
				#ifdef PRINTF_DEBUG
				printf("\r\nSIM900 TCP连接失败\r\n");
				#endif
				sysErrorHandle(sim900StartError);
				goto checkSim900;

			}
			for(i=0;i<10;i++)
			{
				delay_ms(1500);
				if(strstr(USART2_RX_buffer,"CONNECT OK")||strstr(USART2_RX_buffer,"ALREADY CONNECT"))
				{
					break;
				}  				
				if(strstr(USART2_RX_buffer,"ERROR") || strstr(USART2_RX_buffer,"DEACT")|| strstr(USART2_RX_buffer,"FAIL")|| strstr(USART2_RX_buffer,"CLOSE"))
				{
					#ifdef PRINTF_DEBUG
					printf("\r\nSIM900 NET ESTABLISH FAIL\r\n");
					#endif
					
					sysErrorHandle(sim900NetWeak);
					goto checkSim900;	
//					return;			
				} 					
			}
			if(i>=10)//建立连接失败						
			{
				#ifdef PRINTF_DEBUG
				printf("\r\nSIM900 NET ESTABLISH FAIL\r\n");
				#endif
				sysErrorHandle(sim900NetWeak);
				goto checkSim900;
			}
		
		//----------------------------------------------------------------------------------------------------
			for(i=0;i<10;i++)
			{
				if(Send_AT_And_Wait("AT+CIPHEAD=1\r","OK",1500))
				break;

			}
			if(i>=10)
			{
				#ifdef PRINTF_DEBUG
				printf("\r\nSIM900 set socket config\r\n");
				#endif
				sysErrorHandle(sim900NetWeak);
				goto checkSim900;
			}
		}
	}
	else			//SIM900启动失败++
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nSIM900启动失败\r\n");
		#endif
		sysErrorHandle(sim900StartError);
		goto checkSim900;
	
	}
	#ifdef PRINTF_DEBUG
	printf("\r\nSIM900 reconnect OK\r\n");
	printf("\r\nSEND ONLINE MESSAGE\r\n");
	#endif


	online.MSG_TAG[0] = '*';
	online.MSG_TAG[1] = '*';
	online.MSG_TAG[2] = '*';
	online.MSG_LENGTH = 0;
	online.MSG_ID = 0;
	online.MSG_TYPE = ONLINE_MSG;
	for(i= 0;i<8;i++){ //get MDT_ID
		online.MDT_ID[i] = sysStateNow.G_CODE[i];
	}	
//	online.MSG_CRC = checkCrc((char *)(online.MSG_TAG+4),16);
	//数据准备完成	
	// memset(netSend,'\0',20);


	i=0;
	if(!Send_AT_And_Wait(netSend,">",1000))//命令执行成功
	{
		delay_ms(1000);
		while(1)
		{  										
			if(strstr(USART2_RX_buffer,">"))
				break;//退出循环，发送数据					    
			if(strstr(USART2_RX_buffer,"ERROR"))
				Send_AT_And_Wait(netSend,">",1000);
			//错误重发
			delay_ms(1000);
			i++;
			if(i>10){
				#ifdef PRINTF_DEBUG
				printf("\r\nSIM900数据发送失败,尝试重新发送\r\n");
				#endif
				goto  checkSim900;
//				sysErrorHandle(sim900NetWeak);
//				return;
			}
		}
	} 		
//-------------------------------发送数据报文---------------------------------------------- 


	for(i=0;i<20;i++)//data
	{
		USART_SendData(USART2,online.MSG_TAG[i]);	
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);						
	}
	i = 0;
	while(1){
		delay_ms(100);
		if(strstr(USART2_RX_buffer,"SEND OK")){
			#ifdef PRINTF_DEBUG	
			printf("\r\nFIND SEND OK,CONTINUE\r\n"); 
			#endif
			break;	
		}
		if(strstr(USART2_RX_buffer,"ERROR")){
			#ifdef PRINTF_DEBUG
			printf("\r\nFIND ERROR.GOTO STORE AND ERROR\r\n"); 	
			#endif
			goto checkSim900;
			//stroe message
		}
		i++;
		if(i>60)	{
			#ifdef PRINTF_DEBUG	
			printf("\r\nTIME OUT ,GO ON RECONNECT???\r\n");
			#endif
			// return 1;
			break;

		}
	}
	checkAckMsg();//检查返回信息
	return 1; 
	
}
void checkIpdMsg(char charTmp)
{
	static int charIndex = 1;
	static int tmp1 = 0;
	static char std[5];
	
	if(charTmp == '+')
		charIndex = 1;
	switch(charIndex)
	{
		case 1:
			if(charTmp == '+')
			{
				std[0] = charTmp;
				charIndex++;
			}
			break;
		case 2:
			if((std[0] == '+')&&(charTmp == 'I'))
			{
				std[1] = 'I';
				charIndex++;
			}
			else
			{
				charIndex = 1;
				memset(std,'\0',5);
			}
			break;
		case 3:
			if((std[1] == 'I')&&(charTmp == 'P') )
			{
				std[2] = 'P';
				charIndex++;
			}
			else
			{
				charIndex = 1;
				memset(std,'\0',5);
			}
			break;
		case 4:
			if((std[2] == 'P')&&(charTmp == 'D') )
			{
				std[3] = 'D';
				charIndex++;
			}
			else
			{
				charIndex = 1;
				memset(std,'\0',5);
			}
			break;
		case 5:
			if((std[3] == 'D')&&(charTmp == ',') )
			{
				//printf("\r\nFIND IPD\r\n");


				IPD_FLAG_1 = 1;

				rxBufferFlag = 1;
				charIndex = 1;
				memset(std,'\0',5);
			}
			else
			{
				charIndex = 1;
				memset(std,'\0',5);
			}
			break;
		default:
			charIndex = 1;
			memset(std,'\0',5);
			break;
	}
	if(rxBufferFlag)
	{

		rxBuffer1[tmp1++] = charTmp;
		if(tmp1 >= IPD_BUFFER)
		{
			tmp1 = 0;
			rxBufferFlag = 0;//stop receive
		}		
	}
	return;
}
void rtc(void)
{
	rtcData.second++;
	if(rtcData.second >= 59)
	{
		rtcData.second = 0;
		rtcData.minute++;
		if(rtcData.minute >= 59)
		{
			rtcData.minute = 0;
			if(rtcData.hour++ >= 23)
			{
				rtcData.hour = 0;
				switch(rtcData.month)
				{
					case 1:
					case 3:
					case 5:
					case 7:
					case 8:
					case 10:
					case 12:
						if(rtcData.day++ >= 30)
						{
							rtcData.day = 1;
							if(rtcData.month++ >= 12)
							{
								rtcData.month = 0;
								rtcData.year++;
							}
						}
						break;
					case 4:
					case 6:

					case 9:

					case 11:
						if(rtcData.day++ >= 29)
						{
							rtcData.day = 1;
							if(rtcData.month++ >= 12)
							{
								rtcData.month = 0;
								rtcData.year++;
							}
						}
						break;
					case 2:
						if(rtcData.day++ >= 27)
						{
							rtcData.day = 1;
							if(rtcData.month++ >= 12)
							{
								rtcData.month = 0;
								rtcData.year++;
							}
						}
						break;

				}
			} 	
		}	
	}	
}




void checkAckMsg(void)
{
// 	delay_ms(2000);
	
	dealFifoMsg(&fifoHead);
}
void showCanData(CAN_TX_BUFFER can)
{
	#ifdef PRINTF_DEBUG	
	int i;	
	char *pointer =(char*) (can.txBuffer);
		
	printf("\r\ncan数据包个数：%d\r\n",can.canNum);
	printf("\r\n数据包内容：\r\n");
	for(i=0;i<300;i++)
	{
		if(i%20 == 0)
		{
			printf("\r\n");
		}
		printf("%3X",pointer[i]);
		
	}
	#endif
}
/*******************************************************************************
 * 函数名称:lockCan                                                                     
 * 描    述:锁机函数，锁机成功自动保存锁机参数到flash中                                                                    
 *                                                                               
 * 输    入:type 1：一级锁机 2：二级锁机                                                                     
 * 输    出:无                                                                     
 * 返    回:1:锁机成功 2：锁机失败，仅仅调低电压                                                                  
 * 作    者:蛋蛋                                                                     
 * 修改日期:2011年7月13日                                                                    
 *******************************************************************************/
int  lockCan(int type)
{
	int i;
	CanTxMsg tx;
	DATA_IN_FLASH temp = sysStateNow;

	#ifdef PRINTF_DEBUG			
	printf("\r\n当前系统锁机状态：%d\r\n",sysStateNow.lockType);
	printf("\r\n当前can信息状态 ：%d\r\n",canMsgLockType);
	#endif
	CAN_LOCK_FLAG = 0;
	tx.StdId = 0;
	tx.ExtId = 0xC9;
	tx.DLC = 8;
	tx.IDE = CAN_ID_EXT ;
	tx.RTR = CAN_RTR_DATA;
	tx.Data[0]=0x31;
	tx.Data[1]=0x06;
	switch (type){
		case 1:
			if(sysStateNow.lockType == 1 && canMsgLockType == 1)
			{
				#ifdef PRINTF_DEBUG			
				printf("\r\n设备已经一级锁机\r\n");
				#endif
				return 1;
			}
			#ifdef PRINTF_DEBUG			
			printf("\r\n开始一级锁机\r\n");
			#endif
			tx.Data[2]=0x01;
			break;
		case 2:
			if(sysStateNow.lockType == 2 && canMsgLockType == 2)
			{
				#ifdef PRINTF_DEBUG			
				printf("\r\n设备已经二级锁机\r\n");
				#endif
				return 1;
			}
			#ifdef PRINTF_DEBUG			
			printf("\r\n开始二级锁机\r\n");
			#endif
			tx.Data[2]=0x02;
			break;
		default:
			#ifdef PRINTF_DEBUG			
			printf("\r\n锁机参数错误，取消操作\r\n");
			#endif
			return 0;
	}

	tx.Data[3]=0x07;
	tx.Data[4]=0;
	tx.Data[5]=0;
	tx.Data[6]=0;
	tx.Data[7]=0;
	CAN_Transmit(CAN1,&tx);
//	readFlashCfg(FLASH_ADR,&temp);
	for(i=0;i<180;i++)
	{
		delay_ms(1000);
		#ifdef PRINTF_DEBUG			
		printf("\r\n锁机已用时间：%d秒\r\n",i);
		#endif
		if(CAN_LOCK_FLAG)
		{
			#ifdef PRINTF_DEBUG			
			printf("\r\n锁机成功,恢复电压输出\r\n");
			#endif
//			wrtieDac(0x98,0,LOCK_VOLTAGE_25);
			if(type == 1)
			{
				temp.lockType	= 1;
			}
			else
			{
				temp.lockType	= 2;
			}
//			writeFlashCfg(FLASH_ADR,(uint32_t *)&temp);
			sysStateNow.lockType = temp.lockType;
			delay_ms(3000);
			return 1;
		}
	}
	
	#ifdef PRINTF_DEBUG		
	printf("\r\n锁机超时，改变DA输出\r\n");
	#endif
//	wrtieDac(0x98,0,0);
	if(type == 1)
	{
		temp.lockType	= 3;
	}
	else
	{
		temp.lockType	= 4;
 	}
//	writeFlashCfg(FLASH_ADR,(uint32_t *)&temp);
	sysStateNow.lockType = temp.lockType;
//	delay_ms(3000);
	return 2;

}
int  unlockCan(void)
{
	int i = 0;
	CanTxMsg tx;
//	DATA_IN_FLASH temp = sysStateNow;
	CAN_UN_LOCK_FLAG = 0;
//	wrtieDac(0x98,0,LOCK_VOLTAGE_25);	//调整电压到2.5V
	if(sysStateNow.lockType == 0 || canMsgLockType == 0 )
	{
		#ifdef PRINTF_DEBUG		
		printf("\r\n设备已经正常工作\r\n");
		#endif
		return 1;	
	}
	tx.ExtId = 0xC9;
	tx.DLC = 8;
	tx.IDE = CAN_ID_EXT ;
	tx.RTR = CAN_RTR_DATA;
	tx.Data[0]=0x31;
	tx.Data[1]=0x07;
	tx.Data[2]=0x0;
	tx.Data[3]=0x07;
	tx.Data[4]=0;
	tx.Data[5]=0;
	tx.Data[6]=0;
	tx.Data[7]=0;
	CAN_Transmit(CAN1,&tx);
	for(i=0;i<180;i++)
	{
		delay_ms(1000);
		#ifdef PRINTF_DEBUG			
		printf("\r\n解锁已用时间：%d秒\r\n",i);
		#endif
		if(CAN_UN_LOCK_FLAG)
		{
			#ifdef PRINTF_DEBUG	
			printf("\r\n解锁成功\r\n");
			#endif
//			temp.lockType = 0;
			sysStateNow.lockType = 0;
//			writeFlashCfg(FLASH_ADR,(uint32_t *)&temp);

			return 1;
		}
	}
	#ifdef PRINTF_DEBUG
	printf("\r\n解锁失败\r\n");
	#endif
//	temp.lockType = 5;
	sysStateNow.lockType = 5;
//	writeFlashCfg(FLASH_ADR,(uint32_t *)&temp);
	return 2;

}

void setCan(int enable)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	
	if(enable)
	{
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);	
	}
	else
	{
		NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
}

void storeToFlash(char *pointer,int length,int head)
{
	static char msg[400];
	u32 i;
	SANY_POS_REPORT *gpsPointer;
	CAN_TX_BUFFER 	*canPointer;
	TIME time;
	#ifdef PRINTF_DEBUG
	printf("\r\n存储数据到flash中\r\n");
	#endif
	if(!head)//设备回应下发数据，直接丢弃
	{
		return;
	}
	#ifdef PRINTF_DEBUG
	printf("\r\n数据长度:%d\r\n",length);
	printf("\r\n数据头标志:%d\r\n",head);
	printf("\r\n数据包头:\r\n");
	for(i=0;i<sizeof(SANY_DATA);i++)//head
	{
		printf("%3X",((char *)&sanyData)[i]);						
	}
	printf("\r\n数据载荷:\r\n");
	for(i = 0;i<length;i++)
	{
		printf("%3X",pointer[i]);
	}
	#endif
	memcpy(msg,&sanyData,20); //copy head
	memcpy(&msg[20],pointer,length);//copy data
	printf("\r\ncopy ok\r\n");
/* 	for(i = 0;i<400;i++)
	{
		printf("%3X",msg[i]);
	} */
//	feedDog();
	switch(sanyData.MSG_TYPE)
	{
		case REPORT_GPS_MSG:
//			getDs1302(&time);
			gpsPointer = (SANY_POS_REPORT *)(&msg[20]);
			gpsPointer->timeYY = time.year;
			gpsPointer->timemm = time.month;
			gpsPointer->timeDD = time.date;
			gpsPointer->timeHH = time.hour;
			gpsPointer->timeMM = time.minute;
			gpsPointer->timeSS = time.second;
			//re calcate crc
			msg[3] = checkCrc(&msg[4],sanyData.MSG_LENGTH+16);
//			inFlashFifo((u8 *)msg,sanyData.MSG_LENGTH+20);


			break;
		case REPORT_CAN_MSG:
//			getDs1302(&time);
			canPointer = (CAN_TX_BUFFER *)(&msg[20]);
			canPointer->timeYY = time.year;
			canPointer->timemm = time.month;
			canPointer->timeDD = time.date;
			canPointer->timeHH = time.hour;
			canPointer->timeMM = time.minute;
			canPointer->timeSS = time.second;
			//re calcate crc
			msg[3] = checkCrc(&msg[4],sanyData.MSG_LENGTH+16);
//			inFlashFifo((u8 *)msg,sanyData.MSG_LENGTH+20);
			


			break;
		default:
			break;
	}
}
u8	reSendFlashMsg(void)
{
	static u8 msg[400];
	SANY_DATA *point;
//	return 0;
//	if(!outFlashFifo(msg,400))
//	{
//		#ifdef PRINTF_DEBUG
//		printf("\r\n没有数据需要补发~~~~\r\n");
//		#endif
//		return 0;
//	}
//	else
	{
		point = (SANY_DATA *)msg;
		#ifdef PRINTF_DEBUG
		printf("\r\n开始FLASH数据校验------\r\n");
		#endif
		if(point->MSG_CRC != checkCrc((char *)(msg+4),point->MSG_LENGTH+16))
		{
			#ifdef PRINTF_DEBUG
			printf("\r\nFLASH数据校验失败，取消上报~~~~\r\n");
			#endif
			return 0;
		}
		else{
			#ifdef PRINTF_DEBUG
			printf("\r\nFLASH数据校验通过，可以上报~~~~\r\n");
			#endif
//			RED_ON;
			sendData((char *)msg,20+point->MSG_LENGTH,0,1,1);
//			RED_OFF;
			return 1;
		}
	}
}
void checkLockState(void)
{
	static int i= 0;
	
	
	#ifdef PRINTF_DEBUG
	printf("\r\n系统锁定参数：%d\r\n",sysStateNow.lockType);
	printf("\r\n总线锁定参数：%d\r\n",canMsgLockType);
	#endif
	if(systemStopState)//0:运动 1：静止
	{
		#ifdef PRINTF_DEBUG			
		printf("\r\n设备关机中，取消解锁机操作\r\n");
		#endif	
		return;
	}
	if(i%120 != 0){//每小时
		i++;
		return;
	}
	switch(sysStateNow.lockType)
	{
		case 0:
			#ifdef PRINTF_DEBUG
			printf("\r\n系统正常工作\r\n");
			#endif
//			wrtieDac(0x98,0,LOCK_VOLTAGE_25);	//调整电压到2.5V
			break;
		case 1:
			#ifdef PRINTF_DEBUG
			printf("\r\n系统一级锁机中--------\r\n");
			#endif
			break;

		case 2:
			#ifdef PRINTF_DEBUG
			printf("\r\n系统二级锁机中--------\r\n");
			#endif
			break;
		case 3:
			#ifdef PRINTF_DEBUG
			printf("\r\n系统一级锁机失败，尝试重新锁机------\r\n");
			#endif
			lockCan(1);
			break;
		case 4:
			#ifdef PRINTF_DEBUG
			printf("\r\n系统二级锁机失败，尝试重新锁机------\r\n");
			#endif
			lockCan(2);
			break;
		case 5:
			#ifdef PRINTF_DEBUG
			printf("\r\n系统解锁失败，尝试重新解锁------\r\n");
			#endif
			unlockCan();
			break;
		default:
//			wrtieDac(0x98,0,LOCK_VOLTAGE_25);	//调整电压到2.5V
			break;		

	}
	delay_ms(1000);
}
/*******************************************************************************
 * 函数名称:checkSysState                                                                     
 * 描    述:检查车辆静止状态                                                                     
 *                                                                               
 * 输    入:无                                                                     
 * 输    出:无                                                                     
 * 返    回:无                                                                     
 * 作    者:蛋蛋                                                                     
 * 修改日期:2011年7月13日                                                                    
 *******************************************************************************/
void checkSysState(void)
{
	static int  timeCounter= 0;	//计时器

	if(canCounter < 2)  //每秒钟can包数据不够
	{
		timeCounter++;
		if(timeCounter > 10)//一分钟无can数据视为停止状态
		{
			systemStopState = 1;//标记为静止
//			printf("\r\n设备静止\r\n");	
		}			
	}
	else
	{
		timeCounter = 0;
		systemStopState = 0;  
	}
	
	canCounter = 0;	
}
//void	checkBlueToothState(void)
//{
////	static int blueToothPower = 0;
////	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==1)	{
////		//蓝牙连接成功？？
////		blueToothState = 1;
////	}
////	else{
////	 	//蓝颜未连接？
////		blueToothState = 0;
////	}
////	blueToothCounter = 0;
////	blueToothPower++;
////	if(blueToothPower > 15000){//每隔5个小时断掉一次蓝牙
////		blueToothPower = 0;
////		GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET);//关闭蓝牙
////		delay_ms(1000);
////		GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_SET);//打开蓝牙
////	}
//}
//void	checkBlueInput(char temp)
//{
//		
//}

void	reportPowerState(void)
{
	static int timer = 0;//计时器功能，没3-5分钟发送一次
	static int powerState = 0;//0：off 1：on
	timer++;
	#ifdef PRINTF_DEBUG
	printf("\r\npower:%d\r\n",powerState);
	printf("\r\ntimer:%d\r\n",timer);
	#endif
	if (powerState == GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)){//没有变化
		if(timer < 20){//不到10分钟，30秒一次
			#ifdef PRINTF_DEBUG
			printf("\r\n设备电源正常，暂不上报电源信息:%d\r\n",GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6));
			#endif
			return;			
		}
	}
	//上报电源信息
	timer = 0;//复位电源上报计时间隔
	powerState = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);//更新电源标志
	#ifdef PRINTF_DEBUG
	printf("\r\n设备电源信息上报\r\n");
	#endif
	if(powerState)
	{
		#ifdef PRINTF_DEBUG
		printf("\r\n设备正常供电信息上报\r\n");
		#endif
		getDataHead(REPORT_POWER_ON_MSG,NULL,0);
	}
	else
	{
		#ifdef PRINTF_DEBUG
		printf("\r\n设备异常断电信息上报\r\n");
		#endif
		getDataHead(REPORT_POWER_OFF_MSG,NULL,0);
	}
	sendData(NULL,0,1,0,1);
	
	
	
}
/*******************************************************************************
 * 函数名称:setNetArgument                                                                     
 * 描    述:设置网络上报服务器参数                                                                     
 *                                                                               
 * 输    入:存放数据的缓冲区                                                                    
 * 输    出:无                                                                     
 * 返    回:无                                                                     
 * 作    者:蛋蛋                                                                     
 * 修改日期:2011年7月13日                                                                    
 *******************************************************************************/
void setNetArgument(char * temp,int length)
{
	int i;
	int type = 0;//0:IP 1:URL
	DATA_IN_FLASH  sysStateTemp;
	memset(sysStateTemp.IP,'\0',16);
	memset(sysStateTemp.URL,'\0',40);
	memset(sysStateTemp.NET_CMD,'\0',64);
//	readFlashCfg(FLASH_ADR,&sysStateTemp);
	for(i = 0;*temp != ',';temp++){//get g_code
			sysStateTemp.G_CODE[i] = *temp;
		
	}
	temp++;//skip '|'
	length -= 9;
	for(i=0;*(temp+i) != ',';i++){
		if((*(temp+i)>='A'&&*(temp+i)<='Z')||(*(temp+i)>='a'&&*(temp+i)<='z'))
		{
			type = 1;//URL格式
			break;
		}
	
	}
	
	if(type){//URL
		
		for(i=0;(*temp)!=',';i++){
			sysStateTemp.URL[i] = *(temp++);
			length--;
		}
		sysStateTemp.IP_OR_URL = 1;//URL
	}
	else{//IP
		for(i=0;(*temp)!=',';i++)	{
			sysStateTemp.IP[i] = *(temp++);
			length--;
		}
		sysStateTemp.IP_OR_URL = 0;//IP
	}
	
	temp++;
	length--;
	for(i=0;length>=0;i++){
		sysStateTemp.PORT[i] = *(temp++);
		length--;
	}
	if(type == 0){
		strcat(sysStateTemp.NET_CMD,"AT+CIPSTART=\"TCP\",\"");
		strcat(sysStateTemp.NET_CMD,sysStateTemp.IP);
		strcat(sysStateTemp.NET_CMD,"\",\"");
		strcat(sysStateTemp.NET_CMD,sysStateTemp.PORT);
		strcat(sysStateTemp.NET_CMD,"\"\r");
	}
	
//	writeFlashCfg(FLASH_ADR,(uint32_t *)&sysStateTemp);
	
}
void	checkOnlineTimeOut(void)
{
	static int timeOutCounter = 0;
	if(onlineTimeOutFlag == 0){
		timeOutCounter = 0;
//		reConnectFlag = 0;
		return;//do nothing
	}
	else{
		timeOutCounter++;
		if(timeOutCounter > 30){
			 //重新建立连接
//			 reConnectFlag = 1;
		}	
	}		
}










