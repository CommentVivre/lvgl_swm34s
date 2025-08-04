/***
	*************************************************************************************************
	*	@file  	main.c
	*	@version V1.0
	*  @date    2023-5-19
	*	@author  反客科技
   ************************************************************************************************
   *  @description
	*
	*	实验平台：反客 SWM34SVET6 核心板 （型号：FK-SWM34SVE-M1）+ 反客800*480分辨率的RGB屏幕
	*	淘宝地址：https://shop212360197.taobao.com
	*	QQ交流群：536665479
	*
>>>>> 功能说明：
	*
	*	移植LVGL V8.3.7  +  FreeRTOS  V10.4.6 版本
	*	
	************************************************************************************************
***/

#include "SWM341.h"
#include "delay.h"
#include "led.h"  
#include "key.h"
#include "uart.h"
#include "sdram.h"   
#include "lcd_rgb.h"
#include "lcd_image.h"
#include "lcd_pwm.h"
#include "touch_800x480.h"
#include "sfc_w25q64.h"

#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_demo_widgets.h"
#include "lv_demo_benchmark.h"

#include "cmsis_os2.h"

#include "main_windows.h"

/*************************************** RTOS任务属性 ***************************************************/



const osThreadAttr_t Main_Task_Attr = 	// 主任务，优先级最高
{
	.priority = osPriorityHigh5,
	.stack_size = 4096,
};

const osThreadAttr_t Msg_Task_Attr = 	// 消息处理任务
{
	.priority = osPriorityHigh2,
	.stack_size = 1024,
};

const osThreadAttr_t LED_Task_Attr = 	// 点灯任务
{

	.priority = osPriorityHigh1,
	.stack_size = 128,
};

osThreadId_t Main_Task_Id 	= NULL;	// 主任务ID
osThreadId_t Msg_Task_Id 	= NULL;	// 消息处理任务ID
osThreadId_t LED_Task_Id 	= NULL;	// 点灯任务ID


/*************************************** 函数声明 ***************************************************/

void RTOS_Main_Task(void *argument);	// 主任务
void RTOS_Msg_Task(void *argument);		// 消息处理任务
void RTOS_LED_Task(void *argument);		// 点灯任务


/***************************************************************************************************
*	函 数 名: main
*
*	函数功能: 运行主程序
*	
****************************************************************************************************/

int main(void)
{	
// 参考华芯微特官方的例程和手册，将这三个函数放在启动文件执行，在芯片完成复位之后，
// 将SPI Flash的LVGL资源数据(图片、字库等)拷贝到SDRAM,之后再进入main函数
//	SystemInit();			// 初始化系统时钟，主频150M，函数定义在 system_SWM341.c
//	SFC_W25Qxx_Init();	// 初始化W25Q64	
//	SDRAM_Config();		// SDRAM初始化	
	
	LED_Init();				// LED引脚初始化
	KEY_Init();				// 初始化按键引脚
	UART0_Init();			// UART0初始化函数
	
	osKernelInitialize();   // 初始化RTOS系统内核   	                            

	Main_Task_Id = osThreadNew(RTOS_Main_Task, 	NULL, &Main_Task_Attr);  	// 创建主任务
	Msg_Task_Id  = osThreadNew(RTOS_Msg_Task, 	NULL, &Msg_Task_Attr);  	//	消息处理任务	
	LED_Task_Id  = osThreadNew(RTOS_LED_Task, 	NULL, &LED_Task_Attr);  	//	LED任务

	osKernelStart();			// 启动RTOS多任务	
	
}


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
		osDelay(20);
		lv_task_handler();	// 如果在多任务中需要进行LVGL相关操作，需要考虑加互斥锁，具体可以参考LVGL官方的介绍
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
		LED1_Toggle;			// LED1 状态翻转
		osDelay(100);	// 延时
	}
}

//	LVGL节拍，使用 FreeRTOS 钩子函数
//
void vApplicationTickHook(void)
{
	lv_tick_inc(1);
}
