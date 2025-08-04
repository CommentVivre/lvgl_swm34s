/***
	*************************************************************************************************
	*	@version V1.0
	*  @date    2023-3-20
	*	@author  反客科技	
	*	@brief  	systick相关配置函数
   *************************************************************************************************
   *  @description
	*
	*	实验平台：反客 SWM34S 核心板 
	*	淘宝地址：https://shop212360197.taobao.com
	*	QQ交流群：536665479
	*
>>>>> 文件说明：
	*
	*	systick相关配置函数、延时函数等，移植于官方评估板的SDK
	*
	************************************************************************************************
***/

#include "delay.h"

static __IO uint32_t systick = 0;	// 全局计数变量

// 系统定时器初始化
//
void SysTick_Init(void)
{
    SysTick_Config(SystemCoreClock / 1000);    // 1ms中断一次 
}


//	计数+1，在中断中调用
//
void SWM341_IncTick(void)
{
    systick++;
}



//	获取当前计数值
//
uint32_t SWM341_GetTick(void)
{
    return systick;
}



//	获取当前计数值
//
void Delay_ms(uint32_t ms)
{
    uint32_t tickstart = 0;
	
    tickstart = SWM341_GetTick();
    while ((SWM341_GetTick() - tickstart) < ms)
    {
        ;
    }
}
