#include "sysdef.h"


/*ȫ�ֱ���*/

char USART1_RX_buffer[1300];	 //����1���ջ���
char USART2_RX_buffer[1300];	 //����2���ջ���
char USART3_RX_buffer[300];	 //����3���ջ���
char UART4_RX_buffer[300];	 //����4���ջ���
char UART5_RX_buffer[300];	 //����5���ջ���


unsigned int USART1_RX_buffer_count=0; //����1���ջ���λ�ñ���
unsigned int USART2_RX_buffer_count=0; //����2���ջ���λ�ñ���
unsigned int USART3_RX_buffer_count=0; //����3���ջ���λ�ñ���
unsigned int UART4_RX_buffer_count=0; //����4���ջ���λ�ñ���
unsigned int UART5_RX_buffer_count=0; //����5���ջ���λ�ñ���


int ISP_DIRECTION	= 0;	//ISP��������ת������
int BLUE_DIRECTION  = 0;	//������������ת������


int	BLUE_OBD		= 0;

FLASH_CONFIG  sysCfg;







