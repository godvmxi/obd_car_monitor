#include "flashFifo.h"
FLASH_DATA flashData;
/*******************************************************************************
 * ��������:initFlashFifo                                                                     
 * ��    ��:��ʼ��FIFOͷ���ṹ��Ȼ������flash��һ������
                                                                   
 *                                                                               
 * ��    ��:��                                                                    
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:����                                                                     
 * �޸�����:2011��7��13��                                                                    
 *******************************************************************************/
u32	initFlashFifo(void)
{
	flashData.bufferNum = 0;
	flashData.inIndex = 1;
	flashData.outIndex = 1;
	flashData.crc = 0 ^ flashData.bufferNum ^ flashData.inIndex ^ flashData.outIndex;
	writeFifoHead();
	return 1;
//	if(writeFifoHead())
//	{
//		#ifdef PRINTF_DEBUG
//		printf("\r\nд��flash�ɹ�\r\n");
//		#endif
//	}
//	else
//	{
//		#ifdef PRINTF_DEBUG
//		printf("\r\nд��flashʧ��\r\n");
//		#endif
//	}
//	return 1;
}

/*******************************************************************************
 * ��������:inFlashFifo                                                                     
 * ��    ��:��flash����������ݲ���������ָ�����ȵ�����
 *			���ָ��ʼ��ָ��һ���հ׵Ĵ洢��Ԫ�����˺��Զ�����ԭ������                                                                    
 *                                                                               
 * ��    ��:�������ָ�룬������ݳ���                                                                    
 * ��    ��:��                                                                     
 * ��    ��:����ִ�н�� 0��ʧ��                                                                     
 * ��    ��:����                                                                     
 * �޸�����:2011��7��13��                                                                    
 *******************************************************************************/
u32		inFlashFifo(u8 *pointer,u32 length)
{
	getFifoHead();
	if(flashData.crc != (0 ^ flashData.bufferNum ^ flashData.inIndex ^ flashData.outIndex))
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nfifo data header crc error��reinit fifo\r\n");
		#endif
		//re init fifo
		initFlashFifo();
		return 0;
	}//crc error
	#ifdef PRINTF_DEBUG
	else
	{
		printf("\r\nin queeen get FIFO head crc right\r\n");	
	}
	#endif

	//����д��
	AT45DB041B_BufferToMainMemoryPageProgramWithBuilt_inErase(1,flashData.inIndex,0,pointer,length);



	
	if(flashData.bufferNum >= FLASHH_MAX_DATA_NUM)
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nflash si full ,covery old data\r\n");
		#endif
		flashData.outIndex++;//	��������ָ��
		if(flashData.outIndex > FLASHH_MAX_DATA_NUM)
		{
			flashData.outIndex = 1;
		}
		flashData.inIndex++;  //�������ָ��
		if(flashData.inIndex > FLASHH_MAX_DATA_NUM)
		{
			flashData.inIndex = 1;
		}

	}//�豸��������
	else
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nflash is ok,go on\r\n");
		#endif
		flashData.bufferNum++;
		flashData.inIndex++;  //�������ָ��
		if(flashData.inIndex > FLASHH_MAX_DATA_NUM)
		{
			flashData.inIndex = 1;
		}
	}

	flashData.crc = 0 ^ flashData.bufferNum ^ flashData.inIndex ^ flashData.outIndex;
	writeFifoHead();
	return 1;
//	if(writeFifoHead())
//	{
//		#ifdef PRINTF_DEBUG
//		printf("\r\nд��flash�ɹ�\r\n");
//		#endif
//		return 1;
//	}
//	else
//	{
//		#ifdef PRINTF_DEBUG
//		printf("\r\nд��flashʧ��\r\n");
//		#endif
//		return 0;
//	}
	
}
/*******************************************************************************
 * ��������:outFlashFifo                                                                     
 * ��    ��:��������ȡ��ָ�����ȵ�����                                                                    
 *                                                                               
 * ��    ��:ȡ�����ݳ���                                                                     
 * ��    ��:ȡ��������                                                                     
 * ��    ��:����ִ�н�� 0��ʧ�� 1���ɹ�                                                                     
 * ��    ��:����                                                                     
 * �޸�����:2011��7��13��                                                                    
 *******************************************************************************/
u32		outFlashFifo(u8 *pointer,u32 length)
{
	getFifoHead();
	if(flashData.crc != (0 ^ flashData.bufferNum ^ flashData.inIndex ^ flashData.outIndex))
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nfifo head CRC error��re init fifo\r\n");
		#endif
		initFlashFifo();
		//re init fifo
		return 0;
	}//crc error
	#ifdef PRINTF_DEBUG
	else
	{
		printf("\r\nfifo head CRC ok\r\n");
	}
	#endif
	if(flashData.bufferNum <= 0)
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nfifo empty,return \r\n");
		#endif
		return 0;
	}
	AT45DB041B_ContinuousArrayRead(flashData.outIndex,0,pointer,length);
	flashData.bufferNum--;
	if(flashData.outIndex++ > FLASHH_MAX_DATA_NUM)
	{
		flashData.outIndex = 0;
	}
	flashData.crc = 0 ^ flashData.bufferNum ^ flashData.inIndex ^ flashData.outIndex;//
	writeFifoHead();
	return 1;
}
/*******************************************************************************
 * ��������:getFifoHead                                                                     
 * ��    ��:��ȡfifo��ͷ�����Ϣ                                                                     
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:���ؽ��յ�������                                                                     
 * ��    ��:����                                                                     
 * �޸�����:2011��7��22��                                                                    
 *******************************************************************************/
u32		getFifoHead(void)
{
	
//	AT45DB041B_MainMemoryPageRead(FLASH_HEAD_PAGE,0,(u8 *)(&flashData),sizeof(FLASH_DATA));
//	u32 bufferNum; 		//fifo�ڵ����ݿ����
//	u32 inIndex;  	//������ݵ����ݿ�����	1---FLASH_INDEX_MAX
//	u32 outIndex; 	//�������ݵ����ݿ�����
//	u32	crc;	  	//���ݿ���飬��ֹд�����
	readFlashFifo(FLASH_FIFO,(uint32_t *)(&flashData),4);
	#ifdef PRINTF_DEBUG
	printf("\r\nread fifo head");
	printf("\r\nnode number :%d",flashData.bufferNum);
	printf("\r\nin queen data index :%d",flashData.inIndex);
	printf("\r\nout queen data index :%d",flashData.outIndex);
	printf("\r\nblock crc:%d",flashData.crc);
	#endif
	return 1;	
}
/*******************************************************************************
 * ��������:writeFifoHead                                                                     
 * ��    ��:����fifo��ͷ���                                                                    
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:���ؽ��յ�������                                                                     
 * ��    ��:����                                                                     
 * �޸�����:2011��7��22��                                                                    
 *******************************************************************************/
u32		writeFifoHead(void)
{
	#ifdef PRINTF_DEBUG
	printf("\r\nread fifo head");
	printf("\r\nnode number :%d",flashData.bufferNum);
	printf("\r\nin queen data index :%d",flashData.inIndex);
	printf("\r\nout queen data index :%d",flashData.outIndex);
	printf("\r\nblock crc:%d",flashData.crc);
	#endif
	writeFlashFifo(FLASH_FIFO,(uint32_t *)(&flashData),4);
//	AT45DB041B_BufferToMainMemoryPageProgramWithBuilt_inErase(1,FLASH_HEAD_PAGE,0,(u8 *)(&flashData),sizeof(FLASH_DATA));
	return 1;
}
void	flashFifoTest(void)
{
	u8 in[20];
	u8 out[20];
	int i = 0;
	initFlashFifo();
	printf("\r\nFlash Fifo test----------------\r\n");
	while(1)
	{
		printf("\r\n data in queen \r\n");
		for(i=0;i<FLASHH_MAX_DATA_NUM;i++){
//			for(i=0;i<FLASHH_MAX_DATA_NUM;i++)
			{
				memset(in,i,20);  			
				printf("\r\nNo. %d write into data\r\n",i);
				showBuffer(in,20);
				inFlashFifo(in,20);
				delay_ms(1000);
			}
			delay_ms(3000);
			printf("\r\ndata out of queen\r\n");
//			for(i=0;i<(FLASHH_MAX_DATA_NUM+12);i++)
			{
				memset(out,0,20);
				
				printf("\r\nNo. %d read out data\r\n",i);
				if(outFlashFifo(out,20))
				{
					showBuffer(out,20);
				}
				else
				{
					printf("\r\nFIFO empty ,no data return\r\n");
				}
				delay_ms(1000);
			}
		}
		delay_ms(3000);
	}
}
void	writeFlashFifo(uint32_t address,uint32_t *pointer,int length)
{
	int i = 0;
	FLASH_Unlock();
	FLASH_ErasePage(address);
	FLASH_Lock();
	for(i = 0;i < length;i++)
	{
		writeFlash(address,*(pointer+i));
		address+=4; 
	}
}
void	readFlashFifo(uint32_t address,uint32_t *pointer,int length)
{
	vu32 *source;
	uint32_t *destination;
	int i;

	source=(vu32*)(address);
	destination = (uint32_t *)pointer;
	for(i = 0;i < length;i++)
	{
		*(destination+i) = *(source+i);		
	}
}
