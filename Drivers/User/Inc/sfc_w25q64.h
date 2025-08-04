#ifndef __SFC_w25q64_H
#define __SFC_w25q64_H

#include "SWM341.h"
#include "uart.h"

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

void 		SFC_W25Qxx_SectorErase(uint32_t SectorAddress);			// 扇区擦除，4K字节， 参考擦除时间 45ms	
void 		SFC_W25Qxx_BlockErase_32K (uint32_t SectorAddress);	// 块擦除，  32K字节，参考擦除时间 120ms
void 		SFC_W25Qxx_BlockErase_64K (uint32_t SectorAddress);	// 块擦除，  64K字节，参考擦除时间 150ms
void 		SFC_W25Qxx_ChipErase (void);                          // 整片擦除，参考擦除时间 20S

void 		SFC_W25Qxx_WriteBuffer(uint32_t* pBuffer, uint32_t WriteAddr, uint32_t Size);				// 写入数据，只能 4 字节（32bit） 访问
void 		SFC_W25Qxx_ReadBuffer(uint32_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);	// 读取数据

#endif // __SFC_w25q64_H 





