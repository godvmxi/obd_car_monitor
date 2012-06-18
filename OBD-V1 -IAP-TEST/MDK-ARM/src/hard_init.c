
#include "hard_init.h"


int debug=1;

extern char rxBuffer1[IPD_BUFFER];

extern char *IMEI;
extern DATA_IN_FLASH sysStateNow;
//extern int USART2_RX_buffer_count;


extern int SYSTEM_PC_ERROR;
extern SLEEP_TYPE sleepType;
extern int voltage;

extern int WORKING_MODE;


extern SANY_DATA sanyData;
extern SANY_POS_REPORT sanyPosData;
int ERROR_FLAG =0;



extern char cimi[5],csq[2],new_cellid[8];

extern int WORKING_MODE; // 0�����㱨  1����
extern int H_MODE;  //����|��
extern int L_MODE;  //����|Сʱ
int H_MODE_BAK = 300;
//int L_MODE_BAK;
int errorHandleState;//0��ʾϵͳ����������1��ϵͳ���ڴ�����״̬
extern int systemStopState;

//--------------------------------------------------------------




extern unsigned int SYSTEM_ON_TIME;
unsigned int CURRENT_TIME=0;

extern int gps_data_value;

extern int  voltage;

//char net_cmd[51]="AT+CIPSTART=\"UDP\",\"117.040.196.131\",\"12021\"\r";

extern int ReciveFlag; //���ݽ������. ���һ�� GPRMC ��䷢�������1

extern int SYSTEM_PC; // 0 �����㱨 1��������绰 2���ӵ绰 3���Ҷϵ绰  7��Ҫ�ػ� 8���Ѿ��ɹ����͹ػ���־ 9:���ֶ��ػ��л���||��ʼֵΪ0

extern volatile int TIMER_COUNTER;
extern volatile int feedDogCounter;//ι���źţ�3��������Ҫ����һ��



//------------------------------------�����ź�GSV��������----------------------------------------------	  \

extern int satellite_have_signal;	   //���ź�ֵ�����Ǹ���
extern int satellite_below_10;		   //�ź�ֵ>10�����Ǹ���
extern int satellite_below_20;		   //�ź�ֵ>20�����Ǹ���
extern int satellite_below_30;		   //�ź�ֵ>30�����Ǹ���
extern int satellite_below_40;		   //�ź�ֵ>40�����Ǹ���
//----------------------------------------------------

//---------------------------------------------------//
//����ת������





void delay_ms(unsigned int time)
{
	unsigned int x,y;
//	feedDog();
	for(x=0;x<time;x++)
		for(y=0;y<13000;y++);	
}

//void RCC_Configuration(void)
//{
//	ErrorStatus HSEStartUpStatus;
//	//������ RCC�Ĵ�������Ϊȱʡֵ
//	RCC_DeInit();
//	
//
//    //ʹ���ⲿ����
//    RCC_HSEConfig(RCC_HSE_ON);
//    //�ȴ��ⲿ�����ȶ�
//    HSEStartUpStatus = RCC_WaitForHSEStartUp();
//    //����ⲿ���������ɹ����������һ������
//    if(HSEStartUpStatus==SUCCESS)
//    {
//        //����HCLK��AHBʱ�ӣ�=SYSCLK
//        RCC_HCLKConfig(RCC_SYSCLK_Div1);
//
//        //PCLK1(APB1) = HCLK/2
//        RCC_PCLK1Config(RCC_HCLK_Div2);
//
//        //PCLK2(APB2) = HCLK
//        RCC_PCLK2Config(RCC_HCLK_Div1);
//
//        //FLASHʱ�����
//        //�Ƽ�ֵ��SYSCLK = 0~24MHz   Latency=0
//        //        SYSCLK = 24~48MHz  Latency=1
//        //        SYSCLK = 48~72MHz  Latency=2
//        FLASH_SetLatency(FLASH_Latency_2);
//        //����FLASHԤȡָ����
//        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
//
//        //PLL���� SYSCLK/1 * 9 = 8*1*9 = 72MHz
//        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
//        //����PLL
//        RCC_PLLCmd(ENABLE);
//        //�ȴ�PLL�ȶ�
//        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
//        //ϵͳʱ��SYSCLK����PLL���
//        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
//        //�л�ʱ�Ӻ�ȴ�ϵͳʱ���ȶ�
//        while(RCC_GetSYSCLKSource()!=0x08);
//
//
//        /*
//        //����ϵͳSYSCLKʱ��ΪHSE����
//        RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);
//        //�ȴ�ʱ���л��ɹ�
//        while(RCC_GetSYSCLKSource() != 0x04);
//        */
//    }	
//
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|
//							   RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE );
//	   	
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3|RCC_APB1Periph_PWR | RCC_APB1Periph_BKP|
//							   RCC_APB1Periph_TIM3|RCC_APB1Periph_CAN1, ENABLE );
//							   
//}


void RCC_Configuration(void) //�ڲ�
{

//������ RCC�Ĵ�������Ϊȱʡֵ
	RCC_DeInit();
	RCC_HSICmd(ENABLE);

	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
	{
	}

	if(1)
	{
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	FLASH_SetLatency(FLASH_Latency_2);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div1);


//���� PLL ʱ��Դ����Ƶϵ��
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);
//ʹ�ܻ���ʧ�� PLL,�����������ȡ��ENABLE����DISABLE
	RCC_PLLCmd(ENABLE);//���PLL������ϵͳʱ��,��ô�����ܱ�ʧ��
//�ȴ�ָ���� RCC ��־λ���óɹ� �ȴ�PLL��ʼ���ɹ�
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	{
	}
//����ϵͳʱ�ӣ�SYSCLK�� ����PLLΪϵͳʱ��Դ


	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 
//�ȴ�PLL�ɹ�������ϵͳʱ�ӵ�ʱ��Դ
// 0x00��HSI ��Ϊϵͳʱ��
// 0x04��HSE��Ϊϵͳʱ��
// 0x08��PLL��Ϊϵͳʱ��
	while(RCC_GetSYSCLKSource() != 0x08)
	{
	} 
	/*
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);

	while(RCC_GetSYSCLKSource() != 0x00)
	{
	} 
	*/
}	

	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|
							   RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO, ENABLE );
	   	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3|RCC_APB1Periph_UART4|RCC_APB1Periph_UART5|RCC_APB1Periph_PWR | RCC_APB1Periph_BKP|
							   RCC_APB1Periph_TIM3|RCC_APB1Periph_CAN1, ENABLE );
							   
}

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;



	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|
							   RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO, ENABLE );
	   	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3|RCC_APB1Periph_UART4|RCC_APB1Periph_UART5|RCC_APB1Periph_PWR | RCC_APB1Periph_BKP|
							   RCC_APB1Periph_TIM3|RCC_APB1Periph_CAN1, ENABLE );


/*********************************OBD_GPIO_64**********************************************


LED	
		LED1				PB3							���				
		LED2				PB4							���
		LED3				PB5							���


KEY
		KEY1				PA0
		KEY2				PC1

//
//WATCH_DOG
//		DOG					PB13
		
SIM
		TX_UART3			PB10
		RX_UART3			PB11

		SIM_EN				PB13
		SIM_RST				PA4
		SIM_DTR				PA6

GPS
		TX_UART5			PC12
		RX_UART5			PD2

		GPS_EN				PC3

OBD
		TX_UART4			PC10	
		RX_UART4			PC11

		OBD_EN				PB12

		OBD_RST			  	PB15
		OBD_NVM				PB14

		
						 
BLUE_TOOCH
		TX_UART2			PA2	
		RX_UART2			PA3

		BLUE_EN				PA1
		BLUE_STATE			PB7	
		BLUE_CLR			PB6	

		BLUE_MS				PB8
		BLUE_HSW			PB9

FLASH
		FLASH_SI		  	PA5
		FLASH_SO		  	PC4
		FLASH_SCK		  	PA7
		FLASH_RST			PC5
		FLASH_CS			PB0

******************************************************************************/
	
		
	
//-----------------------------������������Ϊģ�����롪����ʡ��---------	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
//-----------------------�������ų�ʼ��---------------------------------
	
	//��ʼ������1����
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);				


	//��ʼ������2����
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//��ʼ������3����
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&GPIO_InitStructure);	


		/*
	*  USART1_TX -> PC10 , USART1_RX ->	PC11
	*/				
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);		   
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);

		/*
	*  USART1_TX -> PB10 , USART1_RX ->	PB11
	*/				
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);		   
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

//---------------can �˿ڳ�ʼ��---------------------------
	/*PA11-CAN RX */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*PA12-CAN TX */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 


	
	//-----------------------------------LED �������---------------------------------
/*
	
		LED1				PB3							���				 
		LED2				PB4							���
		LED3				PB5							���
*/						

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOB,GPIO_Pin_3,Bit_RESET);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_RESET);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_RESET);

/**************************KEY*******************************************************
		KEY1				PA0
		KEY2				PC1
************************************************************************************/

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
/*******************************************************************************
		DOG					PB13
***********************************************************************************/
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
/**************************SIM*******************************************************

		SIM_EN				PB13
		SIM_RST				PA4				OD
		SIM_DTR				PA6
**************************************************************************/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);

	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);
//

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_RESET);

/**************************GPS*******************************************************
		GPS_EN				PC3


************************************************************************************/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOC,GPIO_Pin_3,Bit_RESET);

/**************************OBD*******************************************************
		OBD_EN				PB12

		OBD_RST				PB15
		OBD_NVM				PB14

************************************************************************************/

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET);

	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET); 
	
		
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET); 		
						 
/**************************BLUE_TOOCH*******************************************************

		BLUE_EN				PA1
		BLUE_STATE			PB7	
		BLUE_CLR			PB6	

		BLUE_MS				PB8
		BLUE_HSW			PB9
************************************************************************************/

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	  //����
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_RESET);


	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOB,GPIO_Pin_8,Bit_RESET);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOB,GPIO_Pin_9,Bit_RESET);


/**************************BLUE_TOOCH*******************************************************

		CHARGE_EN				PC13
************************************************************************************/

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);

///***************************************************
//		FLASH_SI		  	PA5
//		FLASH_SO		  	PC4
//		FLASH_SCK		  	PA7
//		FLASH_RST			PC5	
//***************************************************/
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_8,Bit_RESET);
//	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_8,Bit_RESET);
//
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_8,Bit_RESET);
//	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_8,Bit_RESET);   				
}

void GPIO_POWER_SAVING(void)
{ 	
	//PA7 LKC    PA11  GPS����	  	POWERON	PA0	   sim900 txd PA3
	GPIO_InitTypeDef GPIO_InitStructure;
	gpsPowerOff(1);
	if(WORKING_MODE==0)			//�����㱨ģʽ,���˵�Դ����(PB70)��ˮ���ж�(PB9)��ȫ������Ϊ����
	{
		#ifdef PRINTF_DEBUG
		printf("\r\n�豸����ʡ��ʡ��ģʽ\r\n");
		#endif

		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10
							   |GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	 
		GPIO_Init(GPIOA,&GPIO_InitStructure);
			
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6/*|GPIO_Pin_7*/|GPIO_Pin_8|/*GPIO_Pin_9|*/GPIO_Pin_10
							   |GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
		GPIO_Init(GPIOB,&GPIO_InitStructure);


		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_All;
		
		GPIO_Init(GPIOC,&GPIO_InitStructure);
	}
	else   //��������ģʽ��IGT(PA0 ),DTR(PA5),GP1(PC13),PB7�绰���к����(USART2 RXDC3---PA3)������(PA2------����)
	{
		#ifdef PRINTF_DEBUG
		printf("\r\n�豸����ʡ��ʡ��ģʽ\r\n");
		#endif
		
		GPIO_InitStructure.GPIO_Pin=/*GPIO_Pin_0|*/GPIO_Pin_1|GPIO_Pin_2|/*GPIO_Pin_3|*/GPIO_Pin_4|/*GPIO_Pin_5|*/GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10
							   |GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8
								|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12/*|GPIO_Pin_13*/|GPIO_Pin_14|GPIO_Pin_15;
		GPIO_Init(GPIOC,&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|/*GPIO_Pin_7|*/GPIO_Pin_8
								|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
		GPIO_Init(GPIOB,&GPIO_InitStructure);
	}	 
}
void USART_Configuration(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
   
  /***************************USART1*******************************************/
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
  /*
  *  USART1_TX -> PA9 , USART1_RX ->	PA10
  */				
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);		   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 230400;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure); 
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

  USART_Cmd(USART1, ENABLE);



  /***************************USART2**********SIM*********************************/
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2,ENABLE);
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA ,ENABLE);
  /*
  *  USART1_TX -> PA2 , USART1_RX ->	PA3
  */				
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);		   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART2, &USART_InitStructure); 
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//  USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
  USART_Cmd(USART2, ENABLE);
//  return ;

  /***************************USART3*********OBD**********************************/
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3,ENABLE);
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB,ENABLE);
	/*
	*  USART1_TX -> PB10 , USART1_RX ->	PB11
	*/				
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);		   
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	        
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 38400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART3, &USART_InitStructure); 
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	//  USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	USART_Cmd(USART3, ENABLE);


  /***************************USART4********BLUE***********************************/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC,ENABLE);
	/*
	*  USART1_TX -> PC10 , USART1_RX ->	PC11
	*/				
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);		   
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(UART4, &USART_InitStructure); 
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	//  USART_ITConfig(UART4, USART_IT_TXE, ENABLE);
	USART_Cmd(UART4, ENABLE);

  /***************************USART5*******************************************/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOD,ENABLE);
  /*
  *  USART1_TX -> PC12 , USART1_RX ->	PD2
  */				
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);		   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(UART5, &USART_InitStructure); 
  USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
//  USART_ITConfig(UART5, USART_IT_TXE, ENABLE);
  USART_Cmd(UART5, ENABLE);

 
}




void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;	  
	//--------------------------------RTC�����ж�����---------------------------------
 	EXTI_ClearITPendingBit(EXTI_Line17);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	
	//--------------------------------�ⲿkey1�ж�----PA0-----------------------------	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

 	EXTI_ClearITPendingBit(EXTI_Line0);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//�����ش����ж�
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	
	//--------------------------------�ⲿkey1�ж�----PA1-----------------------------
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);

 	EXTI_ClearITPendingBit(EXTI_Line1);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//�����ش����ж�
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	
	
	//--------------------------------����״̬�ж�--------PE3-------------------------	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);	
 	EXTI_ClearITPendingBit(EXTI_Line3);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	//�����½��ش����ж�
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	

//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);	 
// 	EXTI_ClearITPendingBit(EXTI_Line9);
//  	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//�½��ش����ж�
//  	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
//  	EXTI_Init(&EXTI_InitStructure);	



}

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
//	#ifdef IAP_FUNCTION
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,SYS_MAIN -NVIC_VectTab_FLASH); 
//  	#endif
//--------------------------------�򿪴����ж� ---------------------
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);


	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);


//---------------------------------RTC����--------------------------------
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  	NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);


//*************************KEY1*******************************************************************
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;  
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);

//*************************KEY2*******************************************************************
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;  
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);
//*************************����״̬***************************************************************
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;  
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);
 
//-----------------------------------��ʱ���ж�------------------------------------
	/* Timer2�ж�*/
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

	/* Timer3�ж�*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

//---------------------Can ����-------------------------------------------
		/* CAN-RX*/
//	NVIC_InitStructure.NVIC_IRQChannel=USB_LP_CAN1_RX0_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);	
//
//	/* Enable the RTC Interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);


} 


void stop_time(u32 time)
{
		#ifdef PRINTF_DEBUG
		printf("\r\nI have stopped system in TIME mode\r\nThe SYSTEM_PC is %d\r\n",SYSTEM_PC);
		#endif
//		if(SYSTEM_PC_BAK == 7)
//		{ 
//			#ifdef PRINTF_DEBUG
//			printf("\r\nϵͳ�����ػ�\r\nThe SYSTEM is %d\r\n",SYSTEM_PC);
//			#endif
//			return;
//		}
		GPIO_POWER_SAVING();//������������Ϊģ�����롪��ʡ��

	 /* Wait till RTC Second event occurs */
        RTC_ClearFlag(RTC_FLAG_SEC);
        while(RTC_GetFlagStatus(RTC_FLAG_SEC) == RESET);        
        /* Alarm in 3 second */
        RTC_SetAlarm(RTC_GetCounter()+ time);
        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();
    //    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SRAM|RCC_AHBPeriph_FLITF, DISABLE);
        /* Request to enter STOP mode with regulator in low power mode*/
//		if(SYSTEM_PC_BAK == 7)
//		{ 
//			#ifdef PRINTF_DEBUG
//			printf("\r\nϵͳ�����ػ�\r\nThe SYSTEM is %d\r\n",SYSTEM_PC);
//			#endif
//			return;
//		}	
        PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);

}

void stop_notime(void)
{
		#ifdef PRINTF_DEBUG
		printf("\r\nI have stopped system in NOTIME mode\r\nThe SYSTEM_PC is %d\r\nshut down in notime mode\r\n",SYSTEM_PC);
		#endif

		GPIO_POWER_SAVING();//������������Ϊģ�����롪��ʡ��


        PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);

}

void RTC_Configuration(void)
{
  /* RTC clock source configuration ------------------------------------------*/
  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);
  /* Reset Backup Domain */
  BKP_DeInit();
  /* Enable the LSE OSC */
  //RCC_LSEConfig(RCC_LSE_ON);
  RCC_LSICmd(ENABLE);
  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);
  /* RTC configuration -------------------------------------------------------*/
  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();
  /* Set the RTC time base to 1s */
  RTC_SetPrescaler(40000);  
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  /* Enable the RTC Alarm interrupt */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}



void TIM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 /************************************************��ʱ��2����********************************************************/
//	/* ��������*/
//	TIM_TimeBaseStructure.TIM_Period = 20000;		//����ֵ   
//	TIM_TimeBaseStructure.TIM_Prescaler = 800;    	//Ԥ��Ƶ
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  	
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//���ϼ���
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
//	
//	/*ʹ��Ԥװ��*/
//	TIM_ARRPreloadConfig(TIM2, ENABLE);
//	/*Ԥ����������ж�λ*/
//	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);	
//	/* 4��ͨ��������������ж�*/
//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//	/* ����TIM2��ʼ���� */
//	TIM_Cmd(TIM2, DISABLE);

 /************************************************��ʱ��3����********************************************************/
	/* ��������*/
	TIM_TimeBaseStructure.TIM_Period = 10000;		//����ֵ   			1s
	TIM_TimeBaseStructure.TIM_Prescaler = 7200;    	//Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//���ϼ���
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	/*ʹ��Ԥװ��*/
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	/*Ԥ����������ж�λ*/
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);	
	/*����������ж�*/
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	/* ����TIM2��ʼ���� */
	TIM_Cmd(TIM3, ENABLE);
}



void hardware_init(void)
{	
	RCC_Configuration();
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);		   //��ֹJTAG
	GPIO_Configuration();

	USART_Configuration();
	RTC_Configuration(); 	
	EXTI_Configuration();
//	CAN_Interrupt();
	NVIC_Configuration();


	TIM_Configuration();
}

void open_sys(void)	  //LED2û��
{
//	RED_OFF;
//	GREEN_ON;
//	delay_ms(500);
//	GREEN_OFF;
}


void open_power_button(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
 	EXTI_ClearITPendingBit(EXTI_Line0);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//�½��ش����ж�
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);
}

void close_power_button(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
 	EXTI_ClearITPendingBit(EXTI_Line0);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//�½��ش����ж�
  	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  	EXTI_Init(&EXTI_InitStructure);
}


void setLedOn(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

}



void open_shuiyin(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	#ifdef PRINTF_DEBUG
	printf("\r\nopen shuiyin interrupt\r\n");
	#endif
	//PB9
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);
 	EXTI_ClearITPendingBit(EXTI_Line9);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	//�½��ش����ж�
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		 
	
}

void close_shuiyin(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	#ifdef PRINTF_DEBUG
	printf("\r\nclose shuiyin interrupt\r\n");
	#endif
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);
 	EXTI_ClearITPendingBit(EXTI_Line9);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	//�½��ش����ж�
  	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  	EXTI_Init(&EXTI_InitStructure);

}

void DATA_TO_SIM900(void)
{	
	int i;
	int time;
//	FlagStatus Status;
//---------------------------sim900����--------------------------------------
	 
	ISP_DIRECTION = 2;
	initData();
	sim900_boot();
	gpsPowerOn();

	ERROR_RETURN:
	Send_AT_And_Wait("AT+CMGDA=6\r","OK",500);
//-------------SIM900��Ƭ���------------------------------------------------------------
	#ifdef PRINTF_DEBUG
	printf("\r\n��Ƭ������----\r\n\r\n");
	printf("\r\nERROR_FLAG:%d\r\n\r\n",ERROR_FLAG);
	#endif
	ERROR_FLAG = 0;
	if(strstr(USART2_RX_buffer,"NOT INSERTED"))// || GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) == 1)
	{
		sysErrorHandle(sim900NotInsert);
		goto ERROR_RETURN;
	} 
	checkDns();
	if(reConnect()== 0)
	{
		#ifdef PRINTF_DEBUG
		printf("\r\n��������ʧ��\r\n");
		#endif
		sysErrorHandle(sim900NotInsert);
		goto ERROR_RETURN;	
	}
	

	while(1)
	{
		SYSTEM_ON_TIME = 0;
		feedDogCounter = 0;// ����ι������
		gpsDataInit();//��ʼ��gps����
		checkLockState();
		#ifdef 	PRINTF_DEBUG
//		showTime();
		printf("\r\n��鲹����Ϣ\r\n");
		printf("\r\n\r\n\r\n\r\n\r\n��������״̬��%d\r\n",systemStopState);
		#endif
		while(systemStopState){
			gpsDataInit();//��ʼ��gps����
			#ifdef 	PRINTF_DEBUG
			printf("\r\n������ֹ����ʼ������Ϣ\r\n");
			#endif
			delay_ms(2000);
			while(systemStopState){
				if(reSendFlashMsg() == 0)
				{
					#ifdef 	PRINTF_DEBUG
					printf("\r\nû��������Ҫ����\r\n");
					#endif
					break;
				}
			}
//			reportheartHeat();
//			reportPos(soc);
//			reportPowerState();
			for(i=0;i<15;i++)
			{
				 delay_ms(3000);
				 checkAckMsg();
				 if(systemStopState == 0)
				 {
				 	break;
				 }
	
		 	}
		}
				 
//		reportCan();	
//		reportPos();
//		reportPowerState();


		#ifdef 	PRINTF_DEBUG
		printf("\r\nϵͳ��ʱ��%d\r\n",SYSTEM_ON_TIME);
		#endif
		
		if(SYSTEM_ON_TIME < 20)
		{
			 time =30 - SYSTEM_ON_TIME;
			 for(i=0;i< time;i+=2)
			 {
				 delay_ms(2000);
				 checkAckMsg();
	
		 	}
		}
// 
			  
	}

}
int sysErrorHandle(QIDINGTONG_ERROR error)
{
	int i = 0;
	#ifdef PRINTF_DEBUG
	printf("\r\nError handle\r\n");
	#endif

	for(i=0;i<10;i++){

		LED1(Bit_SET);
		LED2(Bit_SET);
		LED3(Bit_SET);
		delay_ms(500);
		LED1(Bit_RESET);
		LED2(Bit_RESET);
		LED3(Bit_RESET);
		delay_ms(500);
	}
	printf("\r\nsystem error\r\n");	
	return 0;

}

void openPowerResumeInt(void)
{
//--------------------------------�򿪵�Դ�ָ��ж� ---------------------
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	#ifdef PRINTF_DEBUG
	printf("\r\n���õ�Դ�ָ��ж�\r\n");
	#endif

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
 	EXTI_ClearITPendingBit(EXTI_Line1);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//�½��ش����ж�
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);
  									 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void closePowerResumeInt(void)
{
//--------------------------------�򿪵�Դ�ָ��ж� ---------------------
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	#ifdef PRINTF_DEBUG
	printf("\r\n�رյ�Դ�ָ��ж�\r\n");
	#endif

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
 	EXTI_ClearITPendingBit(EXTI_Line1);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//�½��ش����ж�
  	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  	EXTI_Init(&EXTI_InitStructure);
  									 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//+IPD,30:*A10654568|222.66.15.67|12013#
void setIpPort(char *temp)
{
	int i;
	DATA_IN_FLASH  sysStateTemp;
	if(temp != NULL)
	{
		#ifdef PRINTF_DEBUG
		printf("\r\n�յ�������������%s\r\n",temp);
		#endif
		temp = temp + 2;//����*A
		memset(&sysStateTemp,'\0',sizeof(DATA_IN_FLASH));	
		for(i=0;;i++)
		{
			sysStateTemp.G_CODE[i] = *(temp);

			temp++;
			if((*temp)=='|' || (*temp)=='%')
				break;
		} 
		#ifdef PRINTF_DEBUG
		printf("\r\nG_CODE is :%s\r\n",sysStateTemp.G_CODE);
		#endif
		temp++;//��|
		for(i=0;;i++)
		{
			sysStateTemp.IP[i] = *(temp);

			temp++;
			if((*temp)=='|' || (*temp)=='%')
				break;
		}
		#ifdef PRINTF_DEBUG
		printf("\r\nIP is :%s\r\n",sysStateTemp.IP);
		#endif
		temp++;
		for(i=0;(*temp)!= '#';i++)
		{
			sysStateTemp.PORT[i] = *(temp);

			temp++;
		}
		#ifdef PRINTF_DEBUG
		printf("\r\nPORT is :%s\r\n",sysStateTemp.PORT);
		#endif


		memset(sysStateTemp.NET_CMD,'\0',60);
		strcat(sysStateTemp.NET_CMD,"AT+QIOPEN=\"UDP\",\"");
		strcat(sysStateTemp.NET_CMD,sysStateTemp.IP);
		strcat(sysStateTemp.NET_CMD,"\",\"");
		strcat(sysStateTemp.NET_CMD,sysStateTemp.PORT);
		strcat(sysStateTemp.NET_CMD,"\"\r");
		#ifdef PRINTF_DEBUG
		printf("\r\nNET_CMD is :%s\r\n",sysStateTemp.NET_CMD);
		#endif
		sysStateTemp.H_MODE = H_MODE;
		sysStateTemp.WORKING_MODE = WORKING_MODE;
		sysStateTemp.initFlag = 0;
//		writeFlashCfg(FLASH_ADR,(uint32_t *)&sysStateTemp);
		delay_ms(3000);
		#ifdef PRINTF_DEBUG
		printf("\r\n�޸Ĳ����ɹ���������������\r\n");
		#endif
		__disable_irq();//�����ж�
		__disable_fault_irq();
		delay_ms(2000);
		NVIC_SystemReset();	
	}
	else
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nû���յ�����������\r\n");
		#endif
		return;
	}  	

}



int powerDownCheck(void)
{
	int i=0;
	GPIO_InitTypeDef GPIO_InitStructure;
	if(power_sim900())	//���sim900������Ҫ�޸� 
	{
		if(read_voltage())
		{
			if(voltage > POWER_EMPTY)
			{
				
				#ifdef PRINTF_DEBUG
				printf("\r\n��ѹ��������������\r\n");
				#endif
				return 0;
			}
		}
		#ifdef PRINTF_DEBUG
		else//��ѹ��ȡʧ��
		{
			printf("\r\n�͵��Դ����ػ�\r\n");		
		}
		#endif  
	}
	else
		return sysErrorHandle(sim900StartError);
	#ifdef PRINTF_DEBUG
	printf("\r\n��ʱ�ȴ����¼���ѹ\r\n");
	#endif

	delay_ms(10000);

	if(power_sim900())	//���sim900������Ҫ�޸� 
	{
		if(read_voltage())
		{
			if(voltage > POWER_EMPTY)
			{
				#ifdef PRINTF_DEBUG
				printf("\r\n��ѹ����,��������\r\n");
				#endif
				return 0;
			}
		}
		#ifdef PRINTF_DEBUG
		else//��ѹ��ȡʧ��
		{
			printf("\r\n�͵��Դ����ػ�\r\n");		
		}
		#endif
	}
	
//	close_sys(); 
		
//	ENRECHARGE; //���Գ��
	#ifdef PRINTF_DEBUG
	printf("\r\n�͵�ѹ���ر�SIM900��GPS\r\n");
	#endif
	Send_AT_And_Wait("AT+CPOWD=1\r","OK",200);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_RESET);

	for(i =0;i<5;i++)
	{
		delay_ms(5000);
		#ifdef PRINTF_DEBUG
		printf("\r\n���͵�ػ���Ч��\r\n");
		#endif
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1))	//��������ʱ��ѹ���ȣ��豸��λ��������
		{//�����ϱ�������Ϊ20��
			#ifdef PRINTF_DEBUG
			printf("\r\n��⵽����źţ�ϵͳ��λ\r\n");
			#endif
			return 1;
//			__disable_irq();//�����ж�
//			__disable_fault_irq();
//			delay_ms(2000);
//			NVIC_SystemReset();	 			
		}
		#ifdef PRINTF_DEBUG	
		else
		{
			printf("\r\nδ��⵽����źţ���ʱ5����\r\n");
		}
		#endif	
	}
	if(i > 4)//���е���������ߺ���	�����Ƕ�ʱ����Ƿ�ָ�����
	{
		#ifdef PRINTF_DEBUG
		printf("\r\n��ѹ����ػ���ɣ������Դ�ָ����ģʽ\r\n");
		#endif
		close_sys();
		openPowerResumeInt();
		SYSTEM_PC = 9;	//ʹϵͳ���ֶ��ػ�
//		WORKING_MODE=0;
		return 0;

	}
	return 1;
}




void	watchDogInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//----------ι�������ʼ��---------	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
}
//void feedDog(void)
//{
//	static int side =0;
//	side = ~side;
//	return;
//
//	if(side)
//	{
//		GPIO_WriteBit(GPIOA,GPIO_Pin_7,Bit_SET);
//	}
//	else
//	{
//		GPIO_WriteBit(GPIOA,GPIO_Pin_7,Bit_RESET);
//	}
//}

void	openUsart1(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate            = 230400;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Tx|USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1, ENABLE); 
}
void	closeUsart1(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_Init(USART1, &USART_InitStructure);
	USART_InitStructure.USART_BaudRate            = 230400;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Tx|USART_Mode_Rx;
	
	USART_ITConfig(USART1, USART_IT_RXNE,DISABLE);
	USART_Cmd(USART1, ENABLE); 
}
void close_sys(void)
{
//	GREEN_OFF;
//	RED_OFF;
//	delay_ms(1000);
//	RED_ON;
//	delay_ms(2000);
//	RED_OFF;

}

void	checkDns(void)
{
	char *p;//������DNS��������
	int i;
	static int dnsFlag = 0;//���й�dns������
	static char dnsCmd[60];
	static char ip[16];
	#ifdef PRINTF_DEBUG
	printf("\r\nDNS�������ܼ��\r\n");
	#endif
	if(sysStateNow.IP_OR_URL==0)
	{
		#ifdef PRINTF_DEBUG
		printf("\r\n����ҪDNS��������\r\n");
		#endif
		return;
	}
	if(dnsFlag)
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nDNS�Ѿ�����ͨ���������ٴν���\r\n");
		#endif
		return;	 //�����������ٽ���
	}
	checkDnsConnect:
	#ifdef PRINTF_DEBUG
	printf("\r\n��ҪDNS��������\r\n");
	#endif
	if(power_sim900())//�ɹ�����SIM900
	{
	//	Send_AT_And_Wait("ATE0\r","OK",300);
		Send_AT_And_Wait("AT+CREG=0\r","OK",200);
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
			goto checkDnsConnect;

		}
		else  //����ע��ɹ�
		{
			// read_cellid();
			// read_cimi();
			read_csq();
		}
	}
	delay_ms(4000);
	
	memset(dnsCmd,'\0',60);
	strcat(dnsCmd,"AT+CDNSGIP=\"");
	strcat(dnsCmd,sysStateNow.URL);
	strcat(dnsCmd,"\"\r");

	Send_AT_And_Wait("AT+CSTT\r","OK",1000);
	Send_AT_And_Wait("AT+CIICR\r","OK",1000);
	Send_AT_And_Wait("AT+CIFSR\r","OK",1000);
	for(i = 0;i<10;i++)
	{
		if(Send_AT_And_Wait(dnsCmd,"OK",1000))
		{
			break;
		}
		if(i>9)
		{
			goto checkDnsConnect;
		}
	}
//	Send_AT_And_Wait(dnsCmd,"OK",6000);
	delay_ms(5000);
	if((p = strstr(USART2_RX_buffer,"+CDNSGIP:")) == NULL)//
	{
		printf("\r\npointer is NULL\r\n");
		dnsFlag = 0;
		goto checkDnsConnect;
	}
	else
	{
		printf("\r\nBuffer data is :%s\r\n",p);
		p = strlen(sysStateNow.URL)	+ 16 + p;
		memset(ip,'\0',16);
		for(i=0; *(p+i) != '"';i++)
		{
			ip[i] = *(p+i);
		}
		#ifdef PRINTF_DEBUG	
		printf("\r\n%s ---->IP:%s\r\n",sysStateNow.URL,ip);
		#endif
		strcat(sysStateNow.NET_CMD,"AT+CIPSTART=\"TCP\",\"");
		strcat(sysStateNow.NET_CMD,ip);
		strcat(sysStateNow.NET_CMD,"\",\"");
		strcat(sysStateNow.NET_CMD,sysStateNow.PORT);
		strcat(sysStateNow.NET_CMD,"\"\r");
		dnsFlag = 1;
	}
	Send_AT_And_Wait("AT+CIPSHUT\r","OK",1000);	  //�ر�����  			
	Send_AT_And_Wait("AT+CIPSTATUS\r","OK",1000);
	Send_AT_And_Wait("AT+CIPCLOSE\r","OK",500);	  //�ر�����
}

