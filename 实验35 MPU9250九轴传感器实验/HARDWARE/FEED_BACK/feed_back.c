#include "feed_back.h"
#include "usart.h"
#include "data.h"

extern struct _data_package data_package;
extern u16 feed_back[57];

void feed_back_send(void)
{
	
	if(data_write())
	{
		HAL_UART_Transmit(&UART1_Handler,(uint8_t*)feed_back,58,1000);
		while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//µÈ´ý·¢ËÍ½áÊø
		data_package.commond=0;
	}
}

