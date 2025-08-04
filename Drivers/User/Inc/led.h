#ifndef __LED_H
#define __LED_H

#include "SWM341.h"

/*------------------------------------------ LED配置宏 ----------------------------------*/

#define LED1_PIN            			 PIN14        				 	// LED1 引脚      
#define LED1_PORT           			 GPIOD                 		// LED1 GPIO端口     

///*----------------------------------------- LED控制宏 ----------------------------------*/

#define LED1_ON			GPIO_AtomicClrBit(LED1_PORT, LED1_PIN)				// 输出低电平，点亮LED1	
#define LED1_OFF			GPIO_AtomicSetBit(LED1_PORT, LED1_PIN)				// 输出高电平，关闭LED1	
#define LED1_Toggle		GPIO_AtomicInvBit(LED1_PORT, LED1_PIN)				// 翻转IO口状态
	

/*---------------------------------------- 函数声明 ------------------------------------*/

void LED_Init(void);

#endif //__LED_H


