#pragma once

#include "SWM341.h"
#include "uart.h"
#include "sdram.h"  
#include "lcd_fonts.h"
#include <stdio.h>

//定义颜色格式
#define	LCD_ColorMode   LCD_FMT_RGB565   // 一个像素点占16位

// #define	LCD_ColorMode   LCD_FMT_RGB888		// 需要注意，SWM34S的RGB888格式，一个像素点占32位,不只是24位

/*---------------------------------------------------------- 函数声明 -------------------------------------------------------*/

void  RGB_LCD_Init(void);		// 屏幕控制器初始化配置


/*---------------------------------------------------------- 屏幕相关参数 -------------------------------------------------------*/

#define HBP  80	// 根据屏幕的手册进行设置
#define VBP  40
#define HSW  1
#define VSW  1
#define HFP  200
#define VFP  22


// 显存所需空间 = 分辨率 * 每个像素所占字节数，例如 800*480的屏，使用16位色（RGB565），需要显存 800*480*2 = 768000 字节

#define LCD_Width     		800						//	LCD的像素长度
#define LCD_Height    		480						//	LCD的像素宽度
#define LCD_MemoryAdd   	SDRAM_BANK_ADDR 		//	显存的起始地址  

	
#if LCD_ColorMode == LCD_FMT_RGB565 

	#define	BytesPerPixel		2		//16位色模式每个像素占2字节
	#define DMA2D_ColorMode 	DMA2D_FMT_RGB565

#elif LCD_ColorMode == LCD_FMT_RGB888

	#define BytesPerPixel		4		// 需要注意，SWM34S的24位色 RGB888格式，一个像素点占32位,不只是24位，计算坐标时要按32位宽计算
	#define DMA2D_ColorMode		DMA2D_FMT_RGB888

#endif	



/*-------------------------------------------------------- LCD背光引脚 -------------------------------------------------------*/

#define LCD_Backlight_PIN		PIN9								
#define	LCD_Backlight_PORT		GPIOD									


#define	LCD_Backlight_OFF		GPIO_AtomicClrBit(LCD_Backlight_PORT, LCD_Backlight_PIN)		// 关闭背光
#define LCD_Backlight_ON		GPIO_AtomicSetBit(LCD_Backlight_PORT, LCD_Backlight_PIN)		// 开启背光

	 

