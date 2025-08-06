#include "SWM341.h"
#include "uart.h"  

#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"

#include "ui.h"

//  LVGL V8.3.7  +  FreeRTOS  V10.4.6 版本

// 主任务
const osThreadAttr_t Main_Task_Attr = 	
{
	.priority = osPriorityHigh5,
	.stack_size = 4096,
};
osThreadId_t Main_Task_Id = NULL;
void RTOS_Main_Task(void *argument);


// 消息处理任务
const osThreadAttr_t Msg_Task_Attr = 	
{
	.priority = osPriorityHigh2,
	.stack_size = 1024,
};
osThreadId_t Msg_Task_Id = NULL;	
void RTOS_Msg_Task(void *argument);


// 点灯任务
const osThreadAttr_t LED_Task_Attr = 
{
	.priority = osPriorityHigh1,
	.stack_size = 128,
};
osThreadId_t LED_Task_Id = NULL;
void RTOS_LED_Task(void *argument);


// 下发IM1281B读取数据
// 可设定周期 最短为1秒
const osThreadAttr_t IM1281B_Task_Attr =
{
	.priority =osPriorityNormal,
	.stack_size = 256,
};
osThreadId_t IM1281B_Task_Id = NULL;
void RTOS_IM1281B_Task(void *argument);

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
	#if 0
	SystemInit();			// 初始化系统时钟，主频150M，函数定义在 system_SWM341.c
	SFC_W25Qxx_Init();	// 初始化W25Q64	
	SDRAM_Config();		// SDRAM初始化	
	#endif
	
	LED_Init();				// LED引脚初始化
	KEY_Init();				// 初始化按键引脚
	UART0_Init();			// UART0初始化函数
	UART1_Init();
	timer_moudle_init();
	timer_fg_init();		// FG测量建议加隔时测量 避免影响主函数执行
	// 初始化RTOS系统内核
	osKernelInitialize();      	                            

	Main_Task_Id = osThreadNew(RTOS_Main_Task, NULL, &Main_Task_Attr);  	// 创建主任务
	Msg_Task_Id = osThreadNew(RTOS_Msg_Task, NULL, &Msg_Task_Attr);  	//	消息处理任务	
	LED_Task_Id = osThreadNew(RTOS_LED_Task, NULL, &LED_Task_Attr);
	// IM1281B_Task_Id = osThreadNew(RTOS_IM1281B_Task, NULL, &IM1281B_Task_Attr);
	// 启动RTOS多任务	
	osKernelStart();			
	
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
//
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

