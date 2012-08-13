
#include "gps.h"
#include "sysdef.h"


//GPS���ݽ��յ��ù���
/*
���ȵ���gpsDataInit()��������GPS_DATA�ṹ���е����ݽ��и�λ

Ȼ��ѭ������gps_data_process������������ͣץȡGPS���ݣ����ж�λ��Ϣ��������ֹͣGPS��

*/	
//-------------------------------------------------------------- 
#define GPS_NULL     0x00 //GPS������� 
#define GPS_GPGGA    0x01 
#define GPS_GPGSV    0x04 
#define GPS_GPRMC    0x08 

int		GPSDataType=GPS_NULL; //GPS������� 
char	GPSDataTypeStrBuff[]="$GPxxx,"; //GPS������ͻ���, �ж�����ʱʹ��, 
int		GPSDataTypeStrBuffIndex=0; //GPS��������ַ����ĵ�ǰλ�� 
char	GPSDataStart=0; //GPS��俪ʼ. ��⵽ $ ʱ��1 
int		ReciveFlag=0; //���ݽ������. ���һ�� GPRMC ��䷢�������1
int		gps_data_process_temp=0;//gps���ݽ���������ʱ����
int		GPS_GGA_DINGWEI_OK=0;//��GPS��λ���򽫴˱�����1������Ϊ0
int		GPS_SHUJU_OK=0;//��GPS���������Ϣ���򽫴˱�����1������Ϊ0
int		GPS_RMC_DINGWEI_OK = 0;



extern int gps_data_value;

extern unsigned int SYSTEM_ON_TIME;	   //ϵͳ����ʱ����������ÿS�Լ�1

extern void delay_ms(unsigned int time);



stru_GPSRMC  	GPS_RMC_Data; 
stru_GPSGGA  	GPS_GGA_Data; 
stru_GPSGSV  	GPS_GSV_Data;
stru_GPS_DATA  	GPS_DATA;


int satellite_have_signal=0;
int satellite_below_10=0;
int satellite_below_20=0;
int satellite_below_30=0;
int satellite_below_40=0;
//------------------------------------------------------------------

void gpsPowerOn(void)
{
//	delay_ms(2000);
//	#ifdef PRINTF_DEBUG
	delay_ms(1500);
	printf("\r\nOPEN GPS Power........\r\n");
	printf("\r\nOPEN GPS Power........\r\n");
	printf("\r\nOPEN GPS Power........\r\n");
	printf("\r\nOPEN GPS Power........\r\n");
	printf("\r\nOPEN GPS Power........\r\n");
	printf("\r\nOPEN GPS Power........\r\n");
	printf("\r\nOPEN GPS Power........\r\n");
//	#endif

	GPS_EN(Bit_RESET);
	delay_ms(500);
	GPS_EN(Bit_SET);
	delay_ms(500);
					
}

void gpsPowerOff(int type)
{

	#ifdef PRINTF_DEBUG
	printf("\r\nCLOSE GPS\r\n");
	#endif
	GPS_EN(Bit_RESET);
}
void gpsTest(void)
{
	ISP_DIRECTION=USART_GPS;
	printf("\r\nGPS test\r\n");
	delay_ms(2000);
	gpsPowerOn();
	while(1);
}


void gpsDataInit(void)
{
	USART_Cmd(USART3, DISABLE); 
	#ifdef 	PRINTF_DEBUG
	printf("\r\nGPS Init------\r\n");
	#endif
	GPS_RMC_DINGWEI_OK = 0;
	GPS_GGA_DINGWEI_OK = 0;	
	for(gps_data_process_temp=0;gps_data_process_temp<9;gps_data_process_temp++)
	{
		GPS_DATA.A_Latitude[gps_data_process_temp]='x';
		GPS_DATA.V_Latitude[gps_data_process_temp]='x';
		GPS_DATA.A_Longitude[gps_data_process_temp]='x';
		GPS_DATA.V_Longitude[gps_data_process_temp]='x';
	}
	
	GPS_DATA.A_NS='x';
	GPS_DATA.V_NS='x';	        
   	GPS_DATA.A_EW='x';
	GPS_DATA.V_EW='x';
	    
 	for(gps_data_process_temp=0;gps_data_process_temp<5;gps_data_process_temp++)
	{
		GPS_DATA.Speed[gps_data_process_temp]='x';
		GPS_DATA.Course[gps_data_process_temp]='x';
	}

	GPS_DATA.HDOP=999;	 //ˮƽ��������Ϊ���

	GPS_GGA_DINGWEI_OK=0;//��GPS��λ���򽫴˱�����1������Ϊ0
	GPS_RMC_DINGWEI_OK=0;//��GPS��λ���򽫴˱�����1������Ϊ0
	GPS_SHUJU_OK=0;//��GPS���������Ϣ���򽫴˱�����1������Ϊ0

	GPS_DATA.A_SateUsed[0]='0';	   //��λ���Ǹ���
	GPS_DATA.A_SateUsed[1]='0';	   //��λ���Ǹ���

	GPS_DATA.V_SateUsed[0]='0';	   //��λ���Ǹ���
	GPS_DATA.V_SateUsed[1]='0';	   //��λ���Ǹ���


//�����ź�ͳ�Ʋ�����ʼ������
	satellite_have_signal=0;		 //���ź�ֵ�����Ǹ���
	satellite_below_10=0;			 //�ź�ֵ>10�����Ǹ���
	satellite_below_20=0;			 //�ź�ֵ>20�����Ǹ���
	satellite_below_30=0;			 //�ź�ֵ>30�����Ǹ���
	satellite_below_40=0;			 //�ź�ֵ>40�����Ǹ���

	GPSDataStart = 0;
	GPSDataType = GPS_NULL;
	GPSDataTypeStrBuffIndex = 0;
	memset(GPSDataTypeStrBuff,'\0',7);
	
	USART_Cmd(USART3, ENABLE);
}
void gpsDataIrq(char temp)
{
	if(GPSDataStart) 
	{		//�Ѿ����յ�GPS���ͷ�ַ�$ 
		switch(GPSDataTypeStrBuffIndex) 
		{		//�Ѿ���ȡ�˶��ٸ���ʾGPS�������͵��ַ�
			case 6: 
			{		//������ͷ�Ѿ�ȫ����ȡ, ��ʼ�ж� 
				if(GPSDataTypeStrBuff[3]=='G'&&GPSDataTypeStrBuff[4]=='G'&&GPSDataTypeStrBuff[5]=='A') 
				{	
					GPSDataType=GPS_GPGGA;

			//		if(SYSTEM_PC!=7&&SYSTEM_PC!=8)
			//		GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);

					//GGA ���� ��ʼ�� ���㣬ÿ�����½���ǰ����Ϊ��x���������жϱ��������Ƿ���£�
					for(gps_data_process_temp=0;gps_data_process_temp<9;gps_data_process_temp++)
						GPS_GGA_Data.Latitude[gps_data_process_temp]='x'; 

					GPS_GGA_Data.NS='x'; 

					for(gps_data_process_temp=0;gps_data_process_temp<10;gps_data_process_temp++)
					GPS_GGA_Data.Longitude[gps_data_process_temp]='x'; 

					GPS_GGA_Data.EW='x';
					GPS_GGA_Data.PositionFix='x';
					for(gps_data_process_temp=0;gps_data_process_temp<2;gps_data_process_temp++)
					GPS_GGA_Data.SateUsed[gps_data_process_temp]='x'; 

					for(gps_data_process_temp=0;gps_data_process_temp<4;gps_data_process_temp++)
					{
						GPS_GGA_Data.HDOP[gps_data_process_temp]='x';
					}
					for(gps_data_process_temp=0;gps_data_process_temp<10;gps_data_process_temp++)
					{
						GPS_GGA_Data.HEIGHT[gps_data_process_temp]=0;
					}
				}

				if(GPSDataTypeStrBuff[5]=='V') 
				{

					GPSDataType=GPS_GPGSV;
					for(gps_data_process_temp=0;gps_data_process_temp<12;gps_data_process_temp++)
					{ 
						GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SatelliteID[0]='x'; 
						GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SatelliteID[1]='x';
						GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[0]='x'; 
						GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[1]='x';
					}
				}

				if(GPSDataTypeStrBuff[5]=='C') 
				{
					GPSDataType=GPS_GPRMC;
					
					for(gps_data_process_temp=0;gps_data_process_temp<10;gps_data_process_temp++)
					GPS_RMC_Data.UTCTime[gps_data_process_temp]='x';
					
					GPS_RMC_Data.Status='x';
					
					for(gps_data_process_temp=0;gps_data_process_temp<9;gps_data_process_temp++) 
						GPS_RMC_Data.Latitude[gps_data_process_temp]='x';
						
					GPS_RMC_Data.NS='x';

					for(gps_data_process_temp=0;gps_data_process_temp<10;gps_data_process_temp++)
						GPS_RMC_Data.Longitude[gps_data_process_temp]='x';

					GPS_RMC_Data.EW='x';
					
					for(gps_data_process_temp=0;gps_data_process_temp<5;gps_data_process_temp++) 
					GPS_RMC_Data.Speed[gps_data_process_temp]='x';

					for(gps_data_process_temp=0;gps_data_process_temp<5;gps_data_process_temp++)
						GPS_RMC_Data.Course[gps_data_process_temp]='x';
					
					for(gps_data_process_temp=0;gps_data_process_temp<6;gps_data_process_temp++) 
						GPS_RMC_Data.UTCDate[gps_data_process_temp]='x';
				}
				if(temp==',') GPSDataTypeStrBuffIndex=255; //�ж����, ���ݽ��տ�ʼ
				break;
			}
			case 255: 
			{		//GPS�������͵��ַ�ȫ����ȡ���ж����, ���������� 
				switch(GPSDataType) 
				{
					case GPS_GPGGA: 
					{ 
						switch(temp)
						{
							case'*':		//������
								GPSDataStart=0;
								if(GPS_GGA_Data.PositionFix!='0')
								{
									if(GPS_GGA_Data.HDOP[1]=='.')
									{
										GPS_GGA_Data.hdop_int=(GPS_GGA_Data.HDOP[0]-'0')*10+(GPS_GGA_Data.HDOP[2]-'0');
									}
									else if(GPS_GGA_Data.HDOP[2]=='.')
									{
										GPS_GGA_Data.hdop_int=(GPS_GGA_Data.HDOP[0]-'0')*100+(GPS_GGA_Data.HDOP[1]-'0')*10+(GPS_GGA_Data.HDOP[3]-'0');																																		
									}
									else
									{
										GPS_GGA_Data.hdop_int=1000;		
									}
									if(GPS_GGA_Data.hdop_int<=GPS_DATA.HDOP)
									{
									//���Ӷ�λ����ѡ��GGA����ͨ��4λ�ַ����ա���������ת����int�͡�
										for(gps_data_process_temp=0;gps_data_process_temp<9;gps_data_process_temp++)
										GPS_DATA.A_Latitude[gps_data_process_temp]=GPS_GGA_Data.Latitude[gps_data_process_temp];
										for(gps_data_process_temp=0;gps_data_process_temp<10;gps_data_process_temp++)
										GPS_DATA.A_Longitude[gps_data_process_temp]=GPS_GGA_Data.Longitude[gps_data_process_temp];
										GPS_DATA.A_NS =GPS_GGA_Data.NS; 
										GPS_DATA.A_EW=GPS_GGA_Data.EW;

										GPS_DATA.A_SateUsed[0]=GPS_GGA_Data.SateUsed[0];
										GPS_DATA.A_SateUsed[1]=GPS_GGA_Data.SateUsed[1];

										GPS_DATA.HDOP=GPS_GGA_Data.hdop_int;

										GPS_GGA_DINGWEI_OK=1;//��GPS��λ���򽫴˱�����1������Ϊ0
									}
								}
								else
								{
									//		if(SYSTEM_PC!=7&&SYSTEM_PC!=8)
									//		GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET);
//																	printf("\r\nPOSITION FIX :  %c  \r\n",GPS_GGA_Data.PositionFix);
								} 
										     break; 
								case ',':       //���ֶν���, ��һ�� 
									GPS_GGA_Data.Block++; 
									GPS_GGA_Data.BlockIndex=0;      //�ֶ�������0��һ���ַ�
									break; 
								default: 
								{                      //�ֶ��ַ� 
									switch(GPS_GGA_Data.Block) 
									{			//�жϵ�ǰ�����ĸ��ֶ� 
									
										case 1: 
											GPS_GGA_Data.Latitude[GPS_GGA_Data.BlockIndex]=temp; 
											break; 
										case 2: 
											GPS_GGA_Data.NS=temp; 
											break; 
										case 3: 
											GPS_GGA_Data.Longitude[GPS_GGA_Data.BlockIndex]=temp; 
											break; 
										case 4: 
											GPS_GGA_Data.EW=temp; 
											break; 
										case 5: 
											GPS_GGA_Data.PositionFix=temp;
											break;
										case 6: 
											GPS_GGA_Data.SateUsed[GPS_GGA_Data.BlockIndex]=temp; 
											break;
										case 7: 
											GPS_GGA_Data.HDOP[GPS_GGA_Data.BlockIndex]=temp; 
										case 8: //��ȡ�߶�����
											GPS_GGA_Data.HEIGHT[GPS_GGA_Data.BlockIndex]=temp; 
											break;
//										
										default:break; 
									}
									GPS_GGA_Data.BlockIndex++;        //�ֶ�����++, ָ����һ���ַ�  
								}
				}
						break; 
					}
					case GPS_GPRMC: 
					{
						switch(temp) 
						{
							case '*': 
							{
								GPSDataStart=0; 
								GPSDataTypeStrBuffIndex=0; 
								ReciveFlag=1;       //�������, ���Դ���																		//
												//���GPS��δ��λ�������ݣ�������Ϣ��GPS_DATA�ṹ����
								if(GPS_RMC_Data.Status == 'A') //����ٶȺͷ���
								{
									for(gps_data_process_temp=0;gps_data_process_temp<9;gps_data_process_temp++)
									GPS_DATA.V_Latitude[gps_data_process_temp]=GPS_RMC_Data.Latitude[gps_data_process_temp];

									for(gps_data_process_temp=0;gps_data_process_temp<10;gps_data_process_temp++)
									GPS_DATA.V_Longitude[gps_data_process_temp]=GPS_RMC_Data.Longitude[gps_data_process_temp];
									
									GPS_DATA.V_NS =GPS_RMC_Data.NS; 
									GPS_DATA.V_EW=GPS_RMC_Data.EW;

									for(gps_data_process_temp=0;gps_data_process_temp<5;gps_data_process_temp++)
										GPS_DATA.Speed[gps_data_process_temp]=GPS_RMC_Data.Speed[gps_data_process_temp];
									//����
									for(gps_data_process_temp=0;gps_data_process_temp<5;gps_data_process_temp++)
									{
										GPS_DATA.Course[gps_data_process_temp]=GPS_RMC_Data.Course[gps_data_process_temp];
									}
									GPS_DATA.V_SateUsed[0]=GPS_GGA_Data.SateUsed[0];
									GPS_DATA.V_SateUsed[1]=GPS_GGA_Data.SateUsed[1];
									GPS_RMC_DINGWEI_OK = 1;
									return;
								}
								if(GPS_RMC_Data.NS!='x')
								{
									for(gps_data_process_temp=0;gps_data_process_temp<9;gps_data_process_temp++)
									GPS_DATA.V_Latitude[gps_data_process_temp]=GPS_RMC_Data.Latitude[gps_data_process_temp];

									for(gps_data_process_temp=0;gps_data_process_temp<10;gps_data_process_temp++)
									GPS_DATA.V_Longitude[gps_data_process_temp]=GPS_RMC_Data.Longitude[gps_data_process_temp];
									
									GPS_DATA.V_NS =GPS_RMC_Data.NS; 
									GPS_DATA.V_EW=GPS_RMC_Data.EW;
									//�ٶ�
									for(gps_data_process_temp=0;gps_data_process_temp<5;gps_data_process_temp++)
										GPS_DATA.Speed[gps_data_process_temp]=GPS_RMC_Data.Speed[gps_data_process_temp];
									//����
									for(gps_data_process_temp=0;gps_data_process_temp<5;gps_data_process_temp++)
									{
										GPS_DATA.Course[gps_data_process_temp]=GPS_RMC_Data.Course[gps_data_process_temp];
									}
									GPS_DATA.V_SateUsed[0]=GPS_GGA_Data.SateUsed[0];
									GPS_DATA.V_SateUsed[1]=GPS_GGA_Data.SateUsed[1];
									GPS_SHUJU_OK=1;//GPS���������Ϣ���򽫴˱�����1	 
								}
								else
								{
							//		if(SYSTEM_PC!=7&&SYSTEM_PC!=8)
							//		GPIO_WriteBit(GPIOB,GPIO_Pin_7,Bit_SET);
								}
								break;
							}
							case ',':
							{ 
								GPS_RMC_Data.Block++; 
								GPS_RMC_Data.BlockIndex=0; 
								break; 
							}
							default:
							{ 
								switch(GPS_RMC_Data.Block)
								{ 
									case 0: 
											GPS_RMC_Data.UTCTime[GPS_RMC_Data.BlockIndex]=temp; 
											break; 
									case 1: 
											GPS_RMC_Data.Status=temp; 
											break; 
									case 2: 
											GPS_RMC_Data.Latitude[GPS_RMC_Data.BlockIndex]=temp; 
											break; 
									case 3: 
											GPS_RMC_Data.NS=temp; 
											break; 
									case 4: 
											GPS_RMC_Data.Longitude[GPS_RMC_Data.BlockIndex]=temp; 
											break; 
									case 5: 
											GPS_RMC_Data.EW=temp; 
											break; 
									case 6: 
											GPS_RMC_Data.Speed[GPS_RMC_Data.BlockIndex]=temp; 
											break; 
									case 7: 
											GPS_RMC_Data.Course[GPS_RMC_Data.BlockIndex]=temp; 
											break; 
									case 8: 
											GPS_RMC_Data.UTCDate[GPS_RMC_Data.BlockIndex]=temp; 
											break;
									
									default:break; 
								}
								GPS_RMC_Data.BlockIndex++;
							}
						}
						break;
					}
					case GPS_GPGSV: 
					{
						switch(temp) 
						{ 
							case '*': 
							GPSDataStart=0; 

							if(GPS_GSV_Data.GSVID4=='1')
							{	
								satellite_have_signal=0;		 //���ź�ֵ�����Ǹ���
								satellite_below_10=0;			 //�ź�ֵ>10�����Ǹ���
								satellite_below_20=0;			 //�ź�ֵ>20�����Ǹ���
								satellite_below_30=0;			 //�ź�ֵ>30�����Ǹ���
								satellite_below_40=0;			 //�ź�ֵ>40�����Ǹ���			

								for(gps_data_process_temp=0;gps_data_process_temp<4;gps_data_process_temp++)
								{
									if(GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[1]!='x')
									{
										satellite_have_signal++;
										
										if(((GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[0]-'0')*10+(GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[1]-'0'))>=10)
										{
											satellite_below_10++;	
										}	

										if(((GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[0]-'0')*10+(GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[1]-'0'))>=20)
										{
											satellite_below_20++;	
										}

										if(((GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[0]-'0')*10+(GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[1]-'0'))>=30)
										{
											satellite_below_30++;	
										}	

										if(((GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[0]-'0')*10+(GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[1]-'0'))>=40)
										{
											satellite_below_40++;	
										}	
									}
								}
							}
							else if(GPS_GSV_Data.GSVID4=='2')
							{	
								for(gps_data_process_temp=4;gps_data_process_temp<8;gps_data_process_temp++)
								{
									if(GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[1]!='x')
									{
										satellite_have_signal++;
										
										if(((GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[0]-'0')*10+(GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[1]-'0'))>=10)
										{
											satellite_below_10++;	
										}	

										if(((GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[0]-'0')*10+(GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[1]-'0'))>=20)
										{
											satellite_below_20++;	
										}	

										if(((GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[0]-'0')*10+(GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[1]-'0'))>=30)
										{
											satellite_below_30++;	
										}	

										if(((GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[0]-'0')*10+(GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[1]-'0'))>=40)
										{
											satellite_below_40++;	
										}	
									}
								}
							}
							else if(GPS_GSV_Data.GSVID4=='3')
							{	
								for(gps_data_process_temp=8;gps_data_process_temp<12;gps_data_process_temp++)
								{
									if(GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[1]!='x')
									{
										satellite_have_signal++;
										
										if(((GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[0]-'0')*10+(GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[1]-'0'))>=10)
										{
											satellite_below_10++;	
										}	

										if(((GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[0]-'0')*10+(GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[1]-'0'))>=20)
										{
											satellite_below_20++;	
										}	

										if(((GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[0]-'0')*10+(GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[1]-'0'))>=30)
										{
											satellite_below_30++;	
										}	

										if(((GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[0]-'0')*10+(GPS_GSV_Data.SatelliteInfo[gps_data_process_temp].SNR[1]-'0'))>=40)
										{
											satellite_below_40++;	
										}	
									}
								}
							}
							break; 
							case ',': 
								GPS_GSV_Data.Block++; 
								GPS_GSV_Data.BlockIndex=0; 
								break;
								default:
								{
									switch(GPS_GSV_Data.Block) 
									{

										case 0:
												GPS_GSV_Data.SateInView=temp; 	//�ܵ�GSV��������
												break;
                                        case 1: 
                                                GPS_GSV_Data.GSVID4=temp;    //��ǰGPFSV�������, ����ż����������������Ӧ�ô����������ĸ�λ��
												if(GPS_GSV_Data.GSVID4=='1')
												GPS_GSV_Data.Satellite_location=0;
												else if(GPS_GSV_Data.GSVID4=='2')
												GPS_GSV_Data.Satellite_location=4;
												else if(GPS_GSV_Data.GSVID4=='3')
												GPS_GSV_Data.Satellite_location=8;
												else
												GPS_GSV_Data.Satellite_location=8;
												
												break; 
										case 2: break; 
											
										case 3: 
												GPS_GSV_Data.SatelliteInfo[GPS_GSV_Data.Satellite_location].SatelliteID[GPS_GSV_Data.BlockIndex]=temp; 
												break;
										
										case 4: break; 
										
										case 5: break; 
										
										case 6:
												GPS_GSV_Data.SatelliteInfo[GPS_GSV_Data.Satellite_location].SNR[GPS_GSV_Data.BlockIndex]=temp; 
										break;
										
										case 7:
												GPS_GSV_Data.SatelliteInfo[GPS_GSV_Data.Satellite_location+1].SatelliteID[GPS_GSV_Data.BlockIndex]=temp; 
										break;
										
										case 8:
												break; 
										
										case 9: break; 
										
										case 10:
												GPS_GSV_Data.SatelliteInfo[GPS_GSV_Data.Satellite_location+1].SNR[GPS_GSV_Data.BlockIndex]=temp; 
										break; 
										
										case 11:
												GPS_GSV_Data.SatelliteInfo[GPS_GSV_Data.Satellite_location+2].SatelliteID[GPS_GSV_Data.BlockIndex]=temp; 
										break; 
										
										case 12:break; 
										
										case 13:break;
										
										case 14:
												GPS_GSV_Data.SatelliteInfo[GPS_GSV_Data.Satellite_location+2].SNR[GPS_GSV_Data.BlockIndex]=temp; 
										break; 
										
										case 15:
												GPS_GSV_Data.SatelliteInfo[GPS_GSV_Data.Satellite_location+3].SatelliteID[GPS_GSV_Data.BlockIndex]=temp; 
										break; 
										
										case 16:break;
										
										case 17:break;
										
										case 18:
												GPS_GSV_Data.SatelliteInfo[GPS_GSV_Data.Satellite_location+3].SNR[GPS_GSV_Data.BlockIndex]=temp; 
										break;
										
										default:break; 
									}
									GPS_GSV_Data.BlockIndex++; 
								} 
							} 
						break; 
					}
					default: 
						{       //GPS�����ж�ʧ��, ����δ���յ�GPS���($)��״̬, ����ѭ��
							GPSDataStart=0;
						} 
				} 
				break; 
			}
			default: 
			{											//δ�ж�, ��������, Index++ 
				GPSDataTypeStrBuff[GPSDataTypeStrBuffIndex]=temp; 
				GPSDataTypeStrBuffIndex++; 
				if(GPSDataTypeStrBuffIndex>7)
					GPSDataStart = 0;
			}
		}
	} 
	else 
	{													//δ�������ͷ�ַ�, �������ղ��ж�
		if (temp=='$') 
		{												//���յ�$, ��һ���ַ���Ϊ�����ж��ַ�, �Ƚ�����ر�����ʼ�� 
			memset(GPSDataTypeStrBuff,'\0',7);
			GPSDataTypeStrBuff[0]=temp; 
			GPSDataStart=1;								//��ͷ���GPS�����ַ�������
			GPSDataType=GPS_NULL;
			GPSDataTypeStrBuffIndex=1;
			GPS_RMC_Data.Block=0;						//ÿ�����ı�־λ
			GPS_RMC_Data.BlockIndex=0;
			GPS_GSV_Data.Block=0;
			GPS_GSV_Data.BlockIndex=0;
			GPS_GGA_Data.Block=0;
			GPS_GGA_Data.BlockIndex=0;
			ReciveFlag=0;//��ʼ
		}
	}
}

//void debugPrintf(void)
//{
//	printf("\r\nGPSDataStart:%d\r\n",GPSDataStart);
//	printf("\r\nGPS_GGA_DINGWEI_OK:%d\r\n",GPS_GGA_DINGWEI_OK);
//	printf("\r\nGPS_RMC_DINGWEI_OK:%d\r\n",GPS_RMC_DINGWEI_OK);
//	printf("\r\nGPS_SHUJU_OK:%d\r\n",GPS_SHUJU_OK);
//
//}	



		  
