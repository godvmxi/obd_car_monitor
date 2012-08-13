#include "sysdef.h"	 
#ifndef		BULE_TOOTH_H_H
#define		BULE_TOOTH_H_H


/***********************************蓝牙命令***************************************************
AT
AT+RESET						软复位
AT+VERSION						版本
AT+HELP							显示当前命令列表
AT+NAME							设置，获取名字
AT+PIN							配对PIN码 
AT+BAUD							波特率
AT+CLEAR						清除记忆的蓝牙连接列表
AT+LADDR						本地MAC
AT+RNAME						远端设备名
AT+DEFAULT						恢复出厂模式
AT+CMODE						连接模式
AT+BIND							绑定蓝牙地址
AT+COD							本地设备类
AT+IAC							查询mode
AT_ROLE							主从模式设置
AT+STATE						当前状态
AT+SENM							加密模式
AT+IPSCAN						扫描参数
AT+SNIFF						sniff table 参数
AT+LOWPOWER						低功耗模式
AT+UARTMODE						串口参数，停止位和校验


<M ONLY>
AT+AUTOINQ						自动查询
AT+AUTOCONN						自动连接
AT+INQM							查询参数
AT+INQ							开始查询
AT+INQC							取消进行中的查询
AT+CONNECT						通过BD地址连接远端

**********************************************************************************************/

int blueToothPower(int state);
int initBlueTooth(int mode);
int blueAtAndWait(char *at,char *key,int ms);
int blueDma(char *add,int channel,int state); 
void blueTest(void);




#endif


