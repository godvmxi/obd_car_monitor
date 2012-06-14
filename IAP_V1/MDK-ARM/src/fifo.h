#include "sysdef.h"
#ifndef	FIFO_H_H
#define	FIFO_H_H		
#define FIFO_BUFFER_SIZE	128
#define	FIFO_MAX_SIZE			10

typedef struct fifi_node
{
	char  buffer[FIFO_BUFFER_SIZE];  	//���ݻ������ռ�
	u32 bufferNum;					 	//���ݻ�����ʹ����
	struct fifi_node *next;		   		//��һ���ڵ�ָ��
}FIFO_NODE;
typedef struct 
{
	FIFO_NODE *front;//ָ������׵�ָ��
	FIFO_NODE *rear; //ָ�����β��ָ��
	u32	nodeNum;	 //��������Ŀ�ĸ���
}FIFO_HEAD;
/*	initialize fifo data
���룺����ͷ��node��ʼ�ڴ棬node�ڵ�����
node�ڵ����������ʽ����
*/
void initFifo(FIFO_HEAD *head,FIFO_NODE *start,u32 num);
/*
	initialize a fifo node
	��ʼ�����и����ڵ�
*/
void initFifoNode(FIFO_NODE **node);

/*check the fifo is empty
�������Ƿ�Ϊ��*/
u32  fifoIsFull(const FIFO_HEAD *head);

/*get data into fifo
�������Ƿ�Ϊ��*/
u32  fifoIsEmpty(const FIFO_HEAD *head);

/*get data pointer from fifo
��Ӳ���
��ȡһ�����нڵ���Դ
�����Ƿ��ȡ�ɹ���ʾ
*/
u32 getInFifoPointer(FIFO_HEAD *head,FIFO_NODE **node);

/*get data pointer from fifo
���Ӳ���
��ȡһ�����нڵ���Դ
�����Ƿ��ȡ�ɹ���ʾ
*/
u32 getOutFifoPointer(FIFO_HEAD *head,FIFO_NODE **node);

/*��ն���*/
u32 emptyFifo(FIFO_HEAD *head);



void initData(void);

void dealFifoMsg(FIFO_HEAD *head);

void dealMsg(FIFO_NODE *nodeP);
void simDataIrq(char charTmp);

void ackFifoTest(void);





#endif

