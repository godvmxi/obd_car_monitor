#include "sysdef.h"	 
//-------------------------------------------------------



int fputc(int ch, FILE *f);//printf


extern char cimi[5],csq[2];//,new_cellid[8],old_cellid[8];
extern int voltage = 4000; //默认满电 

int WORKING_MODE=0; // 0主动汇报  1休眠

int H_MODE=0;  //分钟|秒

//
int L_MODE=900;  //分钟|小时
char IMEI[18]="*G000000000000000#";
int powerKeyFlag = 0;//关机键相应标志，当系统正常运行时，不响应修改SYSTEM_PC的命令，避免系统出现为止情况崩溃
 
//------------------------------------------------------------------------------------------------

volatile unsigned int SYSTEM_ON_TIME=0;		//系统运行时间变量
int SYSTEM_PC=0; // 0 正常汇报 1：紧急打电话 2：接电话 3：挂断电话  7：要关机 8：已经成功发送关机标志 9:关机成功，进入长休眠
				// 10 正常休眠 11手动唤醒 12被动休眠 13被动休眠手动唤醒	14被动休眠电话唤醒	15:设备故障，重启启动 16:设备故障，关机 


DATA_IN_FLASH sysStateNow;
int shuiyin_counter = 0;
extern int USART1_DIRECTION;
extern RTC_STRUCT rtcData;

extern DATA_HEAD obdHeader;

int main(void)
{	
//	int8_t src[] = "AAAA";
//	int8_t des[100];
//	int16_t length = 0 ;
	uint32_t i = 0;

	
	hardware_init();

	ISP_DIRECTION=USART_SIM; 

	
	delay_ms(2000);

	while(1){
		printf("\r\nI am in APP :%d\r\n",i++);
		LED3(Bit_RESET);
		delay_ms(1000);
		LED3(Bit_SET);
		delay_ms(1000);

	}


	obdTest();
	sim900Test();
	gpsTest();
	rtcTest();
	obdTest();
	collectAndSend();
	

//	if(*(vu32*)(FLASH_ADR) != 0) //量产程序
//	{			
//		massProduction();  		
//	}	 	  	
//	
//	GPIO_Configuration();
//	readFlashCfg(FLASH_ADR,&sysStateNow);
//	
////	
//	DATA_TO_SIM900();		
}
int fputc(int ch, FILE *f)//printf
{
  /* Write a character to the USART */
  USART_SendData(USART1, (u8) ch);

  /* Loop until the end of transmission */
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  {
  }

  return ch;
}

