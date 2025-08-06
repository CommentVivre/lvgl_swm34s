#include "uart.h"



uint8_t uart1_rx_buffer[uart1_tx_rx_max_len]={0},uart1_tx_buffer[uart1_tx_rx_max_len]={0};

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
	UART_initStruct.TimeoutIEn = 1;								// 超时中断，超过 TimeoutTime/(Baudrate/10) 秒没有在RX线上接收到数据时触发中断
 	UART_Init(UART1, &UART_initStruct);
	UART_Open(UART1);
	
	// DMA配置
	DMA_InitStructure DMA_initStruct;
	// 接收配置
	DMA_initStruct.Mode = DMA_MODE_SINGLE;
	DMA_initStruct.Unit = DMA_UNIT_BYTE;
	DMA_initStruct.Count = uart1_tx_rx_max_len;
	DMA_initStruct.SrcAddr = (uint32_t)&UART1->DATA;
	DMA_initStruct.SrcAddrInc = 0;
	DMA_initStruct.DstAddr = (uint32_t)uart1_rx_buffer;
	DMA_initStruct.DstAddrInc = 1;
	DMA_initStruct.Handshake = DMA_CH0_UART1RX;
	DMA_initStruct.Priority = DMA_PRI_LOW;
	DMA_initStruct.INTEn = 0;
	DMA_CH_Init(DMA_CH0, &DMA_initStruct);
	// 发送配置
	DMA_initStruct.Mode = DMA_MODE_SINGLE;
	DMA_initStruct.Unit = DMA_UNIT_BYTE;
	DMA_initStruct.SrcAddrInc = 1;
	DMA_initStruct.DstAddr = (uint32_t)&UART1->DATA;
	DMA_initStruct.DstAddrInc = 0;
	DMA_initStruct.Handshake = DMA_CH1_UART1TX;
	DMA_initStruct.Priority = DMA_PRI_LOW;
	DMA_initStruct.INTEn = DMA_IT_DONE;
	DMA_CH_Init(DMA_CH1, &DMA_initStruct);
	
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




uint16_t calculate_crc16(const uint8_t *data, uint16_t length) 
{
    uint16_t crc = 0xFFFF;  // 步骤1：预置CRC寄存器为0xFFFF

    for (uint16_t i = 0; i < length; i++) {
        crc ^= data[i];     // 步骤2：当前字节与CRC低字节异或

        for (uint16_t j = 0; j < 8; j++) {  // 处理每个字节的8位
            uint8_t lsb = crc & 1;     // 获取最低位
            crc >>= 1;                 // 步骤3：右移一位（高位补0）

            if (lsb) {                 // 步骤4：检测移出的最低位
                crc ^= 0xA001;         // 若为1则与预设值异或
            }
        }
    }
    return crc;  // 步骤7：返回最终CRC值
}

void read_im1281b(uint8_t dev_addr,uint16_t reg_addr,uint16_t reg_len,uint8_t *buf)
{
	uint16_t calc_crc=0x0000;
	buf[0]=dev_addr; // 地址位
	buf[1]=0x03; // 读命令
	buf[2]=reg_addr >>8; // 高地址
	buf[3]=reg_addr&0xFF;
	buf[4]=reg_len>>8;
	buf[5]=reg_len&0xFF;
	calc_crc=calculate_crc16(buf,6);
	buf[6]=calc_crc&0xFF;
	buf[7]=calc_crc>>8;
}
