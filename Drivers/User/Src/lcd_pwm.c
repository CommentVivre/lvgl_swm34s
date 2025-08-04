/***
	************************************************************************************************
	*	@version V1.0
	*  @date    2023-3-28
	*	@author  反客科技	
   ************************************************************************************************
   *  @description
	*
	*	实验平台：反客 SWM34S 核心板 
	*	淘宝地址：https://shop212360197.taobao.com
	*	QQ交流群：536665479
	*
>>>>> 文件说明：
	*
	*  初始化PWM引脚，配置占空比、频率等
	*
	************************************************************************************************
***/


#include "lcd_pwm.h"




/*************************************************************************************************
*	函 数 名:		LCD_PWMinit
*
*	入口参数:		pulse - PWM占空比，范围 0~100
*	
*	函数功能:		初始化PWM配置
*	
*************************************************************************************************/

void  LCD_PWMinit(uint8_t pulse)
{
	PWM_InitStructure  PWM_initStruct;	

	PORT_Init(LCD_PWM_PORT, LCD_PWM_PIN,  PORTD_PIN9_PWM1B,  0);  // 初始化引脚
	
	PWM_initStruct.Mode     = PWM_CENTER_ALIGNED;   // 中心对称模式
	PWM_initStruct.Clkdiv   = 75;			            // PWM内核频率 = 系统时钟 150MHz / 75 = 2MHz	
	PWM_initStruct.Period   = 100;				      // 重载值， 则PWM输出频率 =  PWM内核频率 2MHz / 100 /2 = 10KHz       
	PWM_initStruct.HdutyB   = pulse;		            // 设置高电平时长，也就是占空比
	
	PWM_initStruct.OutputInvB	= 0;						// 输出不取反
	
	PWM_initStruct.DeadzoneB 	= 0;				         // 以下参数在本例程用不到，设为0
	PWM_initStruct.IdleLevelB 	= 0;
	
	PWM_initStruct.UpOvfIE    = 0;
	PWM_initStruct.DownOvfIE  = 0;
	PWM_initStruct.UpCmpAIE   = 0;
	PWM_initStruct.DownCmpAIE = 0;
	PWM_initStruct.UpCmpBIE   = 0;
	PWM_initStruct.DownCmpBIE = 0;

	PWM_Init(LCD_PWM, &PWM_initStruct);    // 初始化PWM
	
	PWM_Start(PWM1_MSK);	      // 启动PWM1
}


/*************************************************************************************************
*	函 数 名:	LCD_PWMSetPulse
*
*	入口参数:	pulse - PWM占空比，范围 0~100
*	
*	函数功能:	设置PWM占空比
*	
*************************************************************************************************/
	
void  LCD_PWMSetPulse (uint8_t pulse)
{
   PWM_SetHDuty(LCD_PWM, LCD_PWM_CH, pulse);  
}