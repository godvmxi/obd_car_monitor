#include "sim900.h"	
extern SIM_STRUCT simState; 
//extern char USART1_RX_buffer[1300]; //串口2接收缓存位置变量
//extern char USART2_RX_buffer[1300];	 //串口2接收缓存

extern unsigned int USART1_RX_buffer_count; //串口2接收缓存位置变量
extern unsigned int USART2_RX_buffer_count; //串口2接收缓存位置变量	

extern int SYSTEM_PC; // 0 正常汇报 1：紧急打电话 2：接电话 3：挂断电话  8：关机  9:从手动关机中唤醒||初始值为0


extern int SYSTEMP_PC_BAK;
char cimi[5],csq[2],new_cellid[8];//,old_cellid[8];

extern char IMEI[18];

//extern char IP[];
//extern char PORT[];
extern DATA_IN_FLASH sysStateNow;

char phone[44] = "00300030003000300030003000300030003000300030";
char text[300];

SIM_STRUCT simState;



void sim900_boot(void)
{
	sim900_power_on(); 	
}
void sim900_power_on(void)
{
	// GPIO_InitTypeDef GPIO_InitStructure;
//	delay_ms(3000);
	#ifdef PRINTF_DEBUG
	printf("\r\nOPEN SIM900....\r\n");
	#endif
	SIM_DTR(Bit_RESET);
	SIM_EN(Bit_SET);
	delay_ms(6000);
	SIM_EN(Bit_RESET);
	delay_ms(5000);

//	
	
//
//	GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_SET);
//	delay_ms(2500);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_RESET);	 //GP1
//	#ifdef PRINTF_DEBUG
//	printf("\r\nsim900 GP1上电\r\n");
//	#endif
//	SIM_RST(Bit_RESET);
//	delay_ms(1000);
//	SIM_RST(Bit_SET);
	delay_ms(3000);	


}
void sim900_power_off(int type)	  //0:视具体情况关机 1:强制关机
{
	GPIO_InitTypeDef GPIO_InitStructure;
	if(type)
	{
		#ifdef PRINTF_DEBUG
		printf("\r\n普通汇报模式，关闭SIM900\r\nCLOSE SIM900\r\n");
		#endif
		Send_AT_And_Wait("AT+CPOWD=1\r","OK",200);
		GPIO_WriteBit(GPIOA,GPIO_Pin_14,Bit_SET);
		delay_ms(100);
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&GPIO_InitStructure);

	}
	else
	{

	}
}

int Send_AT_And_Wait(char *cmd,char *right_return,unsigned int delay_time)
{
	unsigned int x;	

//	GREEN_ON; 	
	memset(SIM_BUF,'\0',300);	 //串口2接收缓存清零 
	SIM_COUNT=0; //串口2接收缓存位置变量	 
	for(x=0;*(cmd+x)!='\0';x++)
	{
		USART_SendData(SIM,*(cmd+x));	
		while( USART_GetFlagStatus(SIM,USART_FLAG_TC)==RESET ); 
	}
//	GREEN_OFF;
	LED1(Bit_SET);
		
	delay_ms(delay_time);

	LED1(Bit_RESET);
	if(strstr(SIM_BUF,right_return))		
	{ 
		return 1;
	} 
	else 
	{			
		return 0;
	}  		 		
} 

int power_sim900(void)
{
	#ifdef PRINTF_DEBUG
	printf("\r\nname:%s\r\n",__FUNCTION__);
	#endif
	
	if(!Send_AT_And_Wait("AT\r","OK",500))
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nFirst AT fail\r\n");
		#endif
		delay_ms(2000);	
		if(!Send_AT_And_Wait("AT\r","OK",500))
		{
			#ifdef PRINTF_DEBUG
			printf("\r\nSecond AT fail\r\n");
			#endif
			sim900_power_on();
			delay_ms(2000);

			if(Send_AT_And_Wait("AT\r","OK",500))
			{
			  	
				#ifdef PRINTF_DEBUG
				printf("\r\nPOWER_SIM900 OK ,RETURN 1\r\n");
				#endif
				return 1;		   //启动成功
			}
			else
			{
				#ifdef PRINTF_DEBUG
				printf("\r\nIN POWER_SIM900 FAIL RETURN 0\r\n");
				#endif
				return 0;		   //启动失败
			}
		}
		else
		{
			#ifdef PRINTF_DEBUG
			printf("\r\nPOWER_SIM900 OK ,RETURN 1\r\n");
			#endif
			return 1; //1表示已经启动	
		}
		 
	}
	return 1;//1表示已经启动

}

int read_voltage(void)
{
	char *p;
	

	memset(USART2_RX_buffer,'\0',300);	 //串口2接收缓存清零
	USART2_RX_buffer_count=0; //串口2接收缓存位置变量

	Send_AT_And_Wait("AT+CBC\r","OK",1000);

	p=strstr(SIM_BUF,",");
	
	while(*(p+1)!=',')
	{
		p++;	
	}		
//--------------------------防止出错--------------------------------------------

	if(	  (*(p+2)>='0')&&(*(p+2)<='9')
		&&(*(p+3)>='0')&&(*(p+3)<='9')
		&&(*(p+4)>='0')&&(*(p+4)<='9')
		&&(*(p+5)>='0')&&(*(p+5)<='9')
  	)

	{
		simState.voltage=(*(p+2)-'0')*1000+(*(p+3)-'0')*100+(*(p+4)-'0')*10+(*(p+5)-'0');
		return 1;			
	}
	else
	{
		simState.voltage=3700;
		return 0;		
	} 
}

int read_cimi(void)   //此函数读取时必须ATE0，否则会出错
{	
	int i;
	char *p;

		memset(simState.cimi,0,6);

		Send_AT_And_Wait("AT+CIMI\r","OK",500);

		p=strstr(SIM_BUF,"CIMI");

	//--------------------------防止出错--------------------------------------------

		if(	  (*(p+7)>='0')&&(*(p+7)<='9')
			&&(*(p+8)>='0')&&(*(p+8)<='9')
			&&(*(p+9)>='0')&&(*(p+9)<='9')
			&&(*(p+10)>='0')&&(*(p+10)<='9')
	  	  )
			{
				for(i=0;i<5;i++)
					{
						simState.cimi[i]=*(p+7+i);
					}
					printf("\r\ncimi\t:%s\r\n",simState.cimi);
					return 1;				
			}
		else
		{

				return 0;					
		}			
}

int read_cellid(void)		 //此函数在没有注册时会出错
{
	char *p;
	int i;
	memset(simState.newCellid,0,9);


	Send_AT_And_Wait("AT+CREG=2\r","OK",200);
	delay_ms(500);
	Send_AT_And_Wait("AT+CREG?\r","OK",200);
	delay_ms(500);
	p=strstr(SIM_BUF,"+CREG:");

	if((*(p+12)>='0'&&*(p+12)<='9')||(*(p+12)>='A'&&*(p+12)<='Z'))
	{
		for(i=1;i<5;i++)
			simState.newCellid[i-1]=*(p+i+11);

		for(i=8;i<12;i++)
			simState.newCellid[i-4]=*(p+i+11);	 
			
		Send_AT_And_Wait("AT+CREG=0\r","OK",200);
		printf("\r\ncellid is :%s\r\n",simState.newCellid);
		return 1;
	}
	else
	{
		for(i=0;i<8;i++)
			simState.newCellid[i]='0';
			
		Send_AT_And_Wait("AT+CREG=0\r","OK",200);	
		return 0;				
	}
}

int read_csq(void)
{
	char *p;



		Send_AT_And_Wait("AT+CSQ\r","OK",1000);
		p=strstr(SIM_BUF,"+CSQ:");

		if((*(p+7)==',')&&(*(p+6)>='0')&&(*(p+6)<='9'))
		{
			simState.csq[0]='0';	
			simState.csq[1]=*(p+6);
			printf("\r\ncsq\t:%s\r\n",simState.csq);
			return 1;
		}
		else if((*(p+6)>='0')&&(*(p+6)<='9')&&(*(p+7)>='0')&&(*(p+7)<='9'))
		{
			simState.csq[0]=*(p+6);	
			simState.csq[1]=*(p+7);
			printf("\r\ncsq\t:%s\r\n",simState.csq);
			return 1;
		}
		else
		{
//			csq[0]='0';	
//			csq[1]='0';
			return 0;		
		}	 
}

/*
****IMEI|HV_SV#
****DEV_ID|IAP_IP|PORT|NET_IP|PORT|BLUE_NAME|BLUE_PASS#	
*/
void massProduction(void)
{
	int i;
	int j=0;
	char *temp;
	LED1(Bit_SET);
	LED2(Bit_SET);
	LED3(Bit_SET);

	
	delay_ms(3000);
	
	if(*(vu32*)(IAP_PARAMETER+2) != 0xFFFFFFFF && *(vu32*)(SYS_PARAMETER+2) != 0xFFFFFFFF) {//决定是否需要量产程序
		printf("\r\ndevice start.......\r\n");
		return ;	
	}
	
	ISP_DIRECTION = 1;
	#ifdef PRINTF_DEBUG
	printf("\r\nmassProduction.......\r\n");
	printf("\r\ndevice init...............\r\n");
	#endif

//	ISP_DIRECTION=USART_SIM;

	USART_Cmd(ISP, ENABLE);
	sim900_power_on();
	while(!power_sim900());
	

//	Send_AT_And_Wait("AT\r","OK",500);
//	Send_AT_And_Wait("AT\r","OK",500);
//	Send_AT_And_Wait("AT\r","OK",500);
	Send_AT_And_Wait("AT\r","OK",500);
	Send_AT_And_Wait("AT\r","OK",500);
	readImei();

		

	

	

	TIM_Cmd(TIM3, DISABLE);
	USART_Cmd(SIM, DISABLE);

	USART_Cmd(ISP, ENABLE);
	openUsart1();

	

	while(1)
	{
		
		memset(ISP_BUF,'\0',300);
		ISP_COUNT = 0;
		for(i=0;i<4;i++) //send ****
		{
			USART_SendData(ISP,'*');	
			while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);
		}
		USART_SendData(ISP,'|');	
		while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);
		for(i=0;i<15;i++)//send IMEI
		{
			USART_SendData(ISP,simState.imei[i]);	
			while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);
		}
		USART_SendData(ISP,'|');	
		while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);
		//send HV_SV
		USART_SendData(ISP,'1');	
		while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);
		USART_SendData(ISP,'1');	
		while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);
		USART_SendData(ISP,'#');	
		while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);

		delay_ms(1000);
		setReport(1);
		delay_ms(2000);
	}
/* 	TIM_Cmd(TIM3, ENABLE); */


	
}
void sendSMS(char *msg)
{	
	int i;
	char *indicator;  
	indicator = strstr(msg,"*S2|");//获取长度指针
	indicator += 4;//移动指针到长度

	memset(text,'\0',300);
	indicator = strstr(msg,"11000D91");//获取电话号码
	indicator += 10;//移动指针到电话号码
	phone[3] = indicator[1];
	phone[7] = indicator[0];
	phone[11] = indicator[3];
	phone[15] = indicator[2];
	phone[19] = indicator[5];
	phone[23] = indicator[4];
	phone[27] = indicator[7];
	phone[31] = indicator[6];
	phone[35] = indicator[9];
	phone[39] = indicator[8];
	phone[43] = indicator[11];
//	phone[44] = indicator[10]; //应该是F

	#ifdef PRINTF_DEBUG 
	printf("\r\nPhone Number is %d\r\n",phone);
	#endif
	indicator = strstr(msg,"000800");//获取短信内容
	indicator += 6;
	for(i = 0; indicator[i]!='#' ; i++)
	{
		text[i] = indicator[i];
	}//多获取一个‘|’
//	text[i] = 0x1a;//加入Ctrl+Z
	#ifdef PRINTF_DEBUG 
	printf("\r\nPhone Messsage is:\r\n %d\r\n",text);
	#endif 	
	Send_AT_And_Wait("AT+CMGF=1\r","OK",500);
	Send_AT_And_Wait("AT+CSMP=17,0,2,25\r","OK",500);
	Send_AT_And_Wait("AT+CSCS=\"UCS2\"\r","OK",500);
	//
	Send_AT_And_Wait("AT+CMGS=\"","OK",500);
	for(i = 0 ;i < 44;i++)
	{
		USART_SendData(USART2,phone[i]);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
	}
	USART_SendData(USART2,'\"');
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
	USART_SendData(USART2,'\r');
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
	delay_ms(500);
	for(i = 2;text[i]!='|';i++)
	{
		USART_SendData(USART2,text[i]);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
	}
	USART_SendData(USART2,0x1a);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
	delay_ms(5000);		

}

void open_sim900_int(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	#ifdef PRINTF_DEBUG
	printf("\r\nopen sim900 interrupt\r\n");
	#endif

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);
 	EXTI_ClearITPendingBit(EXTI_Line3);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//上升沿下降沿触发中断
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);

}

void close_sim900_int(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	#ifdef PRINTF_DEBUG
	printf("\r\nclose sim900 interrupt\r\n");
	#endif

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);
 	EXTI_ClearITPendingBit(EXTI_Line3);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//降沿触下发中断 //上升沿下降沿触发中断
  	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  	EXTI_Init(&EXTI_InitStructure);

}

void sim900_sleep(void)
{

	


}
void sim900_wakeup(void)
{
	#ifdef PRINTF_DEBUG
	printf("\r\nI wake up from sleep\r\n");
	#endif

}
void sim900Test(void){

	ISP_DIRECTION=USART_SIM;
	delay_ms(1000);
	printf("\r\nSIM Start----------------------------\r\n");
	SIM_DTR(Bit_RESET);

	printf("\r\npower1\r\n");
	SIM_EN(Bit_SET);
	delay_ms(3000);
	printf("\r\npower2\r\n");
	SIM_EN(Bit_RESET);
	delay_ms(3000);
	printf("\r\npower3\r\n");
	SIM_EN(Bit_SET);
	delay_ms(3000);
	printf("\r\npower4\r\n");
	SIM_EN(Bit_RESET);
	delay_ms(3000);
	while(1){
		printf("\r\nbegin test at cmd\r\n");
		Send_AT_And_Wait("AT\r","OK",500);
		Send_AT_And_Wait("AT\r","OK",500);
		Send_AT_And_Wait("AT\r","OK",500);
		Send_AT_And_Wait("AT\r","OK",500);
		Send_AT_And_Wait("AT\r","OK",500);
//		openUsart1();
//		while(1);

//		printf("\r\nreset sim \r\n");
//		SIM_RST(Bit_RESET);
//		delay_ms(1000);
//		SIM_RST(Bit_SET);
//		
//		printf("\r\nreset sim over\r\n");
////		delay_ms(8000);

		Send_AT_And_Wait("AT\r","OK",500);
		Send_AT_And_Wait("AT\r","OK",500);
		Send_AT_And_Wait("AT\r","OK",500);
		Send_AT_And_Wait("AT\r","OK",500);
		Send_AT_And_Wait("AT\r","OK",500);
		read_cimi();
		read_csq();
		Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,1",1000);
		Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,5",1000);
		Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,1",1000);
		Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,5",1000);
		Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,1",1000);
		Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,5",1000);
		Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,1",1000);
		Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,5",1000);
		Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,1",1000);
		Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,5",1000);
		Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,1",1000);
		Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,5",1000);
		Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,1",1000);
		Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,5",1000);
		Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,1",1000);
		Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,5",1000);
		Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,1",1000);
		Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,5",1000);
		Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,1",1000);
		Send_AT_And_Wait("AT+CREG?\r","+CREG: 0,5",1000);  

		read_cimi();
		read_cellid();
		read_csq();
		read_voltage();
		Send_AT_And_Wait("AT+MIPCALL?\r","OK",500);
		Send_AT_And_Wait("AT+MIPCALL=1,\"CMWAP\"\r","OK",5000);
		Send_AT_And_Wait("AT+MIPCALL?\r","OK",500);
		while(1){
			printf("\r\nbegin TCP test\r\n");

			
			Send_AT_And_Wait("AT+MIPCALL?\r","OK",500);
			Send_AT_And_Wait("AT+MIPOPEN?\r","OK",500);
//			Send_AT_And_Wait("AT+MPING=1,\"www.baidu.com\"\r","OK",8000);
			Send_AT_And_Wait("AT+MIPOPEN=1,5000,\"in.phonewingstech.com\",9001,1\r","OK",3000);
//			Send_AT_And_Wait("AT+MIPOPEN=1,4000,\"114.91.113.28\",80,1\r","OK",3000);
//			Send_AT_And_Wait("AT+MSDNS?\r","OK",5000);
			Send_AT_And_Wait("AT+MIPOPEN?\r","OK",500);
//			Send_AT_And_Wait("AT+MIPSTAT:1\r","OK",500);
	
			Send_AT_And_Wait("AT+MIPSETS=1,5,300\r","OK",500);  //set 
			Send_AT_And_Wait("AT+MIPSEND=1,\"3031323334\"\r","OK",5000);
			
			printf("\r\n\r\n");
			Send_AT_And_Wait("AT+MIPTSEND=1\r",">",5000);

			USART_SendData(SIM,'A');
			while( USART_GetFlagStatus(SIM,USART_FLAG_TC)==RESET );
			USART_SendData(SIM,'A');
			while( USART_GetFlagStatus(SIM,USART_FLAG_TC)==RESET );
			USART_SendData(SIM,'A');
			while( USART_GetFlagStatus(SIM,USART_FLAG_TC)==RESET );
			USART_SendData(SIM,'A');
			while( USART_GetFlagStatus(SIM,USART_FLAG_TC)==RESET );

			USART_SendData(SIM,'A');
			while( USART_GetFlagStatus(SIM,USART_FLAG_TC)==RESET );
	
			USART_SendData(SIM,0x1A);	
			while( USART_GetFlagStatus(SIM,USART_FLAG_TC)==RESET );
			delay_ms(3000);
			printf("\r\n\r\n");
//			Send_AT_And_Wait("AT+MIPCLOSE=1\r","OK",500);
	
	
	


			
		} 

	}

}
void	sim900Reboot(void){
	printf("sim900 reboot\r\n");
	SIM_RST(Bit_RESET);
	delay_ms(1000);
	SIM_RST(Bit_SET);
}
/*******************************************************************************
 * 函数名称:setSimApn                                                                     
 * 描    述:设置APN状态                                                               
 *                                                                               
 * 输    入: 0，auto(2G网络wap优先) 1:cmwap 2:cmnet 3:uniwap 4:uninet 5:3gwap 6;3gnet 7:CDMA(not support)                                                                   
 * 输    出:无                                                                    
 * 返    回:-1：发生错误 1-7 设置的接入点名字                                                                   
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月9日                                                                    
 *******************************************************************************/
int setSimApn(int type)
{
	static int apnType = 0;//0 :未设置APN
	int cimi = 0;

	read_cimi();
	if(!Send_AT_And_Wait("AT+MIPCALL?\r","+MIPCALL: 0",500)){
		printf("\r\napn set ok ,type :%d\r\n",apnType);
		return apnType;
	}
	cimi = atoi(simState.cimi);
	printf("\r\ncimi:%d\r\n",cimi);
	switch(cimi){
		case 46000:
		case 46002:
		case 46007:
			printf("\r\nsim type:cmcc\r\n");
			if(type == 0){
				printf("\r\nset apn cmwap\r\n");
				Send_AT_And_Wait("AT+MIPCALL=1,\"CMWAP\"\r","OK",500);
//				apnType = 1;
				return 1;	
			}
			else{
				printf("\r\nset apn cmnet\r\n");
				Send_AT_And_Wait("AT+MIPCALL=1,\"CMNET\"\r","OK",500);
//				apnType = 1;
				return 2;
			}

		case 46001:
		case 46010:
			printf("\r\nsim type:unicom\r\n");

			if(type == 0){
				printf("\r\nset apn cmwap\r\n");
				Send_AT_And_Wait("AT+MIPCALL=1,\"3GWAP\"\r","OK",500);
//				apnType = 1;
				return 1;	
			}
			else{
				printf("\r\nset apn cmnet\r\n");
				Send_AT_And_Wait("AT+MIPCALL=1,\"UNINET\"\r","OK",500);
//				apnType = 1;
				return 2;
			}
//			switch(type){
//				case  3:				
//					printf("\r\nset apn uniwap\r\n");
//					Send_AT_And_Wait("AT+MIPCALL=1,\"UNIWAP\"\r","OK",500);
////					apnType = 1;
//					return 3;
//				case  4: 				
//					printf("\r\nset apn uninet\r\n");
//					Send_AT_And_Wait("AT+MIPCALL=1,\"UNINET\"\r","OK",500);
////					apnType = 1;
//					return 4;
////				case  5:				
////					printf("\r\nset apn uniwap\r\n");
////					Send_AT_And_Wait("AT+MIPCALL=1,\"3GWAP\"\r","OK",500);
//////					apnType = 1;
////					return 5;
////				case  6: 				
////					printf("\r\nset apn uniwap\r\n");
////					Send_AT_And_Wait("AT+MIPCALL=1,\"3GNET\"\r","OK",500);
//////					apnType = 1;
////					return 6;
//				default :
//					return -1;	
//			}
//			
		case 46003:
			printf("\r\nsim type:telecom\r\n");
			return -1;
		default:
			printf("sim type :unknown");
			return -1;
	}
}

int	checkSocketConfig(SOCKET net){
	uint16_t crc = calBufCrc(net.ipUrl-2,sizeof(SOCKET)-2);
	printf("\r\ncrc :%d\r\ncal crc :%d\r\n",net.crc,crc);
	if(crc != net.crc){
		return -1;	
	}
	return 1;
}
/*******************************************************************************
 * 函数名称:calSocketConfig                                                                     
 * 描    述: 计算校验socket数组                                                              
 *                                                                               
 * 输    入:无                                                          
 * 输    出:无                                                                    
 * 返    回: 负:强制使用USART升级  其余：正常检查系统并确定是否升级                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年5月9日                                                                    
 *******************************************************************************/
int calSocketConfig(SOCKET net){
	uint16_t crc = calBufCrc(net.ipUrl-2,sizeof(SOCKET)-2);
	printf("\r\ncrc :%d\r\ncal crc :%d\r\n",net.crc,crc);
	if(crc != net.crc ||net.flag == 0 ){
//		memset(net.connectCmd,'\0',60);
//		memset(net.connectState,'\0',60);
////		memset(net.connectSet,'\0',20);
////		memset(net.connectSend,'\0',20);
//
//		sprintf(net.connectCmd,"AT+MIPOPEN=%d,%d,\"%s\",%d,%d\r",net.udpTcp,net.localPort,net.ipUrl,net.desPort,1);
//		//Send_AT_And_Wait("AT+MIPOPEN=1,5000,\"114.91.112.239\",80,1\r","+MIPOPEN: 1,1",2000);
//		sprintf(net.connectState,"+MIPOPEN: %d,%d",net.socketNum,1);
//		sprintf(net.socketNum,"%d",socketNum);
//		sprintf(net.connectSend,"%d",socketNum);
//
//		printf("\r\nsocket cmd :%s\r\n",net.connectCmd);
//		printf("\r\nsocket num :%s\r\n",net.socketNum);
//		printf("\r\nsocket state:%s\r\n",net.connectState);



	}
	
	net.crc = crc;
	
	return 1;
}

int readImei(void){
	uint16_t i = 0;
	int8_t *p;
	int8_t tmp[10];
	memset(simState.imei,'\0',18);
	if(Send_AT_And_Wait("AT+CGSN\r","OK",1000) == 0){
		printf("\r\nget iemi error\r\n");
		return -1;
	}
//	Send_AT_And_Wait("AT+CGSN=?\r","OK",1500);
	p=strstr(SIM_BUF,"OK");
	p -= 19;
	for(i=0;i<15;i++){
		simState.imei[i] = p[i];
	}
	printf("\r\nimei:%s\r\n",simState.imei);
	memcpy(tmp,simState.imei,7);
	simState.imeiHigh = atoi(tmp);
	memset(tmp,'\0',10);
	strcat(tmp,simState.imei+7);
	simState.imeiLow = atoi(tmp);
	printf("\r\niemi :%d  %d\r\n",simState.imeiHigh,simState.imeiLow);
	return 1;

}


/*
****IMEI|HV_SV#
****DEV_ID|IAP_IP|PORT|NET_IP|PORT|BLUE_NAME|BLUE_PASS#	
*/

void setReport(int echo)
{
	int i;
	char *temp;
	int type = 0;
	uint8_t devId[20];
	uint8_t port[10];
	SOCKET iap,net;
	BLUE_CONFIG blue;
//	delay_ms(1000);

	temp = strstr(ISP_BUF,"****");


	if(temp)
	{	
//		printf("\r\nreceive data:\r\n");
		for(i = 0;i<strlen(temp);i++){
			USART_SendData(USART1,*(temp+i));	
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);	
		}

		temp+=4;
//		printf("\r\n**** :%s\r\n",temp);
		for(i=0;*temp != '|';i++){//DEV ID
			devId[i] = *temp;
			temp++;
		}
//		printf("\r\nDEV_ID :%s\r\n",devId);
//		printf("\r\nafter dev id:  %s\r\n",temp);
		temp++;//skip '|' 

		for(i = 0;*temp != '|';i++){// IAP IP
			iap.ipUrl[i] = *temp;
			temp++;

		}
		temp++;
//		printf("\r\nIAP_IP :%s\r\n",iap.ipUrl);
		memset(port,'\0',10);
		for(i = 0;*temp != '|';i++){ //IAP PORT
			port[i] = *temp;
			temp++;
		}
		iap.desPort = atoi(port);
//		printf("\r\nIAP_PORT :%d\r\n",iap.desPort);
		temp++;//skip '|'
		for(i = 0;*temp != '|';i++){// NET IP
			net.ipUrl[i] = *temp;
			temp++;

		}
		temp++;//skip '|'
//		printf("\r\nNET_IP :%s\r\n",net.ipUrl);
		memset(port,'\0',10);
		for(i = 0;*temp != '|';i++){ //NET PORT
			port[i] = *temp;
			temp++;
		}
		net.desPort = atoi(port);
//		printf("\r\nNET_PORT :%d\r\n",net.desPort);
		temp++;//skip '|'
		for(i = 0;*temp != '|';i++){ //BLUE NAME
			blue.name[i] = *temp;
			temp++;
		}
//		printf("\r\nBLUE_NAME :%s\r\n",blue.name);
		temp++;//skip '|'
		for(i = 0;*temp != '|';i++){ //BLUE PASSWORD
			blue.password[i] = *temp;
			temp++;
		}
//		printf("\r\nBLUE_PASS :%s\r\n",blue.password);
		temp++;//skip '|'
		memset(port,'\0',10);
		for(i = 0;*temp != '#';i++){ //BLUE BAUDRATE
			port[i] = *temp;
			temp++;
		}
		blue.baudrate = atoi(port);
//		
//		printf("\r\nBLUE_BAUD :%d\r\n",blue.baudrate);
	}
	else
	{
		return;
	}

	delay_ms(3000);


	iap.flag = 0;
	iap.udpTcp = 0;//TCP
	iap.socketNum = 1;
	iap.localPort = 5000;
	iap.crc = calBufCrc(iap.ipUrl-2,sizeof(SOCKET)-2);

	net.flag = 0;
	net.udpTcp = 1;//UDP
	net.socketNum = 1;
	net.localPort = 5001;  
	net.crc = calBufCrc(net.ipUrl-2,sizeof(SOCKET)-2);

//typedef struct {
//	uint16_t crc;
//	uint16_t other;
//	uint32_t baudrate;
//	uint8_t  reserved[20];	
//}OBD_CONFIG;
	blue.crc = calBufCrc((char *)(&(blue.crc)) + 2,sizeof(SOCKET)-2);

//int16_t updateFlashConfig(SOCKET *net,SOCKET *iap,IAP_BRIFE *iapBrife,BLUE_CONFIG *blue,OBD_CONFIG *obd)
	updateFlashConfig(&net,&iap,NULL,&blue,NULL);
	#ifdef PRINTF_DEBUG
	printf("\r\nCONFIG UPDATE OK ,REBOOT\r\n");
	#endif
	
	__disable_irq();//关总中断
	__disable_fault_irq();
	delay_ms(1000);
	NVIC_SystemReset();	
}

void showConfig(FLASH_CONFIG *p){
	#ifdef PRINTF_DEBUG
	int i = 0;
//	char *q = (char *)p;
//	printf("\r\nSHOW CONFIG DATA:\r\n");
//	for(i = 0 ;i <sizeof(FLASH_CONFIG);i++){
//		printf("%3X",q[i]);
//
//	}
	printf("\r\n\r\n\r\nCONFIG DATA++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n");
	printf("\r\nCRC	     	:%X\r\n",p->crc);
	printf("\r\nFLAG     	:%X\r\n",p->flag);

	printf("\r\n\r\nNET SOCKET\r\n");
	printf("\r\nCRC     	:\t%X\r\n",p->netConfig.crc);
	printf("\r\nFLAG     	:\t%X\r\n",p->netConfig.flag);
	printf("\r\nIP       	:\t%s\r\n",p->netConfig.ipUrl);
	printf("\r\nUDPTCP  	:\t%X\r\n",p->netConfig.udpTcp);
	printf("\r\nSOCKET 		:\t%X\r\n",p->netConfig.socketNum);
	printf("\r\nLPORT		:\t%d\r\n",p->netConfig.localPort);
	printf("\r\nDPORT		:\t%d\r\n",p->netConfig.desPort);
	printf("\r\nRESERVED DATA:\r\n");
	for(i = 0;i < 20;i++){
		printf("%3X",p->netConfig.reserved[i]);
	}

	printf("\r\n\r\nIAP SOCKET\r\n");
	printf("\r\nCRC 		:\t%X\r\n",p->iapConfig.crc);
	printf("\r\nFLAG		:\t%X\r\n",p->iapConfig.flag);
	printf("\r\nIP			:\t%s\r\n",p->iapConfig.ipUrl);
	printf("\r\nUDPTCP   	:\t%X\r\n",p->iapConfig.udpTcp);
	printf("\r\nSOCKET  	:\t%X\r\n",p->iapConfig.socketNum);
	printf("\r\nLPORT    	:\t%X\r\n",p->iapConfig.localPort);
	printf("\r\nDPORT    	:\t%X\r\n",p->iapConfig.desPort);
	printf("\r\nRESERVED DATA:\r\n");
	for(i = 0;i < 20;i++){
		printf("%3X",p->iapConfig.reserved[i]);
	}

	printf("\r\n\r\nIAP BRIFE\r\n");
	printf("\r\nSTART  		:\t%s\r\n",p->iapBrife.start);
	printf("\r\nCRC   		:\t%X\r\n",p->iapBrife.crc);
	printf("\r\nHV    		:\t%X\r\n",p->iapBrife.hv);
	printf("\r\nSV   		:\t%X\r\n",p->iapBrife.sv);
	printf("\r\nPACK_NUM	:\t%d\r\n",p->iapBrife.packetNum);
	printf("\r\nCRC_APP  	:\t%X\r\n",p->iapBrife.appCrc);
	printf("\r\nSIZE_APP	:\t%X\r\n",p->iapBrife.sizeOfApp);
//	printf("\r\nRESERVED DATA:\r\n");
//	for(i = 0;i < 20;i++){
//		printf("%3X\",p->obdConfig.reserved[i]);
//	}


	printf("\r\n\r\nBLUE CONFIG\r\n");
	printf("\r\nCRC     	:\t%X\r\n",p->blueConfig.crc);
	printf("\r\nOTHER   	:\t%X\r\n",p->blueConfig.other);
	printf("\r\nNAME   		:\t%s\r\n",p->blueConfig.name);
	printf("\r\nPASS   		:\t%s\r\n",p->blueConfig.password);
	printf("\r\nbaudrate 	:\t%X\r\n",p->blueConfig.baudrate);
	printf("\r\nRESERVED DATA:\r\n");
	for(i = 0;i < 20;i++){
		printf("%3X",p->obdConfig.reserved[i]);
	}

	printf("\r\n\r\nOBD CONFIG\r\n");
	printf("\r\nCRC   		:\t%X\r\n",p->obdConfig.crc);
	printf("\r\nFLAG    	:\t%X\r\n",p->obdConfig.other);
	printf("\r\nBAUDRATE 	:\t%X\r\n",p->obdConfig.baudrate);
	printf("\r\nRESERVED DATA:\r\n");
	for(i = 0;i < 20;i++){
		printf("%3X",p->obdConfig.reserved[i]);
	} 

	#endif 
}

void	getSimTime(void)
{
	printf("\r\nGET GSM TIME++++++++++++++\r\n");
	Send_AT_And_Wait("AT+CMGL?\r","OK",1000);
	Send_AT_And_Wait("AT+CCLK?\r","OK",1000);

}






