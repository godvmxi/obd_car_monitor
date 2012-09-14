#include "simfifo.h"
FIFO_NODE fifoNode[FIFO_MAX_SIZE];
FIFO_HEAD fifoHead;
volatile int onlineTimeOutFlag = 0;

/*	initialize fifo data
*/
void initFifo(FIFO_HEAD *head,FIFO_NODE *start,u32 num)
{
	u32 i;
	head->nodeNum = 0;
	head->front = start; //ͷβָ��һ���ڵ�
	head->rear = start;
	for(i=0;i<num-1;i++)
	{
		(start+i)->next= (start+i+1); //���ӳɻ�
	}
	(start+num-1)->next = start;
}
/*
	initialize a fifo node
	��ʼ�����и����ڵ�
*/
void initFifoNode(FIFO_NODE **node)
{
	(*node)->bufferNum = 0;
	memset((*node)->buffer,'\0',FIFO_BUFFER_SIZE);		
}

/*check the fifo is empty
�������Ƿ�Ϊ��
����ֵ��0����*/
u32  fifoIsFull(const FIFO_HEAD *head)
{

	return(head->nodeNum == FIFO_MAX_SIZE);	
}

/*get data into fifo
�������Ƿ�Ϊ��
����ֵ��0�ǿ�*/
u32  fifoIsEmpty(const FIFO_HEAD *head)
{
	return (head->nodeNum == 0);
}

/*get data pointer from fifo
��Ӳ���
��ȡһ�����нڵ���Դnode
�����Ƿ��ȡ�ɹ���ʾ
����ֵ��0����*/
u32 getInFifoPointer(FIFO_HEAD *head,FIFO_NODE **node)
{
	if(fifoIsFull(head))//������
	{
		return 0;
	}
	else
	{
		*node = head->rear;	//�����ڴ�ָ��
		head->rear = head->rear->next;//�ƶ�ָ�뵽һ�����е��ڴ�ռ�
		head->nodeNum++;
		return 1;//��ȡ�ɹ�
	}
}


/*get data pointer from fifo
���Ӳ���
��ȡһ�����нڵ���Դ
�����Ƿ��ȡ�ɹ���ʾ
����ֵ��0����*/
u32 getOutFifoPointer(FIFO_HEAD *head,FIFO_NODE **node)
{
	if(fifoIsEmpty(head))
	{
		
//		#ifdef PRINTF_DEBUG
//		printf("\r\n��ǰ����Ϊ��\r\n");
//		#endif
		node = NULL;
		return 0;//fifoʹ��
	}
	else
	{
		*node = head->front;	//�����ڴ�ָ��
		head->front = head->front->next;
		head->nodeNum--;
		return 1;	
	}
	
}

/*��ն���*/
u32 emptyFifo(FIFO_HEAD *head)
{

	return 0;	
}

void initData(void)
{
	initFifo(&fifoHead,fifoNode,FIFO_MAX_SIZE);	
}
 //+MIPRUDP
void simDataIrq(char charTmp)
{
	static int charIndex = 1;
	static char std;
	static FIFO_NODE *bufferPointer = 0;
	
	if(charTmp == '+')
		charIndex = 1;
	switch(charIndex)
	{
		case 1:
			if(charTmp == '+')
			{
				std = charTmp;
				charIndex++;
			}
			break;
		case 2:
			if((std == '+')&&(charTmp == 'M'))
			{
				std = 'M';
				charIndex++;
			}
			else
			{
				charIndex = 1;

			}
			break;
		case 3:
			if((std == 'M')&&(charTmp == 'I') )
			{
				std = 'I';
				charIndex++;
			}
			else
			{
				charIndex = 1; 
			}
			break;
		case 4:
			if((std == 'I')&&(charTmp == 'P') )
			{
				std = 'P';
				charIndex++;
			}
			else
			{
				charIndex = 1;
			}
			break;
		case 5:
			if((std == 'P')&&(charTmp == 'R') )
			{
				//printf("\r\nFIND +MIPR\r\n");
				if(!getInFifoPointer(&fifoHead,&bufferPointer))//��ȡ�µĻ�����ָ��
				{
					//error
					charIndex = 1;
					return ;

				}
				bufferPointer->bufferNum = 0;
	//			initFifoNode(&bufferPointer);
			}
			charIndex = 1;
			break;
		default:
			charIndex = 1;

			break;
	}
	if(bufferPointer->bufferNum < (FIFO_BUFFER_SIZE-1) && bufferPointer != 0)	//д������
	{
		bufferPointer->buffer[bufferPointer->bufferNum++] = charTmp;
	}
}

void simErrorIrq(char charTmp)
{
	static int charIndex = 1;
	static char std;
	
	if(charTmp == '+')
		charIndex = 1;
	switch(charIndex)
	{
		case 1:
			if(charTmp == '+')
			{
				std = charTmp;
				charIndex++;
			}
			break;
		case 2:
			if((std == '+')&&(charTmp == 'M'))
			{
				std = 'M';
				charIndex++;
			}
			else
			{
				charIndex = 1;

			}
			break;
		case 3:
			if((std == 'M')&&(charTmp == 'I') )
			{
				std = 'I';
				charIndex++;
			}
			else
			{
				charIndex = 1; 
			}
			break;
		case 4:
			if((std == 'I')&&(charTmp == 'P') )
			{
				std = 'P';
				charIndex++;
			}
			else
			{
				charIndex = 1;
			}
			break;
		case 5:
			if((std == 'P')&&(charTmp == 'S') )
			{
				printf("\r\nFIND +MIPR\r\n");

				charIndex = 1;


			}
			charIndex = 1;
			break;
		default:
			charIndex = 1;

			break;
	}
	printf("\r\nSIM STATE\r\n");
}


int dealFifoMsg(FIFO_HEAD *head,int num)
{
	int i = 0;
	FIFO_NODE *nodeP = NULL;
	#ifdef PRINTF_DEBUG
	printf("\r\n%s\r\n",__FUNCTION__);
	#endif

	if(fifoIsEmpty(head))//û���·����ݴ�����
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nno data to deal\r\n");
		#endif
		return 0;
	}
	else
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nwait for deal data items-->%d\r\n",head->nodeNum);	
		#endif
	}
	num = num == 0 ? 100 : num;
	printf("\r\ncurrent deal msg :%5d\r\n",num);
	for(i = 0;i<num;i++){
		if(fifoIsEmpty(head))
			break;
		#ifdef PRINTF_DEBUG
		printf("\r\nremaining data items-->%d\r\n",head->nodeNum);
		#endif	  		
		getOutFifoPointer(head,&nodeP);	 		
		#ifdef PRINTF_DEBUG	 
		printf("\r\nnode data length--->%d\r\n",nodeP->bufferNum);
		#endif		
		dealMsg(nodeP);
		nodeP->bufferNum = 0;
		memset(nodeP->buffer,'\0',FIFO_BUFFER_SIZE);
		printf("\r\ndeal msg index -->%d\r\n",i);
//		delay_ms(3000);
	}
	return i;
}

void dealMsg(FIFO_NODE *nodeP)
{
	
	char *charP; 	
	DATA_HEAD *header;
	int i;

	#ifdef PRINTF_DEBUG
	
	printf("\r\nBUFFERT DADA:\r\n");
	
	for(i= 0;i<FIFO_BUFFER_SIZE;i++)
	{
		printf("%3X",nodeP->buffer[i]);
	}
	#endif
	charP = strstr(nodeP->buffer,"**");

	return;
	
	if( charP != NULL)
	{

		header = (DATA_HEAD *)charP;
		printf("\r\nCRC->%4X MSG_TYPE->%4X \r\n",header->MSG_CRC,header->MSG_TYPE);
//		if(header->MSG_CRC == calBufCrc(charP,header->MSG_LENGTH - 4))
		if(1)
		{
			#ifdef PRINTF_DEBUG
			printf("\r\nCRC OK!!\r\n");
			#endif

			switch(header->MSG_TYPE)
			{
				case SET_IAP_PARA://update
					//do something
//					feedDog();
					#ifdef PRINTF_DEBUG
					printf("\r\n\r\n\r\nreceive iap para cmd--\r\n");
					#endif
//					charP = (char *)(header->MDT_ID+8);
					for(i=0;i<header->MSG_LENGTH;i++)
					{
						printf("%3x",charP[i]);
					}
					#ifdef PRINTF_DEBUG
					printf("\r\nsemd ack msg\r\n");
					#endif
					header->MSG_TYPE |= 0x8000;
					header->MSG_LENGTH = 0;
					header->MSG_CRC = calBufCrc(charP+4,sizeof(DATA_HEAD)-4);
					dataSend(charP,sizeof(DATA_HEAD),0,0,0,sysCfg.netConfig,1);
					delay_ms(2000);	
					break;

			case SET_NET_PARA://update
					//do something
//					feedDog();
					#ifdef PRINTF_DEBUG
					printf("\r\n\r\n\r\nreceive net para cmd--\r\n");
					#endif
//					charP = (char *)(header->MDT_ID+8);
					for(i=0;i<header->MSG_LENGTH;i++)
					{
						printf("%3x",charP[i]);
					}
					#ifdef PRINTF_DEBUG
					printf("\r\nsemd ack msg\r\n");
					#endif
					header->MSG_TYPE |= 0x8000;
					header->MSG_LENGTH = 0;
					header->MSG_CRC = calBufCrc(charP+4,sizeof(DATA_HEAD)-4);
					dataSend(charP,sizeof(DATA_HEAD),0,0,0,sysCfg.netConfig,1);
					delay_ms(2000);	
					break;

			case SET_WORK_PARA://update
					//do something
//					feedDog();
					#ifdef PRINTF_DEBUG
					printf("\r\n\r\n\r\nreceive work para cmd--\r\n");
					#endif
//					charP = (char *)(header->MDT_ID+8);
					for(i=0;i<header->MSG_LENGTH;i++)
					{
						printf("%3x",charP[i]);
					}
					#ifdef PRINTF_DEBUG
					printf("\r\nsemd ack msg\r\n");
					#endif
					header->MSG_TYPE |= 0x8000;
					header->MSG_LENGTH = 0;
					header->MSG_CRC = calBufCrc(charP+4,sizeof(DATA_HEAD)-4);
					dataSend(charP,sizeof(DATA_HEAD),0,0,0,sysCfg.netConfig,1);
					delay_ms(2000);	
					break;
			case SET_BLUE_PARA://update
					//do something
//					feedDog();
					#ifdef PRINTF_DEBUG
					printf("\r\n\r\n\r\nreceive blue para cmd--\r\n");
					#endif
//					charP = (char *)(header->MDT_ID+8);
					for(i=0;i<header->MSG_LENGTH;i++)
					{
						printf("%3x",charP[i]);
					}
					#ifdef PRINTF_DEBUG
					printf("\r\nsemd ack msg\r\n");
					#endif
					header->MSG_TYPE |= 0x8000;
					header->MSG_LENGTH = 0;
					header->MSG_CRC = calBufCrc(charP+4,sizeof(DATA_HEAD)-4);
					dataSend(charP,sizeof(DATA_HEAD),0,0,0,sysCfg.netConfig,1);
					delay_ms(2000);	
					break;
			default:
					//error handler
					#ifdef PRINTF_DEBUG
					printf("\r\nERROR MESG TYPE--\r\n");
					#endif
					break;
			}


		}
		else
		{
			printf("\r\nCRC ERROR\r\n");
			printf("\r\nMSG TYPE: %5X\r\n",header->MSG_TYPE);
//			printf("\r\nMSG ID:   %5X\r\n",header->MSG_ID);
			printf("\r\nBUFFERT DADA:\r\n");
			for(i= 0;i<50;i++)
			{
				printf("%3X",((char *)header)[i]);
			}

			return;
		}
	
	}

}
void ackFifoTest(void)
{
	FIFO_NODE *test;
	initData();
	while (1){
	
	getInFifoPointer(&fifoHead,&test);//1
	getInFifoPointer(&fifoHead,&test);
	getInFifoPointer(&fifoHead,&test);
	getInFifoPointer(&fifoHead,&test);
	getInFifoPointer(&fifoHead,&test);
	getInFifoPointer(&fifoHead,&test);
	getInFifoPointer(&fifoHead,&test);
	getInFifoPointer(&fifoHead,&test);
	getInFifoPointer(&fifoHead,&test);	
	getInFifoPointer(&fifoHead,&test);
	getInFifoPointer(&fifoHead,&test);
	getInFifoPointer(&fifoHead,&test);
	getInFifoPointer(&fifoHead,&test);
	getInFifoPointer(&fifoHead,&test);
	getInFifoPointer(&fifoHead,&test);
	getInFifoPointer(&fifoHead,&test);
	getInFifoPointer(&fifoHead,&test);
	getOutFifoPointer(&fifoHead,&test);
	getOutFifoPointer(&fifoHead,&test);
	getOutFifoPointer(&fifoHead,&test);
	getOutFifoPointer(&fifoHead,&test);
	getOutFifoPointer(&fifoHead,&test);
	getOutFifoPointer(&fifoHead,&test);
	getOutFifoPointer(&fifoHead,&test);
	getOutFifoPointer(&fifoHead,&test);
	getOutFifoPointer(&fifoHead,&test);
	getOutFifoPointer(&fifoHead,&test);
	getOutFifoPointer(&fifoHead,&test);
	getOutFifoPointer(&fifoHead,&test);
	getOutFifoPointer(&fifoHead,&test);
	getOutFifoPointer(&fifoHead,&test);
	getOutFifoPointer(&fifoHead,&test);
	}	
}




