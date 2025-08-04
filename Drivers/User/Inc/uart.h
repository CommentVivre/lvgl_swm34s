#pragma once

#include "SWM341.h"
#include "stdio.h"

/*-------------------------------------------- USART配置宏 ---------------------------------------*/

#define UART0_BaudRate  115200

#define	UART0_TX_PIN									PIN1					// TX 引脚
#define	UART0_TX_PORT									PORTM					// TX 引脚端口
#define UART0_RX_PIN									PIN0             		// RX 引脚
#define	UART0_RX_PORT									PORTM					// RX 引脚端口

#define UART1_BaudRate  4800

#define	UART1_TX_PIN									PIN3					// TX 引脚
#define	UART1_TX_PORT									PORTM					// TX 引脚端口
#define UART1_RX_PIN									PIN2					// RX 引脚
#define	UART1_RX_PORT									PORTM                 	// RX 引脚端口



void UART0_Init(void) ;	// UART0初始化函数






