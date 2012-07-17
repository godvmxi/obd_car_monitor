#include "sysdef.h"

#ifndef RTC_H_H
#define RTC_H_H





/*******************************************************************************
 * ��������:rtcConfig                                                                     
 * ��    ��:����TCP����                                                               
 *                                                                               
 * ��    ��:�������ӵ�����0��TCP 1��UDP                                                                    
 * ��    ��:��                                                                    
 * ��    ��:-1���������� 0��������ָ����socket 1���ҵ�ָ��socket                                                                    
 * ��    ��:����                                                                     
 * �޸�����:2012��3��9��                                                                    
 *******************************************************************************/
void rtcConfig(void);
/*******************************************************************************
 * ��������:getTime                                                                     
 * ��    ��:����TCP����                                                               
 *                                                                               
 * ��    ��:�������ӵ�����0��TCP 1��UDP                                                                    
 * ��    ��:��                                                                    
 * ��    ��:-1���������� 0��������ָ����socket 1���ҵ�ָ��socket                                                                    
 * ��    ��:����                                                                     
 * �޸�����:2012��3��9��                                                                    
 *******************************************************************************/
void getTime(void);
/*******************************************************************************
 * ��������:setTime                                                                     
 * ��    ��:����TCP����                                                               
 *                                                                               
 * ��    ��:�������ӵ�����0��TCP 1��UDP                                                                    
 * ��    ��:��                                                                    
 * ��    ��:-1���������� 0��������ָ����socket 1���ҵ�ָ��socket                                                                    
 * ��    ��:����                                                                     
 * �޸�����:2012��3��9��                                                                    
 *******************************************************************************/
void setTime(void);
int8_t RTC_Get_Week (int16_t year,int8_t month,int8_t day);
void rtcTest(void);
int8_t RTC_Set(int16_t syear,int8_t smon,int8_t sday,int8_t hour,int8_t min,int8_t sec);







#endif 

