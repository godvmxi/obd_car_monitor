#include "sysdef.h"
/****************************************************************************** 
at45db041b���ԣ�
	����������ѹΪ2.7~3.6V��ʵ���з��ֵ���ѹ����4.25V�������״̬�ֽ�Ϊ9A������  
	��״̬�ֽ�ֵΪ9D�������Ҷ�д���ݾ���׼ȷ������Ӧ����֤��Ƭ�Ĺ����ѹ������  
	4.25V��                                               
SPI�淶��
	Data is always clocked into the device on the rising edge of SCK a- 
   	nd clocked out of the device on the falling edge of SCK.All instruction-
   	s,addresses and data are transferred with the most significant bit(MSB)  
   	first.                                              
                                             						2005-06-02 
******************************************************************************/ 
/*
��������initAT45DB041B
���ܣ���ʼ��io����
���룺��
�������
���أ���
���ߣ�����
�޸�ʱ�䣺2011��7��19��
*/
/*FLASH
		IO����
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
		io����
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

	// initFlashFifo();//��ʼ��FIFO_FLASH
}
void	spiNop(int delay)
{
//	for(;delay>0;delay--)
	return;
}
/*
��������SPI_HostWriteByte
���ܣ�spi��ʽд��һ���ֽ�
���룺����д����ֽ�
�������
���أ���
���ߣ�����
�޸�ʱ�䣺2011��7��19��
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
��������SPI_HostWriteByte
���ܣ�spi��ʽ����һ���ֽ�
���룺��
�������
���أ��������ֽ�
���ߣ�����
�޸�ʱ�䣺2011��7��19��
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
��������AT45DB041B_BufferRead
���ܣ�spi��ʽ����һ���ֽ�
���룺��
�������
���أ��������ֽ�
���ߣ�����
�޸�ʱ�䣺2011��7��19��
*/
/******************************************************************************
������                                                
   	When the end of a buffer is reached.the device will continue reading back at
    the beginning of the buffer.
	bufferΪѭ����
������                                               
   	buffer  - ָ��buffer ID-1��2                                  
   	BFA     - ָ��BUFFER�е���ʼд���ַ                          
   	pHeader - ָ���洢���ݵ��׵�ַ                               
   	len     - ָ�����ݵĳ���                                  
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
��������AT45DB041B_StatusRegisterRead
���ܣ�spi��ʽ����һ���ֽ�
���룺��
�������
���أ��������ֽ�
���ߣ�����
�޸�ʱ�䣺2011��7��19��
*/
/******************************************************************************/ 
/*Status Register Format:                                   */ 
/*   ----------------------------------------------------------------------- */ 
/* | bit7 | bit6 | bit5 | bit4 | bit3 | bit2 | bit1 | bit0 | */ 
/* |--------|--------|--------|--------|--------|--------|--------|--------| */ 
/* |RDY/BUSY| COMP |   0   |   1   |   1   |   1   |   X   |   X   | */ 
/*   ----------------------------------------------------------------------- */ 
/* bit7 - æ��ǣ�0Ϊæ1Ϊ��æ��                               */ 
/*       ��Status Register��λ0�Ƴ�֮�󣬽�������ʱ���������н�ʹSPI��������*/ 
/*       �����µ�״̬�ֽ��ͳ���                               */ 
/* bit6 - ������һ��Main Memory Page��Buffer�ıȽϽ����0��ͬ��1��ͬ��   */ 
/* bit5                                               */ 
/* bit4                                               */ 
/* bit3                                               */ 
/* bit2 - ��4λ������������ܶȣ�����AT45DB041B����4λӦ����0111��һ���ܱ�� */ 
/*       16�ֲ�ͬ�ܶȵ�������                               */ 
/* bit1                                               */ 
/* bit0 - ��2λ��ʱ��Ч                                     */ 
/******************************************************************************/ 
u8 AT45DB041B_StatusRegisterRead(void) { 
	u8 status; 
	SPI_CS_0;   
  	SPI_HostWriteByte(Status_Register_Read);//#define Status_Register_Read 0xd7//д���ȡAT45DB041B״̬�Ĵ�������'0xd7' 
  	status = SPI_HostReadByte(); 
  	SPI_CS_1; 
//	printf("\r\nflash״̬�֣�%3X\r\n",status);
	return status;   
}
/*
��������AT45DB041B_ContinuousArrayRead
���ܣ�spi��ʽ����һ���ֽ�
���룺��
�������
���أ��������ֽ�
���ߣ�����
�޸�ʱ�䣺2011��7��19��
*/ 
/******************************************************************************
������                                                
   	When the last bit in the main memory array has been read,the device will 
   	continue reading back at the beginning of the first page of memory.As w- 
   	ith crossing over page boundaries,no delays will be incurred when wrapp- 
   	ing around from the end of the array to the beginning of the array.    
������                                               
   	PA     - ҳ��ַ��0~2047                                  
   	BFA   - ָ��BUFFER�е���ʼд���ַ                          
   	pHeader - ָ�����ݵ��׵�ַ                               
   	len   - ָ�����ݵĳ���                                  
******************************************************************************/ 
void AT45DB041B_ContinuousArrayRead(u16 PA, u16 BFA, u8 *pHeader, u16 len) {     
  	unsigned int i = 0;     
	while(i++ < 255) {//i++ < 255������ʱ�ȴ�AT...����
		if(AT45DB041B_StatusRegisterRead() & 0x80) {//��ȡ״̬�Ĵ������λ,�ж��Ƿ�Ϊæ
			break;//Ϊ1��ʾ����������
		}
	} 
  	SPI_CS_0;   
  	SPI_HostWriteByte(Continuous_Array_Read);//#define Continuous_Array_Read 0xe8//�����������ȡ   
  	SPI_HostWriteByte((u8)(PA >> 6));//�˴�������,PA >> 7���Ƿ���PA�ĸ�4λ�Լ��ʼ��4λ�޹ؽ�Ҫλ   
  	SPI_HostWriteByte((u8)((PA << 2) | (BFA >> 8)));//��ΪPA��2���ֽڹ�16λ,����PAֻ�õ��˵�1λ��11λ,
  	SPI_HostWriteByte((u8)BFA);//PA����һλ�ٻ���BFA >> 8���Ƿ���PA�ĵ�7λ��BFA���λ,����ٷ���BFA�ĵ�8λ
  	for(i = 0; i < 4; i++) {
		SPI_HostWriteByte(0x00);
	} 
  	for(i = 0; i < len; i++) {
		pHeader[i] = SPI_HostReadByte();
	} 
  	SPI_CS_1; 
}
/*
��������AT45DB041B_MainMemoryPageRead
���ܣ�spi��ʽ����һ���ֽ�
���룺��
�������
���أ��������ֽ�
���ߣ�����
�޸�ʱ�䣺2011��7��19��
*/
/*******************************************************************************
������
   	When the end of a page in main memory is reached during a Main Memory Page 
	Read,the device will continue reading at the beginning of the same page.A 
	low-to-high transition on the /C/S pin will terminate the read operation and
	tri-state the SO pin.
������                                               
   	PA     - ҳ��ַ��0~2047                                  
   	BFA   - ָ��BUFFER�е���ʼд���ַ                          
   	pHeader - ָ�����ݵ��׵�ַ                               
   	len   - ָ�����ݵĳ���                                  
******************************************************************************/
void AT45DB041B_MainMemoryPageRead(u16 PA, u16 BFA, u8 *pHeader, u16 len) {     
  	unsigned int i = 0;     
	while(i++ < 255) {//i++ < 255������ʱ�ȴ�AT...����
		if(AT45DB041B_StatusRegisterRead() & 0x80) {//��ȡ״̬�Ĵ������λ,�ж��Ƿ�Ϊæ
			break;//Ϊ1��ʾ����������
		}
	} 
  	SPI_CS_0;   
  	SPI_HostWriteByte(Main_Memory_Page_Read);//#define Continuous_Array_Read 0xe8//�����������ȡ   
  	SPI_HostWriteByte((u8)(PA >> 6));//�˴�������,PA >> 7���Ƿ���PA�ĸ�4λ�Լ��ʼ��4λ�޹ؽ�Ҫλ   
  	SPI_HostWriteByte((u8)((PA << 2) | (BFA >> 8)));//��ΪPA��2���ֽڹ�16λ,����PAֻ�õ��˵�1λ��11λ,
  	SPI_HostWriteByte((u8)BFA);//PA����һλ�ٻ���BFA >> 8���Ƿ���PA�ĵ�7λ��BFA���λ,����ٷ���BFA�ĵ�8λ
  	for(i = 0; i < 4; i++) {
		SPI_HostWriteByte(0x00);//����32λ�޹ؽ�Ҫ������
	} 
  	for(i = 0; i < len; i++) {
		pHeader[i] = SPI_HostReadByte();
	} 
  	SPI_CS_1; 
}
/*
��������AT45DB041B_BufferWrite
���ܣ�spi��ʽ����һ���ֽ�
���룺��
�������
���أ��������ֽ�
���ߣ�����
�޸�ʱ�䣺2011��7��19��
*/
/****************************************************************************** 
������ 
	��ָ������д���ĳ����ַ��0~263����ʼ��BUFFER�С�              
������                                              
   	buffer - ѡ��BUFFER��01Hѡ��BUFFER 1��02Hѡ��BUFFER 2              
    �ڸ�ָ�������У�������84Hѡ��BUFFER 1��87Hѡ��BUFFER 2     
   	BFA   - BUFFER�е���ʼ��ַ��0~263                         
   	pHeader - �������ݵ�ͷָ��                                
   	len   - �������ݵĳ���1~264                              
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
��������AT45DB041B_BufferToMainMemoryPageProgramWithBuilt_inErase
���ܣ�spi��ʽ����һ���ֽ�
���룺��
�������
���أ��������ֽ�
���ߣ�����
�޸�ʱ�䣺2011��7��19��
*/
/****************************************************************************** 
������                                               
   ��ָ������д���ĳ����ַ��0~263����ʼ��ҳ�У�����2�����������Ƚ�ָ������ 
   д�뵽BUFFER 1����BUFFER 2�У����п���ָ��BUFFER�е���ʼд���ַ����д�� 
   ������Ӱ��BUFFER��������ַ�е����ݣ�Ȼ���ٽ�BUFFER�е���������д�뵽ĳָ 
   ��ҳ��(��Ԥ����)��                                      
������                                                
   buffer - ѡ��BUFFER��01Hѡ��BUFFER 1��02Hѡ��BUFFER 2              
   PA     - ҳ��ַ��0~4096                                  
   BFA   - ָ��BUFFER�е���ʼд���ַ                          
   pHeader - ָ�����ݵ��׵�ַ                                
   len   - ָ�����ݵĳ���                                 
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
��������AT45DB041B_BufferToMainMemoryPageProgramWithoutBuilt_inErase
���ܣ�spi��ʽ����һ���ֽ�
���룺��
�������
���أ��������ֽ�
���ߣ�����
�޸�ʱ�䣺2011��7��19��
*/
/****************************************************************************** 
������                                                
   ����һ��������Ψһ�����ǲ���Ԥ������                          
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
  	SPI_HostWriteByte(0x87 + buffer);//��buffer=1,��д��0x88����.��buffer=2��д��0x89����,Ч��������ע�ʹ��빦��һ�� 
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
��������AT45DB041B_PageErase
���ܣ�spi��ʽ����һ���ֽ�
���룺��
�������
���أ��������ֽ�
���ߣ�����
�޸�ʱ�䣺2011��7��19��
*/
/****************************************************************************** 
������                                               
	When a low-to-high transition occurs on the /C/S pin,the part will erase the
   	selected page to 1s.The erase operation is internally self-timed and should
   	take place in a maximum time of tPE.During this time,the status register will
   	indicate that the part is busy.                             
������                                                             
   	PA     - ҳ��ַ��0~2047                                                                  
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
	SPI_HostWriteByte((u8)(PA >> 7));//ҳ��ַ��4λ��4Ϊ�޹�λ
	SPI_HostWriteByte((u8)(PA << 1));//ҳ��ַ��7λ��1λ�޹�λ
	SPI_HostWriteByte(0x00);//8λ�޹�λ
	SPI_CS_1;
	while(!(AT45DB041B_StatusRegisterRead() & 0x80));//�ȴ��������
}
/*
��������AT45DB041B_BufferRead
���ܣ�spi��ʽ����һ���ֽ�
���룺��
�������
���أ��������ֽ�
���ߣ�����
�޸�ʱ�䣺2011��7��19��
*/
/****************************************************************************** 
������                                               
	When a low-to-high transition occurs on the /C/S pin,the part will erase the
   	selected block of eight pages to 1s.The erase operation is internally self-t-
	imed and should take place in a maximum time of tBE.During this time,the sta-
	tus register will indicate that the part is busy.                             
������                                                             
   	BLKA     - ���ַ��0~255                                                                  
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
	SPI_HostWriteByte((u8)(BLKA >> 4));//���ַ��4λ��4Ϊ�޹�λ
	SPI_HostWriteByte((u8)(BLKA << 4));//���ַ��4λ��4λ�޹�λ
	SPI_HostWriteByte(0x00);//8λ�޹�λ
	SPI_CS_1;
	while(!(AT45DB041B_StatusRegisterRead() & 0x80));//�ȴ��������
}
/*
��������AT45DB041B_MainMemoryPageToBufferTransfer
���ܣ�spi��ʽ����һ���ֽ�
���룺��
�������
���أ��������ֽ�
���ߣ�����
�޸�ʱ�䣺2011��7��19��
*/
/****************************************************************************** 
������                                               
	When a low-to-high transition occurs on the /C/S pin,the part will erase the
   	selected block of eight pages to 1s.The erase operation is internally self-t-
	imed and should take place in a maximum time of tBE.During this time,the sta-
	tus register will indicate that the part is busy.                             
������                                                             
   	buffer     - ѡ��BUFFER��01Hѡ��BUFFER1��02Hѡ��BUFFER2
    PA		- ҳ��ַ��0~2047                                                              
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
	while(!(AT45DB041B_StatusRegisterRead() & 0x80));//�ȴ��������
}
/*
��������AT45DB041B_MainMemoryPageToBufferCompare
���ܣ�spi��ʽ����һ���ֽ�
���룺��
�������
���أ��������ֽ�
���ߣ�����
�޸�ʱ�䣺2011��7��19��
*/
/****************************************************************************** 
������                                               
	When a low-to-high transition occurs on the /C/S pin,the part will erase the
   	selected block of eight pages to 1s.The erase operation is internally self-t-
	imed and should take place in a maximum time of tBE.During this time,the sta-
	tus register will indicate that the part is busy.                             
������                                                             
   	buffer     - ѡ��BUFFER��01Hѡ��BUFFER1��02Hѡ��BUFFER2
    PA		- ҳ��ַ��0~2047                                                              
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
	return(((AT45DB041B_StatusRegisterRead() & 0x40) ? 0 : 1));//���رȽϽ����if StatusRegister's bit6 is 0,then match, is 1,then not match
}
/*
��������AT45DB041B_MainMemoryPageProgramThroughBuffer
���ܣ�spi��ʽ����һ���ֽ�
���룺��
�������
���أ��������ֽ�
���ߣ�����
�޸�ʱ�䣺2011��7��19��
*/
/****************************************************************************** 
������                                               
	When a low-to-high transition occurs on the /C/S pin,the part will erase the
   	selected block of eight pages to 1s.The erase operation is internally self-t-
	imed and should take place in a maximum time of tBE.During this time,the sta-
	tus register will indicate that the part is busy.                             
������                                                             
   	buffer  - ѡ��BUFFER��01Hѡ��BUFFER1��02Hѡ��BUFFER2
    PA		- ҳ��ַ��0~2047 
    BFA     - buffer��ַ��0~263
    pHeader - ��д�����ݵ��׵�ַ
    len     - д�����ݵĳ���                                                 
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
��������AT45DB041B_AutoPageRewrite
���ܣ�spi��ʽ����һ���ֽ�
���룺��
�������
���أ��������ֽ�
���ߣ�����
�޸�ʱ�䣺2011��7��19��
*/
/****************************************************************************** 
������                                               
	When a low-to-high transition occurs on the /C/S pin,the part will erase the
   	selected block of eight pages to 1s.The erase operation is internally self-t-
	imed and should take place in a maximum time of tBE.During this time,the sta-
	tus register will indicate that the part is busy.                             
������                                                             
   	buffer  - ѡ��BUFFER��01Hѡ��BUFFER1��02Hѡ��BUFFER2
    PA		- ҳ��ַ��0~2047 
    BFA     - buffer��ַ��0~263
    pHeader - ��д�����ݵ��׵�ַ
    len     - д�����ݵĳ���                                                 
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
	printf("\r\n��ʼ��ȡ�洢����\r\n");
	for(i=0;i<4000;i++)
	{
		memset(in,i,20);
		printf("\r\n\r\n\r\n\r\n�ڵ�%dҳд�������Ϊ:",i);
		showBuffer(in,20);
		AT45DB041B_BufferToMainMemoryPageProgramWithBuilt_inErase(1,i,0,in,20);


		memset(out,0,20);
		delay_ms(100);
		AT45DB041B_BufferRead(1,0,out,20);
		
		printf("\r\n��ȡbuffer������Ϊ:");
		showBuffer(out,20);

	   	printf("\r\n�ڵ�%dҳ����������Ϊ:",i);
		memset(out,0,20);
		AT45DB041B_ContinuousArrayRead(i,0,out,20);
		printf("\r\n��ȡContinuousArrayRead������Ϊ:");
		showBuffer(out,20);


		memset(out,0,20);
		AT45DB041B_MainMemoryPageRead(i,0,out,20);
		printf("\r\n��ȡMainMemoryPageRead������Ϊ:");
		showBuffer(out,20);
		delay_ms(500);

	}
}







