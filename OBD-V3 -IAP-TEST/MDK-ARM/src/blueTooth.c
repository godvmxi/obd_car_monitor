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
	if(state == 0){
		printf("\r\n�ر�������Դ\r\n");
		BLUE_EN(Bit_RESET);
		return 1;
	}
	else{
		printf("\r\n����������Դ\r\n");
		BLUE_EN(Bit_SET);
		delay_ms(2000);
		for(i=0;i<5;i++){
			if(blueAtAndWait("AT\r","OK",1000)){//ִ�гɹ�
				printf("\r\n���������ɹ�\r\n");
				return 1;
			}
		}
		
		printf("\r��������ʧ��\n");
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

	printf("\r\nset bluetooth mode\r\n");
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



