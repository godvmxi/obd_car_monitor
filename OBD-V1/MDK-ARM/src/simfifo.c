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
				printf("\r\nFIND +MIPR\r\n");
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

void dealFifoMsg(FIFO_HEAD *head)
{
	FIFO_NODE *nodeP = NULL;

	if(fifoIsEmpty(head))//û���·����ݴ�����
	{
		// #ifdef PRINTF_DEBUG
		// printf("\r\nû���·����ݴ�����\r\n");
		// #endif
		return ;
	}
	else
	{
		#ifdef PRINTF_DEBUG
		printf("\r\n��������·���������%d\r\n",head->nodeNum);	
		#endif
	}
	while(!fifoIsEmpty(head))
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nʣ���账�����Ϣ����%d\r\n",head->nodeNum);
		#endif	
		
		getOutFifoPointer(head,&nodeP);
		
		#ifdef PRINTF_DEBUG	 
		printf("\r\n�ڵ���������%d\r\n",nodeP->bufferNum);
		#endif		
//		dealMsg(nodeP);
		nodeP->bufferNum = 0;
		memset(nodeP->buffer,'\0',FIFO_BUFFER_SIZE);
	}
}

//void dealMsg(FIFO_NODE *nodeP)
//{
//	
//	char *charP;
//	
//	SANY_DATA *sanyP;
//	int i;
//	#ifdef PRINTF_DEBUG
//	
//	printf("\r\nBUFFERT DADA:\r\n");
//	
//	for(i= 0;i<FIFO_BUFFER_SIZE;i++)
//	{
//		printf("%3X",nodeP->buffer[i]);
//	}
//	#endif
//	charP = strstr(nodeP->buffer,":***");
//	charP++;
//	
//	if( charP != NULL)
//	{
//
////		sanyP = (SANY_DATA *)charP;
//		if(sanyP->MSG_CRC == checkCrc((char *)(&sanyP->MSG_TYPE),sanyP->MSG_LENGTH+16))
//		{
//			#ifdef PRINTF_DEBUG
//			printf("\r\nCRC OK!!\r\n");
//			#endif
//
//			switch(sanyP->MSG_TYPE)
//			{
////				case ACK_POWER_ON_MSG://heart head
//					
////				case ACK_POWER_OFF_MSG://heart head
//					//do something
//					#ifdef PRINTF_DEBUG
//					printf("\r\nACK_POWER_STATE_MSG--\r\n");
//					#endif
//					break;
////				case ACK_SOFT_UPDATE_MSG://heart head
//					//do something
//					#ifdef PRINTF_DEBUG
//					printf("\r\nACK_SOFT_UPDATE_MSG--\r\n");
//					#endif
//						
//					//�����������
//					break;
////				case ACK_SET_NET_MSG://heart head
//					//do something
//					#ifdef PRINTF_DEBUG
//					printf("\r\nACK_SET_NET_MSG--\r\n");
//					#endif
//					//�������ù���
//					break;
////				case ACK_SET_REPORT_MSG://heart head
//					//do something
//					#ifdef PRINTF_DEBUG
//					printf("\r\nACK_SET_REPORT_MSG--\r\n");
//					printf("\r\n��ʼ���������ϱ�����--\r\n");
//					delay_ms(2000);
//					#endif
//					charP = (char *)sanyP;
//					charP+=20;
//					setNetArgument(charP,sanyP->MSG_LENGTH);
//					
//					break;
////				case ACK_ONLINE_MSG://heart head
//					//do something
//					#ifdef PRINTF_DEBUG
//					printf("\r\nACK_ONLINE_MSG--\r\n");
//					#endif
//					onlineTimeOutFlag = 1;//���߳�ʱȷ��
//					break;
////				case ACK_OFFLINE_MSG://heart head
//					//do something
//					#ifdef PRINTF_DEBUG
//					printf("\r\nACK_OFFLINE_MSG--\r\n");
//					#endif
//					break;			
//				
////				case ACK_HEART_MSG://heart head
//					//do something
//					#ifdef PRINTF_DEBUG
//					printf("\r\nACK_HEART_MSG--\r\n");
//					#endif
//					break;
////				case ACK_GPS_MSG://gps
//					//do something
//					#ifdef PRINTF_DEBUG
//					printf("\r\nACK_GPS_MSG--\r\n");
//					#endif
//	
//					break;
////				case ACK_CAN_MSG://can
//					//do something
//					#ifdef PRINTF_DEBUG
//					printf("\r\nACK_CAN_MSG--\r\n");
//					#endif
//					break;
////				case SOFT_UPDATE_MSG://update
//					//do something
////					feedDog();
//					#ifdef PRINTF_DEBUG
//					printf("\r\n\r\n\r\n�յ�����ָ��--\r\n");
//					#endif
//					charP = (char *)(sanyP->MDT_ID+8);
//					for(i=0;i<sanyP->MSG_LENGTH;i++)
//					{
//						printf("%3x",charP[i]);
//					}
//					#ifdef PRINTF_DEBUG
//					printf("\r\n�ط�����ȷ������\r\n");
//					#endif
//					sanyP->MSG_TYPE |= 0x8000;
//					sanyP->MSG_LENGTH = 0;
//					sanyP->MSG_CRC = checkCrc((char *)(&sanyP->MSG_TYPE),16);
//					sendData((char *)sanyP,20,0,0,0);
//					delay_ms(2000);
//	
//					break;
////				case SET_NET_MSG://net
//					//do something
////					feedDog();
//					#ifdef PRINTF_DEBUG
//					printf("\r\n\r\n\r\n�յ������������ָ��--\r\n");
//					#endif
//					charP = (char *)(sanyP->MDT_ID+8);
//					for(i=0;i<sanyP->MSG_LENGTH;i++)
//					{
//						printf("%3x",charP[i]);
//					}
//					#ifdef PRINTF_DEBUG
//					printf("\r\n�ط�ȷ������\r\n");
//					#endif
//					sanyP->MSG_TYPE |= 0x8000;
//					sanyP->MSG_LENGTH = 0;
//					sanyP->MSG_CRC = checkCrc((char *)(&sanyP->MSG_TYPE),16);
//					sendData((char *)sanyP,20,0,0,0);
//					delay_ms(2000);
//					break;
////				case SET_REPORT_MSG://report
//					//do something
////					#ifdef PRINTF_DEBUG
////					printf("\r\nSET_REPORT_MSG--\r\n");
////					#endif
//////					feedDog();
////					#ifdef PRINTF_DEBUG
////					printf("\r\n\r\n\r\n�յ��ϱ���������ָ��--\r\n");
////					#endif
////					charP = (char *)(sanyP->MDT_ID+8);
////					for(i=0;i<sanyP->MSG_LENGTH;i++)
////					{
////						printf("%3x",charP[i]);
////					}
////					#ifdef PRINTF_DEBUG
////					printf("\r\n�ط�ȷ������\r\n");
////					#endif
////					sanyP->MSG_TYPE |= 0x8000;
////					sanyP->MSG_LENGTH = 0;
////					sanyP->MSG_CRC = checkCrc((char *)(&sanyP->MSG_TYPE),16);
////					sendData((char *)sanyP,20,0,0,0);
////					delay_ms(2000);
////					break;
//
////				case SET_CAN_UNLOCK_MSG://report
//					//do something
//					#ifdef PRINTF_DEBUG
//					printf("\r\n\r\n\r\n�յ�����ָ��--\r\n");
//					#endif
//					
//					// charP = (char *)(sanyP->MDT_ID+8);
//					// #ifdef PRINTF_DEBUG
//					// for(i=0;i<sanyP->MSG_LENGTH;i++)
//					// {
//						// printf("%3x",charP[i]);
//					// }
//					// #endif
//					#ifdef PRINTF_DEBUG
//					printf("\r\n�ط�ȷ������\r\n");
//					#endif
//					sanyP->MSG_TYPE |= 0x8000;
//					sanyP->MSG_LENGTH = 0;
//					sanyP->MSG_CRC = checkCrc((char *)(&sanyP->MSG_TYPE),16);
//					sendData((char *)sanyP,20,0,0,0);					
//					#ifdef PRINTF_DEBUG
//					printf("\r\n�豸���Խ���\r\n");
//					#endif
////					unlockCan();
//				
//					break;
//					
//				/* 	delay_ms(2000); */
//
//
////				case SET_CAN_LOCK_1_MSG://report
//					//do something
////					feedDog();
//					#ifdef PRINTF_DEBUG
//					printf("\r\n\r\n\r\n�յ�һ������ָ��--\r\n");
//					#endif
//					charP = (char *)(sanyP->MDT_ID+8);
//					// #ifdef PRINTF_DEBUG
//					// for(i=0;i<sanyP->MSG_LENGTH;i++){
//						// printf("%3x",charP[i]);
//					// }					
//					// printf("\r\n�ط�ȷ������\r\n");
//					// #endif
//					sanyP->MSG_TYPE |= 0x8000;
//					sanyP->MSG_LENGTH = 0;
//					sanyP->MSG_CRC = checkCrc((char *)(&sanyP->MSG_TYPE),16);
//					sendData((char *)sanyP,20,0,0,0);	
//					#ifdef PRINTF_DEBUG
//					printf("\r\n�豸����һ������\r\n");
//					#endif
//					lockCan(1);
//					break;	
////				case SET_CAN_LOCK_2_MSG://��������
//					//do something
////					feedDog();
//					#ifdef PRINTF_DEBUG
//					printf("\r\n\r\n\r\n�յ���������ָ��--\r\n");
//					#endif
//					charP = (char *)(sanyP->MDT_ID+8);
//					#ifdef PRINTF_DEBUG
//					// for(i=0;i<sanyP->MSG_LENGTH;i++){
//						// printf("%3x",charP[i]);
//					// }					
//					printf("\r\n�ط�ȷ������\r\n");
//					#endif
//					sanyP->MSG_TYPE |= 0x8000;
//					sanyP->MSG_LENGTH = 0;
//					sanyP->MSG_CRC = checkCrc((char *)(&sanyP->MSG_TYPE),16);
//					sendData((char *)sanyP,20,0,0,0);	
//					#ifdef PRINTF_DEBUG
//					printf("\r\n�豸���Զ�������\r\n");
//					#endif
//					lockCan(2);
//					break;
//
//				default:
//					//error handler
//					#ifdef PRINTF_DEBUG
//					printf("\r\nERROR MESG TYPE--\r\n");
//					#endif
//					break;
//			}
//
//
//		}
//		else
//		{
//			printf("\r\nCRC ERROR\r\n");
//			printf("\r\nMSG TYPE: %5X\r\n",sanyP->MSG_TYPE);
//			printf("\r\nMSG ID:   %5X\r\n",sanyP->MSG_ID);
//			printf("\r\nBUFFERT DADA:\r\n");
//			for(i= 0;i<50;i++)
//			{
//				printf("%3X",((char *)sanyP)[i]);
//			}
//
//			return;
//		}
//	
//	}
//
//}
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




