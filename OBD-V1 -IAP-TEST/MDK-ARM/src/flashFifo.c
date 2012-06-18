#include "flashFifo.h"
FLASH_DATA flashData;
/*******************************************************************************
 * 函数名称:initFlashFifo                                                                     
 * 描    述:初始化FIFO头结点结构，然后存放在flash第一个扇区
                                                                   
 *                                                                               
 * 输    入:无                                                                    
 * 输    出:无                                                                     
 * 返    回:无                                                                     
 * 作    者:蛋蛋                                                                     
 * 修改日期:2011年7月13日                                                                    
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
//		printf("\r\n写入flash成功\r\n");
//		#endif
//	}
//	else
//	{
//		#ifdef PRINTF_DEBUG
//		printf("\r\n写入flash失败\r\n");
//		#endif
//	}
//	return 1;
}

/*******************************************************************************
 * 函数名称:inFlashFifo                                                                     
 * 描    述:对flash就行入队数据操作，存入指定长度的数据
 *			入队指针始终指向一个空白的存储单元，满了后自动覆盖原来数据                                                                    
 *                                                                               
 * 输    入:入队数据指针，入队数据长度                                                                    
 * 输    出:无                                                                     
 * 返    回:操作执行结果 0：失败                                                                     
 * 作    者:蛋蛋                                                                     
 * 修改日期:2011年7月13日                                                                    
 *******************************************************************************/
u32		inFlashFifo(u8 *pointer,u32 length)
{
	getFifoHead();
	if(flashData.crc != (0 ^ flashData.bufferNum ^ flashData.inIndex ^ flashData.outIndex))
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nfifo头结点CRC失败，重新初始化fifo\r\n");
		#endif
		//re init fifo
		initFlashFifo();
		return 0;
	}//crc error
	#ifdef PRINTF_DEBUG
	else
	{
		printf("\r\n入队获取FIFO头校验正确\r\n");	
	}
	#endif

	//数据写入
	AT45DB041B_BufferToMainMemoryPageProgramWithBuilt_inErase(1,flashData.inIndex,0,pointer,length);



	
	if(flashData.bufferNum >= FLASHH_MAX_DATA_NUM)
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nflash写满，覆盖原先数据\r\n");
		#endif
		flashData.outIndex++;//	调整出队指针
		if(flashData.outIndex > FLASHH_MAX_DATA_NUM)
		{
			flashData.outIndex = 1;
		}
		flashData.inIndex++;  //调整入队指针
		if(flashData.inIndex > FLASHH_MAX_DATA_NUM)
		{
			flashData.inIndex = 1;
		}

	}//设备数据已满
	else
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nflash未满，可以继续写入\r\n");
		#endif
		flashData.bufferNum++;
		flashData.inIndex++;  //调整入队指针
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
//		printf("\r\n写入flash成功\r\n");
//		#endif
//		return 1;
//	}
//	else
//	{
//		#ifdef PRINTF_DEBUG
//		printf("\r\n写入flash失败\r\n");
//		#endif
//		return 0;
//	}
	
}
/*******************************************************************************
 * 函数名称:outFlashFifo                                                                     
 * 描    述:给队列中取出指定长度的数据                                                                    
 *                                                                               
 * 输    入:取出数据长度                                                                     
 * 输    出:取出的数据                                                                     
 * 返    回:操作执行结果 0：失败 1：成功                                                                     
 * 作    者:蛋蛋                                                                     
 * 修改日期:2011年7月13日                                                                    
 *******************************************************************************/
u32		outFlashFifo(u8 *pointer,u32 length)
{
	getFifoHead();
	if(flashData.crc != (0 ^ flashData.bufferNum ^ flashData.inIndex ^ flashData.outIndex))
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nfifo头结点CRC失败，重新初始化fifo\r\n");
		#endif
		initFlashFifo();
		//re init fifo
		return 0;
	}//crc error
	#ifdef PRINTF_DEBUG
	else
	{
		printf("\r\nfifo头结点CRC校验成功\r\n");
	}
	#endif
	if(flashData.bufferNum <= 0)
	{
		#ifdef PRINTF_DEBUG
		printf("\r\nfifo为空，返回\r\n");
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
 * 函数名称:getFifoHead                                                                     
 * 描    述:获取fifo的头结点信息                                                                     
 *                                                                               
 * 输    入:无                                                                     
 * 输    出:无                                                                     
 * 返    回:返回接收到的数据                                                                     
 * 作    者:蛋蛋                                                                     
 * 修改日期:2011年7月22日                                                                    
 *******************************************************************************/
u32		getFifoHead(void)
{
	
//	AT45DB041B_MainMemoryPageRead(FLASH_HEAD_PAGE,0,(u8 *)(&flashData),sizeof(FLASH_DATA));
//	u32 bufferNum; 		//fifo内的数据块个数
//	u32 inIndex;  	//入队数据的数据块索引	1---FLASH_INDEX_MAX
//	u32 outIndex; 	//出队数据的数据块索引
//	u32	crc;	  	//数据块检验，防止写入出错
	readFlashFifo(FLASH_FIFO,(uint32_t *)(&flashData),4);
	#ifdef PRINTF_DEBUG
	printf("\r\n\r\n读取后数据头内容");
	printf("\r\n数据块个数:%d",flashData.bufferNum);
	printf("\r\n入队数据的数据块索引:%d",flashData.inIndex);
	printf("\r\n出队数据的数据块索引:%d",flashData.outIndex);
	printf("\r\n数据块检验:%d",flashData.crc);
	#endif
	return 1;	
}
/*******************************************************************************
 * 函数名称:writeFifoHead                                                                     
 * 描    述:保存fifo的头结点                                                                    
 *                                                                               
 * 输    入:无                                                                     
 * 输    出:无                                                                     
 * 返    回:返回接收到的数据                                                                     
 * 作    者:蛋蛋                                                                     
 * 修改日期:2011年7月22日                                                                    
 *******************************************************************************/
u32		writeFifoHead(void)
{
	#ifdef PRINTF_DEBUG
	printf("\r\n\r\n写入前数据头内容");
	printf("\r\n数据块个数: %d",flashData.bufferNum);
	printf("\r\n入队数据的数据块索引:%d",flashData.inIndex);
	printf("\r\n出队数据的数据块索引:%d",flashData.outIndex);
	printf("\r\n数据块检验:%d",flashData.crc);
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
		printf("\r\n数据入队\r\n");
		for(i=0;i<FLASHH_MAX_DATA_NUM;i++)
		{
			memset(in,i,20);  			
			printf("\r\n第%d次写入数据\r\n",i);
			showBuffer(in,20);
			inFlashFifo(in,20);
			delay_ms(10);
		}
		delay_ms(3000);
		printf("\r\n数据出队\r\n");
		for(i=0;i<(FLASHH_MAX_DATA_NUM+12);i++)
		{
			memset(out,0,20);
			
			printf("\r\n第%d次读出数据\r\n",i);
			if(outFlashFifo(out,20))
			{
				showBuffer(out,20);
			}
			else
			{
				printf("\r\nFIFO为空，没有数据返回\r\n");
			}
			delay_ms(10);
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

