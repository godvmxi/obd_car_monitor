#include "blueTooth.h"


/*******************************************************************************
 * 函数名称:blueAtAndWait                                                                     
 * 描    述:蓝牙AT命令执行                                                                     
 *                                                                               
 * 输    入:at命令串，返回值查询关键字，命令执行等待时间                                                                    
 * 输    出:无                                                                     
 * 返    回:0：命令未出现期望结果，1：命令出现期望结果                                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2011年7月13日                                                                    
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
 * 函数名称:blueToothPower                                                                     
 * 描    述:设置蓝牙电源                                                                     
 *                                                                               
 * 输    入:0：关闭  1：启动                                                                     
 * 输    出:无                                                                    
 * 返    回:命令执行状态                                                                     
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月1日                                                                    
 *******************************************************************************/
int blueToothPower(int state){

	int i=0;
	if(state == 0){
		printf("\r\n关闭蓝牙电源\r\n");
		BLUE_EN(Bit_RESET);
		return 1;
	}
	else{
		printf("\r\n启动蓝牙电源\r\n");
		BLUE_EN(Bit_SET);
		delay_ms(2000);
		for(i=0;i<5;i++){
			if(blueAtAndWait("AT\r","OK",1000)){//执行成功
				printf("\r\n蓝牙启动成功\r\n");
				return 1;
			}
		}
		
		printf("\r蓝牙启动失败\n");
		return 0;		
	}
}
/*******************************************************************************
 * 函数名称:initBlueTooth                                                                     
 * 描    述:设置蓝牙主从模式                                                                   
 *                                                                               
 * 输    入:0 主模式 1 从模式                                                                    
 * 输    出:无                                                                     
 * 返    回:0 失败 1 成功                                                                     
 * 作    者:蛋蛋                                                                     
 * 修改日期:2011年7月13日                                                                    
 *******************************************************************************/
int initBlueTooth(int mode){

	printf("\r\nset bluetooth mode\r\n");
	return 1;
}

/*******************************************************************************
 * 函数名称:blueDma                                                                     
 * 描    述:启动蓝牙DMA传输                                                                   
 *                                                                               
 * 输    入:DMA地址，DMA通道，DMA启动状态                                                                     
 * 输    出:无                                                                     
 * 返    回:无                                                                     
 * 作    者:蛋蛋                                                                     
 * 修改日期:2011年7月13日                                                                    
 *******************************************************************************/
int blueDma(char *add,int channel,int state){

	return 1;
}



/*******************************************************************************
 * 函数名称:blueTest                                                                     
 * 描    述:蓝牙测试程序                                                                  
 *                                                                               
 * 输    入:DMA地址，DMA通道，DMA启动状态                                                                     
 * 输    出:无                                                                     
 * 返    回:无                                                                     
 * 作    者:蛋蛋                                                                     
 * 修改日期:2011年7月13日                                                                    
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



