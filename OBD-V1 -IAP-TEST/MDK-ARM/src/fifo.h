#include "sysdef.h"
#ifndef	FIFO_H_H
#define	FIFO_H_H		
#define FIFO_BUFFER_SIZE	128
#define	FIFO_MAX_SIZE			10

typedef struct fifi_node
{
	char  buffer[FIFO_BUFFER_SIZE];  	//数据缓冲区空间
	u32 bufferNum;					 	//数据缓冲区使用量
	struct fifi_node *next;		   		//下一个节点指针
}FIFO_NODE;
typedef struct 
{
	FIFO_NODE *front;//指向队列首的指针
	FIFO_NODE *rear; //指向队列尾的指针
	u32	nodeNum;	 //队列中项目的个数
}FIFO_HEAD;
/*	initialize fifo data
输入：队列头，node起始内存，node节点数量
node节点采用数组形式申请
*/
void initFifo(FIFO_HEAD *head,FIFO_NODE *start,u32 num);
/*
	initialize a fifo node
	初始化队列给定节点
*/
void initFifoNode(FIFO_NODE **node);

/*check the fifo is empty
检查队列是否为满*/
u32  fifoIsFull(const FIFO_HEAD *head);

/*get data into fifo
检查队列是否为空*/
u32  fifoIsEmpty(const FIFO_HEAD *head);

/*get data pointer from fifo
入队操作
获取一个队列节点资源
返回是否获取成功表示
*/
u32 getInFifoPointer(FIFO_HEAD *head,FIFO_NODE **node);

/*get data pointer from fifo
出队操作
获取一个队列节点资源
返回是否获取成功表示
*/
u32 getOutFifoPointer(FIFO_HEAD *head,FIFO_NODE **node);

/*清空队列*/
u32 emptyFifo(FIFO_HEAD *head);



void initData(void);

void dealFifoMsg(FIFO_HEAD *head);

void dealMsg(FIFO_NODE *nodeP);
void simDataIrq(char charTmp);

void ackFifoTest(void);





#endif

