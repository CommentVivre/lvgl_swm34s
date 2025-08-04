#ifndef __KEY_H
#define __KEY_H

#include "SWM341.h"
#include "delay.h"
#include "cmsis_os2.h"

#define	KEY_ON	 1		//按键按下
#define	KEY_OFF	 0		//按键放开

/*------------------------------------------ IO口配置宏 ----------------------------------*/


#define 	KEY_PIN           					PIN15        				// KEY 引脚      
#define	KEY_PORT          					GPIOD                 	// KEY GPIO端口     

/*------------------------------------------ 函数声明 ------------------------------------*/

void 		KEY_Init(void);	//按键IO口初始化函数
uint8_t	KEY_Scan(void);   //按键扫描

#endif //__KEY_H











