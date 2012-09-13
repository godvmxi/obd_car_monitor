#ifndef GPS_H
#define GPS_H

void gpsTest(void);


typedef struct 
{ 
//¶¨Î»×ø±êÊı¾İ
	char A_Latitude[9];        //Î³¶Èddmm.mmmm(¶È·Ö)¸ñÊ½
	char A_NS;                 //Î³¶È°ëÇòN(±±°ëÇò)»òS(ÄÏ°ëÇò) 
	char A_Longitude[10];      //¾­¶Èdddmm.mmmm(¶È·Ö)¸ñÊ½ 
	char A_EW;                 //¾­¶È°ëÇòE(¶«¾­)»òW(Î÷¾­)
	int  HDOP;        	 	   //Ë®Æ½¾«È·¶È£¬0.5µ½99.9
	
	char A_SateUsed[2];        //Ê¹ÓÃÎÀĞÇÁ¿£¬´Ó00µ½12
	//Î´¶¨Î»×ø±êÊı¾İ
	char V_Latitude[9];        //Î³¶Èddmm.mmmm(¶È·Ö)¸ñÊ½
	char V_NS;                 //Î³¶È°ëÇòN(±±°ëÇò)»òS(ÄÏ°ëÇò) 
	char V_Longitude[10];      //¾­¶Èdddmm.mmmm(¶È·Ö)¸ñÊ½ 
	char V_EW;                 //¾­¶È°ëÇòE(¶«¾­)»òW(Î÷¾­)
	
	char V_SateUsed[2];        //Ê¹ÓÃÎÀĞÇÊıÁ¿£¬´Ó00µ½12
	
	char Speed[5];           //µØÃæËÙÂÊ(000.0~999.9½Ú£¬Ç°ÃæµÄ0Ò²½«±»´«Êä) 
	char Course[5];          //µØÃæº½Ïò(000.0~359.9È 

}stru_GPS_DATA; 



/*
$GPRMC,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12> 
<1> UTCÊ±¼ä£¬hhmmss(Ê±·ÖÃë)¸ñÊ½ 
<2> ¶¨Î»×´Ì¬£¬A=ÓĞĞ§¶¨Î»£¬V=ÎŞĞ§¶¨Î» 
<3> Î³¶Èddmm.mmmm(¶È·Ö)¸ñÊ½(Ç°ÃæµÄ0Ò²½«±»´«Êä) 
<4> Î³¶È°ëÇòN(±±°ëÇò)»òS(ÄÏ°ëÇò) 
<5> ¾­¶Èdddmm.mmmm(¶È·Ö)¸ñÊ½(Ç°ÃæµÄ0Ò²½«±»´«Êä) 
<6> ¾­¶È°ëÇòE(¶«¾­)»òW(Î÷¾­) 
<7> µØÃæËÙÂÊ(000.0~999.9½Ú£¬Ç°ÃæµÄ0Ò²½«±»´«Êä) 
<8> µØÃæº½Ïò(000.0~359.9¶È£¬ÒÔÕæ±±Îª²Î¿¼»ù×¼£¬Ç°ÃæµÄ0Ò²½«±»´«Êä) 
<9> UTCÈÕÆÚ£¬ddmmyy(ÈÕÔÂÄê)¸ñÊ½ 
<10> ´ÅÆ«½Ç(000.0~180.0¶È£¬Ç°ÃæµÄ0Ò²½«±»´«Êä) 
<11> ´ÅÆ«½Ç·½Ïò£¬E(¶«)»òW(Î÷) 
<12> Ä£Ê½Ö¸Ê¾(½öNMEA0183 3.00°æ±¾Êä³ö£¬A=×ÔÖ÷¶¨Î»£¬D=²î·Ö£¬E=¹ÀËã£¬N=Êı¾İÎŞĞ§)
*/
typedef struct 
{ 
	char Block; 
	char BlockIndex; 
	char UTCTime[10];        //UTCÊ±¼ä£¬hhmmss(Ê±·ÖÃë)¸ñÊ½ 
	char Status;         	 //¶¨Î»×´Ì¬£¬A=ÓĞĞ§¶¨Î»£¬V=ÎŞĞ§¶¨Î» 
	char Latitude[9];        //Î³¶Èddmm.mmmm(¶È·Ö)¸ñÊ½
	char NS;                 //Î³¶È°ëÇòN(±±°ëÇò)»òS(ÄÏ°ëÇò) 
	char Longitude[10];      //¾­¶Èdddmm.mmmm(¶È·Ö)¸ñÊ½ 
	char EW;                 //¾­¶È°ëÇòE(¶«¾­)»òW(Î÷¾­)
	char Speed[5];           //µØÃæËÙÂÊ(000.0~999.9½Ú£¬Ç°ÃæµÄ0Ò²½«±»´«Êä) 
	char Course[5];          //µØÃæº½Ïò(000.0~359.9¶È 
	char UTCDate[6];         //UTCÈÕÆÚ£¬
}stru_GPSRMC; 

/*
$GPGGA£ºÆğÊ¼Òıµ¼·û¼°Óï¾ä¸ñÊ½ËµÃ÷(±¾¾äÎªGPS¶¨Î»Êı¾İ)£»
<2>   Î³¶È£¬¸ñÊ½Îªddmm.mmmm(µÚÒ»Î»ÊÇÁãÒ²½«´«ËÍ)£»
<3>   Î³¶È°ëÇò£¬N»òS(±±Î³»òÄÏÎ³)
<4>   ¾­¶È£¬¸ñÊ½Îªdddmm.mmmm(µÚÒ»Î»ÁãÒ²½«´«ËÍ)£»
<5>   ¾­¶È°ëÇò£¬E»òW(¶«¾­»òÎ÷¾­)
<6>   ¶¨Î»ÖÊÁ¿Ö¸Ê¾£¬0=¶¨Î»ÎŞĞ§£¬1=¶¨Î»ÓĞĞ§£»
<7>   Ê¹ÓÃÎÀĞÇÊıÁ¿£¬´Ó00µ½12(µÚÒ»¸öÁãÒ²½«´«ËÍ)
<8>   Ë®Æ½¾«È·¶È£¬0.5µ½99.9
<9>   ÌìÏßÀëº£Æ½ÃæµÄ¸ß¶È£¬-9999.9µ½9999.9Ã×
M Ö¸µ¥Î»Ã×
<10> ´óµØË®×¼Ãæ¸ß¶È£¬-9999.9µ½9999.9Ã×
M Ö¸µ¥Î»Ã×
<11> ²î·ÖGPSÊı¾İÆÚÏŞ(RTCM SC-104)£¬×îºóÉèÁ¢RTCM´«ËÍµÄÃëÊıÁ¿
<12>   ²î·Ö²Î¿¼»ùÕ¾±êºÅ£¬´Ó0000µ½1023(Ê×Î»0Ò²½«´«ËÍ)¡£
* Óï¾ä½áÊø±êÖ¾·û
*/
typedef struct 
{ 
	char Block;
	char BlockIndex;  
	char Latitude[9];        //Î³¶È£¬¸ñÊ½Îªddmm.mmmm 
	char NS;                 //Î³¶È°ëÇò£¬N»òS(±±Î³»òÄÏÎ³)
	char Longitude[10];      //¾­¶È£¬¸ñÊ½Îªdddmm.mmmm 
	char EW;                 //¶È°ëÇò£¬E»òW(¶«¾­»òÎ÷¾­)
	char PositionFix;        //¶¨Î»ÖÊÁ¿Ö¸Ê¾£¬0=¶¨Î»ÎŞĞ§£¬1=¶¨Î»ÓĞĞ§£»
	char SateUsed[2];        //Ê¹ÓÃÎÀĞÇÊıÁ¿£¬´Ó00µ½12
	char HDOP[4];        	 //Ë®Æ½¾«È·¶È£¬0.5µ½99.9
	char HEIGHT[10];
	int  hdop_int;
//	char altitude[7];		//		 
}stru_GPSGGA; 



typedef struct 
{ 
	char SatelliteID[2];	//ÎÀĞÇ±àºÅ 
	char SNR[2];        	//0-99 dbHz 
}stru_SatelliteInfo; 



/*
$GPGSV£¬(1)£¬(2)£¬(3)£¬(4)£¬(5)£¬(6)£¬(7)£¬¡­(4),(5)£¬(6)£¬(7)*hh(CR)(LF) ¡¡¡¡
¸÷²¿·Öº¬ÒåÎª£º ¡¡¡¡
(1)×ÜµÄGSVÓï¾äµçÎÄÊı;
(2)µ±Ç°GSVÓï¾äºÅ; ¡¡¡¡
(3)¿ÉÊÓÎÀĞÇ×ÜÊı; ¡¡¡¡
(4)ÎÀĞÇºÅ; ¡¡¡¡¡¡¡¡
(7)ĞÅÔë±È(00¡«99dB); 

*/
typedef struct 
{ 
	char Block; 
	char BlockIndex; 
	char SateInView; 	//×ÜµÄGSVÓï¾äµçÎÄÊı
	char GSVID4 ;			//µ±Ç° GSVÓï¾ä±àºÅ
	int  Satellite_location;
	stru_SatelliteInfo SatelliteInfo[12]; 
}stru_GPSGSV;
void gpsDataInit(void);
void gpsDataIrq(char temp);
void gpsPowerOn(void);
void gpsPowerOff(int type);
void gpsDealChar(char *gps);
void showGpsData(void);

typedef struct 
{
	char mode[2];
	char type[2];
	char stars[4];
	char pdop[6];
	char hdop[6];
	char vdop[6];
	char check[2];
	char reserved[50];
		
}GPGSA;
typedef struct 
{
	char utc[12];
	char state[2];
	char latitude[12];		//ddmm.mmmm	 Î³¶Èddmm.mmmm£¨¶È·Ö£©¸ñÊ½
	char ns[2];				//ns
	char longitude[12];	 	//dddmm.mmmm	 ¾­¶Èdddmm.mmmm£¨¶È·Ö£©¸ñÊ½
	char ew[2];	
	char speed[6];			//speed  000.0~999.9
	char angle[8];			//direction relative north
	char utcDate[8];		//date
	char magAngle[6];		//´ÅÆ«½Ç 000.0~180.0
	char magDirection[2];	//´ÅÆ«½Ç·½Ïò£¬E£¨¶«£©»òW£¨Î÷£©
	char mode[2];  			//Ä£Ê½Ö¸Ê¾£¨½öNMEA0183 3.00°æ±¾Êä³ö£¬A=×ÔÖ÷¶¨Î»£¬D=²î·Ö£¬E=¹ÀËã£¬N=Êı¾İÎŞĞ§£©
		
}GPRMC;
typedef struct
{
	char utc[12];		//hhmmss.000
	char latitude[12];		//ddmm.mmmm
	char ns[2];				//ns
	char longitude[12];	 	//dddmm.mmmm
	char ew[2];				//ew
	char state[2];			//0=not£¬1=not diff£¬2=diff£¬6= cal ing
	char stars[4];		   	//working stars
	char hdop[6];			//Level of accuracy factor : 0.5~99.9
	char height[8];			//height : -9999.9~99999.9
	char height2[4]; 		//µØÇòÍÖÇòÃæÏà¶Ô´óµØË®×¼ÃæµÄ¸ß¶È 
	char diffTime[12];		//²î·ÖÊ±¼ä
	char diffId[6];			//²î·ÖÕ¾IDºÅ0000~1023
	char reserved1[2];
	char reserved2[2];	
}GPGGA;

extern GPGGA gpsGpgga;
extern GPRMC gpsGprmc;
#endif

