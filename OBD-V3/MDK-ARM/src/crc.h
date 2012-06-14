#include "sysdef.h"

#ifndef		CRC_H_H
#define 	CRC_H_H


#define		LOW_BYTE( word )		( ( uint8_t ) ( word ) )
#define		HIGH_BYTE( word )		( ( uint8_t ) ( ( word ) >> 8 ) )
#define		MAKE_WORD( high, low )	( ( ( uint16_t ) ( high ) << 8 ) | ( uint16_t ) ( low ) )

#define		YES			1
#define		NO			0

#endif

