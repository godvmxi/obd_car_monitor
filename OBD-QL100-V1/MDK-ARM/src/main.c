#include "sysdef.h"	 
//-------------------------------------------------------

int fputc(int ch, FILE *f){//printf
  /* Write a character to the USART */
	USART_SendData(USART1, (u8) ch);
	
	/* Loop until the end of transmission */
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	{
	}
	
	return ch;

}

int main(void)
{	
	hardware_init();
//	getObdPids();
//	DEVICE_STATE = 1;
//	while(1){
////		obdGetPidDatas(1);
//		obdCollectData();
//		delay_ms(100);
//	}
//	while(1){
//		delay_ms(1000);
//		printf("\r\nETR : %d\r\n",TIM_GetCounter(TIM2));
//
//	}
//	obdGetAllData();

//	rtcTest();
//	while(1){
//		printf("\r\ntimer : %d \r\n",TIM_GetCounter(TIM2));
//		delay_ms(10);
//	}
//	enableCanExti(1);
//	obdPower(1);
//	while(1);
//	TIM_Cmd(TIM3, DISABLE);
//	while(1);
//	enableCanExti(0);
//	obdGetAllData();
//	while(1){
//		printf("\r\nI am runing\r\n");
//		delay_ms(1000);
//	}

//	rtcTest();
//	gpsPowerOn();
//	ISP_DIRECTION=USART_GPS;								    
//	while(1);
//	
	setDebugPara();	 
//	blueTest();
//	ISP_DIRECTION=USART_SIM;

//	TIM_Cmd(TIM3, ENABLE);
//	while(1);
//	printf("\r\ntest can receive\r\n");
//	while(1);
//	sim900_power_on(); 	

	
//	delay_ms(2000);
//	OBD_START = 0;
//	sim900Test();
	
//	printf("\r\nREAD SYSTEM CONFIG :%10X %10X\r\n",IAP_PARAMETER,SYS_PARAMETER);
//	flashRead(IAP_PARAMETER,(uint8_t *)&sysCfg,sizeof(FLASH_CONFIG));
//	showConfig(&sysCfg);
//	flashRead(SYS_PARAMETER,(uint8_t *)&sysCfg,sizeof(FLASH_CONFIG));
//	showConfig(&sysCfg); 
//	massProduction();
//	printf("\r\nflash config :%d\r\n",sizeof(FLASH_CONFIG));


//	ISP_DIRECTION=USART_SIM;
// 	obdBlue();
//	obdGetAllData();
//	obdTest();
	collectAndSend();
		
}



