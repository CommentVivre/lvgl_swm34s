#include "lvgl.h"
#include "device.h"



//// 系统定时器中断，1ms中断
#if 0

void SysTick_Handler(void)
{
	SWM341_IncTick();	// 计数+1

	lv_tick_inc(1);	// LVGL时钟节拍	
}

#endif


void UART1_Handler(void)
{
	if(UART_INTStat(UART1, UART_IT_RX_TOUT))
	{
		UART_INTClr(UART1, UART_IT_RX_TOUT);
	}
}

void DMA_Handler(void)
{
	if(DMA_CH_INTStat(DMA_CH1,DMA_IT_DONE))
	{
		DMA_CH_INTClr(DMA_CH1,DMA_IT_DONE);
		DMA_CH_SetDstAddress(DMA_CH0, (uint32_t)uart1_rx_buffer);
		DMA_CH_SetCount(DMA_CH0, uart1_tx_rx_max_len);
		DMA_CH_Open(DMA_CH0);
	}
}

void TIMR2_Handler(void)
{
	if(TIMR_IC_CaptureH_INTStat(TIMR2))
	{
		TIMR_IC_CaptureH_INTClr(TIMR2);
		input_h =TIMR_IC_GetCaptureH(TIMR2);
	}
	else if(TIMR_IC_CaptureL_INTStat(TIMR2))
	{
		TIMR_IC_CaptureL_INTClr(TIMR2);
		input_l =TIMR_IC_GetCaptureL(TIMR2);
	}
}
