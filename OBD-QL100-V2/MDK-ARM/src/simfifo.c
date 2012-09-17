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
	
	char *charP,*charOld; 	
	DATA_HEAD *header;
	char tmp[50];
	int i,j;

	#ifdef PRINTF_DEBUG
	
	printf("\r\nBUFFERT DADA:\r\n");
	
	for(i= 0;i<FIFO_BUFFER_SIZE;i++)
	{
		printf("%3X",nodeP->buffer[i]);
	}
	printf("\r\nstring all data --> %s\r\n",nodeP->buffer);
	#endif
	charP = strstr(nodeP->buffer,"**");

	
	if( charP != NULL)
	{

		charOld = charP;
		header = (DATA_HEAD *)charP;
		printf("\r\nCRC->%4X MSG_TYPE->%4X MSG_LEN->%5d\r\n",header->MSG_CRC,header->MSG_TYPE,header->MSG_LENGTH);
//		if(header->MSG_CRC == calBufCrc(charP+4,header->MSG_LENGTH - 4))
		if(1)
		{
			#ifdef PRINTF_DEBUG
			printf("\r\nCRC OK!!\r\n");
			#endif

			switch(header->MSG_TYPE)
			{
				case SET_IAP_PARA://update
					//do something
					#ifdef PRINTF_DEBUG
					printf("\r\n\r\n\r\nreceive iap para cmd--\r\n");
					#endif
					charP += sizeof(DATA_HEAD);
					for(i=0;i<header->MSG_LENGTH;i++)
					{
						printf("%3X",charP[i]);
					}
					printf("\r\n");
					//get hardware version
					memset(tmp,0,50);
					for(i = 0; charP[i] != '+' ;i++){
							tmp[i] = charP[i];
							if(i > header->MSG_LENGTH)
							{
								printf("\r\niap get HV error\r\n");
								return ;
							}
					}
					
					sysCfg.iapBrife.hv = atoi(tmp);
					printf("\r\niap msg hv :%s <-->%d\r\n",tmp,sysCfg.iapBrife.hv);
					//get software version
					i++;
					memset(tmp,0,50);
					for(j=0;charP[i] !='+';i++) {
							tmp[j] = charP[i];
							j++;
							if(i > header->MSG_LENGTH)
							{
								printf("\r\niap get SV error\r\n");
								return ;
							}
					}					
					sysCfg.iapBrife.sv =  atoi(tmp);
					printf("\r\nIAP msg sv :%s -->%d\r\n",tmp,sysCfg.iapBrife.sv);

					//get ip url version
					i++;
					memset(tmp,0,50);
					for(j=0;	charP[i] != '+' ;i++) {
							tmp[j] = charP[i];
							j++;
							if(i > header->MSG_LENGTH)
							{
								printf("\r\niap get ipurl error\r\n");
								return ;
							}
					}
					memset(sysCfg.iapConfig.ipUrl,0,40);
					strcat(sysCfg.iapConfig.ipUrl,tmp);
					printf("\r\niap URL :%s -->%s\r\n",tmp,sysCfg.iapConfig.ipUrl);
					//get port
					i++;
					memset(tmp,0,50);
					for(j=0;charP[i] != '+' ;i++) {
							tmp[j] = charP[i];
							j++;
							if(i > header->MSG_LENGTH)
							{
								printf("\r\niap data get port error\r\n");
								return ;
							}
					}
					sysCfg.iapConfig.desPort = atoi(tmp); 					
					printf("\r\niap PORT :%s -->%d\r\n",tmp,sysCfg.iapConfig.desPort);
					//get update flag
					i++;
					memset(tmp,0,50);
					if(charP[i] == '1'){
						printf("\r\nbegin to upgrade system\r\n");
						printf("\r\nstore the config data\r\n");
						printf("\r\nwaiting for reboot\r\n");
						//ready to store data
					
						header->MSG_TYPE |= 0x8000;
						header->MSG_LENGTH = 0;
						header->MSG_CRC = calBufCrc(charP+4,sizeof(DATA_HEAD)-4);
						dataSend(charOld,sizeof(DATA_HEAD),0,0,0,sysCfg.netConfig,1);
						return ;
					}
					else {
						printf("just update the system para");
						//store the data without reboot
						//update the config
					}
					delay_ms(2000);	
					break;

			case SET_NET_PARA://update
					//do something;
					#ifdef PRINTF_DEBUG
					printf("\r\n\r\n\r\nreceive net para cmd-->\r\n");
					#endif
					charP += sizeof(DATA_HEAD);
					for(i=0;i<header->MSG_LENGTH;i++)
					{
						printf("%3X",charP[i]);
					}
					printf("\r\n");
					//get ip url version
					memset(tmp,0,50);
					for(i=0;charP[i] != '+' ;i++) {
							tmp[i] = charP[i];
							if(i > header->MSG_LENGTH)
							{
								printf("\r\nnet data ipurl error\r\n");
								return ;
							}
					}					
					memset(sysCfg.iapConfig.ipUrl,0,40);
					strcat(sysCfg.iapConfig.ipUrl,tmp);
					printf("\r\nnet URL :%s -->%s\r\n",tmp,sysCfg.iapConfig.ipUrl);
					//get port
					i++;
					memset(tmp,0,50);
					for(j=0;i< header->MSG_LENGTH ;i++) {
							tmp[j] = charP[i];
							j++;
							if(i > header->MSG_LENGTH)
							{
								printf("\r\nnet data get port error\r\n");
								return ;
							}
					}
					sysCfg.iapConfig.desPort = atoi(tmp); 					
					printf("\r\nnet PORT :%s -->%d\r\n",tmp,sysCfg.iapConfig.desPort);
					//get update flag
					
					#ifdef PRINTF_DEBUG
					printf("\r\nsemd ack msg\r\n");
					#endif
					header->MSG_TYPE |= 0x8000;
					header->MSG_LENGTH = 0;
					header->MSG_CRC = calBufCrc(charP+4,sizeof(DATA_HEAD)-4);
					dataSend(charOld,sizeof(DATA_HEAD),0,0,0,sysCfg.netConfig,1);
					delay_ms(2000);	
					break;

			case SET_WORK_PARA://update
					printf("\r\n\r\n\r\nreceive work para cmd--\r\n");
					charP += sizeof(DATA_HEAD);	
					printf("\r\nhex print:\r\n");
					for(i=0;i<header->MSG_LENGTH;i++)
					{
						printf("%3X",charP[i]);
					}
					printf("\r\nstring  print: %s\r\n",charP); 					
					memset(tmp,0,50);
					for(i = 0; charP[i] != '+' ;i++) {
							tmp[i] = charP[i];
							if(i > header->MSG_LENGTH)
							{
								printf("\r\nworking get intervals  error\r\n");
								return ;
							}
					}  			
					sysCfg.sysConfig.interval = atoi(tmp);
					printf("\r\nworking msg interval :%s -->%d\r\n",tmp,sysCfg.sysConfig.interval);
					//
					i++;
					memset(tmp,0,50);
					for(j=0;charP[i] != '+' ;i++) {
							tmp[j] = charP[i];
							j++;
							if(i > header->MSG_LENGTH)
							{
								printf("\r\nworking msg proportion data error\r\n");
								return ;
							}
					}
					i++;
					sysCfg.sysConfig.proportion =  atoi(tmp);
					printf("\r\nnet msg proportion :%s -->%d\r\n",tmp,sysCfg.sysConfig.proportion);
					memset(tmp,0,50);
					for(j=0;i< header->MSG_LENGTH ;i++) {
							tmp[j] = charP[i];
							j++;
							if(i > header->MSG_LENGTH)
							{
								printf("\r\nworking canConter error\r\n");
								return ;
							}
					}
					sysCfg.sysConfig.canCounter =  atoi(tmp);
					printf("\r\nnet msg canCounter :%s -->%d\r\n",tmp,sysCfg.sysConfig.canCounter);

					#ifdef PRINTF_DEBUG
					printf("\r\nsemd working ack msg\r\n");
					#endif
					header->MSG_TYPE |= 0x8000;
					header->MSG_LENGTH = 0;
					header->MSG_CRC = calBufCrc(charP+4,sizeof(DATA_HEAD)-4);
					dataSend(charOld,sizeof(DATA_HEAD),0,0,0,sysCfg.netConfig,1);
					delay_ms(2000);
					break;
			case SET_BLUE_PARA://update
					//do something
					#ifdef PRINTF_DEBUG
					printf("\r\n\r\n\r\nreceive blue para cmd--\r\n");
					#endif
					charP += sizeof(DATA_HEAD);
					for(i=0;i<header->MSG_LENGTH;i++)
					{
						printf("%3x",charP[i]);
					}
					printf("\r\nstring  print: %s\r\n",charP); 					
					memset(tmp,0,50);
					for(i = 0; charP[i] != '+' ;i++) {
							tmp[i] = charP[i];
							if(i > header->MSG_LENGTH)
							{
								printf("\r\nblue get name error\r\n");
								return ;
							}
					}
					memset(sysCfg.blueConfig.name,0,20); 
					strcat(sysCfg.blueConfig.name,tmp); 			
					printf("\r\nnet msg interval :%s -->%s\r\n",tmp,sysCfg.blueConfig.name);
					//
					i++;  
					memset(tmp,0,50);
					for(j = 0; charP[i] != '+' ;i++) {
							tmp[j++] = charP[i];
							if(i > header->MSG_LENGTH)
							{
								printf("\r\nblue get passwdord data error\r\n");
								return ;
							}
					}
					memset(sysCfg.blueConfig.password,0,20); 
					strcat(sysCfg.blueConfig.password,tmp); 			
					printf("\r\nblue password :%s -->%s\r\n",tmp,sysCfg.blueConfig.password);

					i++;  
					memset(tmp,0,50);
					for(j = 0; i < header->MSG_LENGTH ;i++) {
							tmp[j] = charP[i];
							j++;
							if(i > header->MSG_LENGTH)
							{
								printf("\r\nblue get baudrate error\r\n");
								return ;
							}
					}
				   	sysCfg.blueConfig.baudrate = atoi(tmp); 			
					printf("\r\nblue baudrate :%s -->%d\r\n",tmp,sysCfg.blueConfig.baudrate);


					#ifdef PRINTF_DEBUG
					printf("\r\nsemd blue ack msg\r\n");
					#endif
					header->MSG_TYPE |= 0x8000;
					header->MSG_LENGTH = 0;
					header->MSG_CRC = calBufCrc(charP+4,sizeof(DATA_HEAD)-4);
					dataSend(charOld,sizeof(DATA_HEAD),0,0,0,sysCfg.netConfig,1);
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




