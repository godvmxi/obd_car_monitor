#include "rtc.h"



/*******************************************************************************
 * 函数名称:rtcConfig                                                                     
 * 描    述:建立TCP连接                                                               
 *                                                                               
 * 输    入:建立连接的类型0：TCP 1：UDP                                                                    
 * 输    出:无                                                                    
 * 返    回:-1：发生错误 0：不存在指定的socket 1：找到指定socket                                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月9日                                                                    
 *******************************************************************************/
void rtcConfig(void){
	int16_t int16_t_WaitForOscSource;
	//我们在BKP的后备寄存器1中，存了一个特殊字符0xA5A5
	//第一次上电或后备电源掉电后，该寄存器数据丢失，
	//表明RTC数据丢失，需要重新配置
	printf("\r\nset rtc\r\n");
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5A5A)
	{
		//重新配置RTC
		/* Enable PWR and BKP clocks */
		printf("\r\nreconfig rtc\r\n");
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
		PWR_BackupAccessCmd(ENABLE);
		
		/* Reset Backup Domain */
		BKP_DeInit();
		RCC_LSICmd(ENABLE);
		
		/* Enable LSE */
//		RCC_LSEConfig(RCC_LSE_ON);
//		RCC_LSEConfig(RCC_LSE_OFF);
//		RCC_LSEConfig(RCC_LSE_Bypass);
		
		for(int16_t_WaitForOscSource=0;int16_t_WaitForOscSource<5000;int16_t_WaitForOscSource++)
		{
		}
		/* Wait till LSE is ready */
//		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
		printf("\r\nwait for lsi ready\r\n");
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
		/* Select LSE as RTC Clock Source */
//		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);	
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		/* Enable RTC Clock */
		RCC_RTCCLKCmd(ENABLE);
		
		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();
		
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
		
		/* Enable the RTC Second */
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
		
		/* Set RTC prescaler: set RTC period to 1sec */
		RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
		
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
		//配置完成后，向后备寄存器中写特殊字符0xA5A5
		BKP_WriteBackupRegister(BKP_DR1, 0x5A5A);
		printf("\r\nset rtc over\r\n");
//        RTC_Set(2011,01,01,0,0,0);//默认时间
	}
	else
	{
        printf("\r\nno need to config rtc\r\n");
		rtcModifyFlag = 1;

		//若后备寄存器没有掉电，则无需重新配置RTC
        //这里我们可以利用RCC_GetFlagStatus()函数查看本次复位类型
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
            for(int16_t_WaitForOscSource=0;int16_t_WaitForOscSource<5000;int16_t_WaitForOscSource++);
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        {

			//这是上电复位


		}
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
		//这是外部RST管脚复位
		}
		//清除RCC中复位标志
		RCC_ClearFlag();
		//虽然RTC模块不需要重新配置，且掉电后依靠后备电池依然运行
		//但是每次上电后，还是要使能RTCCLK
		//RCC_RTCCLKCmd(ENABLE);
		//等待RTC时钟与APB1时钟同步
		//RTC_WaitForSynchro();
		//使能秒中断
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		//等待操作完成
		RTC_WaitForLastTask();
	}

}
//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
int8_t Is_Leap_Year(int16_t year)
{                    
	if(year%4==0) //必须能被4整除
	{
		if(year%100==0)
		{
			if(year%400==0)
				return 1;//如果以00结尾,还要能被400整除         
			else
				return 0;  
		}
		else 
			return 1;  
	}
	else 
		return 0;
}
//设置时钟
//把输入的时钟转换为秒钟
//以1970年1月1日为基准
//1970~2099年为合法年份
//返回值:0,成功;其他:错误代码.
//月份数据表                                                      
int8_t const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表 
//平年的月份 日期表 

const int8_t mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31}; 

int8_t RTC_Set(int16_t syear,int8_t smon,int8_t sday,int8_t hour,int8_t min,int8_t sec) 

{ 
	
	int16_t t;
	u32 seccount = 0; 
	if(syear<2000 || syear>2099)
		return 1;//syear范围1970-2099，此处设置范围为2000-2099        
	for (t = 1970;t<syear ;t++) //把所有年份的秒钟相加 
	{ 
		if(Is_Leap_Year (t))
			seccount+= 31622400;//闰年的秒钟数 
		else 
			seccount+= 31536000;                    //平年的秒钟数 
	}	
	smon-= 1; 	
	for (t= 0;t<smon ;t++)         //把前面月份的秒钟数相加 	
	{ 											 		
		seccount+=(u32)mon_table[t]*86400;//月份秒钟数相加 			
		if(Is_Leap_Year (syear)&&t == 1)
			seccount += 86400;//闰年2月份增加一天的秒钟数     	
	} 	 	
	seccount += (u32)(sday-1)*86400;//把前面 日期的秒钟数相加  	
	seccount += (u32)hour*3600;//小时秒钟数 	  	
	seccount += (u32)min*60;      //分钟秒钟数  	
	seccount += sec;//最后的秒钟加上去                                                                  
	//设置时钟 
	/*  RCC->APB1ENR | 1<<28;//使能电源时钟 
	RCC->APB1ENR | 1<<27;//使能备份时钟 
	PWR->CR | 1<<8;    //取消备份区写保护 
	//上面三步是必须的!*/ 
	PWR_BackupAccessCmd(ENABLE);	
	RTC_WaitForLastTask (); 
	RTC_SetCounter (seccount); 
	RTC_WaitForLastTask (); 
	return 0;      
	} 
//得到当前的时间 
//返回值:0,成功;其他:错误代码. 
	
int8_t RTC_Get(void) 
{ 
	static int16_t daycnt = 0; 
	u32 timecount = 0; 
	u32 temp = 0; 
	int16_t temp1 = 0; 	
	timecount = RTC_GetCounter (); 	

//	timecount RTC->CNTH ;//得到计数器中的值 (秒钟数) 	 
//	timecount<< 16; 	  
//	timecount+ RTC->CNTL ;  
     
	temp  = timecount/86400;   //得到天数 (秒钟数对应的) 

	if(daycnt != temp)//超过一天了 	
	{     	
		daycnt = temp ;		
		temp1 = 1970;  //从1970年开始  		
		while(temp>=365) 
		{                          
			if(Is_Leap_Year (temp1))//是闰年 		
			{	
				if(temp> 366)
					temp-= 366;//闰年的秒钟数 
				else  
				{
					temp1++;
					break ;
				}  
			} 
			else
			{ 
				temp-=365;       //平年 
			}
			temp1++;
		}  
	
   
	
		timer.year = temp1;//得到年份 
		
		temp1 = 0; 
	
		while(temp>= 28)//超过了一个月 	
		{	
			if(Is_Leap_Year(timer.year)&&temp1 == 1)//当年是不是闰年/2月份 		
			{ 
			
				if(temp> 29)
					temp-= 29;//闰年的秒钟数 			
				else
					break ;  		
			}	
			else	
			{ 	
				if(temp>=mon_table[temp1])
					temp-=mon_table[temp1];//平年 	
				else 
					break ;		
			}  
			temp1++;	
		} 	
		timer.month = temp1+1;//得到月份 	
		timer.date = temp+1;  //得到日期 
	
	} 
	
	temp = timecount%86400;     //得到秒钟数 	
	timer.hour = temp/3600;     //小时 		
	timer.minute = (temp%3600)/60; //分钟  	
	timer.second = (temp%3600)%60; //秒钟 	
//	timer.week = RTC_Get_Week(timer.year,timer.month,timer.date);//获取星期  	
	return 1; 
	
}     
	
	//获得现在是星期几 
	
	//功能描述:输入公历日期得到星期(只允许1901-2099年) 
	
	//输入参数：公历年月日 
	
	//返回值：星期号                                                                                                                                            
	
	
int8_t RTC_Get_Week (int16_t year,int8_t month,int8_t day)
{  	
	int16_t temp2;
	int8_t yearH,yearL;		
	yearH=year/100;     
	yearL=year%100;
	// 如果为21世纪,年份数加100 
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的 
	temp2=yearL+yearL/4;
	temp2=temp2%7;
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}
             
                 
/*******************************************************************************
 * 函数名称:getTime                                                                     
 * 描    述:建立TCP连接                                                               
 *                                                                               
 * 输    入:建立连接的类型0：TCP 1：UDP                                                                    
 * 输    出:无                                                                    
 * 返    回:-1：发生错误 0：不存在指定的socket 1：找到指定socket                                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月9日                                                                    
 *******************************************************************************/
void getTime(void);
/*******************************************************************************
 * 函数名称:setTime                                                                     
 * 描    述:建立TCP连接                                                               
 *                                                                               
 * 输    入:建立连接的类型0：TCP 1：UDP                                                                    
 * 输    出:无                                                                    
 * 返    回:-1：发生错误 0：不存在指定的socket 1：找到指定socket                                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月9日                                                                    
 *******************************************************************************/
void setTime(void);



/*******************************************************************************
 * 函数名称:setTime                                                                     
 * 描    述:建立TCP连接                                                               
 *                                                                               
 * 输    入:建立连接的类型0：TCP 1：UDP                                                                    
 * 输    出:无                                                                    
 * 返    回:-1：发生错误 0：不存在指定的socket 1：找到指定socket                                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月9日                                                                    
 *******************************************************************************/
void rtcTest(void){
	rtcConfig();
	RTC_Set(2012,6,4,5,15,0);
	while(1){
		RTC_Get();
		printf("\r\nTIME:%d-%d-%d-%d-%d-%d\r\n",timer.year,timer.month,timer.date,timer.hour,timer.minute,timer.second);
		delay_ms(2000);
	}

}
void showTime(TIME *t){
	if(t != NULL){
		printf("\r\nTIME:%d-%d-%d  -->  %d-%d-%d\r\n",t->year,t->month,t->date,t->hour,t->minute,t->second);
	}
	else{
		printf("\r\nTIME:%d-%d-%d  -->  %d-%d-%d\r\n",timer.year,timer.month,timer.date,timer.hour,timer.minute,timer.second);	
	}

}















