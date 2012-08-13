#include "sysdef.h"	 
#ifndef		BULE_TOOTH_H_H
#define		BULE_TOOTH_H_H


/***********************************��������***************************************************
AT
AT+RESET						��λ
AT+VERSION						�汾
AT+HELP							��ʾ��ǰ�����б�
AT+NAME							���ã���ȡ����
AT+PIN							���PIN�� 
AT+BAUD							������
AT+CLEAR						�����������������б�
AT+LADDR						����MAC
AT+RNAME						Զ���豸��
AT+DEFAULT						�ָ�����ģʽ
AT+CMODE						����ģʽ
AT+BIND							��������ַ
AT+COD							�����豸��
AT+IAC							��ѯmode
AT_ROLE							����ģʽ����
AT+STATE						��ǰ״̬
AT+SENM							����ģʽ
AT+IPSCAN						ɨ�����
AT+SNIFF						sniff table ����
AT+LOWPOWER						�͹���ģʽ
AT+UARTMODE						���ڲ�����ֹͣλ��У��


<M ONLY>
AT+AUTOINQ						�Զ���ѯ
AT+AUTOCONN						�Զ�����
AT+INQM							��ѯ����
AT+INQ							��ʼ��ѯ
AT+INQC							ȡ�������еĲ�ѯ
AT+CONNECT						ͨ��BD��ַ����Զ��

**********************************************************************************************/

int blueToothPower(int state);
int initBlueTooth(int mode);
int blueAtAndWait(char *at,char *key,int ms);
int blueDma(char *add,int channel,int state); 
void blueTest(void);




#endif


