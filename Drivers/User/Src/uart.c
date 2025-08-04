#include "uart.h"

/**
 * @function     UART0_Init
 * @brief        串口初始化
 * @date         2025-08-04
*/
void UART0_Init(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(UART0_RX_PORT, UART0_RX_PIN, PORTM_PIN0_UART0_RX, 1);	// 配置UART0输入引脚
	PORT_Init(UART0_TX_PORT, UART0_TX_PIN, PORTM_PIN1_UART0_TX, 0);	// 配置UART0输出引脚
	
 	UART_initStruct.Baudrate = UART0_BaudRate;			// 设置波特率
	UART_initStruct.DataBits = UART_DATA_8BIT;			// 8位数据宽度
	UART_initStruct.Parity = UART_PARITY_NONE;		// 无校验
	UART_initStruct.StopBits = UART_STOP_1BIT;			// 停止位1
	UART_initStruct.RXThreshold = 3;							// RX FIFO阈值	
	UART_initStruct.RXThresholdIEn = 0;							// 当RX FIFO中数据个数 >  RXThreshold时触发中断
	UART_initStruct.TXThreshold = 3;							// TX FIFO阈值	
	UART_initStruct.TXThresholdIEn = 0;							// 当TX FIFO中数据个数 <= TXThreshold时触发中断
	UART_initStruct.TimeoutTime = 10;							// 超时时长 = TimeoutTime/(Baudrate/10) 秒
	UART_initStruct.TimeoutIEn = 0;							// 超时中断，超过 TimeoutTime/(Baudrate/10) 秒没有在RX线上接收到数据时触发中断
	
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}

/**
 * @function     UART1_Init
 * @brief        串口初始化
 * @date         2025-08-04
*/
void UART1_Init(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(UART1_RX_PORT, UART1_RX_PIN, PORTM_PIN2_UART1_RX, 1);
	PORT_Init(UART1_TX_PORT, UART1_TX_PIN, PORTM_PIN3_UART1_TX, 0);
	
 	UART_initStruct.Baudrate = UART1_BaudRate;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThreshold = 3;							// RX FIFO阈值	
	UART_initStruct.RXThresholdIEn = 0;							// 当RX FIFO中数据个数 >  RXThreshold时触发中断
	UART_initStruct.TXThreshold = 3;							// TX FIFO阈值	
	UART_initStruct.TXThresholdIEn = 0;							// 当TX FIFO中数据个数 <= TXThreshold时触发中断
	UART_initStruct.TimeoutTime = 10;							// 超时时长 = TimeoutTime/(Baudrate/10) 秒
	UART_initStruct.TimeoutIEn = 0;								// 超时中断，超过 TimeoutTime/(Baudrate/10) 秒没有在RX线上接收到数据时触发中断
	
 	UART_Init(UART1, &UART_initStruct);
	UART_Open(UART1);
}






/*************************************************************************************************
*	LVGL因为需要用__aeabi_assert，因此不能再勾选 microLib 以使用printf
*	添加以下代码，让标准C库支持重定向fput
*  根据编译器，选择对应的代码即可
*************************************************************************************************/

// AC6编译器使用这段代码
__asm (".global __use_no_semihosting\n\t");
void _sys_exit(int x) 
{
  x = x;
}
//__use_no_semihosting was requested, but _ttywrch was 
void _ttywrch(int ch)
{
    ch = ch;
}

FILE __stdout;


/*************************************************************************************************
*	函 数 名:	fputc
*	入口参数:	ch - 要输出的字符 ，  f - 文件指针（这里用不到）
*	返 回 值:	正常时返回字符，出错时返回 EOF（-1）
*	函数功能:	重定向 fputc 函数，目的是使用 printf 函数
*	说    明:	无		
*************************************************************************************************/

int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);	
	while(UART_IsTXBusy(UART0));
 	
	return (ch);
}

