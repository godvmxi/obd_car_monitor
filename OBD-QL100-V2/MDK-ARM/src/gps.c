
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
				if(GPSDataTypeStrBuff[3]=='G'&&GPSDataTypeStrBuff[4]=='G'&&GPSDataTypeStrBuff[5]=='A')//$GPGGA 
				{	
					GPSDataType=GPS_GPGGA;

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

				if(GPSDataTypeStrBuff[5]=='V') 	//$GPGSV
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

				if(GPSDataTypeStrBuff[5]=='C') //$GPRMC
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
										default:
											break; 
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
			GPSDataTypeStrBuffIndex=1;
			GPSDataStart=1;								//��ͷ���GPS�����ַ�������
			GPSDataType=GPS_NULL;			
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
//typedef struct
//{
//	char utc[12];		//hhmmss.000
//	char longitude[12];		//ddmm.mmmm
//	char ns[2];				//ns
//	char latitude[12];	 	//dddmm.mmmm
//	char ew[2];				//ew
//	char state[2];			//0=not��1=not diff��2=diff��6= cal ing
//	char stars[4];		   	//working stars
//	char hdop[6];			//Level of accuracy factor : 0.5~99.9
//	char height[8];			//height : -9999.9~99999.9
//	char height2[4]; 		//������������Դ��ˮ׼��ĸ߶� 
//	char diffTime[12];		//���ʱ��
//	char diffId[6];			//���վID��0000~1023
//	char reserved1[2];
//	char reserved2[2];	
//}GPGGA;
//
//typedef struct 
//{
//	char mode[2];
//	char type[2];
//	char stars[4];
//	char pdop[6];
//	char hdop[6];
//	char vdop[6];
//	char check[2];
//	char reserved[50];
//		
//}GPGSA;
//typedef struct 
//{
//	char utc[12];
//	char state[2];
//	char longitude[12];		//ddmm.mmmm
//	char ns[2];				//ns
//	char latitude[12];	 	//dddmm.mmmm
//	char ew[2];	
//	char speed[6];			//speed  000.0~999.9
//	char angle[6];			//direction relative north
//	char utcDate[8];		//date
//	char magAngle[6];		//��ƫ�� 000.0~180.0
//	char magDirection[2];	//��ƫ�Ƿ���E��������W������
//	char mode[2];  			//ģʽָʾ����NMEA0183 3.00�汾�����A=������λ��D=��֣�E=���㣬N=������Ч��
//		
//}GPRMC;


GPGGA gpsGpgga;
GPRMC gpsGprmc;

#define GPS_MAX_CHARS 100
#define GPS_MIN_CHARS 10
char GPS_BUF[GPS_MAX_CHARS];
int16_t GPS_INDEX = 0;
__inline  char  *mymemccpy(char **des,char *src,char *ch,int len)
{
	return NULL;
}
void copyGpsData(){
	//CUT DOWN THE HEAD AND END
	char *point;//jump to the first data
	int32_t i = 0;
	point = GPS_BUF+6;
	if(GPS_BUF[2] == 'R'&&GPS_BUF[3] == 'M'&&GPS_BUF[4] == 'C'){//RMC 
		if(GPS_INDEX < GPS_MIN_CHARS)
			return;

		for(i = 0;point[i] != ',';i++)
		{
			gpsGprmc.utc[i] = point[i];	
		}
		if(i > 12)
			return;
		point = point + i + 1;

		for(i = 0;point[i] != ',';i++)
		{
			gpsGprmc.state[i] = point[i];	
		}
		if(i > 2)
			return;
		point = point + i + 1;
		
		for(i = 0;point[i] != ',';i++)
		{
			gpsGprmc.latitude[i] = point[i];	
		}
		if(i > 12)
			return;
		point = point + i + 1;
		
		for(i = 0;point[i] != ',';i++)
		{
			gpsGprmc.ns[i] = point[i];	
		}
		if(i > 12)
			return;
		point = point + i + 1;
		
		for(i = 0;point[i] != ',';i++)
		{
			gpsGprmc.longitude[i] = point[i];	
		}
		if(i > 12)
			return;
		point = point + i + 1;
		
		for(i = 0;point[i] != ',';i++)
		{
			gpsGprmc.ew[i] = point[i];	
		}
		if(i > 12)
			return;
		point = point + i + 1;
		
		for(i = 0;point[i] != ',';i++)
		{
			gpsGprmc.speed[i] = point[i];	
		}
		if(i > 12)
			return;
		point = point + i + 1;

		for(i = 0;point[i] != ',';i++)
		{
			gpsGprmc.angle[i] = point[i];	
		}
		if(i > 12)
			return;
		point = point + i + 1;

		for(i = 0;point[i] != ',';i++)
		{
			gpsGprmc.utcDate[i] = point[i];	
		}
		if(i > 12)
			return;
		point = point + i + 1;

		for(i = 0;point[i] != ',';i++)
		{
			gpsGprmc.magAngle[i] = point[i];	
		}
		if(i > 12)
			return;
		point = point + i + 1;

		for(i = 0;point[i] != ',';i++)
		{
			gpsGprmc.magDirection[i] = point[i];	
		}
		if(i > 12)
			return;
		point = point + i + 1;

		for(i = 0;point[i] != '*';i++)
		{
			gpsGprmc.mode[i] = point[i];
			if(i > 2)
				return;	
		}
		return;								
	}
	///+++++++++++++++++++++++++++++++++++++++++++++++++++/////
	if(GPS_BUF[2] == 'G'&&GPS_BUF[3] == 'G'&&GPS_BUF[4] == 'A'){//GGA
		if(GPS_INDEX < GPS_MIN_CHARS)
			return;
		for(i = 0;point[i] != ',';i++)
		{
			gpsGpgga.utc[i] = point[i];	
		}
		if(i > 12)
			return;
		point = point + i +1;

		for(i = 0;point[i] != ',';i++)
		{
			gpsGpgga.latitude[i] = point[i];	
		}
		if(i > 12)
			return;
		point = point + i+1;

		for(i = 0;point[i] != ',';i++)
		{
			gpsGpgga.ns[i] = point[i];	
		}
		if(i > 12)
			return;
		point = point + i +1;

		for(i = 0;point[i] != ',';i++)
		{
			gpsGpgga.longitude[i] = point[i];	
		}
		if(i > 12)
			return;
		point = point + i +1;

		
		for(i = 0;point[i] != ',';i++)
		{
			gpsGpgga.ew[i] = point[i];	
		}
		if(i > 12)
			return;
		point = point + i +1;
		
		for(i = 0;point[i] != ',';i++)
		{
			gpsGpgga.state[i] = point[i];	
		}
		if(i > 12)
			return;
		point = point + i +1;
		
		for(i = 0;point[i] != ',';i++)
		{
			gpsGpgga.stars[i] = point[i];	
		}
		if(i > 12)
			return;
		point = point + i +1;
		
		for(i = 0;point[i] != ',';i++)
		{
			gpsGpgga.hdop[i] = point[i];	
		}
		if(i > 12)
			return;
		point = point + i +1;
		
		for(i = 0;point[i] != ',';i++)
		{
			gpsGpgga.height[i] = point[i];	
		}
		if(i > 12)
			return;
		point = point + i +1;
		
		for(i = 0;point[i] != ',';i++)
		{
			gpsGpgga.height2[i] = point[i];	
		}
		if(i > 12)
			return;
		point = point + i +1;
		
		for(i = 0;point[i] != ',';i++)
		{
			gpsGpgga.diffTime[i] = point[i];	
		}
		if(i > 12)
			return;
		point = point + i +1;

		
		for(i = 0;point[i] != ',';i++)
		{
			gpsGpgga.diffId[i] = point[i];
			if(i > 2)
				return;		
		}
		
		return;

	}
}
void gpsDealChar(char *gps){
	switch(*gps){
		case '$':
			GPS_INDEX = 0;
			memset(GPS_BUF,0,GPS_MAX_CHARS);
			break;
		case '*'://we have two char can be miss ,time may be enough
			//ok analyse gps data
			GPS_BUF[GPS_INDEX++] = *gps;
			copyGpsData();
			break;
		default:
			GPS_BUF[GPS_INDEX++] = *gps;
			if(GPS_INDEX >= GPS_MAX_CHARS)
			{
				GPS_INDEX = 0;
				memset(GPS_BUF,0,GPS_MAX_CHARS);	
			}
			break;
	} 
}
void showGpsData(void){
	printf("\r\nGGA\r\n");
	printf("\r\nutc :%s\r\n",gpsGpgga.utc);
	printf("\r\nlongitude :%s\r\n",gpsGpgga.longitude);
	printf("\r\nns :%s\r\n",gpsGpgga.ns);
	printf("\r\nlatitude :%s\r\n",gpsGpgga.latitude);
	printf("\r\new :%s\r\n",gpsGpgga.ew);
	printf("\r\nstate :%s\r\n",gpsGpgga.state);
	printf("\r\nstars :%s\r\n",gpsGpgga.stars);
	printf("\r\nhdop:%s\r\n",gpsGpgga.hdop);
	printf("\r\nheight :%s\r\n",gpsGpgga.height);
	printf("\r\nheight2:%s\r\n",gpsGpgga.height2);
	printf("\r\ndiffTime:%s\r\n",gpsGpgga.diffTime);

	printf("\r\nRMC\r\n");
	printf("\r\nutc :%s\r\n",gpsGprmc.utc);
	printf("\r\nstate :%s\r\n",gpsGprmc.state);
	printf("\r\nlongitude :%s\r\n",gpsGprmc.longitude);
	printf("\r\nns :%s\r\n",gpsGprmc.ns);
	printf("\r\nlatitude :%s\r\n",gpsGprmc.latitude);
	printf("\r\new :%s\r\n",gpsGprmc.ew);
	printf("\r\nspeed :%s\r\n",gpsGprmc.speed);
	printf("\r\nangle :%s\r\n",gpsGprmc.angle);
	printf("\r\nutcDate :%s\r\n",gpsGprmc.utcDate);
	printf("\r\nmagAngle :%s\r\n",gpsGprmc.magAngle);
	printf("\r\n magDirection:%s\r\n",gpsGprmc.magDirection);
	printf("\r\nmode :%s\r\n",gpsGprmc.mode); 	

}






		  
