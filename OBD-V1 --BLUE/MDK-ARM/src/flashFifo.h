#include "sysdef.h"
#ifndef 	FLASH_FIFO_H_H
#define 	FLASH_FIFO_H_H

typedef struct	 //�����������д��flash�У���Ϊ���еı�ͷ
{
	int bufferNum; 		//fifo�ڵ����ݿ����
	int inIndex;  	//������ݵ����ݿ�����	1---FLASH_INDEX_MAX
	int outIndex; 	//�������ݵ����ݿ�����
	int	crc;	  	//���ݿ���飬��ֹд�����
}FLASH_DATA; 


#define FLASHH_MAX_DATA_NUM	 	4000//�����Դ洢�����ݰ�����
#define	FLASH_HEAD_PAGE			4050

/*******************************************************************************
 * ��������:initFlashFifo                                                                     
 * ��    ��:��ʼ��FLASH fifo�����ݽṹ                                                                     
 *                                                                               
 * ��    ��:rom fifo ��ͷ���                                                                     
 * ��    ��:д��rom��� 1���ɹ� 0��ʧ��                                                                   
 * ��    ��:��                                                                     
 * ��    ��:����                                                                     
 * �޸�����:2011��7��13��                                                                    
 *******************************************************************************/
u32	initFlashFifo(void);
/*******************************************************************************
 * ��������:writeFifoHead                                                                     
 * ��    ��:��fifo head д��rom                                                                     
 *                                                                               
 * ��    ��:fifo head                                                                     
 * ��    ��:д��rom��� 1���ɹ� 0��ʧ��                                                                     
 * ��    ��:���ؽ��յ�������                                                                     
 * ��    ��:����                                                                     
 * �޸�����:2011��7��13��                                                                    
 *******************************************************************************/
u32		writeFifoHead(void);
/*******************************************************************************
 * ��������:IIC_ROM_ReceiveByte                                                                     
 * ��    ��:����һ���ֽ�                                                                     
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:���ؽ��յ�������                                                                     
 * ��    ��:����                                                                     
 * �޸�����:2011��7��13��                                                                    
 *******************************************************************************/
u32		getFifoHead(void);
/*******************************************************************************
 * ��������:IIC_ROM_ReceiveByte                                                                     
 * ��    ��:����һ���ֽ�                                                                     
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:���ؽ��յ�������                                                                     
 * ��    ��:����                                                                     
 * �޸�����:2011��7��13��                                                                    
 *******************************************************************************/
u32		getAddress(u32 index,u32 *address);
/*******************************************************************************
 * ��������:IIC_ROM_ReceiveByte                                                                     
 * ��    ��:����һ���ֽ�                                                                     
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:���ؽ��յ�������                                                                     
 * ��    ��:����                                                                     
 * �޸�����:2011��7��13��                                                                    
 *******************************************************************************/
u32		inFlashFifo(u8 *pointer,u32 length);
/*******************************************************************************
 * ��������:IIC_ROM_ReceiveByte                                                                     
 * ��    ��:����һ���ֽ�                                                                     
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:���ؽ��յ�������                                                                     
 * ��    ��:����                                                                     
 * �޸�����:2011��7��13��                                                                    
 *******************************************************************************/
u32		outFlashFifo(u8 *pointer,u32 length);

void	flashFifoTest(void);
void	writeFlashFifo(uint32_t,uint32_t *pointer,int length);
void	readFlashFifo(uint32_t,uint32_t *pointer,int length);


#endif
