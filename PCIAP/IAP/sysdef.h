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
}IAP_START;	//IAP 参数请求
typedef struct {
        uint8_t 	start[2]; //fill with '#'
        uint16_t	crc;//数据包crc

        int16_t		typeOrIndex;
        uint8_t		hv;
        uint8_t		sv;

        uint16_t	packetNum;
        uint16_t	appCrc;	//整个APP的CRC校验


        uint32_t	sizeOfApp; //整个APP的体积大小信息
//	uint8_t		app[4];	 //fill with '*'
}IAP_BRIFE;	//IAP 参数返回



typedef struct {
        uint8_t 	start[2]; //fill with '#'
        uint16_t	crc;//数据包crc

        int16_t		typeOrIndex;
        uint8_t		hv;
        uint8_t		sv;

        uint16_t	packetCrc;	//附带软件包crc
        uint16_t	packetLength;

        uint8_t		app[4];	 //fill with '*'
}IAP_DATA;	//IAP 数据返回
uint16_t calBufCrc(uint8_t *ptr, uint32_t len);
class sysdef
{
public:
    sysdef();
};

#endif // SYSDEF_H
