#include "device.h"

/*************************************************************************************************
*	函 数 名:	LCDC_IO_Init
*
*	函数功能:	初始化LCDC的IO口、背光引脚等
*		
*************************************************************************************************/

void LCD_IO_Init(void)
{

	#if LCD_ColorMode == LCD_FMT_RGB888 	// 如果使用 24位 RGB888 格式			
	
	PORT_Init(PORTC, PIN4,  PORTC_PIN4_LCD_R0,  0);		// 初始化 RGB 信号的 R2-R0	
	PORT_Init(PORTC, PIN5,  PORTC_PIN5_LCD_R1,  0);
	PORT_Init(PORTC, PIN8,  PORTC_PIN8_LCD_R2,  0);
	
	PORT_Init(PORTA, PIN12, PORTA_PIN12_LCD_G0, 0);		// 初始化 RGB 信号的 G1-G0	
	PORT_Init(PORTA, PIN13, PORTA_PIN13_LCD_G1, 0);	
	
	PORT_Init(PORTB, PIN1,  PORTB_PIN1_LCD_B0,  0);		// 初始化 RGB 信号的 B2-B0
	PORT_Init(PORTB, PIN11, PORTB_PIN11_LCD_B1, 0);
	PORT_Init(PORTB, PIN13, PORTB_PIN13_LCD_B2, 0);	
	
	#else	
	// 如果使用16位色 RGB565，则低位的数据线不能设置为复用模式，并且不能浮空，否则某些颜色会显示不正常或者有噪点，
	//	因为这些引脚虽然没有用到，但是也接到了FPC排线，容易受干扰，此处全部设置为推挽输出

	// RGB 信号的 R2-R0	
	GPIO_Init(GPIOC, PIN4, 1, 0, 0, 0);		// 输出模式、禁止上拉、禁止下拉、推挽，
	GPIO_Init(GPIOC, PIN5, 1, 0, 0, 0);		// 输出模式、禁止上拉、禁止下拉、推挽
	GPIO_Init(GPIOC, PIN8, 1, 0, 0, 0);		// 输出模式、禁止上拉、禁止下拉、推挽
	// RGB 信号的 G1-G0		
	GPIO_Init(GPIOA, PIN12, 1, 0, 0, 0);	// 输出模式、禁止上拉、禁止下拉、推挽
	GPIO_Init(GPIOA, PIN13, 1, 0, 0, 0);	// 输出模式、禁止上拉、禁止下拉、推挽
	// RGB 信号的 B2-B0
	GPIO_Init(GPIOB, PIN1, 1, 0, 0, 0);		// 输出模式、禁止上拉、禁止下拉、推挽
	GPIO_Init(GPIOB, PIN11, 1, 0, 0, 0);	// 输出模式、禁止上拉、禁止下拉、推挽
	GPIO_Init(GPIOB, PIN13, 1, 0, 0, 0);	// 输出模式、禁止上拉、禁止下拉、推挽	
	#endif		

	PORT_Init(PORTC, PIN9,  PORTC_PIN9_LCD_R3,  0);			// 初始化 RGB 信号的 R7-R3
	PORT_Init(PORTC, PIN10, PORTC_PIN10_LCD_R4, 0);
	PORT_Init(PORTC, PIN11, PORTC_PIN11_LCD_R5, 0);
	PORT_Init(PORTC, PIN12, PORTC_PIN12_LCD_R6, 0);
	PORT_Init(PORTC, PIN13, PORTC_PIN13_LCD_R7, 0);
	
	PORT_Init(PORTA, PIN14, PORTA_PIN14_LCD_G2, 0);			// 初始化 RGB 信号的 G7-G2
	PORT_Init(PORTA, PIN15, PORTA_PIN15_LCD_G3, 0);
	PORT_Init(PORTC, PIN0,  PORTC_PIN0_LCD_G4,  0);
	PORT_Init(PORTC, PIN1,  PORTC_PIN1_LCD_G5,  0);
	PORT_Init(PORTC, PIN2,  PORTC_PIN2_LCD_G6,  0);
	PORT_Init(PORTC, PIN3,  PORTC_PIN3_LCD_G7,  0);	
	
	PORT_Init(PORTB, PIN15, PORTB_PIN15_LCD_B3, 0);			// 初始化 RGB 信号的 B7-B3
	PORT_Init(PORTA, PIN2,  PORTA_PIN2_LCD_B4,  0);
	PORT_Init(PORTA, PIN9,  PORTA_PIN9_LCD_B5,  0);
	PORT_Init(PORTA, PIN10, PORTA_PIN10_LCD_B6, 0);
	PORT_Init(PORTA, PIN11, PORTA_PIN11_LCD_B7, 0);	
	
	
	PORT_Init(PORTB, PIN2,  PORTB_PIN2_LCD_VSYNC, 0);		// 初始化 DE、VS、HS、DCLK
	PORT_Init(PORTM, PIN8,  PORTM_PIN8_LCD_HSYNC, 0);
	PORT_Init(PORTM, PIN11, PORTM_PIN11_LCD_DEN,  0);
	PORT_Init(PORTB, PIN5,  PORTB_PIN5_LCD_DCLK,  0);
	
	// 初始化背光引脚
	LCD_Backlight_OFF;	// 先关闭背光引脚，初始化LCDC之后再开启
	GPIO_Init(LCD_Backlight_PORT, LCD_Backlight_PIN, 1, 0, 0, 0);		// 输出模式、禁止上拉、禁止下拉、推挽

}


/*************************************************************************************************
*	函 数 名:	RGB_LCD_Init
*
*	函数功能:	初始化LCDC的参数
*		
*************************************************************************************************/

void RGB_LCD_Init(void)
{
	LCD_InitStructure 	LCD_initStruct;
	DMA2D_InitStructure 	DMA2D_initStruct;
	
	DMA2D_initStruct.Interval = CyclesPerUs;					// 初始化DMA2D
	DMA2D_initStruct.IntEOTEn = 0;
	DMA2D_Init(&DMA2D_initStruct);
	
	LCD_IO_Init();												// 初始化LCD的IO口、背光引脚等

	
	LCD_initStruct.ClkDiv 		= 3; 							// 像素时钟 = 单片机主频（150M）/ ClkDiv ，此时为30MHz
	LCD_initStruct.Format 		= LCD_ColorMode;				// 颜色格式
	LCD_initStruct.HnPixel 		= LCD_Width;					// 水平有效像素
	LCD_initStruct.VnPixel 		= LCD_Height;					// 垂直有效像素
	
	LCD_initStruct.Hfp 			= HFP;							// 其它的参数可参考屏幕的手册进行配置
	LCD_initStruct.Hbp 			= HBP;
	LCD_initStruct.Vfp 			= VFP;
	LCD_initStruct.Vbp 			= VBP;
	LCD_initStruct.HsyncWidth 	= HSW;
	LCD_initStruct.VsyncWidth 	= VSW;
	
	LCD_initStruct.DataSource 	= LCD_MemoryAdd;				// 显存地址
	LCD_initStruct.Background 	= 0xFFFFFF;						// 默认背景色
	LCD_initStruct.SampleEdge 	= LCD_SAMPLE_FALL;				// 数据下降沿有效	
	LCD_initStruct.IntEOTEn 	= 0;							// 不启用帧中断
	LCD_Init(LCD, &LCD_initStruct);								// 初始化配置

	LCD_Start(LCD);	// 启动刷屏
	LCD_Backlight_ON;	// 开启背光
}

