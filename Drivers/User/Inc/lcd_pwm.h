#ifndef __PWM_H
#define __PWM_H 
 
 
#include "SWM341.h"

 
/*-----------------------------------------引脚配置宏 ---------------------------------------*/
#define LCD_PWM                         PWM1                      // 使用PWM1
#define LCD_PWM_CH                      PWM_CH_B                  // 通道B
#define LCD_PWM_PIN            			 PIN9        				 	// 背光 引脚      
#define LCD_PWM_PORT           			 PORTD                 		// 端口  


/*------------------------------------- 函数声明 ----------------------------------------------*/

void 	LCD_PWMinit(uint8_t pulse);			 	//	PWM初始化
void	LCD_PWMSetPulse(uint8_t pulse);	 		//	设置占空比


#endif //__PWM_H



