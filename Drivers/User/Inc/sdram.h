#ifndef  __SDRAM_H
#define	__SDRAM_H

#include "SWM341.h"
#include "uart.h"


#define SDRAM_Size 8*1024*1024  								// 容量8M字节
#define SDRAM_BANK_ADDR     ((uint32_t)0x80000000) 	// SDRAM 地址

/*--------------------------------------- 函数声明 -------------------------------------*/

void	SDRAM_Config(void);			// SDRAM初始化

#endif 
