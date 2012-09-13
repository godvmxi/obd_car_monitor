#ifndef GPS_H
#define GPS_H

void gpsTest(void);


typedef struct 
{ 
//��λ��������
	char A_Latitude[9];        //γ��ddmm.mmmm(�ȷ�)��ʽ
	char A_NS;                 //γ�Ȱ���N(������)��S(�ϰ���) 
	char A_Longitude[10];      //����dddmm.mmmm(�ȷ�)��ʽ 
	char A_EW;                 //���Ȱ���E(����)��W(����)
	int  HDOP;        	 	   //ˮƽ��ȷ�ȣ�0.5��99.9
	
	char A_SateUsed[2];        //ʹ������������00��12
	//δ��λ��������
	char V_Latitude[9];        //γ��ddmm.mmmm(�ȷ�)��ʽ
	char V_NS;                 //γ�Ȱ���N(������)��S(�ϰ���) 
	char V_Longitude[10];      //����dddmm.mmmm(�ȷ�)��ʽ 
	char V_EW;                 //���Ȱ���E(����)��W(����)
	
	char V_SateUsed[2];        //ʹ��������������00��12
	
	char Speed[5];           //��������(000.0~999.9�ڣ�ǰ���0Ҳ��������) 
	char Course[5];          //���溽��(000.0~359.9� 

}stru_GPS_DATA; 



/*
$GPRMC,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12> 
<1> UTCʱ�䣬hhmmss(ʱ����)��ʽ 
<2> ��λ״̬��A=��Ч��λ��V=��Ч��λ 
<3> γ��ddmm.mmmm(�ȷ�)��ʽ(ǰ���0Ҳ��������) 
<4> γ�Ȱ���N(������)��S(�ϰ���) 
<5> ����dddmm.mmmm(�ȷ�)��ʽ(ǰ���0Ҳ��������) 
<6> ���Ȱ���E(����)��W(����) 
<7> ��������(000.0~999.9�ڣ�ǰ���0Ҳ��������) 
<8> ���溽��(000.0~359.9�ȣ����汱Ϊ�ο���׼��ǰ���0Ҳ��������) 
<9> UTC���ڣ�ddmmyy(������)��ʽ 
<10> ��ƫ��(000.0~180.0�ȣ�ǰ���0Ҳ��������) 
<11> ��ƫ�Ƿ���E(��)��W(��) 
<12> ģʽָʾ(��NMEA0183 3.00�汾�����A=������λ��D=��֣�E=���㣬N=������Ч)
*/
typedef struct 
{ 
	char Block; 
	char BlockIndex; 
	char UTCTime[10];        //UTCʱ�䣬hhmmss(ʱ����)��ʽ 
	char Status;         	 //��λ״̬��A=��Ч��λ��V=��Ч��λ 
	char Latitude[9];        //γ��ddmm.mmmm(�ȷ�)��ʽ
	char NS;                 //γ�Ȱ���N(������)��S(�ϰ���) 
	char Longitude[10];      //����dddmm.mmmm(�ȷ�)��ʽ 
	char EW;                 //���Ȱ���E(����)��W(����)
	char Speed[5];           //��������(000.0~999.9�ڣ�ǰ���0Ҳ��������) 
	char Course[5];          //���溽��(000.0~359.9�� 
	char UTCDate[6];         //UTC���ڣ�
}stru_GPSRMC; 

/*
$GPGGA����ʼ������������ʽ˵��(����ΪGPS��λ����)��
<2>   γ�ȣ���ʽΪddmm.mmmm(��һλ����Ҳ������)��
<3>   γ�Ȱ���N��S(��γ����γ)
<4>   ���ȣ���ʽΪdddmm.mmmm(��һλ��Ҳ������)��
<5>   ���Ȱ���E��W(����������)
<6>   ��λ����ָʾ��0=��λ��Ч��1=��λ��Ч��
<7>   ʹ��������������00��12(��һ����Ҳ������)
<8>   ˮƽ��ȷ�ȣ�0.5��99.9
<9>   �����뺣ƽ��ĸ߶ȣ�-9999.9��9999.9��
M ָ��λ��
<10> ���ˮ׼��߶ȣ�-9999.9��9999.9��
M ָ��λ��
<11> ���GPS��������(RTCM SC-104)���������RTCM���͵�������
<12>   ��ֲο���վ��ţ���0000��1023(��λ0Ҳ������)��
* ��������־��
*/
typedef struct 
{ 
	char Block;
	char BlockIndex;  
	char Latitude[9];        //γ�ȣ���ʽΪddmm.mmmm 
	char NS;                 //γ�Ȱ���N��S(��γ����γ)
	char Longitude[10];      //���ȣ���ʽΪdddmm.mmmm 
	char EW;                 //�Ȱ���E��W(����������)
	char PositionFix;        //��λ����ָʾ��0=��λ��Ч��1=��λ��Ч��
	char SateUsed[2];        //ʹ��������������00��12
	char HDOP[4];        	 //ˮƽ��ȷ�ȣ�0.5��99.9
	char HEIGHT[10];
	int  hdop_int;
//	char altitude[7];		//		 
}stru_GPSGGA; 



typedef struct 
{ 
	char SatelliteID[2];	//���Ǳ�� 
	char SNR[2];        	//0-99 dbHz 
}stru_SatelliteInfo; 



/*
$GPGSV��(1)��(2)��(3)��(4)��(5)��(6)��(7)����(4),(5)��(6)��(7)*hh(CR)(LF) ����
�����ֺ���Ϊ�� ����
(1)�ܵ�GSV��������;
(2)��ǰGSV����; ����
(3)������������; ����
(4)���Ǻ�; ��������
(7)�����(00��99dB); 

*/
typedef struct 
{ 
	char Block; 
	char BlockIndex; 
	char SateInView; 	//�ܵ�GSV��������
	char GSVID4 ;			//��ǰ GSV�����
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
	char latitude[12];		//ddmm.mmmm	 γ��ddmm.mmmm���ȷ֣���ʽ
	char ns[2];				//ns
	char longitude[12];	 	//dddmm.mmmm	 ����dddmm.mmmm���ȷ֣���ʽ
	char ew[2];	
	char speed[6];			//speed  000.0~999.9
	char angle[8];			//direction relative north
	char utcDate[8];		//date
	char magAngle[6];		//��ƫ�� 000.0~180.0
	char magDirection[2];	//��ƫ�Ƿ���E��������W������
	char mode[2];  			//ģʽָʾ����NMEA0183 3.00�汾�����A=������λ��D=��֣�E=���㣬N=������Ч��
		
}GPRMC;
typedef struct
{
	char utc[12];		//hhmmss.000
	char latitude[12];		//ddmm.mmmm
	char ns[2];				//ns
	char longitude[12];	 	//dddmm.mmmm
	char ew[2];				//ew
	char state[2];			//0=not��1=not diff��2=diff��6= cal ing
	char stars[4];		   	//working stars
	char hdop[6];			//Level of accuracy factor : 0.5~99.9
	char height[8];			//height : -9999.9~99999.9
	char height2[4]; 		//������������Դ��ˮ׼��ĸ߶� 
	char diffTime[12];		//���ʱ��
	char diffId[6];			//���վID��0000~1023
	char reserved1[2];
	char reserved2[2];	
}GPGGA;

extern GPGGA gpsGpgga;
extern GPRMC gpsGprmc;
#endif

