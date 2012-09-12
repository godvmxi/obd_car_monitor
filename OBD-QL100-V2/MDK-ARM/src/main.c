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
char test[] = "$GPRMC,054823.000,A,3110.4511,N,12123.6058,E,0.00,144.06,110912,,,A*61";
int main(void)
{	
	int i = 0;
	

	
	hardware_init();

//	for(i = 0;i <strlen(test);i++){
//		gpsDealChar(test+i);
//	}
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
	TIM_Cmd(TIM3, ENABLE);
	gpsPowerOn();
	while(1){
		USART_Cmd(GPS, DISABLE);
		showGpsData();
		USART_Cmd(GPS, ENABLE);
		ISP_DIRECTION=USART_GPS;
		delay_ms(5000);
		ISP_DIRECTION=USART_SIM;
		printf("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
	};
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
	
//	initAT45DB041B();
//	initFlashFifo();
//	flashFifoTest();
//	flashTest();
	setDebugPara();	 

	ISP_DIRECTION=USART_SIM;
//	sim900_power_on();
//		ISP_DIRECTION=USART_SIM;
//	sim900_power_on();
//		ISP_DIRECTION=USART_SIM;
//	sim900_power_on();
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
	LED2(Bit_SET);
	LED2(Bit_SET);
	LED2(Bit_SET);
	delay_ms(1000);
	LED2(Bit_RESET);
	LED2(Bit_RESET);
	LED2(Bit_RESET);
	delay_ms(1000);
	LED2(Bit_SET);
	LED2(Bit_SET);
	LED2(Bit_SET);
	delay_ms(1000);
	LED2(Bit_RESET);
	LED2(Bit_RESET);
	LED2(Bit_RESET);
	delay_ms(1000);
	LED2(Bit_SET);
	LED2(Bit_SET);
	LED2(Bit_SET);
	delay_ms(1000);
	



	collectAndSend();
		
}



