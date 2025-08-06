#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"

#include "ui.h"

#include "device.h" 



/***************************************************************************************************
*	函 数 名: RTOS_Main_Task
*
*	函数功能: 主任务，优先级最高
*
*	说    明: 无
****************************************************************************************************/

void RTOS_Main_Task(void *argument)
{
	(void)argument;
	
	
	RGB_LCD_Init();	// RGB屏幕初始化
	//	LCD_PWMinit(40);	// 背光引脚PWM初始化，频率10KHz，占空比40%	
	Touch_Init();			// 触摸屏初始化		

	lv_init();					//	LVGL初始化
	lv_port_disp_init();		//	LVGL显示接口初始化
	lv_port_indev_init();	// LVGL触摸接口初始化
	
	lv_main_window();
	while(1)
	{
		lv_task_handler();	// 如果在多任务中需要进行LVGL相关操作，需要考虑加互斥锁
		osDelay(20);
	}
}

/***************************************************************************************************
*	函 数 名: RTOS_Msg_Task
*
*	函数功能: 消息处理任务
*
*	说    明: 无
****************************************************************************************************/

void RTOS_Msg_Task(void *argument)
{
	(void)argument;
	
	while(1)
	{
		Touch_Scan();	// 触摸扫描
		osDelay(20);
	}
}
uint32_t freq_us =0;
/***************************************************************************************************
*	函 数 名: RTOS_LED_Task
*
*	函数功能: 点灯任务，任务优先级最低
*
*	说    明: 无
****************************************************************************************************/

void RTOS_LED_Task(void *argument)
{
	(void)argument;
	
	while(1)
	{
		#if 0
		TIMR_OC_Init(BTIMR7, high_us, 0, 1); // 修改high_us可调占空比
		
		freq_us +=1000;
		if(freq_us >100000)
		{
			freq_us =1000;
		}
		
		TIMR_Init(BTIMR7, TIMR_MODE_OC, CyclesPerUs, freq_us, 0);
		TIMR_OC_Init(BTIMR7, freq_us/2, 0, 0);		// 高电平时间
		TIMR_Start(BTIMR7);
		#endif
		LED1_Toggle;			// LED1 状态翻转
		osDelay(100);	// 延时
	}
}

//	LVGL节拍，使用 FreeRTOS 钩子函数
void vApplicationTickHook(void)
{
	lv_tick_inc(1);
}


// IM1281B 地址为160 波特率9600
uint8_t im1281b_read_cmd[8] = {0xA0,0x03,0x00,0x48,0x00,0x08,0xDD,0x6B};

// 下发读取命令 启动DMA传输
void RTOS_IM1281B_Task(void *argument)
{
	(void)argument;
	while(1)
	{
		// 设定源地址 以及长度
		DMA_CH_SetSrcAddress(DMA_CH1, (uint32_t)im1281b_read_cmd);
		DMA_CH_SetCount(DMA_CH1, 8);
		DMA_CH_Open(DMA_CH1);
		
		osDelay(1000);
	}
}

