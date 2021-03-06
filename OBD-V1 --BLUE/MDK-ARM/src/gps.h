#ifndef GPS_H
#define GPS_H

void gpsTest(void);


typedef struct 
{ 
//定位坐标数据
	char A_Latitude[9];        //纬度ddmm.mmmm(度分)格式
	char A_NS;                 //纬度半球N(北半球)或S(南半球) 
	char A_Longitude[10];      //经度dddmm.mmmm(度分)格式 
	char A_EW;                 //经度半球E(东经)或W(西经)
	int  HDOP;        	 	   //水平精确度，0.5到99.9
	
	char A_SateUsed[2];        //使用卫星量，从00到12
	//未定位坐标数据
	char V_Latitude[9];        //纬度ddmm.mmmm(度分)格式
	char V_NS;                 //纬度半球N(北半球)或S(南半球) 
	char V_Longitude[10];      //经度dddmm.mmmm(度分)格式 
	char V_EW;                 //经度半球E(东经)或W(西经)
	
	char V_SateUsed[2];        //使用卫星数量，从00到12
	
	char Speed[5];           //地面速率(000.0~999.9节，前面的0也将被传输) 
	char Course[5];          //地面航向(000.0~359.9� 

}stru_GPS_DATA; 



/*
$GPRMC,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12> 
<1> UTC时间，hhmmss(时分秒)格式 
<2> 定位状态，A=有效定位，V=无效定位 
<3> 纬度ddmm.mmmm(度分)格式(前面的0也将被传输) 
<4> 纬度半球N(北半球)或S(南半球) 
<5> 经度dddmm.mmmm(度分)格式(前面的0也将被传输) 
<6> 经度半球E(东经)或W(西经) 
<7> 地面速率(000.0~999.9节，前面的0也将被传输) 
<8> 地面航向(000.0~359.9度，以真北为参考基准，前面的0也将被传输) 
<9> UTC日期，ddmmyy(日月年)格式 
<10> 磁偏角(000.0~180.0度，前面的0也将被传输) 
<11> 磁偏角方向，E(东)或W(西) 
<12> 模式指示(仅NMEA0183 3.00版本输出，A=自主定位，D=差分，E=估算，N=数据无效)
*/
typedef struct 
{ 
	char Block; 
	char BlockIndex; 
	char UTCTime[10];        //UTC时间，hhmmss(时分秒)格式 
	char Status;         	 //定位状态，A=有效定位，V=无效定位 
	char Latitude[9];        //纬度ddmm.mmmm(度分)格式
	char NS;                 //纬度半球N(北半球)或S(南半球) 
	char Longitude[10];      //经度dddmm.mmmm(度分)格式 
	char EW;                 //经度半球E(东经)或W(西经)
	char Speed[5];           //地面速率(000.0~999.9节，前面的0也将被传输) 
	char Course[5];          //地面航向(000.0~359.9度 
	char UTCDate[6];         //UTC日期，ddmmyy(日月年)格�
}stru_GPSRMC; 

/*
$GPGGA：起始引导符及语句格式说明(本句为GPS定位数据)；
<2>   纬度，格式为ddmm.mmmm(第一位是零也将传送)；
<3>   纬度半球，N或S(北纬或南纬)
<4>   经度，格式为dddmm.mmmm(第一位零也将传送)；
<5>   经度半球，E或W(东经或西经)
<6>   定位质量指示，0=定位无效，1=定位有效；
<7>   使用卫星数量，从00到12(第一个零也将传送)
<8>   水平精确度，0.5到99.9
<9>   天线离海平面的高度，-9999.9到9999.9米
M 指单位米
<10> 大地水准面高度，-9999.9到9999.9米
M 指单位米
<11> 差分GPS数据期限(RTCM SC-104)，最后设立RTCM传送的秒数量
<12>   差分参考基站标号，从0000到1023(首位0也将传送)。
* 语句结束标志符
*/
typedef struct 
{ 
	char Block;
	char BlockIndex;  
	char Latitude[9];        //纬度，格式为ddmm.mmmm 
	char NS;                 //纬度半球，N或S(北纬或南纬)
	char Longitude[10];      //经度，格式为dddmm.mmmm 
	char EW;                 //度半球，E或W(东经或西经)
	char PositionFix;        //定位质量指示，0=定位无效，1=定位有效；
	char SateUsed[2];        //使用卫星数量，从00到12
	char HDOP[4];        	 //水平精确度，0.5到99.9
	char HEIGHT[10];
	int  hdop_int;
//	char altitude[7];		//		 
}stru_GPSGGA; 



typedef struct 
{ 
	char SatelliteID[2];	//卫星编号 
	char SNR[2];        	//0-99 dbHz 
}stru_SatelliteInfo; 

/*
$GPGSV，(1)，(2)，(3)，(4)，(5)，(6)，(7)，…(4),(5)，(6)，(7)*hh(CR)(LF) 　　
各部分含义为： 　　
(1)总的GSV语句电文数;
(2)当前GSV语句号; 　　
(3)可视卫星总数; 　　
(4)卫星号; 　　　　
(7)信噪比(00～99dB); 

*/
typedef struct 
{ 
	char Block; 
	char BlockIndex; 
	char SateInView; 	//总的GSV语句电文数
	char GSVID4 ;			//当前 GSV语句编号
	int  Satellite_location;
	stru_SatelliteInfo SatelliteInfo[12]; 
}stru_GPSGSV;
void gpsDataInit(void);
void gpsDataIrq(char temp);
void gpsPowerOn(void);
void gpsPowerOff(int type);

#endif

