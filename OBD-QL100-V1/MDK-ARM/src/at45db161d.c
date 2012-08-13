#include "sysdef.h"
/****************************************************************************** 
at45db041b特性：
	正常操作电压为2.7~3.6V，实验中发现当电压超过4.25V后读出的状态字节为9A（正常  
	的状态字节值为9D），并且读写数据均不准确，所以应当保证卡片的供电电压不超过  
	4.25V。                                               
SPI规范：
	Data is always clocked into the device on the rising edge of SCK a- 
   	nd clocked out of the device on the falling edge of SCK.All instruction-
   	s,addresses and data are transferred with the most significant bit(MSB)  
   	first.                                              
                                             						2005-06-02 
******************************************************************************/ 
/*
函数名：initAT45DB041B
功能：初始化io设置
输入：无
输出：无
返回：无
作者：蛋蛋
修改时间：2011年7月19日
*/
/*FLASH
		IO分配
		RESET  	--->	PB13
		CS		--->	PA13
		CLK		--->	PB12
		SI		--->	PA14
		SO		--->	PA15
		FLASH
		FLASH_RST			PC7
		FLASH_CS			PC6
		FLASH_SCK		  	PC8
		FLASH_SI		  	PC9
		FLASH_SO		  	PA8
		
		
		
*/

void initAT45DB041B(void)
{
	/*
		io设置
	*/
	GPIO_InitTypeDef GPIO_InitStructure;
	//RESET
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	//CS
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	//CLK
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	//SI
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	//SO
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	SPI_RESET_1;

	// initFlashFifo();//初始化FIFO_FLASH
}
void	spiNop(int delay)
{
//	for(;delay>0;delay--)
	return;
}
/*
函数名：SPI_HostWriteByte
功能：spi方式写入一个字节
输入：串行写入的字节
输出：无
返回：无
作者：蛋蛋
修改时间：2011年7月19日
*/
void SPI_HostWriteByte(u8 dat)
{
	u8 i = 0;
	for(i=0;i<8;i++)
	{
		if( (dat << i) & 0x80 )
		{
			SPI_SI_1;
		}
		else
		{
			SPI_SI_0;
		}
		SPI_CLK_0;
//		SPI_NOP;
		SPI_CLK_1;
	}
}
/*
函数名：SPI_HostWriteByte
功能：spi方式读出一个字节
输入：无
输出：无
返回：读出的字节
作者：蛋蛋
修改时间：2011年7月19日
*/
u8   SPI_HostReadByte(void)
{
	u8 i = 0;
	u8 dat = 0;
	for(i=0;i<8;i++)
	{
		SPI_CLK_0;
//		SPI_NOP;
		SPI_CLK_1;
		dat <<= 1;
//		SPI_NOP;
		dat |= SPI_SO_STATE;
	}
	return dat;
}




/*
函数名：AT45DB041B_BufferRead
功能：spi方式读出一个字节
输入：无
输出：无
返回：读出的字节
作者：蛋蛋
修改时间：2011年7月19日
*/
/******************************************************************************
描述：                                                
   	When the end of a buffer is reached.the device will continue reading back at
    the beginning of the buffer.
	buffer为循环读
参数：                                               
   	buffer  - 指定buffer ID-1、2                                  
   	BFA     - 指定BUFFER中的起始写入地址                          
   	pHeader - 指定存储数据的首地址                               
   	len     - 指定数据的长度                                  
******************************************************************************/ 
void AT45DB041B_BufferRead(u8 buffer, u16 BFA, u8 *pHeader, u16 len) {
	unsigned int i = 0; 
  	while(i++ < 255) 
	{
		if(AT45DB041B_StatusRegisterRead() & 0x80)
		{ 
			break;
		}
	} 
  	SPI_CS_0;   
  	switch(buffer) { 
    	case 1 : SPI_HostWriteByte(Read_Buffer1);//#define Write_Buffer1 0x84
			break; 
    	case 2 : SPI_HostWriteByte(Read_Buffer2);////#define Write_Buffer2 0x87
			break; 
  	}
	SPI_HostWriteByte(0x00);//the don't care 15bits of high eight bits
	SPI_HostWriteByte((u8)(BFA >> 8));//the don't care 15bits of low seven bits and the BFA of high one bit
	SPI_HostWriteByte((u8)BFA);//the BFA of low eight bits
	SPI_HostWriteByte(0x00);//the eight don't care bits
	for(i = 0; i < len; i++) {
		pHeader[i] = SPI_HostReadByte();
	} 
  	SPI_CS_1;
}
/*
函数名：AT45DB041B_StatusRegisterRead
功能：spi方式读出一个字节
输入：无
输出：无
返回：读出的字节
作者：蛋蛋
修改时间：2011年7月19日
*/
/******************************************************************************/ 
/*Status Register Format:                                   */ 
/*   ----------------------------------------------------------------------- */ 
/* | bit7 | bit6 | bit5 | bit4 | bit3 | bit2 | bit1 | bit0 | */ 
/* |--------|--------|--------|--------|--------|--------|--------|--------| */ 
/* |RDY/BUSY| COMP |   0   |   1   |   1   |   1   |   X   |   X   | */ 
/*   ----------------------------------------------------------------------- */ 
/* bit7 - 忙标记，0为忙1为不忙。                               */ 
/*       当Status Register的位0移出之后，接下来的时钟脉冲序列将使SPI器件继续*/ 
/*       将最新的状态字节送出。                               */ 
/* bit6 - 标记最近一次Main Memory Page和Buffer的比较结果，0相同，1不同。   */ 
/* bit5                                               */ 
/* bit4                                               */ 
/* bit3                                               */ 
/* bit2 - 这4位用来标记器件密度，对于AT45DB041B，这4位应该是0111，一共能标记 */ 
/*       16种不同密度的器件。                               */ 
/* bit1                                               */ 
/* bit0 - 这2位暂时无效                                     */ 
/******************************************************************************/ 
u8 AT45DB041B_StatusRegisterRead(void) { 
	u8 status; 
	SPI_CS_0;   
  	SPI_HostWriteByte(Status_Register_Read);//#define Status_Register_Read 0xd7//写入读取AT45DB041B状态寄存器命令'0xd7' 
  	status = SPI_HostReadByte(); 
  	SPI_CS_1; 
//	printf("\r\nflash状态字：%3X\r\n",status);
	return status;   
}
/*
函数名：AT45DB041B_ContinuousArrayRead
功能：spi方式读出一个字节
输入：无
输出：无
返回：读出的字节
作者：蛋蛋
修改时间：2011年7月19日
*/ 
/******************************************************************************
描述：                                                
   	When the last bit in the main memory array has been read,the device will 
   	continue reading back at the beginning of the first page of memory.As w- 
   	ith crossing over page boundaries,no delays will be incurred when wrapp- 
   	ing around from the end of the array to the beginning of the array.    
参数：                                               
   	PA     - 页地址，0~2047                                  
   	BFA   - 指定BUFFER中的起始写入地址                          
   	pHeader - 指定数据的首地址                               
   	len   - 指定数据的长度                                  
******************************************************************************/ 
void AT45DB041B_ContinuousArrayRead(u16 PA, u16 BFA, u8 *pHeader, u16 len) {     
  	unsigned int i = 0;     
	while(i++ < 255) {//i++ < 255用来延时等待AT...空闲
		if(AT45DB041B_StatusRegisterRead() & 0x80) {//读取状态寄存器最高位,判断是否为忙
			break;//为1表示空闲则跳出
		}
	} 
  	SPI_CS_0;   
  	SPI_HostWriteByte(Continuous_Array_Read);//#define Continuous_Array_Read 0xe8//连续的数组读取   
  	SPI_HostWriteByte((u8)(PA >> 6));//此处代码很妙啊,PA >> 7则是发送PA的高4位以及最开始的4位无关紧要位   
  	SPI_HostWriteByte((u8)((PA << 2) | (BFA >> 8)));//因为PA有2个字节共16位,但是PA只用到了第1位到11位,
  	SPI_HostWriteByte((u8)BFA);//PA左移一位再或上BFA >> 8则是发送PA的低7位和BFA最高位,最后再发送BFA的低8位
  	for(i = 0; i < 4; i++) {
		SPI_HostWriteByte(0x00);
	} 
  	for(i = 0; i < len; i++) {
		pHeader[i] = SPI_HostReadByte();
	} 
  	SPI_CS_1; 
}
/*
函数名：AT45DB041B_MainMemoryPageRead
功能：spi方式读出一个字节
输入：无
输出：无
返回：读出的字节
作者：蛋蛋
修改时间：2011年7月19日
*/
/*******************************************************************************
描述：
   	When the end of a page in main memory is reached during a Main Memory Page 
	Read,the device will continue reading at the beginning of the same page.A 
	low-to-high transition on the /C/S pin will terminate the read operation and
	tri-state the SO pin.
参数：                                               
   	PA     - 页地址，0~2047                                  
   	BFA   - 指定BUFFER中的起始写入地址                          
   	pHeader - 指定数据的首地址                               
   	len   - 指定数据的长度                                  
******************************************************************************/
void AT45DB041B_MainMemoryPageRead(u16 PA, u16 BFA, u8 *pHeader, u16 len) {     
  	unsigned int i = 0;     
	while(i++ < 255) {//i++ < 255用来延时等待AT...空闲
		if(AT45DB041B_StatusRegisterRead() & 0x80) {//读取状态寄存器最高位,判断是否为忙
			break;//为1表示空闲则跳出
		}
	} 
  	SPI_CS_0;   
  	SPI_HostWriteByte(Main_Memory_Page_Read);//#define Continuous_Array_Read 0xe8//连续的数组读取   
  	SPI_HostWriteByte((u8)(PA >> 6));//此处代码很妙啊,PA >> 7则是发送PA的高4位以及最开始的4位无关紧要位   
  	SPI_HostWriteByte((u8)((PA << 2) | (BFA >> 8)));//因为PA有2个字节共16位,但是PA只用到了第1位到11位,
  	SPI_HostWriteByte((u8)BFA);//PA左移一位再或上BFA >> 8则是发送PA的低7位和BFA最高位,最后再发送BFA的低8位
  	for(i = 0; i < 4; i++) {
		SPI_HostWriteByte(0x00);//发送32位无关紧要的数据
	} 
  	for(i = 0; i < len; i++) {
		pHeader[i] = SPI_HostReadByte();
	} 
  	SPI_CS_1; 
}
/*
函数名：AT45DB041B_BufferWrite
功能：spi方式读出一个字节
输入：无
输出：无
返回：读出的字节
作者：蛋蛋
修改时间：2011年7月19日
*/
/****************************************************************************** 
描述： 
	将指定数据写入从某个地址（0~263）开始的BUFFER中。              
参数：                                              
   	buffer - 选择BUFFER，01H选择BUFFER 1，02H选择BUFFER 2              
    在该指令序列中，操作码84H选择BUFFER 1，87H选择BUFFER 2     
   	BFA   - BUFFER中的起始地址，0~263                         
   	pHeader - 待存数据的头指针                                
   	len   - 待存数据的长度1~264                              
******************************************************************************/ 
void AT45DB041B_BufferWrite(u8 buffer, u8 BFA, u8 *pHeader, u8 len) { 
  	unsigned int i = 0; 
  	while(i++ < 255) 
	{
		if(AT45DB041B_StatusRegisterRead() & 0x80) 
		{ 
			break;
		}
	} 
  	SPI_CS_0;   
  	switch(buffer) { 
    	case 1 : SPI_HostWriteByte(Write_Buffer1);//#define Write_Buffer1 0x84
			break; 
    	case 2 : SPI_HostWriteByte(Write_Buffer2);////#define Write_Buffer2 0x87
			break; 
  	} 
  	SPI_HostWriteByte(0x00); 
  	SPI_HostWriteByte((u8)(BFA >> 8)); 
  	SPI_HostWriteByte((u8)BFA);  
  	for(i = 0; i < len; i++) {
		SPI_HostWriteByte(pHeader[i]);
	} 
  	SPI_CS_1;       
} 
/*
函数名：AT45DB041B_BufferToMainMemoryPageProgramWithBuilt_inErase
功能：spi方式读出一个字节
输入：无
输出：无
返回：读出的字节
作者：蛋蛋
修改时间：2011年7月19日
*/
/****************************************************************************** 
描述：                                               
   将指定数据写入从某个地址（0~263）开始的页中：包含2个动作，首先将指定数据 
   写入到BUFFER 1或者BUFFER 2中，其中可以指定BUFFER中的起始写入地址，此写入 
   动作不影响BUFFER中其它地址中的数据，然后再将BUFFER中的整个数据写入到某指 
   定页中(带预擦除)。                                      
参数：                                                
   buffer - 选择BUFFER，01H选择BUFFER 1，02H选择BUFFER 2              
   PA     - 页地址，0~4096                                  
   BFA   - 指定BUFFER中的起始写入地址                          
   pHeader - 指定数据的首地址                                
   len   - 指定数据的长度                                 
******************************************************************************/ 
void AT45DB041B_BufferToMainMemoryPageProgramWithBuilt_inErase(u8 buffer, u16 PA, u16 BFA, u8 *pHeader, u16 len) { 
  	unsigned int i = 0; 
  	AT45DB041B_BufferWrite(buffer, BFA, pHeader, len); 
 	while(i++ < 1000) {
		if(AT45DB041B_StatusRegisterRead() & 0x80) {
			break;
		}
	} 
    SPI_CS_0;       
  	switch(buffer){ 
    	case 1 : SPI_HostWriteByte(Buffer1_To_MainMemory_PageProgram_WithBuilt_inErase);//#define Buffer1_To_MainMemory_PageProgram_WithBuilt_inErase x083
			break; 
    	case 2 : SPI_HostWriteByte(Buffer2_To_MainMemory_PageProgram_WithBuilt_inErase);//#define Buffer2_To_MainMemory_PageProgram_WithBuilt_inErase x086
			break; 
  	} 
  	SPI_HostWriteByte((u8)(PA >> 6)); 
    SPI_HostWriteByte((u8)(PA << 2)); 
  	SPI_HostWriteByte(0x00);
    for(i = 0; i < len; i++) {
		SPI_HostWriteByte(pHeader[i]);
	}
  	SPI_CS_1; 
} 
/*
函数名：AT45DB041B_BufferToMainMemoryPageProgramWithoutBuilt_inErase
功能：spi方式读出一个字节
输入：无
输出：无
返回：读出的字节
作者：蛋蛋
修改时间：2011年7月19日
*/
/****************************************************************************** 
描述：                                                
   与上一个函数的唯一区别是不带预擦除。                          
******************************************************************************/ 
void AT45DB041B_BufferToMainMemoryPageProgramWithoutBuilt_inErase(u8 buffer, u16 PA, u16 BFA, u8 *pHeader, u16 len){ 
  	unsigned int i = 0; 
  	AT45DB041B_BufferWrite(buffer, BFA, pHeader, len); 
  	while(i++ < 1000) { 
		if(AT45DB041B_StatusRegisterRead() & 0x80) {
			break;
		}
	} 
  	SPI_CS_0; 
  	SPI_HostWriteByte(0x87 + buffer);//当buffer=1,则写入0x88命令.当buffer=2则写入0x89命令,效果和下面注释代码功能一样 
	/*switch(buffer){ 
    	case 1 : SPI_HostWriteByte(Buffer1_To_MainMemory_PageProgram_WithoutBuilt_inErase);//#define Buffer1_To_MainMemory_PageProgram_WithoutBuilt_inErase x088
			break; 
    	case 2 : SPI_HostWriteByte(Buffer2_To_MainMemory_PageProgram_WithoutBuilt_inErase);//#define Buffer2_To_MainMemory_PageProgram_WithoutBuilt_inErase x089
			break; 
  	}*/ 
  	SPI_HostWriteByte((u8)(PA >> 7)); 
  	SPI_HostWriteByte((u8)(PA << 1)); 
  	SPI_HostWriteByte(0x00); 
  	for(i = 0; i < len; i++) {
		SPI_HostWriteByte(pHeader[i]);
	} 
  	SPI_CS_1; 
} 
/*
函数名：AT45DB041B_PageErase
功能：spi方式读出一个字节
输入：无
输出：无
返回：读出的字节
作者：蛋蛋
修改时间：2011年7月19日
*/
/****************************************************************************** 
描述：                                               
	When a low-to-high transition occurs on the /C/S pin,the part will erase the
   	selected page to 1s.The erase operation is internally self-timed and should
   	take place in a maximum time of tPE.During this time,the status register will
   	indicate that the part is busy.                             
参数：                                                             
   	PA     - 页地址，0~2047                                                                  
******************************************************************************/
void AT45DB041B_PageErase(u16 PA) {
	unsigned int i = 0; 
  	while(i++ < 255) {
		if(AT45DB041B_StatusRegisterRead() & 0x80) { 
			break;
		}
	} 
  	SPI_CS_0;	
	SPI_HostWriteByte(Page_Erase);
	SPI_HostWriteByte((u8)(PA >> 7));//页地址高4位和4为无关位
	SPI_HostWriteByte((u8)(PA << 1));//页地址低7位和1位无关位
	SPI_HostWriteByte(0x00);//8位无关位
	SPI_CS_1;
	while(!(AT45DB041B_StatusRegisterRead() & 0x80));//等待擦除完毕
}
/*
函数名：AT45DB041B_BufferRead
功能：spi方式读出一个字节
输入：无
输出：无
返回：读出的字节
作者：蛋蛋
修改时间：2011年7月19日
*/
/****************************************************************************** 
描述：                                               
	When a low-to-high transition occurs on the /C/S pin,the part will erase the
   	selected block of eight pages to 1s.The erase operation is internally self-t-
	imed and should take place in a maximum time of tBE.During this time,the sta-
	tus register will indicate that the part is busy.                             
参数：                                                             
   	BLKA     - 块地址，0~255                                                                  
******************************************************************************/
void AT45DB041B_BlockErase(u16 BLKA) {
	unsigned int i = 0; 
  	while(i++ < 255) {
		if(AT45DB041B_StatusRegisterRead() & 0x80) { 
			break;
		}
	} 
  	SPI_CS_0;	
	SPI_HostWriteByte(Block_Erase);
	SPI_HostWriteByte((u8)(BLKA >> 4));//块地址高4位和4为无关位
	SPI_HostWriteByte((u8)(BLKA << 4));//块地址低4位和4位无关位
	SPI_HostWriteByte(0x00);//8位无关位
	SPI_CS_1;
	while(!(AT45DB041B_StatusRegisterRead() & 0x80));//等待擦除完毕
}
/*
函数名：AT45DB041B_MainMemoryPageToBufferTransfer
功能：spi方式读出一个字节
输入：无
输出：无
返回：读出的字节
作者：蛋蛋
修改时间：2011年7月19日
*/
/****************************************************************************** 
描述：                                               
	When a low-to-high transition occurs on the /C/S pin,the part will erase the
   	selected block of eight pages to 1s.The erase operation is internally self-t-
	imed and should take place in a maximum time of tBE.During this time,the sta-
	tus register will indicate that the part is busy.                             
参数：                                                             
   	buffer     - 选择BUFFER，01H选择BUFFER1，02H选择BUFFER2
    PA		- 页地址，0~2047                                                              
******************************************************************************/
void AT45DB041B_MainMemoryPageToBufferTransfer(u8 buffer, u16 PA) {
	unsigned int i = 0; 
  	while(i++ < 255) {
		if(AT45DB041B_StatusRegisterRead() & 0x80) { 
			break;
		}
	} 
  	SPI_CS_0;
	switch(buffer){ 
    	case 1 : SPI_HostWriteByte(Main_Memory_Page_To_Buffer1_Transfer);
			break; 
    	case 2 : SPI_HostWriteByte(Main_Memory_Page_To_Buffer2_Transfer);
			break; 
  	}
    SPI_HostWriteByte((u8)(PA >> 7)); 
    SPI_HostWriteByte((u8)(PA << 1)); 
  	SPI_HostWriteByte(0x00);
	SPI_CS_1;
	while(!(AT45DB041B_StatusRegisterRead() & 0x80));//等待传送完毕
}
/*
函数名：AT45DB041B_MainMemoryPageToBufferCompare
功能：spi方式读出一个字节
输入：无
输出：无
返回：读出的字节
作者：蛋蛋
修改时间：2011年7月19日
*/
/****************************************************************************** 
描述：                                               
	When a low-to-high transition occurs on the /C/S pin,the part will erase the
   	selected block of eight pages to 1s.The erase operation is internally self-t-
	imed and should take place in a maximum time of tBE.During this time,the sta-
	tus register will indicate that the part is busy.                             
参数：                                                             
   	buffer     - 选择BUFFER，01H选择BUFFER1，02H选择BUFFER2
    PA		- 页地址，0~2047                                                              
******************************************************************************/
u8 AT45DB041B_MainMemoryPageToBufferCompare(u8 buffer, u16 PA) {
	unsigned int i = 0; 
  	while(i++ < 255) {
		if(AT45DB041B_StatusRegisterRead() & 0x80) { 
			break;
		}
	} 
  	SPI_CS_0;
	switch(buffer){ 
    	case 1 : SPI_HostWriteByte(Main_Memory_Page_To_Buffer1_Compare);
			break; 
    	case 2 : SPI_HostWriteByte(Main_Memory_Page_To_Buffer2_Compare);
			break; 
  	}
	SPI_HostWriteByte((u8)(PA >> 7)); 
    SPI_HostWriteByte((u8)(PA << 1)); 
  	SPI_HostWriteByte(0x00);
	SPI_CS_1;
	while(!(AT45DB041B_StatusRegisterRead() & 0x80));
	return(((AT45DB041B_StatusRegisterRead() & 0x40) ? 0 : 1));//返回比较结果，if StatusRegister's bit6 is 0,then match, is 1,then not match
}
/*
函数名：AT45DB041B_MainMemoryPageProgramThroughBuffer
功能：spi方式读出一个字节
输入：无
输出：无
返回：读出的字节
作者：蛋蛋
修改时间：2011年7月19日
*/
/****************************************************************************** 
描述：                                               
	When a low-to-high transition occurs on the /C/S pin,the part will erase the
   	selected block of eight pages to 1s.The erase operation is internally self-t-
	imed and should take place in a maximum time of tBE.During this time,the sta-
	tus register will indicate that the part is busy.                             
参数：                                                             
   	buffer  - 选择BUFFER，01H选择BUFFER1，02H选择BUFFER2
    PA		- 页地址，0~2047 
    BFA     - buffer地址，0~263
    pHeader - 待写入数据的首地址
    len     - 写入数据的长度                                                 
******************************************************************************/
void AT45DB041B_MainMemoryPageProgramThroughBuffer(u8 buffer, u8 PA, u8 BFA, u8 *pHeader, u8 len) { 
  	unsigned int i = 0; 
  	AT45DB041B_BufferWrite(buffer, BFA, pHeader, len); 
 	while(i++ < 1000) {
		if(AT45DB041B_StatusRegisterRead() & 0x80) {
			break;
		}
	} 
    SPI_CS_0;       
  	switch(buffer){ 
    	case 1 : SPI_HostWriteByte(Main_Memory_Page_Program_Through_Buffer1);
			break; 
    	case 2 : SPI_HostWriteByte(Main_Memory_Page_Program_Through_Buffer2);
			break; 
  	}
	SPI_HostWriteByte((u8)(PA >> 7)); 
    SPI_HostWriteByte((u8)(PA << 1 | BFA >> 8)); 
  	SPI_HostWriteByte((u8)BFA);
    for(i = 0; i < len; i++) {
		SPI_HostWriteByte(pHeader[i]);
	}
  	SPI_CS_1;
}
/*
函数名：AT45DB041B_AutoPageRewrite
功能：spi方式读出一个字节
输入：无
输出：无
返回：读出的字节
作者：蛋蛋
修改时间：2011年7月19日
*/
/****************************************************************************** 
描述：                                               
	When a low-to-high transition occurs on the /C/S pin,the part will erase the
   	selected block of eight pages to 1s.The erase operation is internally self-t-
	imed and should take place in a maximum time of tBE.During this time,the sta-
	tus register will indicate that the part is busy.                             
参数：                                                             
   	buffer  - 选择BUFFER，01H选择BUFFER1，02H选择BUFFER2
    PA		- 页地址，0~2047 
    BFA     - buffer地址，0~263
    pHeader - 待写入数据的首地址
    len     - 写入数据的长度                                                 
******************************************************************************/
void AT45DB041B_AutoPageRewrite(u8 buffer, u8 PA) {
	unsigned int i = 0;
	AT45DB041B_MainMemoryPageToBufferTransfer(buffer, PA);
	while(i++ < 1000) { 
		if(AT45DB041B_StatusRegisterRead() & 0x80) {
			break;
		}
	}
	SPI_CS_0;
	switch(buffer){ 
    	case 1 : SPI_HostWriteByte(Auto_Page_Rewrite_With_Buffer1);
			break; 
    	case 2 : SPI_HostWriteByte(Auto_Page_Rewrite_With_Buffer2);
			break; 
  	}
	SPI_HostWriteByte((u8)(PA >> 7)); 
    SPI_HostWriteByte((u8)(PA << 1)); 
  	SPI_HostWriteByte(0x00);
	SPI_CS_1;
	while(!(AT45DB041B_StatusRegisterRead() & 0x80));
}
//void checkReSendMsg(void)
//{
//}
//----------------------------------------------------------------
void showBuffer(u8 *pointer,int length)
{
	int  i = 0;
	
	for(i=0;i<length;i++)
	{
		if(i%10 == 0)
		{
			printf("\r\n");	
		}
		printf("%3X",pointer[i]);
		
	}
}
void	flashTest(void)
{
	u8 in[20];
	u8 out[20];
	int i = 0;
	printf("\r\n开始读取存储测试\r\n");
	for(i=0;i<4000;i++)
	{
		memset(in,i,20);
		printf("\r\n\r\n\r\n\r\n在第%d页写入的数据为:",i);
		showBuffer(in,20);
		AT45DB041B_BufferToMainMemoryPageProgramWithBuilt_inErase(1,i,0,in,20);


		memset(out,0,20);
		delay_ms(100);
		AT45DB041B_BufferRead(1,0,out,20);
		
		printf("\r\n读取buffer的数据为:");
		showBuffer(out,20);

	   	printf("\r\n在第%d页读出的数据为:",i);
		memset(out,0,20);
		AT45DB041B_ContinuousArrayRead(i,0,out,20);
		printf("\r\n读取ContinuousArrayRead的数据为:");
		showBuffer(out,20);


		memset(out,0,20);
		AT45DB041B_MainMemoryPageRead(i,0,out,20);
		printf("\r\n读取MainMemoryPageRead的数据为:");
		showBuffer(out,20);
		delay_ms(500);

	}
}







