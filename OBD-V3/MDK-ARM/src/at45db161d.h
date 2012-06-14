#include "sysdef.h"
#ifndef	 SPI45DB161D_H_H
#define	 SPI45DB161D_H_H
//io定义
/*FLASH
		IO分配
		RESET  	--->	PB13
		CS		--->	PA13
		CLK		--->	PB12
		SI		--->	PA14
		SO		--->	PA15
*/
//
#define SPI_RESET_0			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET)
#define SPI_RESET_1			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET)

#define SPI_CS_0			GPIO_WriteBit(GPIOA,GPIO_Pin_13,Bit_RESET)
#define SPI_CS_1			GPIO_WriteBit(GPIOA,GPIO_Pin_13,Bit_SET)

#define SPI_CLK_0			GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET)	
#define SPI_CLK_1   		GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET)

#define SPI_SI_0			GPIO_WriteBit(GPIOA,GPIO_Pin_14,Bit_RESET)
#define SPI_SI_1			GPIO_WriteBit(GPIOA,GPIO_Pin_14,Bit_SET)

#define SPI_SO_STATE		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)
#define SPI_NOP	spiNop(100)

//指令字定义
#define 	TRUE 														1
#define 	FALSE 														0


#define 	Write_Buffer1 												0x84//写Buffer1
#define 	Write_Buffer2 												0x87//写Buffer2

#define		Read_Buffer1												0xd4
#define		Read_Buffer2												0xd6
#define 	Status_Register_Read 										0xd7//读取flash状态寄存器
#define		Continuous_Array_Read 										0xe8//连续的数组读取
#define		Main_Memory_Page_Read										0xD2

#define 	Buffer1_To_MainMemory_PageProgram_WithBuilt_inErase			0x83//带预擦除先写入Buffer1,再写入指定页
#define 	Buffer2_To_MainMemory_PageProgram_WithBuilt_inErase 		0x86//带预擦除先写入Buffer2,再写入指定页

#define 	Buffer1_To_MainMemory_PageProgram_WithoutBuilt_inErase 		0x88//不带预擦除先写入Buffer1,再写入指定页
#define 	Buffer2_To_MainMemory_PageProgram_WithoutBuilt_inErase 		0x89//不带预擦除先写入Buffer2,再写入指定页

#define 	Page_Erase													0x81
#define 	Block_Erase													0x50

#define 	Main_Memory_Page_Program_Through_Buffer1					0x82
#define 	Main_Memory_Page_Program_Through_Buffer2					0x85

#define		Main_Memory_Page_To_Buffer1_Transfer						0x53
#define		Main_Memory_Page_To_Buffer2_Transfer						0x55

#define		Main_Memory_Page_To_Buffer1_Compare							0x60
#define		Main_Memory_Page_To_Buffer2_Compare							0x61

#define		Auto_Page_Rewrite_With_Buffer1								0x58
#define		Auto_Page_Rewrite_With_Buffer2								0x59





//
void 	initAT45DB041B(void);
void	spiNop(int delay);
u8		AT45DB041B_StatusRegisterRead(void);
void 	SPI_HostWriteByte(u8 dat);
u8   	SPI_HostReadByte(void);

u8		AT45DB041B_StatusRegisterRead(void);
void 	AT45DB041B_ContinuousArrayRead(u16 PA, u16 BFA, unsigned char *pHeader, u16 len);
void 	AT45DB041B_BufferRead(u8 buffer, u16 BFA, u8 *pHeader, u16 len);
void 	AT45DB041B_MainMemoryPageRead(u16 PA, u16 BFA, unsigned char *pHeader, u16 len);

void 	AT45DB041B_BufferToMainMemoryPageProgramWithBuilt_inErase(u8 buffer, u16 PA, u16 BFA, u8 *pHeader, u16 len);
void 	AT45DB041B_BufferToMainMemoryPageProgramWithoutBuilt_inErase(u8 buffer, u16 PA, u16 BFA, u8 *pHeader, u16 len);

void 	AT45DB041B_PageErase(u16 PA);
void 	AT45DB041B_BlockErase(u16 BLKA);

void 	AT45DB041B_MainMemoryPageToBufferTransfer(u8 buffer, u16 PA);
u8 		AT45DB041B_MainMemoryPageToBufferCompare(u8 buffer, u16 PA);

void 	AT45DB041B_MainMemoryPageProgramThroughBuffer(u8 buffer, u8 PA, u8 BFA, u8 *pHeader, u8 len);
void 	AT45DB041B_AutoPageRewrite(u8 buffer, u8 PA);
void showBuffer(u8 *pointer,int length);
void	flashTest(void);










#endif
