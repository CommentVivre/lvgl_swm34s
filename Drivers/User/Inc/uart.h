#pragma once

#include "SWM341.h"
#include "stdio.h"
#include "cmsis_os2.h"

#define UART0_BaudRate  115200

#define	UART0_TX_PIN									PIN1					// TX 引脚
#define	UART0_TX_PORT									PORTM					// TX 引脚端口
#define UART0_RX_PIN									PIN0             		// RX 引脚
#define	UART0_RX_PORT									PORTM					// RX 引脚端口

#define UART1_BaudRate  9600

#define	UART1_TX_PIN									PIN3					// TX 引脚
#define	UART1_TX_PORT									PORTM					// TX 引脚端口
#define UART1_RX_PIN									PIN2					// RX 引脚
#define	UART1_RX_PORT									PORTM                 	// RX 引脚端口

#define uart1_tx_rx_max_len 128


extern uint32_t input_h,input_l;


void UART0_Init(void);	
void UART1_Init(void);

extern uint8_t uart1_rx_buffer[uart1_tx_rx_max_len],uart1_tx_buffer[uart1_tx_rx_max_len];

void timer_moudle_init(void);
void timer_fg_init(void);



#define	KEY_ON 1		// 按键按下
#define	KEY_OFF 0		// 按键放开

#define KEY_PIN PIN15							// KEY 引脚      
#define	KEY_PORT GPIOD							// KEY GPIO端口     

void KEY_Init(void);							//按键IO口初始化函数
uint8_t	KEY_Scan(void);							//按键扫描


#define SDRAM_Size 8*1024*1024 					// 容量8M字节
#define SDRAM_BANK_ADDR ((uint32_t)0x80000000) 	// SDRAM 地址


void SDRAM_Config(void);						// SDRAM初始化

// 定义颜色格式
// LCD_FMT_RGB565 RGB565 一个像素点占16位
// LCD_FMT_RGB888 RGB888 一个像素点占32位
#define	LCD_ColorMode   LCD_FMT_RGB565

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

/*-----------------------------------------引脚配置宏 ---------------------------------------*/
#define LCD_PWM                         PWM1                      // 使用PWM1
#define LCD_PWM_CH                      PWM_CH_B                  // 通道B
#define LCD_PWM_PIN            			 PIN9        				 	// 背光 引脚      
#define LCD_PWM_PORT           			 PORTD                 		// 端口  


/*------------------------------------- 函数声明 ----------------------------------------------*/

void 	LCD_PWMinit(uint8_t pulse);			 	//	PWM初始化
void	LCD_PWMSetPulse(uint8_t pulse);	 		//	设置占空比


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

/*----------------------------------------------- 命名参数宏 -------------------------------------------*/

#define W25Qxx_OK           0		// W25Qxx通信正常
#define W25Qxx_ERROR        -1			// 错误


#define W25Qxx_CMD_JedecID 			0x9F		// JEDEC ID  

#define W25Qxx_CMD_SectorErase 		0x20		// 扇区擦除，4K字节， 参考擦除时间 45ms
#define W25Qxx_CMD_BlockErase_32K 	0x52		// 块擦除，  32K字节，参考擦除时间 120ms
#define W25Qxx_CMD_BlockErase_64K 	0xD8		// 块擦除，  64K字节，参考擦除时间 150ms
#define W25Qxx_CMD_ChipErase 			0xC7		// 整片擦除，参考擦除时间 20S

#define W25Qxx_CMD_QuadInputPageProgram  	0x32  		// 页编程指令，参考写入时间 0.4ms 
#define W25Qxx_CMD_FastReadQuad_IO       	0xEB  		// 快速读取指令


#define W25Qxx_PageSize       				256			// 页大小，256字节
#define W25Qxx_FlashSize       				0x800000		// W25Q64大小，8M字节
#define W25Qxx_FLASH_ID           			0X1740EF    // W25Q64 JEDEC ID
//#define W25Qxx_FLASH_ID           			0X1840EF    // W25Q128 JEDEC ID


/*----------------------------------------------- 函数声明 ---------------------------------------------------*/

int8_t 	SFC_W25Qxx_Init(void);	   // 初始化W25Q64

void SFC_W25Qxx_SectorErase(uint32_t SectorAddress);			// 扇区擦除，4K字节， 参考擦除时间 45ms	
void SFC_W25Qxx_BlockErase_32K (uint32_t SectorAddress);	// 块擦除，  32K字节，参考擦除时间 120ms
void SFC_W25Qxx_BlockErase_64K (uint32_t SectorAddress);	// 块擦除，  64K字节，参考擦除时间 150ms
void SFC_W25Qxx_ChipErase (void);                          // 整片擦除，参考擦除时间 20S

void SFC_W25Qxx_WriteBuffer(uint32_t* pBuffer, uint32_t WriteAddr, uint32_t Size);				// 写入数据，只能 4 字节（32bit） 访问
void SFC_W25Qxx_ReadBuffer(uint32_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);	// 读取数据


/*------------------------------------------ LED配置宏 ----------------------------------*/

#define LED1_PIN            			 PIN14        				 	// LED1 引脚      
#define LED1_PORT           			 GPIOD                 		// LED1 GPIO端口     

///*----------------------------------------- LED控制宏 ----------------------------------*/

#define LED1_ON			GPIO_AtomicClrBit(LED1_PORT, LED1_PIN)				// 输出低电平，点亮LED1	
#define LED1_OFF			GPIO_AtomicSetBit(LED1_PORT, LED1_PIN)				// 输出高电平，关闭LED1	
#define LED1_Toggle		GPIO_AtomicInvBit(LED1_PORT, LED1_PIN)				// 翻转IO口状态
	

/*---------------------------------------- 函数声明 ------------------------------------*/

void LED_Init(void);