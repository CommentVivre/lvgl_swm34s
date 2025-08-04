#ifndef __DELAY_H__
#define __DELAY_H__

#include "SWM341.h"

void SysTick_Init(void);				// 系统定时器初始化
void SWM341_IncTick(void);				// 计数+1
	
uint32_t SWM341_GetTick(void);		// 获取当前计数值

void Delay_ms(uint32_t ms);			// 毫秒延时函数

#endif //__DELAY_H__
