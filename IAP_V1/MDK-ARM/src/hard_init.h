#include "sysdef.h"
#ifndef		HARD_INIT_H
#define		HARD_INIT_H

typedef enum 
{
	sim900StartError,//sim900����ʧ��
	sim900NotInsert,// sim900δ���뿨Ƭ
	sim900RegisterFail,//sim900����ע��ʧ��,������������
	sim900NetWeak,  //sim900����̫��
	sim900SendFailed,//sim900��������ʧ�� 
	gpsStartError,//gps���ϻ���gps�������

}QIDINGTONG_ERROR;

void feedDog(void);
void delay_ms(unsigned int time);
void hardware_stop(void);
void open_power_button(void);
void close_power_button(void);


void hardware_init(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void USART_Configuration(void);
void EXTI_Configuration(void);
void NVIC_Configuration(void);
void RTC_Configuration(void);
void TIM_Configuration(void);

void open_sys(void);
void close_sys(void);


void open_power_button(void); 
void close_power_button(void);


void open_shuiyin(void);
void close_shuiyin(void);




void GPIO_POWER_SAVING(void);
void stop_notime(void);
void stop_time(u32 time); 
void	setUsart1(int baud,int it,int enable);


void setLedOn(void);
																		   


int sysErrorHandle(QIDINGTONG_ERROR error);
void DATA_TO_SIM900(void);

void openPowerResumeInt(void);
void closePowerResumeInt(void);

void setIpPort(char *temp);



int powerDownCheck(void);
void	openUsart1(void);
void	closeUsart1(void);
void	gpsTest(void);

void	feedDog(void);
void	watchDogInit(void);
void	setReport(int echo);
void	checkDns(void);


#endif
