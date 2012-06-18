#include "sysdef.h"
#ifndef SIM900_H_H
#define SIM900_H_H

//typedef struct {
//	char imei[18]; //
//	char csq[3];
//	char cimi[6];
//	char oldCellid[9];
//	char newCellid[9];
//	int  voltage;
//	int	 serverType;//服务供应商
//
//}SIM_STRUCT;
//typedef struct {
//	int16_t	crc;
//	int16_t	flag;
//	int8_t	ipUrl[40];
//	int32_t udpTcp;
//	uint32_t socketNum;
//	int32_t localPort;
//	int32_t	desPort;
//	uint8_t reserved[20]; 
//
//}SOCKET;

//SIM




void sim900_boot(void);
void sim900Reboot(void);
int power_sim900(void);
void sim900_power_on(void);
void sim900_power_off(int type);

void sim900_sleep(void);
void sim900_wakeup(void);


void open_sim900_int(void);
void close_sim900_int(void);

int Send_AT_And_Wait(char *cmd,char *right_return,unsigned int delay_time);

int read_voltage(void);
int read_cimi(void) ;
int read_cellid(void);
int read_csq(void);

void massProduction(void);

void sendSMS(char *msg);
void sim900Test(void);
int setSimApn(int type);

/************************************AT CMD*******************************************\
建立或断开,设置VPN
AT +MIPCALL=<Operation>[<APN>,[<User name>,<Password>]] 
AT+MIPCAL=1,"cmnet","",""\r
AT+MIPCAL=1,\"cmnet\"\r
AT+MIPCAL?	  query
AT+MIPCAL=?	  show

建立连接
AT+MIPOPEN=<Socket_ID>,<Source_Port>,<Remote_IP>,<Remote_Port>,<Protocol> 
<Remote_Port> Port of source site. 
Port range: 0-65535 (decimal digits) 
Port 0 for incoming connection. 
<Protocol> Type of protocol stack. 
0 TCP 
1 UDP 
<State>   
0 Inactive 
1 Active 
2 SSL secured 
AT+MIPOPEN?\r
AT+MIPOPEN=1,4000,\"www.baidu.com\",80,0\r
T+MIPOPEN?\r
AT+MIPOPEN=?\r


关闭连接
AT+MIPCLOSE?
AT+MIPCLOSE:<Socket_ID>
AT+MIPCLOSE=? 



连接属性设置
AT+MIPSETS=<Socket_ID>,<Size>[,<Timeout>] 
	Response: 
		+MIPSETS: 0 or 3 (0 means set success and 3 means Operation not allowed) 
		OK 
 
AT+MIPSETS =1,300\r

发送数据
AT++MIPSEND=<Socket_ID>,<Data>
		Response: 
		+MIPSEND: <Socket_ID>,<Status>,<FreeSize> 

AT+ MIPSEND? 

		<socket_ID> 1,2,3,4 Number of valid socket 
		<FreeSize> Free space in current buffer. Free size is calculated from the 1460. 
		0< Free Size < 1460 
		<Data> User data string is sent encoded with 0-F hexadecimal digits 
		<Status>  
		0 - Success 
		1 - Socket is flowed off 


+MIPTSEND=<Socket_ID> 
	> <Data> <ctrl+z> 
		Response: 
		+MIPPUSH: <Socket_ID>,<Status>,<Length> 
		OK 




\*************************************************************************************/
/*******************************************************************************
 * 函数名称:checkSocketConfig                                                                     
 * 描    述:检查socket数组有效性                                                               
 *                                                                               
 * 输    入:无                                                          
 * 输    出:无                                                                    
 * 返    回: 负:强制使用USART升级  其余：正常检查系统并确定是否升级                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年5月9日                                                                    
 *******************************************************************************/
int	checkSocketConfig(SOCKET net);
/*******************************************************************************
 * 函数名称:calSocketConfig                                                                     
 * 描    述: 计算校验socket数组                                                              
 *                                                                               
 * 输    入:无                                                          
 * 输    出:无                                                                    
 * 返    回: 负:强制使用USART升级  其余：正常检查系统并确定是否升级                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年5月9日                                                                    
 *******************************************************************************/
int calSocketConfig(SOCKET net);
int readImei(void);



#endif
