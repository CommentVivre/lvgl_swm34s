#include "device.h"

uint32_t input_h,input_l;
// 配置基本定时器的PWM输出 参数单位为us
void timer_moudle_init(void)
{
	PORT_Init(PORTN, PIN1, PORTN_PIN1_BTIMR6_OUT, 0);
	TIMR_Init(BTIMR6, TIMR_MODE_OC, CyclesPerUs, 10, 0);
	TIMR_OC_Init(BTIMR6, 5, 0, 0);			// 高电平时间
	// TIMR_Start(BTIMR6);
	
	PORT_Init(PORTN, PIN0, PORTN_PIN0_BTIMR7_OUT, 0);
	TIMR_Init(BTIMR7, TIMR_MODE_OC, CyclesPerUs, 10000, 0);
	TIMR_OC_Init(BTIMR7, 5000, 0, 0);		// 高电平时间
	// TIMR_Start(BTIMR7);
	
	PORT_Init(PORTB, PIN3, PORTB_PIN3_BTIMR2_OUT, 0);
	TIMR_Init(BTIMR2, TIMR_MODE_OC, CyclesPerUs, 100, 0);
	TIMR_OC_Init(BTIMR2, 40, 0, 0);		// 高电平时间
	// TIMR_Start(BTIMR2);
}
// 输入捕获配置
void timer_fg_init(void)
{
	PORT_Init(PORTM, PIN15, PORTM_PIN15_TIMR2_IN, 1);
	TIMR_Init(TIMR2, TIMR_MODE_IC, CyclesPerUs, 0xFFFFFF, 0);
	TIMR_IC_Init(TIMR2, 1, 1);
	// TIMR_Start(TIMR2);
}

