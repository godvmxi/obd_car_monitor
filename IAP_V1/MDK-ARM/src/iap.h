#include "sysdef.h"
#ifndef IAP_H_H
#define IAP_H_H





void usartIap(void);
void tcpIap(void);
void iapfunc(void);
int16_t checkIapType(void);
void iapHardwareInit(void);
int16_t checkAppConfig(void);
int16_t checkAppState(void);
/*******************************************************************************
 * 函数名称:checkIapType                                                                     
 * 描    述: IAP升级方法选择                                                               
 *                                                                               
 * 输    入:无                                                          
 * 输    出:无                                                                    
 * 返    回: 负:强制使用USART升级  其余：正常检查系统并确定是否升级                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年5月9日                                                                    
 *******************************************************************************/
int16_t tcpStart(SOCKET *net,IAP_BRIFE *brife);
/*******************************************************************************
 * 函数名称:tcpStart                                                                     
 * 描    述: 重新进行TCP认证，并返回相应的数据                                                               
 *                                                                               
 * 输    入:无                                                          
 * 输    出:无                                                                    
 * 返    回: 负:强制使用USART升级  其余：正常检查系统并确定是否升级                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年5月9日                                                                    
 *******************************************************************************/
int16_t tcpGetData(SOCKET *net,IAP_BRIFE *brife,int16_t index);
/*******************************************************************************
 * 函数名称:checkAppCrc                                                                     
 * 描    述: 重新进行TCP认证，并返回相应的数据                                                               
 *                                                                               
 * 输    入:无                                                          
 * 输    出:无                                                                    
 * 返    回: 负:强制使用USART升级  其余：正常检查系统并确定是否升级                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年5月9日                                                                    
 *******************************************************************************/
int16_t		checkAppCrc(IAP_BRIFE *brife);
void  setDebugPara(void);





#endif


