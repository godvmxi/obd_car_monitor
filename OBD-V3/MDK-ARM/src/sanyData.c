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
extern int		GPS_GGA_DINGWEI_OK;//��GPS��λ���򽫴˱�����1������Ϊ0
extern int		GPS_SHUJU_OK;//��GPS���������Ϣ���򽫴˱�����1������Ϊ0 
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
int blueToothState = 0;//0:δ���� 1��������

int systemStopState = 0;//0:�˶� 1����ֹ
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
//	printf("\r\nCRC У��ֵΪ��%d --> %X\r\n",crc,crc);
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
//	printf("\r\n��ȡ����ͷ��%X\r\n",type);
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
//	printf("\r\n��ȡ���ݳ��ȣ�%d\r\n",length);
//	#endif

	sanyData.MSG_CRC = getCrc(pointer,length);
}

void sendData(char *pointer,int length,int head,int reSend,int checkAck)//type 0:���Զ��������ݰ�ͷ 1���Զ�����  gpsFlag :����gps���ݰ�
{

//------------------------------��������-----------------------------------	 		
	int i=0;
	static int reTryCounter = 0;
	
	char netSend[20];	
	memset(netSend,'\0',20);
	reTryCounter = 0;
	reTrySend://��������3�κ󱣴�����
	
	if(reTryCounter > 1)
	{
		//�洢����
		if(reSend == 1)
		{
			#ifdef PRINTF_DEBUG
			printf("\r\n�����ط�ʧ��״̬������������\r\n");
			#endif			
			return ;			
		}
		#ifdef PRINTF_DEBUG
		printf("\r\n�����ط�ʧ�ܣ��洢������Flash��\r\n");
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
		printf("\r\n��������ʧ��\r\n");
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
	if(!Send_AT_And_Wait(netSend,">",1000))//����ִ�гɹ�
	{
		delay_ms(1000);
		while(1)
		{  										
			if(strstr(USART2_RX_buffer,">"))
				break;//�˳�ѭ������������					    
			if(strstr(USART2_RX_buffer,"ERROR"))
				Send_AT_And_Wait(netSend,">",1);
			//�����ط�
			delay_ms(1000);
			i++;
			if(i>10)
			{
				#ifdef PRINTF_DEBUG
				printf("\r\nSIM900���ݷ���ʧ��,�������·���\r\n");
				#endif
				goto  reTrySend;
//				sysErrorHandle(sim900NetWeak);
//				return;
			}
		}
	} 		
//-------------------------------�������ݱ���---------------------------------------------- 

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
	if(checkAck)//��鷵������
	{
		checkAckMsg();//��鷵����Ϣ
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
	CAN_InitStructure.CAN_TTCM=ENABLE;		//ʱ�䴥��
	CAN_InitStructure.CAN_ABOM=ENABLE;		//�Զ����߹���
	CAN_InitStructure.CAN_AWUM=ENABLE;		//�Զ�����
	CAN_InitStructure.CAN_NART=ENABLE;		//ENABLE:�����Զ��ش� DISABLE:�ش�
	CAN_InitStructure.CAN_RFLM=ENABLE;
	CAN_InitStructure.CAN_TXFP=ENABLE;
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;		//��������ģʽ
	CAN_InitStructure.CAN_SJW=CAN_SJW_4tq;			//1-4
	CAN_InitStructure.CAN_BS1=CAN_BS1_16tq;			//1-16
	CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;			//1-8
	CAN_InitStructure.CAN_Prescaler=6;				//������Ϊ 8M/(4*(1+1+2))=500k
	CAN_Init(CAN1,&CAN_InitStructure);
	
	/* CAN ���������� */
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
	
	/* ����FMP0�ж�   FIFO��Ϣ�Һ�*/ 
	CAN_ITConfig(CAN1,CAN_IT_FMP0|CAN_IT_FOV0|CAN_IT_FMP1|CAN_IT_FOV1, ENABLE);
}

void fillCanBuffer(CanRxMsg* RxMessage)
{

	#ifndef PRINTF_DEBUG  //����ת������
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

		//��������Ϊ�����ϱ�����

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
		case 0xC9://���ͳ�ȥ������֡
			
			break;
		case 0x12D:
			#ifdef PRINTF_DEBUG
			printf("\r\nȷ��ָ���\r\n");
			#endif
			if(RxMessage->Data[0] == 0x31 && RxMessage->Data[1] == 0x46 &&RxMessage->Data[2] == 0x68 &&RxMessage->Data[3] ==0x07)
			{
				CAN_LOCK_FLAG = 1;
				#ifdef PRINTF_DEBUG	
				printf("\r\n����ȷ�Ϸ���\r\n");	
				#endif
				break;
			}
	
			if(RxMessage->Data[0] == 0x31 && RxMessage->Data[1] == 0x47 &&RxMessage->Data[2] == 0x68 &&RxMessage->Data[3] == 0x07)
			{
								
				CAN_UN_LOCK_FLAG = 1;
				#ifdef PRINTF_DEBUG		
				printf("\r\n����ȷ�Ϸ���\r\n");
				#endif
				break;
			}
			
			#ifdef PRINTF_DEBUG		
			printf("\r\nȷ��ָ�����\r\n");
			#endif
			
			break;
		default :
			return;
		
	}

}

CAN_TX_BUFFER *getCanBufferPointer(int type)
{
	if(type == 0) //��ȡʵʱ���ݻ�����
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
//	else	//��ȡ��ʵʱ���ݻ�����
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
	if(power_sim900())//�ɹ�����SIM900
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
	
		if(i>=14)//����ע��ʧ��						
		{
			#ifdef PRINTF_DEBUG
			printf("\r\nSIM900����ע��ʧ��\r\n");
			#endif
			sysErrorHandle(sim900StartError);
			goto checkSim900;

		}
		else  //����ע��ɹ�
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
			if(i>=9)//����ע��ʧ��						
			{
				#ifdef PRINTF_DEBUG
				printf("\r\nSIM900 TCP����ʧ��\r\n");
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
			if(i>=10)//��������ʧ��						
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
	else			//SIM900����ʧ��++
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nSIM900����ʧ��\r\n");
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
	//����׼�����	
	// memset(netSend,'\0',20);


	i=0;
	if(!Send_AT_And_Wait(netSend,">",1000))//����ִ�гɹ�
	{
		delay_ms(1000);
		while(1)
		{  										
			if(strstr(USART2_RX_buffer,">"))
				break;//�˳�ѭ������������					    
			if(strstr(USART2_RX_buffer,"ERROR"))
				Send_AT_And_Wait(netSend,">",1000);
			//�����ط�
			delay_ms(1000);
			i++;
			if(i>10){
				#ifdef PRINTF_DEBUG
				printf("\r\nSIM900���ݷ���ʧ��,�������·���\r\n");
				#endif
				goto  checkSim900;
//				sysErrorHandle(sim900NetWeak);
//				return;
			}
		}
	} 		
//-------------------------------�������ݱ���---------------------------------------------- 


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
	checkAckMsg();//��鷵����Ϣ
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
		
	printf("\r\ncan���ݰ�������%d\r\n",can.canNum);
	printf("\r\n���ݰ����ݣ�\r\n");
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
 * ��������:lockCan                                                                     
 * ��    ��:���������������ɹ��Զ���������������flash��                                                                    
 *                                                                               
 * ��    ��:type 1��һ������ 2����������                                                                     
 * ��    ��:��                                                                     
 * ��    ��:1:�����ɹ� 2������ʧ�ܣ��������͵�ѹ                                                                  
 * ��    ��:����                                                                     
 * �޸�����:2011��7��13��                                                                    
 *******************************************************************************/
int  lockCan(int type)
{
	int i;
	CanTxMsg tx;
	DATA_IN_FLASH temp = sysStateNow;

	#ifdef PRINTF_DEBUG			
	printf("\r\n��ǰϵͳ����״̬��%d\r\n",sysStateNow.lockType);
	printf("\r\n��ǰcan��Ϣ״̬ ��%d\r\n",canMsgLockType);
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
				printf("\r\n�豸�Ѿ�һ������\r\n");
				#endif
				return 1;
			}
			#ifdef PRINTF_DEBUG			
			printf("\r\n��ʼһ������\r\n");
			#endif
			tx.Data[2]=0x01;
			break;
		case 2:
			if(sysStateNow.lockType == 2 && canMsgLockType == 2)
			{
				#ifdef PRINTF_DEBUG			
				printf("\r\n�豸�Ѿ���������\r\n");
				#endif
				return 1;
			}
			#ifdef PRINTF_DEBUG			
			printf("\r\n��ʼ��������\r\n");
			#endif
			tx.Data[2]=0x02;
			break;
		default:
			#ifdef PRINTF_DEBUG			
			printf("\r\n������������ȡ������\r\n");
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
		printf("\r\n��������ʱ�䣺%d��\r\n",i);
		#endif
		if(CAN_LOCK_FLAG)
		{
			#ifdef PRINTF_DEBUG			
			printf("\r\n�����ɹ�,�ָ���ѹ���\r\n");
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
	printf("\r\n������ʱ���ı�DA���\r\n");
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
//	wrtieDac(0x98,0,LOCK_VOLTAGE_25);	//������ѹ��2.5V
	if(sysStateNow.lockType == 0 || canMsgLockType == 0 )
	{
		#ifdef PRINTF_DEBUG		
		printf("\r\n�豸�Ѿ���������\r\n");
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
		printf("\r\n��������ʱ�䣺%d��\r\n",i);
		#endif
		if(CAN_UN_LOCK_FLAG)
		{
			#ifdef PRINTF_DEBUG	
			printf("\r\n�����ɹ�\r\n");
			#endif
//			temp.lockType = 0;
			sysStateNow.lockType = 0;
//			writeFlashCfg(FLASH_ADR,(uint32_t *)&temp);

			return 1;
		}
	}
	#ifdef PRINTF_DEBUG
	printf("\r\n����ʧ��\r\n");
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
	printf("\r\n�洢���ݵ�flash��\r\n");
	#endif
	if(!head)//�豸��Ӧ�·����ݣ�ֱ�Ӷ���
	{
		return;
	}
	#ifdef PRINTF_DEBUG
	printf("\r\n���ݳ���:%d\r\n",length);
	printf("\r\n����ͷ��־:%d\r\n",head);
	printf("\r\n���ݰ�ͷ:\r\n");
	for(i=0;i<sizeof(SANY_DATA);i++)//head
	{
		printf("%3X",((char *)&sanyData)[i]);						
	}
	printf("\r\n�����غ�:\r\n");
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
//		printf("\r\nû��������Ҫ����~~~~\r\n");
//		#endif
//		return 0;
//	}
//	else
	{
		point = (SANY_DATA *)msg;
		#ifdef PRINTF_DEBUG
		printf("\r\n��ʼFLASH����У��------\r\n");
		#endif
		if(point->MSG_CRC != checkCrc((char *)(msg+4),point->MSG_LENGTH+16))
		{
			#ifdef PRINTF_DEBUG
			printf("\r\nFLASH����У��ʧ�ܣ�ȡ���ϱ�~~~~\r\n");
			#endif
			return 0;
		}
		else{
			#ifdef PRINTF_DEBUG
			printf("\r\nFLASH����У��ͨ���������ϱ�~~~~\r\n");
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
	printf("\r\nϵͳ����������%d\r\n",sysStateNow.lockType);
	printf("\r\n��������������%d\r\n",canMsgLockType);
	#endif
	if(systemStopState)//0:�˶� 1����ֹ
	{
		#ifdef PRINTF_DEBUG			
		printf("\r\n�豸�ػ��У�ȡ������������\r\n");
		#endif	
		return;
	}
	if(i%120 != 0){//ÿСʱ
		i++;
		return;
	}
	switch(sysStateNow.lockType)
	{
		case 0:
			#ifdef PRINTF_DEBUG
			printf("\r\nϵͳ��������\r\n");
			#endif
//			wrtieDac(0x98,0,LOCK_VOLTAGE_25);	//������ѹ��2.5V
			break;
		case 1:
			#ifdef PRINTF_DEBUG
			printf("\r\nϵͳһ��������--------\r\n");
			#endif
			break;

		case 2:
			#ifdef PRINTF_DEBUG
			printf("\r\nϵͳ����������--------\r\n");
			#endif
			break;
		case 3:
			#ifdef PRINTF_DEBUG
			printf("\r\nϵͳһ������ʧ�ܣ�������������------\r\n");
			#endif
			lockCan(1);
			break;
		case 4:
			#ifdef PRINTF_DEBUG
			printf("\r\nϵͳ��������ʧ�ܣ�������������------\r\n");
			#endif
			lockCan(2);
			break;
		case 5:
			#ifdef PRINTF_DEBUG
			printf("\r\nϵͳ����ʧ�ܣ��������½���------\r\n");
			#endif
			unlockCan();
			break;
		default:
//			wrtieDac(0x98,0,LOCK_VOLTAGE_25);	//������ѹ��2.5V
			break;		

	}
	delay_ms(1000);
}
/*******************************************************************************
 * ��������:checkSysState                                                                     
 * ��    ��:��鳵����ֹ״̬                                                                     
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:����                                                                     
 * �޸�����:2011��7��13��                                                                    
 *******************************************************************************/
void checkSysState(void)
{
	static int  timeCounter= 0;	//��ʱ��

	if(canCounter < 2)  //ÿ����can�����ݲ���
	{
		timeCounter++;
		if(timeCounter > 10)//һ������can������Ϊֹͣ״̬
		{
			systemStopState = 1;//���Ϊ��ֹ
//			printf("\r\n�豸��ֹ\r\n");	
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
////		//�������ӳɹ�����
////		blueToothState = 1;
////	}
////	else{
////	 	//����δ���ӣ�
////		blueToothState = 0;
////	}
////	blueToothCounter = 0;
////	blueToothPower++;
////	if(blueToothPower > 15000){//ÿ��5��Сʱ�ϵ�һ������
////		blueToothPower = 0;
////		GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET);//�ر�����
////		delay_ms(1000);
////		GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_SET);//������
////	}
//}
//void	checkBlueInput(char temp)
//{
//		
//}

void	reportPowerState(void)
{
	static int timer = 0;//��ʱ�����ܣ�û3-5���ӷ���һ��
	static int powerState = 0;//0��off 1��on
	timer++;
	#ifdef PRINTF_DEBUG
	printf("\r\npower:%d\r\n",powerState);
	printf("\r\ntimer:%d\r\n",timer);
	#endif
	if (powerState == GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)){//û�б仯
		if(timer < 20){//����10���ӣ�30��һ��
			#ifdef PRINTF_DEBUG
			printf("\r\n�豸��Դ�������ݲ��ϱ���Դ��Ϣ:%d\r\n",GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6));
			#endif
			return;			
		}
	}
	//�ϱ���Դ��Ϣ
	timer = 0;//��λ��Դ�ϱ���ʱ���
	powerState = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);//���µ�Դ��־
	#ifdef PRINTF_DEBUG
	printf("\r\n�豸��Դ��Ϣ�ϱ�\r\n");
	#endif
	if(powerState)
	{
		#ifdef PRINTF_DEBUG
		printf("\r\n�豸����������Ϣ�ϱ�\r\n");
		#endif
		getDataHead(REPORT_POWER_ON_MSG,NULL,0);
	}
	else
	{
		#ifdef PRINTF_DEBUG
		printf("\r\n�豸�쳣�ϵ���Ϣ�ϱ�\r\n");
		#endif
		getDataHead(REPORT_POWER_OFF_MSG,NULL,0);
	}
	sendData(NULL,0,1,0,1);
	
	
	
}
/*******************************************************************************
 * ��������:setNetArgument                                                                     
 * ��    ��:���������ϱ�����������                                                                     
 *                                                                               
 * ��    ��:������ݵĻ�����                                                                    
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:����                                                                     
 * �޸�����:2011��7��13��                                                                    
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
			type = 1;//URL��ʽ
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
			 //���½�������
//			 reConnectFlag = 1;
		}	
	}		
}










