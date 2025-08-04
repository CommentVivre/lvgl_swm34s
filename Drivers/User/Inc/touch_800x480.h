#ifndef __GT9XX_H__
#define __GT9XX_H__

#include "SWM341.h"
#include "uart.h"
#include "cmsis_os2.h"



#define TOUCH_MAX   5	//最大触摸点数

typedef struct 
{
	uint8_t  flag;			//	触摸标志位，为1时表示有触摸操作
	uint8_t  num;				//	触摸点数
	uint16_t x[TOUCH_MAX];	//	x坐标
	uint16_t y[TOUCH_MAX];	//	y坐标
}TouchStructure;

extern volatile TouchStructure touchInfo;	// 触摸数据结构体声明	

#define ACK_OK  	1  			// 响应正常
#define ACK_ERR 	0				// 响应错误



/*------------------------------------------ 触摸引脚宏 ----------------------------------*/

#define Touch_IIC_SCL_PORT   			 PORTA                 				// SCL 引脚端口
#define Touch_IIC_SCL_PIN     		 PORTA_PIN1_I2C0_SCL  				// SCL 引脚
         
#define Touch_IIC_SDA_PORT   			 PORTA                   			// SDA 引脚端口
#define Touch_IIC_SDA_PIN    			 PORTA_PIN0_I2C0_SDA             // SDA 引脚

#define Touch_INT_PORT   				 GPIOD                   			// INT 引脚端口
#define Touch_INT_PIN    				 PIN0             					// INT 引脚

#define Touch_RST_PORT   				 GPIOA                   			// RST 引脚端口
#define Touch_RST_PIN    				 PIN5           						// RST 引脚


#define Touch_INT_L			GPIO_AtomicClrBit(Touch_INT_PORT, Touch_INT_PIN)				// 输出低电平
#define Touch_INT_H			GPIO_AtomicSetBit(Touch_INT_PORT, Touch_INT_PIN)				// 输出高电平	

#define Touch_RST_L			GPIO_AtomicClrBit(Touch_RST_PORT, Touch_RST_PIN)				// 输出低电平
#define Touch_RST_H			GPIO_AtomicSetBit(Touch_RST_PORT, Touch_RST_PIN)				// 输出高电平	

/*------------------------------------ 寄存器定义 -----------------------------------*/  		

#define GT9XX_IIC_RADDR 0xBB			// IIC初始化地址
#define GT9XX_IIC_WADDR 0xBA

#define GT9XX_CFG_ADDR 	0x8047		// 固件配置信息寄存器和配置起始地址
#define GT9XX_READ_ADDR 0x814E		// 触摸信息寄存器
#define GT9XX_ID_ADDR 	0x8140		// 触摸面板ID寄存器



/*------------------------------------ 函数声明 -----------------------------------*/  		

void 	Touch_Init(void);		// 触摸屏初始化
void 	Touch_Scan(void);		// 触摸扫描

#endif //__GT9XX_H__
