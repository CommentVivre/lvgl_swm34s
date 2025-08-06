#include "device.h"

// 1.W25QXX的擦除时间是限定的!!! 手册给出的典型参考值为: 4K-45ms, 32K-120ms ,64K-150ms,整片擦除20S
// 2.W25QXX的写入时间是限定的!!! 手册给出的典型参考值为: 256字节-0.4ms，也就是 1M字节/s （实测大概在600K字节/s左右）

// 摘抄自《SWM34Sxxxx驱动TFT-LCD应用笔记》：	
// LCDC 使用的是 AHB 总线，当其他外设长时间抢占 AHB 总线时（例如 SFC 写入会占用 AHB 总线较大的带宽）有可能会导致 LCDC 无法及时响应输出数据，表现为显示画面上下闪烁条纹/晃动
// 对于 SFC 写入影响 LCD 刷屏，目前有以下做法供参考：
// 1、转用 IO 模拟 SPI 通讯来 Write SPI_Flash (注意: SFC 与 IO_SPI 之间多次反复的端口初始化切换);
// 2、减小 SFC Write 单次写入数据量, 增加 SFC Write 总写入次数(即多次调用, 反复短时占用->释放总线,缓解 LCD 显示需求)。

// 总结：SFC如果要和LCDC一起用，则只适合用来存数据，例如LVGL应用中，用来存各种UI资源，因为程序下载进去之后，单片机运行时SFC只进行读数据，因此不会有上面提到的冲突

/*************************************************************************************************
*	函 数 名: SFC_W25Qxx_Init
*	
*	函数功能: 初始化 SFC 配置，读取W25Q64ID
*	
*************************************************************************************************/

int8_t SFC_W25Qxx_Init(void)
{
	uint32_t Device_ID;
	SFC_InitStructure SFC_initStruct;

	/* SFC使用专用的FSPI（Flash SPI）接口连接SPI Flash */
	PORT_Init(PORTD, PIN5, PORTD_PIN5_FSPI_SCLK, 0);
	PORT_Init(PORTD, PIN6, PORTD_PIN6_FSPI_SSEL, 0);
	PORT_Init(PORTD, PIN8, PORTD_PIN8_FSPI_MOSI, 1);
	PORT_Init(PORTD, PIN7, PORTD_PIN7_FSPI_MISO, 1);
	PORT_Init(PORTD, PIN3, PORTD_PIN3_FSPI_DATA2, 1);
	PORT_Init(PORTD, PIN4, PORTD_PIN4_FSPI_DATA3, 1);
	
	// SFC 驱动时钟 = 内核时钟 150MHz / ClkDiv 
	SFC_initStruct.ClkDiv = SFC_CLKDIV_2;									// 2分频，则驱动时钟为75M
	SFC_initStruct.Cmd_Read = W25Qxx_CMD_FastReadQuad_IO;					// 读命令
	SFC_initStruct.Width_Read = SFC_RDWIDTH_4;								// 4线读操作模式
	SFC_initStruct.Cmd_PageProgram = W25Qxx_CMD_QuadInputPageProgram;		// 页编程命令
	SFC_initStruct.Width_PageProgram = SFC_PPWIDTH_4;						// 4线写操作模式

	SFC_Init(&SFC_initStruct);  

	Device_ID = SFC_ReadJEDEC();	// 读取ID
	
	if( Device_ID == W25Qxx_FLASH_ID )		// 进行匹配
	{
//		printf ("W25Q64 OK,flash ID:%X\r\n",Device_ID);		// 初始化成功
		return W25Qxx_OK;			// 返回成功标志		
	}
	else
	{
//		printf ("W25Q64 ERROR!!!!!  ID:%X\r\n",Device_ID);	// 初始化失败	
		return W25Qxx_ERROR;		// 返回错误标志
	}		
}


/*************************************************************************************************
*
*	函 数 名: SFC_W25Qxx_SectorErase
*
*	入口参数: SectorAddress - 要擦除的地址
*	    
*	函数功能: 进行扇区擦除操作，每次擦除4K字节
*
*	说    明: 1.按照 W25Q64JV 数据手册给出的擦除参考时间，典型值为 45ms，最大值为400ms
*				 2.实际的擦除速度可能大于45ms，也可能小于45ms
*				 3.flash使用的时间越长，擦除所需时间也会越长
*
**************************************************************************************************/

void SFC_W25Qxx_SectorErase(uint32_t SectorAddress)	
{
	SFC_EraseEx(SectorAddress, W25Qxx_CMD_SectorErase, 1);	// 执行擦除，并等待	
}



/*************************************************************************************************
*
*	函 数 名: SFC_W25Qxx_BlockErase_32K
*
*	入口参数: SectorAddress - 要擦除的地址
*
*	函数功能: 进行块擦除操作，每次擦除32K字节
*
*	说    明: 1.按照 W25Q64JV 数据手册给出的擦除参考时间，典型值为 120ms，最大值为1600ms
*				 2.实际的擦除速度可能大于120ms，也可能小于120ms
*				 3.flash使用的时间越长，擦除所需时间也会越长
*
*************************************************************************************************/

void SFC_W25Qxx_BlockErase_32K (uint32_t SectorAddress)	
{
	SFC_EraseEx(SectorAddress, W25Qxx_CMD_BlockErase_32K, 1);	// 执行擦除，并等待	
}


/*************************************************************************************************
*
*	函 数 名: SFC_W25Qxx_BlockErase_64K
*
*	入口参数: SectorAddress - 要擦除的地址
*
*	函数功能: 进行块擦除操作，每次擦除64K字节
*
*	说    明: 1.按照 W25Q64JV 数据手册给出的擦除参考时间，典型值为 150ms，最大值为2000ms
*				 2.实际的擦除速度可能大于150ms，也可能小于150ms
*				 3.flash使用的时间越长，擦除所需时间也会越长
*				 4.实际使用建议使用64K擦除，擦除的时间最快
*
**************************************************************************************************/

void SFC_W25Qxx_BlockErase_64K (uint32_t SectorAddress)	
{
	SFC_EraseEx(SectorAddress, W25Qxx_CMD_BlockErase_64K, 1);	// 执行擦除，并等待	
}


/*************************************************************************************************
*
*	函 数 名: SFC_W25Qxx_ChipErase
*
*	函数功能: 进行整片擦除操作
*
*	说    明: 1.按照 W25Q64JV 数据手册给出的擦除参考时间，典型值为 20s，最大值为100s
*				 2.实际的擦除速度可能大于20s，也可能小于20s
*				 3.flash使用的时间越长，擦除所需时间也会越长
*
*************************************************************************************************/

void SFC_W25Qxx_ChipErase (void)	
{
	SFC_EraseEx(0xFFFFFFFF, W25Qxx_CMD_ChipErase, 1);	// 执行擦除，并等待		
}



/**********************************************************************************************************
*
*	函 数 名: SFC_W25Qxx_WriteBuffer
*
*	入口参数: pBuffer 		 - 要写入的数据（32位宽）
*				 WriteAddr 		 - 要写入 W25Qxx 的地址
*				 NumByteToWrite - 数据长度，最大不能超过flash芯片的大小
*
*	函数功能: 写入数据，最大不能超过flash芯片的大小，请务必完成擦除操作
*
*	说    明: 1.Flash的写入时间和擦除时间一样，是有限定的，并不是说SPI驱动时钟133M就可以以这个速度进行写入
*				 2.按照 W25Q64JV 数据手册给出的 页 写入参考时间，典型值为 0.4ms，最大值为3ms
*				 3.实际的写入速度可能大于0.4ms，也可能小于0.4ms
*				 4.Flash使用的时间越长，写入所需时间也会越长
*				 5.在数据写入之前，请务必完成擦除操作
*
**********************************************************************************************************/

void 	SFC_W25Qxx_WriteBuffer(uint32_t* pBuffer, uint32_t WriteAddr, uint32_t Size)
{	
	
// 重要说明（摘抄自《SWM34Sxxxx驱动TFT-LCD应用笔记》）：	
/*
	LCDC 使用的是 AHB 总线，当其他外设长时间抢占 AHB 总线时（例如 SFC 写入会占用 AHB 总线较大的带
	宽）有可能会导致 LCDC 无法及时响应输出数据，表现为显示画面上下闪烁条纹/晃动，故用户应在应用层通过软
	件设计尽量避免此类情况的发生（例如 SFC 写入时关闭屏幕显示）。
	
	对于 SFC 写入影响 LCD 刷屏，目前有以下做法供参考：
	
	1、转用 IO 模拟 SPI 通讯来 Write SPI_Flash (注意: SFC 与 IO_SPI 之间多次反复的端口初始化切换);
	
	2、减小 SFC Write 单次写入数据量, 增加 SFC Write 总写入次数(即多次调用, 反复短时占用->释放总线,
	缓解 LCD 显示需求)。
	
	总结：SFC如果要和LCDC一起用，则只适合用来存数据，例如LVGL应用中，用来存各种UI资源，因为程序下载进去之后，
			单片机运行时SFC只进行读数据，因此不会有上面的冲突
	
*/
	
	SFC_Write(WriteAddr, pBuffer, Size);		
}



/**********************************************************************************************************************************
*
*	函 数 名: SFC_W25Qxx_ReadBuffer
*
*	入口参数: pBuffer 		 - 要读取的数据（32位宽）
*				 ReadAddr 		 - 要读取 W25Qxx 的地址
*				 NumByteToRead  - 数据长度，最大不能超过flash芯片的大小
*
*	函数功能: 读取数据，最大不能超过flash芯片的大小
*
*	说    明: 1.Flash的读取速度取决于QSPI的通信时钟，最大不能超过133M
*
*****************************************************************************************************************FANKE************/

void SFC_W25Qxx_ReadBuffer(uint32_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
	SFC_Read(ReadAddr, pBuffer, NumByteToRead);
}
