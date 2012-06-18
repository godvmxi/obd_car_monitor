#include "sysdef.h"


/*全局变量*/

char USART1_RX_buffer[1300];	 //串口1接收缓存
char USART2_RX_buffer[1300];	 //串口2接收缓存
char USART3_RX_buffer[300];	 //串口3接收缓存
char UART4_RX_buffer[300];	 //串口4接收缓存
char UART5_RX_buffer[300];	 //串口5接收缓存


unsigned int USART1_RX_buffer_count=0; //串口1接收缓存位置变量
unsigned int USART2_RX_buffer_count=0; //串口2接收缓存位置变量
unsigned int USART3_RX_buffer_count=0; //串口3接收缓存位置变量
unsigned int UART4_RX_buffer_count=0; //串口4接收缓存位置变量
unsigned int UART5_RX_buffer_count=0; //串口5接收缓存位置变量


int ISP_DIRECTION	= 0;	//ISP串口数据转发控制
int BLUE_DIRECTION  = 0;	//蓝牙串口数据转发控制


int	BLUE_OBD		= 0;

FLASH_CONFIG  sysCfg;







