
#include "gps.h"
#include "sysdef.h"


//GPS数据接收调用规则
/*
首先调用gpsDataInit()函数，对GPS_DATA结构体中的数据进行复位

然后循环调用gps_data_process（）函数，不停抓取GPS数据（若有定位信息，则立即停止GPS）

*/	
//-------------------------------------------------------------- 
#define GPS_NULL     0x00 //GPS语句类型 
#define GPS_GPGGA    0x01 
#define GPS_GPGSV    0x04 
#define GPS_GPRMC    0x08 

int		GPSDataType=GPS_NULL; //GPS语句类型 
char	GPSDataTypeStrBuff[]="$GPxxx,"; //GPS语句类型缓存, 判断类型时使用, 
int		GPSDataTypeStrBuffIndex=0; //GPS语句类型字符串的当前位置 
char	GPSDataStart=0; //GPS语句开始. 检测到 $ 时置1 
int		ReciveFlag=0; //数据接收完成. 最后一条 GPRMC 语句发送完毕置1
int		gps_data_process_temp=0;//gps数据接收清零临时变量
int		GPS_GGA_DINGWEI_OK=0;//若GPS定位，则将此变量置1，否则为0
int		GPS_SHUJU_OK=0;//若GPS输出坐标信息，则将此变量置1，否则为0
int		GPS_RMC_DINGWEI_OK = 0;



extern int gps_data_value;

extern unsigned int SYSTEM_ON_TIME;	   //系统运行时间计算变量，每S自加1

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

	GPS_DATA.HDOP=999;	 //水平精度设置为最低

	GPS_GGA_DINGWEI_OK=0;//若GPS定位，则将此变量置1，否则为0
	GPS_RMC_DINGWEI_OK=0;//若GPS定位，则将此变量置1，否则为0
	GPS_SHUJU_OK=0;//若GPS输出坐标信息，则将此变量置1，否则为0

	GPS_DATA.A_SateUsed[0]='0';	   //定位卫星个数
	GPS_DATA.A_SateUsed[1]='0';	   //定位卫星个数

	GPS_DATA.V_SateUsed[0]='0';	   //定位卫星个数
	GPS_DATA.V_SateUsed[1]='0';	   //定位卫星个数


//卫星信号统计参数初始化清零
	satellite_have_signal=0;		 //有信号值的卫星个数
	satellite_below_10=0;			 //信号值>10的卫星个数
	satellite_below_20=0;			 //信号值>20的卫星个数
	satellite_below_30=0;			 //信号值>30的卫星个数
	satellite_below_40=0;			 //信号值>40的卫星个数

	GPSDataStart = 0;
	GPSDataType = GPS_NULL;
	GPSDataTypeStrBuffIndex = 0;
	memset(GPSDataTypeStrBuff,'\0',7);
	
	USART_Cmd(USART3, ENABLE);
}
void gpsDataIrq(char temp)
{
	if(GPSDataStart) 
	{		//已经接收到GPS语句头字符$ 
		switch(GPSDataTypeStrBuffIndex) 
		{		//已经读取了多少个表示GPS数据类型的字符
			case 6: 
			{		//类型字头已经全部读取, 开始判断 
				if(GPSDataTypeStrBuff[3]=='G'&&GPSDataTypeStrBuff[4]=='G'&&GPSDataTypeStrBuff[5]=='A')//$GPGGA 
				{	
					GPSDataType=GPS_GPGGA;

					//GGA 数据 初始化 清零，每次重新接收前都设为‘x’，用以判断本次数据是否更新，
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
				if(temp==',') GPSDataTypeStrBuffIndex=255; //判断完毕, 数据接收开始
				break;
			}
			case 255: 
			{		//GPS数据类型的字符全部读取并判断完毕, 正接收数据 
				switch(GPSDataType) 
				{
					case GPS_GPGGA: 
					{ 
						switch(temp)
						{
							case'*':		//语句结
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
									//增加定位精度选择，GGA数据通过4位字符接收、最终数据转换成int型。
										for(gps_data_process_temp=0;gps_data_process_temp<9;gps_data_process_temp++)
										GPS_DATA.A_Latitude[gps_data_process_temp]=GPS_GGA_Data.Latitude[gps_data_process_temp];
										for(gps_data_process_temp=0;gps_data_process_temp<10;gps_data_process_temp++)
										GPS_DATA.A_Longitude[gps_data_process_temp]=GPS_GGA_Data.Longitude[gps_data_process_temp];
										GPS_DATA.A_NS =GPS_GGA_Data.NS; 
										GPS_DATA.A_EW=GPS_GGA_Data.EW;

										GPS_DATA.A_SateUsed[0]=GPS_GGA_Data.SateUsed[0];
										GPS_DATA.A_SateUsed[1]=GPS_GGA_Data.SateUsed[1];

										GPS_DATA.HDOP=GPS_GGA_Data.hdop_int;

										GPS_GGA_DINGWEI_OK=1;//若GPS定位，则将此变量置1，否则为0
									}
								}
								else
								{
									//		if(SYSTEM_PC!=7&&SYSTEM_PC!=8)
									//		GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET);
//																	printf("\r\nPOSITION FIX :  %c  \r\n",GPS_GGA_Data.PositionFix);
								} 
										     break; 
								case ',':       //该字段结束, 下一个 
									GPS_GGA_Data.Block++; 
									GPS_GGA_Data.BlockIndex=0;      //字段索引置0第一个字符
									break; 
								default: 
								{                      //字段字符 
									switch(GPS_GGA_Data.Block) 
									{			//判断当前处于哪个字段 
									
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
										case 8: //获取高度数据
											GPS_GGA_Data.HEIGHT[GPS_GGA_Data.BlockIndex]=temp; 
											break;
//										
										default:
											break; 
									}
									GPS_GGA_Data.BlockIndex++;        //字段索引++, 指向下一个字符  
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
								ReciveFlag=1;       //接收完毕, 可以处理																		//
												//如果GPS吐未定位坐标数据，复制信息到GPS_DATA结构体中
								if(GPS_RMC_Data.Status == 'A') //填充速度和方向
								{
									for(gps_data_process_temp=0;gps_data_process_temp<9;gps_data_process_temp++)
									GPS_DATA.V_Latitude[gps_data_process_temp]=GPS_RMC_Data.Latitude[gps_data_process_temp];

									for(gps_data_process_temp=0;gps_data_process_temp<10;gps_data_process_temp++)
									GPS_DATA.V_Longitude[gps_data_process_temp]=GPS_RMC_Data.Longitude[gps_data_process_temp];
									
									GPS_DATA.V_NS =GPS_RMC_Data.NS; 
									GPS_DATA.V_EW=GPS_RMC_Data.EW;

									for(gps_data_process_temp=0;gps_data_process_temp<5;gps_data_process_temp++)
										GPS_DATA.Speed[gps_data_process_temp]=GPS_RMC_Data.Speed[gps_data_process_temp];
									//航向
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
									//速度
									for(gps_data_process_temp=0;gps_data_process_temp<5;gps_data_process_temp++)
										GPS_DATA.Speed[gps_data_process_temp]=GPS_RMC_Data.Speed[gps_data_process_temp];
									//航向
									for(gps_data_process_temp=0;gps_data_process_temp<5;gps_data_process_temp++)
									{
										GPS_DATA.Course[gps_data_process_temp]=GPS_RMC_Data.Course[gps_data_process_temp];
									}
									GPS_DATA.V_SateUsed[0]=GPS_GGA_Data.SateUsed[0];
									GPS_DATA.V_SateUsed[1]=GPS_GGA_Data.SateUsed[1];
									GPS_SHUJU_OK=1;//GPS输出坐标信息，则将此变量置1	 
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
								satellite_have_signal=0;		 //有信号值的卫星个数
								satellite_below_10=0;			 //信号值>10的卫星个数
								satellite_below_20=0;			 //信号值>20的卫星个数
								satellite_below_30=0;			 //信号值>30的卫星个数
								satellite_below_40=0;			 //信号值>40的卫星个数			

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
												GPS_GSV_Data.SateInView=temp; 	//总的GSV语句电文数
												break;
                                        case 1: 
                                                GPS_GSV_Data.GSVID4=temp;    //当前GPFSV语句的序号, 该序号计算出该组卫星数据应该存放于数组的哪个位置
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
						{       //GPS类型判断失败, 进入未接收到GPS语句($)的状态, 重新循环
							GPSDataStart=0;
						} 
				} 
				break; 
			}
			default: 
			{											//未判断, 继续接收, Index++ 
				GPSDataTypeStrBuff[GPSDataTypeStrBuffIndex]=temp; 
				GPSDataTypeStrBuffIndex++; 
				if(GPSDataTypeStrBuffIndex>7)
					GPSDataStart = 0;
			}
		}
	} 
	else 
	{													//未发现语句头字符, 继续接收并判断
		if (temp=='$') 
		{												//接收到$, 下一个字符即为类型判断字符, 先进行相关变量初始化 
			memset(GPSDataTypeStrBuff,'\0',7);
			GPSDataTypeStrBuff[0]=temp; 
			GPSDataTypeStrBuffIndex=1;
			GPSDataStart=1;								//从头存放GPS类型字符到变量
			GPSDataType=GPS_NULL;			
			GPS_RMC_Data.Block=0;						//每种语句的标志位
			GPS_RMC_Data.BlockIndex=0;
			GPS_GSV_Data.Block=0;
			GPS_GSV_Data.BlockIndex=0;
			GPS_GGA_Data.Block=0;
			GPS_GGA_Data.BlockIndex=0;
			ReciveFlag=0;//开始
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
//	char state[2];			//0=not，1=not diff，2=diff，6= cal ing
//	char stars[4];		   	//working stars
//	char hdop[6];			//Level of accuracy factor : 0.5~99.9
//	char height[8];			//height : -9999.9~99999.9
//	char height2[4]; 		//地球椭球面相对大地水准面的高度 
//	char diffTime[12];		//差分时间
//	char diffId[6];			//差分站ID号0000~1023
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
//	char magAngle[6];		//磁偏角 000.0~180.0
//	char magDirection[2];	//磁偏角方向，E（东）或W（西）
//	char mode[2];  			//模式指示（仅NMEA0183 3.00版本输出，A=自主定位，D=差分，E=估算，N=数据无效）
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






		  
