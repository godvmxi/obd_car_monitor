
#include "hard_init.h"


void delay_ms(unsigned int time)
{
	unsigned int x,y;
//	feedDog();
	for(x=0;x<time;x++)
		for(y=0;y<9000;y++);	
}

//void RCC_Configuration(void)
//{
//	ErrorStatus HSEStartUpStatus;
//	//将外设 RCC寄存器重设为缺省值
//	RCC_DeInit();
//	
//
//    //使能外部晶振
//    RCC_HSEConfig(RCC_HSE_ON);
//    //等待外部晶振稳定
//    HSEStartUpStatus = RCC_WaitForHSEStartUp();
//    //如果外部晶振启动成功，则进行下一步操作
//    if(HSEStartUpStatus==SUCCESS)
//    {
//        //设置HCLK（AHB时钟）=SYSCLK
//        RCC_HCLKConfig(RCC_SYSCLK_Div1);
//
//        //PCLK1(APB1) = HCLK/2
//        RCC_PCLK1Config(RCC_HCLK_Div2);
//
//        //PCLK2(APB2) = HCLK
//        RCC_PCLK2Config(RCC_HCLK_Div1);
//
//        //FLASH时序控制
//        //推荐值：SYSCLK = 0~24MHz   Latency=0
//        //        SYSCLK = 24~48MHz  Latency=1
//        //        SYSCLK = 48~72MHz  Latency=2
//        FLASH_SetLatency(FLASH_Latency_2);
//        //开启FLASH预取指功能
//        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
//
//        //PLL设置 SYSCLK/1 * 9 = 8*1*9 = 72MHz
//        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
//        //启动PLL
//        RCC_PLLCmd(ENABLE);
//        //等待PLL稳定
//        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
//        //系统时钟SYSCLK来自PLL输出
//        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
//        //切换时钟后等待系统时钟稳定
//        while(RCC_GetSYSCLKSource()!=0x08);
//
//
//        /*
//        //设置系统SYSCLK时钟为HSE输入
//        RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);
//        //等待时钟切换成功
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


void RCC_Configuration(void) //内部
{

//将外设 RCC寄存器重设为缺省值
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


//设置 PLL 时钟源及倍频系数
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);
//使能或者失能 PLL,这个参数可以取：ENABLE或者DISABLE
	RCC_PLLCmd(ENABLE);//如果PLL被用于系统时钟,那么它不能被失能
//等待指定的 RCC 标志位设置成功 等待PLL初始化成功
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	{
	}
//设置系统时钟（SYSCLK） 设置PLL为系统时钟源


	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 
//等待PLL成功用作于系统时钟的时钟源
// 0x00：HSI 作为系统时钟
// 0x04：HSE作为系统时钟
// 0x08：PLL作为系统时钟
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
							   RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB1Periph_CAN1, ENABLE );
							   
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
		LED1				PB12							输出				
		LED2				PB13							输出
		LED3				PB14							输出


KEY
		KEY1				PB15
//		KEY2				PC1

//
//WATCH_DOG
//		DOG					PB13
		
SIM
		TX_UART5			PC12
		RX_UART5			PD2

		SIM_EN				PB9
		SIM_RST				PB4
		SIM_DTR				PB3

GPS
		TX_UART3			PB10
		RX_UART3			PB11

		GPS_EN				PB1

OBD
		TX_UART4			PC10	
		RX_UART4			PC11

		OBD_EN				PB0

		CAN_ETR				PA0
//
//		OBD_RST			  	PB15
//		OBD_NVM				PB14

		
						 
BLUE_TOOCH
		TX_UART4			PC10	
		RX_UART4			PC11

		BLUE_EN				PA14
		BLUE_STATE			PB5	
		BLUE_CLR			PB6	

		BLUE_MS				PB8
		BLUE_HSW			PB7

FLASH
		FLASH_SI		  	PC9
		FLASH_SO		  	PA8
		FLASH_SCK		  	PC8
		FLASH_RST			PC7
		FLASH_CS			PC6

USB_ID  
		USB_ID				PA13
POW
		EN_MP3410			PA1
		PW_CHECK			PA4
		BAT_CHECK			PC0
		CHARGE_STATE		PC1
		EN_CHARGE			PC2
		EN_POW5				PC3




******************************************************************************/
	
		
	
//-----------------------------设置所有引脚为模拟输入———省电---------	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
//-----------------------串口引脚初始化---------------------------------
	
	//初始化串口1引脚
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);				


	//初始化串口2引脚
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//初始化串口3引脚
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

////---------------can 端口初始化---------------------------
//	/*PA11-CAN RX */
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	/*PA12-CAN TX */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
//	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOA, &GPIO_InitStructure); 


	/*PA12-CAN TX */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
//	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	//-----------------------------------LED 输出设置---------------------------------
/*
	
		LED1				PB11							输出				 
		LED2				PB12							输出
		LED3				PB13							输出
*/						

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_SET);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_SET);

/**************************KEY*******************************************************
		KEY1				PB15
//		KEY2				PC1
************************************************************************************/

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
////	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
////	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOC,&GPIO_InitStructure);
/*******************************************************************************
		DOG					PB13
***********************************************************************************/
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
/**************************SIM*******************************************************

		SIM_EN				PB9
		SIM_RST				PB4				OD
		SIM_DTR				PB3
**************************************************************************/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOB,GPIO_Pin_9,Bit_SET);

	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_SET);
//

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOB,GPIO_Pin_3,Bit_RESET);

/**************************GPS*******************************************************
		GPS_EN				PB1


************************************************************************************/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_RESET);

/**************************OBD*******************************************************
		OBD_EN				PB0

//		OBD_RST				PB15
//		OBD_NVM				PB14
		CAN_ETR				PA0

************************************************************************************/

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);

	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET); 
//	
//		
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET); 		
						 
/**************************BLUE_TOOCH*******************************************************

		BLUE_EN				PA14
		BLUE_STATE			PB5	
		BLUE_CLR			PB6	

		BLUE_MS				PB8
		BLUE_HSW			PB7
************************************************************************************/
//
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOA,GPIO_Pin_14,Bit_RESET);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	  //输入
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

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOB,GPIO_Pin_7,Bit_RESET);




/***************************************************
USB_ID  
		USB_ID				PA13
***************************************************/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOA,GPIO_Pin_13,Bit_RESET);

/***************************************************
FLASH
		FLASH_SI		  	PC9
		FLASH_SO		  	PA8
		FLASH_SCK		  	PC8
		FLASH_RST			PC7
		FLASH_CS			PC6
***************************************************/
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

/****************************************************
POW
		EN_MP3410			PA1
		PW_CHECK			PA4
		BAT_CHECK			PC0
		CHARGE_STATE		PC1
		EN_CHARGE			PC2
		EN_POW5				PC3

*****************************************************/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET); 
 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
;
	
	//ADC12_IN10
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
 
 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOC,GPIO_Pin_1,Bit_RESET);  
 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOC,GPIO_Pin_2,Bit_RESET);  
 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOC,GPIO_Pin_3,Bit_RESET);   				
}


void USART_Configuration(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;


/***************************************************************
		USART1				ISP					256000
		USART2				OBD					38400
		USART3				GPS					9600
		USART4			   	BLUE			 	38400
		USART5				SIM			   		115200 
***************************************************************/
   
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



  /***************************USART2**********OBD*********************************/
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

  USART_InitStructure.USART_BaudRate = 38400;
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

  /***************************USART3*********GPS**********************************/
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600;
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

	USART_InitStructure.USART_BaudRate = 38400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(UART4, &USART_InitStructure); 
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	//  USART_ITConfig(UART4, USART_IT_TXE, ENABLE);
	USART_Cmd(UART4, ENABLE);

  /***************************USART5***************SIM****************************/
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

  USART_InitStructure.USART_BaudRate = 115200;
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
	//--------------------------------RTC闹钟中断设置---------------------------------
 	EXTI_ClearITPendingBit(EXTI_Line17);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	
	//--------------------------------外部key1中断----PA0-----------------------------	
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
//
// 	EXTI_ClearITPendingBit(EXTI_Line0);
//  	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//上升沿触发中断
//  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  	EXTI_Init(&EXTI_InitStructure);	
	//--------------------------------外部key1中断----PA1-----------------------------
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
//
// 	EXTI_ClearITPendingBit(EXTI_Line1);
//  	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//上升沿触发中断
//  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  	EXTI_Init(&EXTI_InitStructure);	
	
	//--------------------------------蓝牙状态中断--------PE3-------------------------	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);	
 	EXTI_ClearITPendingBit(EXTI_Line3);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	//上升下降沿触发中断
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	

//can exit test
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);	 
 	EXTI_ClearITPendingBit(EXTI_Line11);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//下降沿触发中断
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);
	
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12);	 
// 	EXTI_ClearITPendingBit(EXTI_Line12);
//  	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	//下降沿触发中断
//  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  	EXTI_Init(&EXTI_InitStructure);		



}

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	#ifdef IAP_FUNCTION
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,NVIC_VectTab_FLASH_ADDR); 
  	#endif
//--------------------------------打开串口中断 ---------------------
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


//---------------------------------RTC闹钟--------------------------------
//  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//
//  	NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
//  	NVIC_Init(&NVIC_InitStructure);


//*************************KEY1*******************************************************************
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;  
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);

//*************************KEY2*******************************************************************
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;  
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);
//*************************蓝牙状态***************************************************************
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;  
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);
 
//-----------------------------------定时器中断------------------------------------
	/* Timer2中断*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Timer3中断*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

//---------------------Can 总线-------------------------------------------
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

//can modify exit modify
	NVIC_InitStructure.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	


} 


void stop_time(u32 time)
{
//		if(SYSTEM_PC_BAK == 7)
//		{ 
//			#ifdef PRINTF_DEBUG
//			printf("\r\n系统即将关机\r\nThe SYSTEM is %d\r\n",SYSTEM_PC);
//			#endif
//			return;
//		}
//		GPIO_POWER_SAVING();//设置所有引脚为模拟输入——省电

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
//			printf("\r\n系统即将关机\r\nThe SYSTEM is %d\r\n",SYSTEM_PC);
//			#endif
//			return;
//		}	
        PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);

}

void stop_notime(void)
{

//		GPIO_POWER_SAVING();//设置所有引脚为模拟输入——省电


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
 /************************************************定时器2设置********************************************************/
	/* 基础设置*/
//	TIM_TimeBaseStructure.TIM_Period = 20000;		//计数值   
//	TIM_TimeBaseStructure.TIM_Prescaler = 800;    	//预分频
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  	
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
//
//	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;		//计数值   
	TIM_TimeBaseStructure.TIM_Prescaler = 4;    	//预分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;  	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ITRxExternalClockConfig(TIM2,TIM_TS_ETRF);
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
//	TIM_ETRClockMode1Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
	
//	/*使能预装载*/
//	TIM_ARRPreloadConfig(TIM2, ENABLE);
//	/*预先清除所有中断位*/
//	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);	
//	/* 4个通道和溢出都配置中断*/
//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//	TIM_SetCounter(TIM2, 65530);
	/* 允许TIM2开始计数 */
	TIM_Cmd(TIM2, ENABLE);

 /************************************************定时器3设置********************************************************/
	/* 基础设置*/
	TIM_TimeBaseStructure.TIM_Period = 10000;		//计数值   			1s
	TIM_TimeBaseStructure.TIM_Prescaler = 7200;    	//预分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	/*使能预装载*/
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	/*预先清除所有中断位*/
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);	
	/*溢出都配置中断*/
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	/* 允许TIM2开始计数 */
	TIM_Cmd(TIM3, ENABLE);
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
	CAN_InitStructure.CAN_TTCM=ENABLE;		//时间触发
	CAN_InitStructure.CAN_ABOM=ENABLE;		//自动离线管理
	CAN_InitStructure.CAN_AWUM=ENABLE;		//自动唤醒
	CAN_InitStructure.CAN_NART=ENABLE;		//ENABLE:错误不自动重传 DISABLE:重传
	CAN_InitStructure.CAN_RFLM=ENABLE;
	CAN_InitStructure.CAN_TXFP=ENABLE;
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;		//正常传输模式
	CAN_InitStructure.CAN_SJW=CAN_SJW_4tq;			//1-4
	CAN_InitStructure.CAN_BS1=CAN_BS1_9tq;			//1-16
	CAN_InitStructure.CAN_BS2=CAN_BS2_6tq;			//1-8
	CAN_InitStructure.CAN_Prescaler=8;				//波特率为 8M/(4*(1+1+2))=500k
	CAN_Init(CAN1,&CAN_InitStructure);
	
	/* CAN 过滤器设置 */
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
	
	/* 允许FMP0中断   FIFO消息挂号*/ 
	CAN_ITConfig(CAN1,CAN_IT_FMP0|CAN_IT_FOV0|CAN_IT_FMP1|CAN_IT_FOV1, ENABLE);
}

void hardware_init(void)
{	
	RCC_Configuration();
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);		   //禁止JTAG
	GPIO_Configuration();

	USART_Configuration();
//RTC_Configuration(); 	
	EXTI_Configuration();
//	CAN_Interrupt();
	NVIC_Configuration();
	

	rtcConfig();


	TIM_Configuration();
	initAT45DB041B();
}

void open_sys(void)	  //LED2没有
{

}


void open_power_button(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
 	EXTI_ClearITPendingBit(EXTI_Line0);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//下降沿触发中断
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
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//下降沿触发中断
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
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	//下降沿触发中断
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
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	//下降沿触发中断
  	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  	EXTI_Init(&EXTI_InitStructure);

}

int sysErrorHandle(int error)
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
//--------------------------------打开电源恢复中断 ---------------------
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	#ifdef PRINTF_DEBUG
	printf("\r\n设置电源恢复中断\r\n");
	#endif

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
 	EXTI_ClearITPendingBit(EXTI_Line1);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//下降沿触发中断
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
//--------------------------------打开电源恢复中断 ---------------------
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	#ifdef PRINTF_DEBUG
	printf("\r\n关闭电源恢复中断\r\n");
	#endif

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
 	EXTI_ClearITPendingBit(EXTI_Line1);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//下降沿触发中断
  	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  	EXTI_Init(&EXTI_InitStructure);
  									 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);
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
	USART_InitStructure.USART_BaudRate            = 256000;
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
	USART_InitStructure.USART_BaudRate            = 256000;
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
void enableCanExti(int32_t mode){
	NVIC_InitTypeDef NVIC_InitStructure; 

//--------------------------------打开串口中断 ---------------------
	if(mode == 0)
		NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	else
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	
  	NVIC_Init(&NVIC_InitStructure);

}


