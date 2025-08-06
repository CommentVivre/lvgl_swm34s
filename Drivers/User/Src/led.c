#include "uart.h"  

//	LED引脚初始化
// 
void LED_Init(void)
{
	GPIO_Init(LED1_PORT, LED1_PIN, 1, 0, 0, 0);		// 输出模式、禁止上拉、禁止下拉、推挽
}

