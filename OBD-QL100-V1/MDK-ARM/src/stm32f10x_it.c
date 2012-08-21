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



extern int	    GPS_GGA_DINGWEI_OK;
extern int	    GPS_RMC_DINGWEI_OK;

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
	while(1);
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
		while(1);
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
	while(1);
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
		while(1);

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
		while(1);

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
		while(1);

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
		while(1);

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
		while(1);

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

/***************************************************************
		USART1				ISP					256000
		USART2				OBD					38400
		USART3				GPS					9600
		USART4			   	BLUE			 	38400
		USART5				SIM			   		115200 
***************************************************************/


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
		ISP_BUF[ISP_COUNT++]=temp;
		ISP_BUF[ISP_COUNT]='\0';

		if(ISP_COUNT>1300){
			ISP_COUNT=0;
		}			
	} 
	USART_ClearFlag(USART1,USART_FLAG_RXNE);
}


void USART2_IRQHandler(void)   
{
//OBD
	static char temp;
	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
    {	
		USART_ClearFlag(USART2,USART_FLAG_ORE);				
        USART_ReceiveData(USART2);		
    }
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{ 
		temp=USART_ReceiveData(USART2);
		
		
//OBD BEGIN	
		if(ISP_DIRECTION==USART_OBD)
		{
			USART_SendData(ISP,temp);	
			while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);			
		} 
		OBD_BUF[OBD_COUNT++]=temp;
		OBD_BUF[OBD_COUNT]='\0';  
		if(OBD_COUNT>300)
			OBD_COUNT=0;
		if(BLUE_OBD){//蓝牙和OBD直通//转发给BLUE
//			USART_SendData(ISP,'X');
			USART_SendData(BLUE,temp);
			while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);	
//			USART_SendData(ISP,temp);
//			while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);
	
		}
//OBD END
				
		USART_ClearFlag(USART2,USART_FLAG_RXNE); 
	} 	 
}

void USART3_IRQHandler(void) 
{
//GPS
	char temp;	
	if (USART_GetFlagStatus(USART3,USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
    {
		USART_ClearFlag(USART3,USART_FLAG_ORE);	
        USART_ReceiveData(USART3);	 		
    }
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{ 
		temp=USART_ReceiveData(USART3);


//GPS START
		gpsDataIrq(temp);	
					
		if(ISP_DIRECTION==USART_GPS)
		{
			USART_SendData(USART1,temp);	
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);			
		}
//GPS END
		
	}
	USART_ClearFlag(USART3,USART_FLAG_RXNE);
}
void UART4_IRQHandler(void){ 
//BLUE
	char temp;
	if (USART_GetFlagStatus(UART4, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
    {
		USART_ClearFlag(UART4,USART_FLAG_ORE);	
        USART_ReceiveData(UART4);			
    }
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{ 								
		temp = USART_ReceiveData(UART4);
//BLUE START
		BLUE_BUF[BLUE_COUNT++]=temp;
		BLUE_BUF[BLUE_COUNT]='\0';
		if(BLUE_COUNT>300)
			BLUE_COUNT=0; 

		if(ISP_DIRECTION==USART_BLUE)
		{
			USART_SendData(ISP,temp);	
//			while(USART_GetFlagStatus(ISP,USART_FLAG_TC)==RESET);			
		} 
		if(BLUE_OBD){//转发给OBD

			USART_SendData(OBD,temp);
				
//			while(USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET);	  
		}
//BLUE END			
	} 
	USART_ClearFlag(UART4,USART_FLAG_RXNE);
}

void UART5_IRQHandler(void){
//SIM
	char temp;
	if (USART_GetFlagStatus(UART5, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
    {
		USART_ClearFlag(UART5,USART_FLAG_ORE);	
        USART_ReceiveData(UART5);			
    }
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{ 
							
		temp = USART_ReceiveData(UART5);



//SIM BEGIN		
//		simDataIrq(temp);
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
//SIM END				
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
void EXTI15_10_IRQHandler(void)
{	
	static int i = 0;
	if ( EXTI_GetITStatus(EXTI_Line11) != RESET ) 
	{
		EXTI_ClearITPendingBit(EXTI_Line11);	
//		if(i%10000 == 0) {
//		#ifdef PRINTF_DEBUG
//		printf("\r\nline 11\r\n");
//		#endif
//		}	
		CAN_DETECTER++;
	}
	if ( EXTI_GetITStatus(EXTI_Line12) != RESET ) 
	{
		EXTI_ClearITPendingBit(EXTI_Line12);	
//		if(i%10000 == 0) {
//		#ifdef PRINTF_DEBUG
//		printf("\r\nline12\r\n");
//		#endif	
//		}
		CAN_DETECTER++;
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
//	rtc();							   
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Reset RTC Counter when Time is 23:59:59 */
    if (RTC_GetCounter() == 0x00015180)    {
      RTC_SetCounter(0x0);
      /* Wait until last write operation on RTC registers has finished */
      RTC_WaitForLastTask();
    }
  }
}


void TIM2_IRQHandler(void)
{
	 
	 if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	 
//		TIM_Cmd(TIM2, DISABLE);
		TIM2->CNT=65530;  
	}
}


void TIM3_IRQHandler(void)
{		
	static int state = 0,counter = 0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{ 

		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		if(OBD_START){
			obdIrq();
		}
		//CHECK CAR STATE START		
		if(counter == 3)
		{//enable can exti
		//	printf("\r\nenable can exti\r\n");
		//	enableCanExti(1);
			TIM_SetCounter(TIM2, 0);
			CAN_DETECTER = 0;
		}
		if(counter > 3 )
		{
			CAN_DETECTER = TIM_GetCounter(TIM2);
			DEVICE_STATE = CAN_DETECTER > 2000? 1 :0;
		//	printf("\r\nDEVICE STATE--->%d -->%d\r\n",DEVICE_STATE,CAN_DETECTER);	////0:STOP 1:RUNNING
			counter = 0;	; 
		}
		counter++;
		//CHECK CAR STATE END
		if(state == 0){
			LED1(Bit_SET);
			LED2(Bit_RESET);
			LED3(Bit_RESET);
			state = 1;
		}
		else{
			LED1(Bit_RESET);
			LED2(Bit_SET);
			LED3(Bit_SET);
			state = 0;
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
		//CanBuffer(&RxMessage);
		printf("\r\nCAN ok!!\r\n");


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
//		blueToothCounter++;	
	}
}





