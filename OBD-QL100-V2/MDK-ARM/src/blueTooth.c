#include "blueTooth.h"


/*******************************************************************************
 * ��������:blueAtAndWait                                                                     
 * ��    ��:����AT����ִ��                                                                     
 *                                                                               
 * ��    ��:at���������ֵ��ѯ�ؼ��֣�����ִ�еȴ�ʱ��                                                                    
 * ��    ��:��                                                                     
 * ��    ��:0������δ�������������1����������������                                                                    
 * ��    ��:����                                                                     
 * �޸�����:2011��7��13��                                                                    
 *******************************************************************************/
int blueAtAndWait(char *at,char *key,int ms){
	int x = 0;
	memset(BLUE_BUF,0,300);
	for(x=0;*(at+x)!='\0';x++)
	{
		USART_SendData(BLUE,*(at+x));	
		while( USART_GetFlagStatus(BLUE,USART_FLAG_TC)==RESET ); 
	}
	BLUE_COUNT = 0;
	delay_ms(ms);
	if(strstr(BLUE_BUF,key)){
		return 1;
	}
	else{
		return 0;
	}


}  

/*******************************************************************************
 * ��������:blueToothPower                                                                     
 * ��    ��:����������Դ                                                                     
 *                                                                               
 * ��    ��:0���ر�  1������                                                                     
 * ��    ��:��                                                                    
 * ��    ��:����ִ��״̬                                                                     
 * ��    ��:����                                                                     
 * �޸�����:2012��3��1��                                                                    
 *******************************************************************************/
int blueToothPower(int state){

	int i=0;
	ISP_DIRECTION = USART_BLUE;
	if(state == 0){
		printf("\r\nblue tooth pwoer off\r\n");
		BLUE_EN(Bit_RESET);
		return 1;
	}
	else{
		printf("\r\nblue tooth pwoer on\r\n");
		BLUE_EN(Bit_SET);
		delay_ms(2000);
		for(i=0;i<5;i++){
			if(blueAtAndWait("AT\r","OK",1000)){//ִ�гɹ�
				printf("\r\nblue start ok\r\n");
				return 1;
			}
		}
		
		printf("\rblue start error\n");
		return 0;		
	}
}
/*******************************************************************************
 * ��������:initBlueTooth                                                                     
 * ��    ��:������������ģʽ                                                                   
 *                                                                               
 * ��    ��:0 ��ģʽ 1 ��ģʽ                                                                    
 * ��    ��:��                                                                     
 * ��    ��:0 ʧ�� 1 �ɹ�                                                                     
 * ��    ��:����                                                                     
 * �޸�����:2011��7��13��                                                                    
 *******************************************************************************/
int initBlueTooth(int mode){

	
	int8_t blueCmd[30];
	USART_InitTypeDef USART_InitStructure;
//	printf("\r\nclear setting\r\n");
//	GPIO_WriteBit(GPIOB,GPIO_Pin_9,Bit_SET);
//	delay_ms(5000);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_9,Bit_RESET);
//	delay_ms(1000);
//
//	printf("\r\nclear setting\r\n");


	printf("\r\nset bluetooth mode\r\n");
	blueAtAndWait("AT\r\n","OK",500);
	blueAtAndWait("AT\r\n","OK",500);
	sprintf(blueCmd,"AT+NAME%s\r\n",sysCfg.blueConfig.name);
	printf("set blue name cmd :%s",blueCmd);
	if(blueAtAndWait(blueCmd,"OK",500) < 0 ){
		return -1;
	}
	blueAtAndWait("AT+NAME\r\n","OK",500);

	memset(blueCmd,'\0',30);
	sprintf(blueCmd,"AT+PIN%s\r\n",sysCfg.blueConfig.password);
	printf("set blue pass cmd :%s",blueCmd);
	if(blueAtAndWait(blueCmd,"OK",500) < 0 ){
		return -1;
	}
	blueAtAndWait("AT+PIN\r\n","OK",500);

	memset(blueCmd,'\0',30);
//	switch(sysCfg.blueConfig.baudrate)
	switch(38400){
		case 1200:
			strcat(blueCmd,"AT+BAUD1\r\n");
			break;
		case 2400:
			strcat(blueCmd,"AT+BAUD2\r\n");
			break;
		case 4800:
			strcat(blueCmd,"AT+BAUD3\r\n");
			break;
		case 9600:
			strcat(blueCmd,"AT+BAUD4\r\n");
			break;
		case 19200:
			strcat(blueCmd,"AT+BAUD5\r\n");
			break;
		case 38400:
			strcat(blueCmd,"AT+BAUD6\r\n");
			break;
		case 58600:
			strcat(blueCmd,"AT+BAUD7\r\n");
			break;
		case 115200:
			strcat(blueCmd,"AT+BAUD8\r\n");
			break;
		case 230400:
			strcat(blueCmd,"AT+BAUD9\r\n");
			break;
		case 460800:
			strcat(blueCmd,"AT+BAUDA\r\n");
			break;
		case 921600:
			strcat(blueCmd,"AT+BAUDB\r\n");
			break;
		case 1382400:
			strcat(blueCmd,"AT+BAUDC\r\n");
			break;
		default:
			strcat(blueCmd,"AT+BAUD6\r\n");
			break;	
	}
	printf("baud cmd :%s",blueCmd);
	if(blueAtAndWait(blueCmd,"OK",500) < 0 ){
		return -1;
	}
	blueAtAndWait("AT+PIN\r\n","OK",500); 

	USART_InitStructure.USART_BaudRate = 38400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(UART4, &USART_InitStructure); 
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	
	return 1;
}

/*******************************************************************************
 * ��������:blueDma                                                                     
 * ��    ��:��������DMA����                                                                   
 *                                                                               
 * ��    ��:DMA��ַ��DMAͨ����DMA����״̬                                                                     
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:����                                                                     
 * �޸�����:2011��7��13��                                                                    
 *******************************************************************************/
int blueDma(char *add,int channel,int state){

	return 1;
}



/*******************************************************************************
 * ��������:blueTest                                                                     
 * ��    ��:�������Գ���                                                                  
 *                                                                               
 * ��    ��:DMA��ַ��DMAͨ����DMA����״̬                                                                     
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:����                                                                     
 * �޸�����:2011��7��13��                                                                    
 *******************************************************************************/
void blueTest(void){
	
	ISP_DIRECTION = USART_BLUE;
	printf("\r\nblue test\r\n");
	blueToothPower(1);
	delay_ms(5000);
	initBlueTooth(1);
	while(1){
		blueAtAndWait("AT\r\n","OK",500);
		blueAtAndWait("AT\r\n","OK",500);
		blueAtAndWait("AT\r\n","OK",500);
		blueAtAndWait("AT\r\n","OK",500);
		blueAtAndWait("AT\r\n","OK",500);
		blueAtAndWait("AT\r\n","OK",500);
		blueAtAndWait("AT\r\n","OK",500);
		blueAtAndWait("AT\r\n","OK",500);
		blueAtAndWait("AT\r\n","OK",500);
		delay_ms(20000);
		continue;
		blueAtAndWait("AT+VERSION\r\n","OK",500);


		blueAtAndWait("AT+NAME\r\n","OK",500);
		blueAtAndWait("AT+NAMEOBD_1\r\n","OK",500);
		blueAtAndWait("AT+NAME\r\n","OK",500);

	   	blueAtAndWait("AT+PIN\r\n","OK",500);
		blueAtAndWait("AT+PIN2345\r\n","OK",500);
		blueAtAndWait("AT+PIN\r\n","OK",500);

		blueAtAndWait("AT+BAUD\r\n","OK",500);
		blueAtAndWait("AT+BAUD\r\n","OK",500);

		blueAtAndWait("AT+ROLE\r\n","OK",500);
		blueAtAndWait("AT+ROLE0\r\n","OK",500);
		blueAtAndWait("AT+ROLE\r\n","OK",500);
		blueAtAndWait("AT+ROLE1\r\n","OK",500);
		blueAtAndWait("AT+ROLE\r\n","OK",500);
		blueAtAndWait("AT+ROLE0\r\n","OK",500);
		blueAtAndWait("AT+ROLE\r\n","OK",500);

		blueAtAndWait("AT+UARTMODE\r\n","OK",500);
		blueAtAndWait("AT+UARTMODE0,0\r\n","OK",500);
		blueAtAndWait("AT+UARTMODE\r\n","OK",500);

		blueAtAndWait("AT+LADDR?\r\n","OK",500);
		blueAtAndWait("AT+STATE?\r\n","OK",500);
		blueAtAndWait("AT+UARTMODE?\r\n","OK",500);
		blueAtAndWait("AT+HELP\r\n","OK",500);
		blueAtAndWait("AT\r\n","OK",500);
		blueAtAndWait("AT\r\n","OK",500);
	} 

}



