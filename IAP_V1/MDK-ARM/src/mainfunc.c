#include "mainfunc.h"

DATA_HEAD obdHeader;
GPS_DATA_REPORT gpsDataReport;
SOCKET socket;


/*******************************************************************************
 * ��������:collectAndSend                                                                     
 * ��    ��:�������ݺ��������������Ϣ�ֻ��ͷ���                                                                     
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                    
 * ��    ��:��                                                                     
 * ��    ��:����                                                                     
 * �޸�����:2012��3��9��                                                                    
 *******************************************************************************/

int16_t	collectAndSend(void){
//	int i = 0;
	int8_t temp[20];
	
	

	int failCounter = 0;
	strcat(socket.ipUrl,"in.phonewingstech.com");
	socket.localPort=80;
	socket.desPort=9000;	
	socket.udpTcp = 1;
	socket.socketNum = 1;


//	gpsPowerOn();	
//	obdPower(1);
//	initObdMode(1);
//  blueToothPower(1);
//	initBlueTooth(1);

	failCounter++;
	ISP_DIRECTION = USART_SIM;
	
	ERROR_ENTRY:
	sim900_power_on();
	Send_AT_And_Wait("AT\r","OK",500);
	Send_AT_And_Wait("AT\r","OK",500);
//	Send_AT_And_Wait("AT\r","OK",500);
//	Send_AT_And_Wait("AT\r","OK",500);
//	Send_AT_And_Wait("AT\r","OK",500);

	readImei();


	if(strstr(SIM_BUF,"NOT INSERTED") || strstr(SIM_BUF,"REMOVE"))// || GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) == 1)
	{
		printf("\r\nsim card not insert\r\n");
		sysErrorHandle(sim900NotInsert);
		goto ERROR_ENTRY;
	}
	while(1){

//		gpsDataInit();//��ʼ��gps����
		if(establishConnect(socket) == 0){
			printf("\r\nestablish network fail :%d\r\n",failCounter);
			goto ERROR_ENTRY;
		}
		failCounter = 0;


		Send_AT_And_Wait("AT+MIPSETS=1,5,1000\r","OK",500);  //set 
		Send_AT_And_Wait("AT+MIPSEND=1,\"3031323334\"\r","OK",2000);
		
		printf("\r\n\r\n");
		Send_AT_And_Wait("AT+MIPTSEND=1\r",">",2000);
		Send_AT_And_Wait("hello,world!!",0,0);

		USART_SendData(SIM,0x1A);	
		while( USART_GetFlagStatus(SIM,USART_FLAG_TC)==RESET );

		delay_ms(1000);
		reportObd(socket);

		printf("\r\n\r\n");

		delay_ms(3000);

//		reportPos(socket);
//		sleep();
//		closeConnect(1);


	}
	
			

}
/*******************************************************************************
 * ��������:checkConnectState                                                                     
 * ��    ��:���ָ����socket�ŵ�����״̬ ���ַ�����ʽ                                                                
 *                                                                               
 * ��    ��:������socket��                                                                    
 * ��    ��:��                                                                    
 * ��    ��:-1���������� 0��������ָ����socket 1���ҵ�ָ��socket                                                                    
 * ��    ��:����                                                                     
 * �޸�����:2012��3��9��                                                                    
 *******************************************************************************/
int		checkConnectState(uint32_t socketNum){
	char num[3];
	if(socketNum > 4 || socketNum <1){
		printf("query socket ID error  :%d\r\n",socketNum);
		delay_ms(2000);
		return -1;
	}
	memset(num,'\0',3);
	sprintf(num,"%d",socketNum);
	Send_AT_And_Wait("ATE1\r","OK",300);
	printf("\r\ncheck socket %s state+++++++++++++\r\n",num);
	if(Send_AT_And_Wait("AT+MIPOPEN?\r","+MIPOPEN",400) == 0){
		printf("\r\nSOKCKET %s CLOSE\r\n",num);
		return -1;
	}
	if(strstr(SIM_BUF,num)){
		printf("\r\nSOKCKET %s CLOSE\r\n",num);
		Send_AT_And_Wait("ATE0\r","OK",300);
		return 0;
	}
	else{
		printf("\r\nSOKCKET %s OK\r\n",num);
		Send_AT_And_Wait("ATE0\r","OK",300);
		return 1;
	}
}

/*******************************************************************************
 * ��������:establishConnect                                                                     
 * ��    ��:����TCP����                                                               
 *                                                                               
 * ��    ��:�������ӵ�����type 0��TCP 1��UDP 2:SSL     sokcet :socket��֧��1-4                                                                   
 * ��    ��:��                                                                    
 * ��    ��:-1���������� 0��������ָ����socket 1���ҵ�ָ��socket   2:�½���socker���ӣ�socket��Ϊ                                                                 
 * ��    ��:����                                                                     
 * �޸�����:2012��3��9��                                                                    
 *******************************************************************************/
int		establishConnect(SOCKET socket){

	int	regieterCounter = 0;
	int i,j;

	static uint8_t socketFlag=0,socketCmd[80],socketState[40];


	
	if(socketFlag == 0){
		delay_ms(1000);
		memset(socketCmd,'\0',80);
		memset(socketState,'\0',40);

		sprintf(socketCmd,"AT+MIPOPEN=%d,%d,\"%s\",%d,%d\r",socket.socketNum,socket.localPort,socket.ipUrl,socket.desPort,socket.udpTcp);
		//Send_AT_And_Wait("AT+MIPOPEN=1,5000,\"114.91.112.239\",80,1\r","+MIPOPEN: 1,1",2000);
		sprintf(socketState,"+MIPOPEN: %d,%d",socket.socketNum,1);

		printf("\r\nsocket cmd :%s\r\n",socketCmd);
		printf("\r\nsocket state:%s\r\n",socketState);


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
		printf("\r\n����ע��ʧ�ܴ�������,�豸���ϣ�����\r\n");
		#endif
		sysErrorHandle(sim900NotInsert);
		regieterCounter = 0;
		return -1;	
	}
	regieterCounter++;
	if(power_sim900())//�ɹ�����SIM900
	{
//		Send_AT_And_Wait("ATE0\r","OK",300);
//		Send_AT_And_Wait("AT+CREG=0\r","OK",200);
//		Send_AT_And_Wait("AT+CIPCLOSE\r","OK",300);	  
//		Send_AT_And_Wait("AT+CIPSHUT\r","OK",300);		

		
		for(i=0;i<20;i++)
		{
			if(Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,1",1000))
			break;	
			if(Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,5",1000))
			break;
		}
	
		if(i>=14)//����ע��ʧ��						
		{
			#ifdef PRINTF_DEBUG
			printf("\r\nSIM900����ע��ʧ��\r\n");
			#endif
			sysErrorHandle(sim900StartError);
			goto checkSimState;

		}
		else  //����ע��ɹ�
		{
			read_cellid();
			read_csq();
		
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
			if(i>=9)//����ע��ʧ��						
			{
				#ifdef PRINTF_DEBUG
				printf("\r\nSIM���罨��ʧ��\r\n");
				#endif
				sysErrorHandle(sim900StartError);
				goto checkSimState;

			}	
		}
	}
	else			//SIM900����ʧ��++
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nSIM900����ʧ��\r\n");
		#endif
		sysErrorHandle(sim900StartError);
		goto checkSimState;		
	}	
	return 1;
}




/*******************************************************************************
 * ��������:establishConnect                                                                     
 * ��    ��:����TCP����                                                               
 *                                                                               
 * ��    ��:�������ӵ�����0��TCP 1��UDP                                                                    
 * ��    ��:��                                                                    
 * ��    ��:-1���������� 0��������ָ����socket 1���ҵ�ָ��socket                                                                    
 * ��    ��:����                                                                     
 * �޸�����:2012��3��9��                                                                    
 *******************************************************************************/
int closeConnect(int socket){

	return 1;
}
/*******************************************************************************
 * ��������:str2hex                                                                     
 * ��    ��:��ָ�����ȵ��ַ���ת����HEX��                                                               
 *                                                                               
 * ��    ��:src ��ת���ַ��� des��Ŀ���ַ���,����ǰ���ʼ����� length����ת���ַ�������                                                                  
 * ��    ��:��                                                                    
 * ��    ��:-1���������� ��ż����ת������ַ�������                                                                    
 * ��    ��:����                                                                     
 * �޸�����:2012��3��9��                                                                    
 *******************************************************************************/
int16_t str2hex(int8_t *src,uint16_t length,int8_t *des){
	int16_t i = 0,j=0;
	int8_t tmp[2],temp = '0';
//	int8_t useless = 0;
	if(src == NULL  || des == NULL)	 {
		printf("\r\nstring init error!\r\n");
		return -1;
	}
	printf("\r\n");
	for(i=0;i<length;i++){
		printf("%4d ",(uint8_t)src[i]);
	}
	printf("\r\n");
	
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


void setDataHeader(uint16_t type,int8_t *pointer,int16_t length)
{

	uint16_t i = 0;

	obdHeader.MSG_TAG[0] = '*';
	obdHeader.MSG_TAG[1] = '*';
	obdHeader.MSG_TAG[2] = '*';
//	obdHeader.MSG_TAG[3] = '*';

	if(type == 0x0102) //first send
		obdHeader.MSG_ID = 0;
	else
		obdHeader.MSG_ID++;//


	obdHeader.MSG_TYPE = type;	

//	for(i= 0;i<8;i++) //get MDT_ID
//	obdHeader.DEV_ID1 = atol(simState.imei);
//	printf("\r\nimei :%s\r\n",simState.imei);
//	printf("\r\nimei :%ld\r\n",obdHeader.DEV_ID1);
	//not set data length 
	obdHeader.MSG_LENGTH = length;
//	#ifdef PRINTF_DEBUG
//	printf("\r\n��ȡ���ݳ��ȣ�%d\r\n",length);
//	#endif	

	obdHeader.MSG_CRC = calBufCrc(pointer,length);
}
																																		  
int16_t dataSend(char *pointer,int length,int head,int reSend,int checkAck,SOCKET socket,int reConnect)//type 0:���Զ��������ݰ�ͷ 1���Զ�����  gpsFlag :����gps���ݰ�
{
	int8_t sendBuf[1000],sendCmd[20],setSocket[20];
	int16_t	dataLength = 0;
	char netSend[20];
	int i=0;
	int reTryCounter = 0;
	int reStr2HexCounter = 0;



	memset(sendBuf,'\0',1000);
	
//------------------------------��������-----------------------------------	 	
		
	memset(netSend,'\0',20);
	reTryCounter = 0;
	reTrySend://��������3�κ󱣴�����
	
	if(reTryCounter > 2)
	{
		//�洢����
		if(reSend == 1)
		{
			#ifdef PRINTF_DEBUG
			printf("\r\n�����ط�ʧ��״̬������������\r\n");
			#endif			
			return -1;			
		}
		#ifdef PRINTF_DEBUG
		printf("\r\n�����ط�ʧ�ܣ��洢������Flash��\r\n");
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
		printf("\r\n��������Ƿ���Ҫ����\r\n");
		if(establishConnect(socket) <= 0)
		{
			#ifdef PRINTF_DEBUG
			printf("\r\n��������ʧ��\r\n");
			#endif
		//		RED_ON;
		//		delay_ms(10000);
		//		RED_OFF;
			goto reTrySend;	
		}
	}
 		
//-------------------------------�������ݱ���---------------------------------------------- 
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
	Send_AT_And_Wait("ATE0\r","OK",500);


	sprintf(setSocket,"AT+MIPSETS=1,%d,500\r",dataLength/2 );
//	sprintf(setSocket,"AT+MIPSETS=1,%d\r",42);
	printf("\r\nset socket cmd:%s\r\n",setSocket);	
	Send_AT_And_Wait(setSocket,"OK",500); 


	
	memset(sendCmd,'\0',20);
	sprintf(sendCmd,"AT+MIPSEND=%d,\"",socket.socketNum);
	printf("\r\ndata  :%s\r\n",sendBuf);
	printf("\r\nsend cmd :%s%s\r\n",sendCmd,sendBuf);
	for(i = 0;i<20;i++){
		printf("%3X",sendCmd[i]);
	}
	printf("\r\n");


	SIM_COUNT = 0;	
	memset(SIM_BUF,'\0',1300);	 //����2���ջ������� 

	
	
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

	printf("\r\ncmd:%d  buf:%d  data:%d\r\n",strlen(sendCmd),strlen(sendBuf),dataLength); 

	delay_ms(1000);

	printf("\r\ncmd size :%d\r\n",sizeof(sendCmd));
	printf("\r\nbuf size :%d\r\n",dataLength);
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
//	while(USART_GetFlagStatus(SIM,USART_FLAG_TC)==RESET);
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
//	if(checkAck)//��鷵������
//	{
//		checkAckMsg();//��鷵����Ϣ
//	} 
	return 1;

}
/*******************************************************************************
 * ��������:reportObd                                                                     
 * ��    ��:����TCP����                                                               
 *                                                                               
 * ��    ��:�������ӵ�����0��TCP 1��UDP                                                                    
 * ��    ��:��                                                                    
 * ��    ��:-1���������� 0��������ָ����socket 1���ҵ�ָ��socket                                                                    
 * ��    ��:����                                                                     
 * �޸�����:2012��3��9��                                                                    
 *******************************************************************************/

void reportObd(SOCKET soc){
	setDataHeader(0xFFFF,"AAAAAA",6);
	dataSend("AAAAAA",6,1,0,0,soc,1);	


}



















