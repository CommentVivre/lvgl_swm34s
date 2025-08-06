#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"

#include "ui.h"

#include "device.h" 

//  LVGL V8.3.7  +  FreeRTOS  V10.4.6 版本

void RTOS_Main_Task(void *argument);
void RTOS_Msg_Task(void *argument);
void RTOS_LED_Task(void *argument);
void RTOS_IM1281B_Task(void *argument);

// 主任务
const osThreadAttr_t Main_Task_Attr = 	
{
	.priority = osPriorityHigh5,
	.stack_size = 4096,
};
osThreadId_t Main_Task_Id = NULL;


// 消息处理任务
const osThreadAttr_t Msg_Task_Attr = 	
{
	.priority = osPriorityHigh2,
	.stack_size = 1024,
};
osThreadId_t Msg_Task_Id = NULL;	


// 点灯任务
const osThreadAttr_t LED_Task_Attr = 
{
	.priority = osPriorityHigh1,
	.stack_size = 128,
};
osThreadId_t LED_Task_Id = NULL;


// 下发IM1281B读取数据
// 可设定周期 最短为1秒
const osThreadAttr_t IM1281B_Task_Attr =
{
	.priority =osPriorityNormal,
	.stack_size = 256,
};
osThreadId_t IM1281B_Task_Id = NULL;

// 主函数
int main(void)
{
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


