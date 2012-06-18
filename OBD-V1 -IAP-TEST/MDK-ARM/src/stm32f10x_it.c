#include "sysdef.h"



int gps_data_value=0;//判断GPS是否开启，串口3每次接收中断将其置1

//char USART3_temp,USART2_temp;

//char USART1_RX_buffer[300];	 //串口2接收缓存
//char USART2_RX_buffer[300];	 //串口2接收缓存
//char USART3_RX_buffer[300];	 //串口2接收缓存
//char UART4_RX_buffer[300];	 //串口2接收缓存
//char UART5_RX_buffer[300];	 //串口2接收缓存
//
//
//unsigned int USART1_RX_buffer_count=0; //串口1接收缓存位置变量
//unsigned int USART2_RX_buffer_count=0; //串口2接收缓存位置变量
//unsigned int USART3_RX_buffer_count=0; //串口2接收缓存位置变量
//unsigned int UART4_RX_buffer_count=0; //串口2接收缓存位置变量
//unsigned int UART5_RX_buffer_count=0; //串口2接收缓存位置变量


int USART3_DIRECTION=1;
extern int SYSTEM_PC_BAK;


int callFlag = 0;//电话中断防外来脉冲，这里只有遇到两个上升沿，才会唤醒系统


extern unsigned int SYSTEM_ON_TIME;

int				SYSTEM_PC_ERROR = 0;

/* Private function prototypes -----------------------------------------------*/
//extern unsigned int shuiyin_count;
int				button_push_down=0;
extern int      shuiyingFlag;
extern int      voltage;
extern int	    GPS_GGA_DINGWEI_OK;
extern int	    GPS_RMC_DINGWEI_OK;
extern int      errorHandleState;//0表示系统正常工作，1，系统处于错误处理状态
volatile int    TIMER_COUNTER = 0;
volatile int	blueToothCounter = 0;
volatile int    feedDogCounter = 0;


extern int shuiyin_counter;




/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
	printf("\r\nERROR_NMI\r\n");
	__disable_irq();//关总中断
	__disable_fault_irq();
	delay_ms(2000);
	NVIC_SystemReset();

}






/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */

  while (1)
  {
  		printf("\r\nERROR_HARD_FAULT\r\n");
		__disable_irq();//关总中断
		__disable_fault_irq();
		delay_ms(2000);
		NVIC_SystemReset();
  }	


}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */

  while (1)
  {
	printf("\r\nERROR_MEM\r\n");
	__disable_irq();//关总中断
	__disable_fault_irq();
	delay_ms(2000);
	NVIC_SystemReset();
  }	


}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs  */

  while (1)
  {
  	printf("\r\nERROR_BUS\r\n");
	__disable_irq();//关总中断
	__disable_fault_irq();
	delay_ms(2000);
	NVIC_SystemReset();
  }	

}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */

  while (1)
  {
  	printf("\r\nERROR_USAGE\r\n");
	__disable_irq();//关总中断
	__disable_fault_irq();
	delay_ms(2000);
	NVIC_SystemReset();
  }	   


}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
	printf("\r\nERROR_SVC\r\n");
	__disable_irq();//关总中断
	__disable_fault_irq();
	delay_ms(2000);
	NVIC_SystemReset();
}

/**
  * @brief  This function handles PRINTF_DEBUG Monitor exception.
  * @param  None
  * @retval None
  */
void PRINTF_DEBUGMon_Handler(void)
{
	printf("\r\nERROR_PRINTF_DEBUG\r\n");
	__disable_irq();//关总中断
	__disable_fault_irq();
	delay_ms(2000);
	NVIC_SystemReset();
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
	printf("\r\nERROR_PENDSV\r\n");
	__disable_irq();//关总中断
	__disable_fault_irq();
	delay_ms(2000);
	NVIC_SystemReset();
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */


void SysTick_Handler(void)
{  

}


void USART1_IRQHandler(void) //ISP
{
	static char temp;
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
    {
		USART_ClearFlag(USART1,USART_FLAG_ORE);	
        USART_ReceiveData(USART1);			
    }

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 
							
		temp = USART_ReceiveData(USART1);

//		USART_SendData(ISP,temp);	
//		while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);

		ISP_BUF[ISP_COUNT++]=temp;
//		ISP_BUF[ISP_COUNT]='\0';

		if(ISP_COUNT>1300){
			ISP_COUNT=0;
		} 

//		USART_SendData(OBD,temp);	
//		while(USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET);
			
	} 	 

	USART_ClearFlag(USART1,USART_FLAG_RXNE);
}


void USART2_IRQHandler(void)   //SIM
{
	static char temp;
	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
    {	
		USART_ClearFlag(USART2,USART_FLAG_ORE);				
        USART_ReceiveData(USART2);		
    }
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{ 
		temp=USART_ReceiveData(USART2);


		#ifdef PRINTF_DEBUG
		if(ISP_DIRECTION==USART_SIM)
		{
			USART_SendData(USART1,temp);	
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 			
		}
		#endif
		

		SIM_BUF[SIM_COUNT++]=temp;
		SIM_BUF[SIM_COUNT]='\0';

		if(SIM_COUNT>1300){
			SIM_COUNT=0;
		} 
	
		
		USART_ClearFlag(USART2,USART_FLAG_RXNE);

	} 

	 
}

void USART3_IRQHandler(void)
{
	char temp;
	gps_data_value=1;

												
	if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
    {
		USART_ClearFlag(USART3,USART_FLAG_ORE);	
        USART_ReceiveData(USART3);	 		
    }

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{ 
			

		temp=USART_ReceiveData(USART3);

		if(ISP_DIRECTION==USART_OBD)
		{
			USART_SendData(ISP,temp);	
			while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);			
		}

		OBD_BUF[OBD_COUNT++]=temp;
		OBD_BUF[OBD_COUNT]='\0';

		if(OBD_COUNT>300){

			OBD_COUNT=0;
		} 

//		if(BLUR_OBD){//蓝牙和OBD直通
//			//转发给OBD
//			USART_SendData(OBD,temp);	
//			while(USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET);  
//			USART_ClearFlag(BLUE,USART_FLAG_RXNE);
//			return;		
//		}
//		
		
		

		USART_ClearFlag(USART3,USART_FLAG_RXNE);

	} 

}
void UART4_IRQHandler(void){ //blue
	char temp;
	if (USART_GetFlagStatus(UART4, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
    {
		USART_ClearFlag(UART4,USART_FLAG_ORE);	
        USART_ReceiveData(UART4);			
    }

	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{ 
							
		temp = USART_ReceiveData(UART4);
		USART_SendData(OBD,temp);	
		while(USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET);

		BLUE_BUF[BLUE_COUNT++]=temp;
		
		

		BLUE_BUF[OBD_COUNT]='\0';

		if(BLUE_COUNT>300){

			BLUE_COUNT=0;
		}
		if(ISP_DIRECTION==USART_BLUE)
		{
			USART_SendData(ISP,temp);	
			while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);			
		}
		

//		if(BLUR_OBD){
//				//转发给OBD
//			USART_SendData(BLUE,temp);	
//			while(USART_GetFlagStatus(BLUE,USART_FLAG_TC)==RESET);
//			USART_ClearFlag(BLUE,USART_FLAG_RXNE);
//			return;			  
//		}
			
	} 	 

	USART_ClearFlag(USART1,USART_FLAG_RXNE);
}

void UART5_IRQHandler(void){

	char temp;
	if (USART_GetFlagStatus(UART5, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
    {
		USART_ClearFlag(UART5,USART_FLAG_ORE);	
        USART_ReceiveData(UART5);			
    }

	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{ 
							
		temp = USART_ReceiveData(UART5);
		gpsDataIrq(temp);	
					
		if(ISP_DIRECTION==USART_GPS)
		{
			USART_SendData(USART1,temp);	
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);			
		} 
			
	} 	 

	USART_ClearFlag(UART5,USART_FLAG_RXNE);
}


void EXTI0_IRQHandler(void)		   //PA0
{
//---------------------------------外部按钮中断---------------------------
	delay_ms(200);
	if ( EXTI_GetITStatus(EXTI_Line0) != RESET ) 
	{
		EXTI_ClearITPendingBit(EXTI_Line0);					
		#ifdef PRINTF_DEBUG
		printf("\r\nkey0\r\n");
		#endif 
		button_push_down=1;	
	}
	
}
void EXTI1_IRQHandler(void)	   //PA1
{
	if ( EXTI_GetITStatus(EXTI_Line1) != RESET ) 
	{
		EXTI_ClearITPendingBit(EXTI_Line1);	
		#ifdef PRINTF_DEBUG
		printf("\r\n进入中断1\r\n");
		#endif
	}
}
void EXTI3_IRQHandler(void)	//PE3
{
	if ( EXTI_GetITStatus(EXTI_Line3) != RESET ) 
	{
		EXTI_ClearITPendingBit(EXTI_Line3);	

		#ifdef PRINTF_DEBUG
		printf("\r\n进入蓝牙状态变化中断！！！\r\n");
		#endif
//		if(BLUE_STATE == 0){
//
//		 	printf("\r\n蓝牙电平---低\r\n");
//		}
//		else{
//			printf("\r\n蓝牙电平---低\r\n");
//		} 
	}
}

void RTCAlarm_IRQHandler(void)
{

  if(RTC_GetITStatus(RTC_IT_ALR) != RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line17);
	RTC_WaitForLastTask();   
	RTC_ClearITPendingBit(RTC_IT_ALR);
	RTC_WaitForLastTask();
  }
}
void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);

    /* Toggle LED1 */


    /* Enable time update */


    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Reset RTC Counter when Time is 23:59:59 */
    if (RTC_GetCounter() == 0x00015180)
    {
      RTC_SetCounter(0x0);
      /* Wait until last write operation on RTC registers has finished */
      RTC_WaitForLastTask();
    }
  }
}


void TIM2_IRQHandler(void)
{
	 static int i = 0;
	 if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		TIM_Cmd(TIM2, DISABLE);
		TIM2->CNT=0;
		printf("\r\ntimer 3\r\n");
		if(i == 0){
			LED1(Bit_RESET);
			LED2(Bit_RESET);
			
			i = 1;
		}
		else
		{
			LED1(Bit_SET);
			LED2(Bit_SET);

			i = 0;
		}
		

	}
}


void TIM3_IRQHandler(void)
{		
	 static int i = 0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		

		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		printf("\r\ntimer 3\r\n");
		if(i == 0){
			LED1(Bit_RESET);
			LED2(Bit_RESET);
			
			i = 1;
		}
		else
		{
			LED1(Bit_SET);
			LED2(Bit_SET);

			i = 0;
		}
	}


}



void USB_LP_CAN1_RX0_IRQHandler(void)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);

	if(CAN_GetITStatus(CAN1,CAN_IT_FOV0))
	{
		CanRxMsg RxMessage;
		#ifdef PRINTF_DEBUG
		printf("\r\nCAN error1!!\r\n");
		#endif
		CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);
		CAN_ClearITPendingBit(CAN1,CAN_IT_FOV0);
			
	}

	if(CAN_GetITStatus(CAN1,CAN_IT_FOV1))
	{
		CanRxMsg RxMessage;
		#ifdef PRINTF_DEBUG
		printf("\r\nCAN error2!!\r\n");
		#endif
		CAN_Receive(CAN1,CAN_FIFO1, &RxMessage);
		CAN_ClearITPendingBit(CAN1,CAN_IT_FOV1);
			
	}

	if(CAN_GetITStatus(CAN1, CAN_IT_FMP0))
	{
		CanRxMsg RxMessage;
	
		CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);
		fillCanBuffer(&RxMessage);
//		printf("\r\nCAN ok!!\r\n");


		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
	}

	if(CAN_GetITStatus(CAN1, CAN_IT_FMP1))
	{
		CanRxMsg RxMessage;
		#ifdef PRINTF_DEBUG
		printf("\r\nCAN error3!!\r\n");
		#endif
		CAN_Receive(CAN1,CAN_FIFO1, &RxMessage);



		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP1);
	}

	GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_SET);

}

void EXTI9_5_IRQHandler(void)
{
	if ( EXTI_GetITStatus(EXTI_Line8) != RESET ) 
	{
		EXTI_ClearITPendingBit(EXTI_Line8);	
//		#ifdef PRINTF_DEBUG
//		printf("\r\n进入蓝牙中断\r\n");
//		#endif
		blueToothCounter++;	
	}
}





