#include "sysdef.h"	 
//-------------------------------------------------------



int fputc(int ch, FILE *f);//printf



int main(void)
{	
//	uint8_t src[1300];
	int i = 0;
	hardware_init(); 
	printf("\r\nlight led\r\n");
	LED1(Bit_SET);
	LED2(Bit_SET);
	LED3(Bit_SET);

//	setDebugPara();

	printf("\r\n\r\n");
	printf("\r\nREAD SYSTEM CONFIG :%10X %10X\r\n",IAP_PARAMETER,SYS_PARAMETER);
	
	flashRead(IAP_PARAMETER,(uint8_t *)&sysCfg,sizeof(FLASH_CONFIG));
	showConfig(&sysCfg);
	flashRead(SYS_PARAMETER,(uint8_t *)&sysCfg,sizeof(FLASH_CONFIG));
	showConfig(&sysCfg); 
	massProduction();
		
	iapfunc();
//	delay_ms(3000);
	
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

