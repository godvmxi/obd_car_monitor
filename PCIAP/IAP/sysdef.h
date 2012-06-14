#ifndef SYSDEF_H
#define SYSDEF_H

#include <QDebug>

typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;


typedef short int16_t;
//typedef  char int8_t;
typedef int int32_t;


typedef struct {
        uint8_t 	start[2];	  //fill with  '#'
        uint16_t 	crc;
        int16_t		typeOrIndex;
        uint8_t		hv;
        uint8_t		sv;
        uint32_t	imeiHigh;
        uint32_t	imeiLow;
}IAP_START;	//IAP ��������
typedef struct {
        uint8_t 	start[2]; //fill with '#'
        uint16_t	crc;//���ݰ�crc

        int16_t		typeOrIndex;
        uint8_t		hv;
        uint8_t		sv;

        uint16_t	packetNum;
        uint16_t	appCrc;	//����APP��CRCУ��


        uint32_t	sizeOfApp; //����APP�������С��Ϣ
//	uint8_t		app[4];	 //fill with '*'
}IAP_BRIFE;	//IAP ��������



typedef struct {
        uint8_t 	start[2]; //fill with '#'
        uint16_t	crc;//���ݰ�crc

        int16_t		typeOrIndex;
        uint8_t		hv;
        uint8_t		sv;

        uint16_t	packetCrc;	//���������crc
        uint16_t	packetLength;

        uint8_t		app[4];	 //fill with '*'
}IAP_DATA;	//IAP ���ݷ���
uint16_t calBufCrc(uint8_t *ptr, uint32_t len);
class sysdef
{
public:
    sysdef();
};

#endif // SYSDEF_H
