#include "sysdef.h"	 
//-------------------------------------------------------



int fputc(int ch, FILE *f);//printf


extern char cimi[5],csq[2];//,new_cellid[8],old_cellid[8];
extern int voltage = 4000; //Ĭ������ 

int WORKING_MODE=0; // 0�����㱨  1����

int H_MODE=0;  //����|��

//
int L_MODE=900;  //����|Сʱ
char IMEI[18]="*G000000000000000#";
int powerKeyFlag = 0;//�ػ�����Ӧ��־����ϵͳ��������ʱ������Ӧ�޸�SYSTEM_PC���������ϵͳ����Ϊֹ�������
 
//------------------------------------------------------------------------------------------------

volatile unsigned int SYSTEM_ON_TIME=0;		//ϵͳ����ʱ�����
int SYSTEM_PC=0; // 0 �����㱨 1��������绰 2���ӵ绰 3���Ҷϵ绰  7��Ҫ�ػ� 8���Ѿ��ɹ����͹ػ���־ 9:�ػ��ɹ������볤����
				// 10 �������� 11�ֶ����� 12�������� 13���������ֶ�����	14�������ߵ绰����	15:�豸���ϣ��������� 16:�豸���ϣ��ػ� 


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
	

//	if(*(vu32*)(FLASH_ADR) != 0) //��������
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

