#include "heart_beat.h"
#include "sys.h"
#include "includes.h"
#include "usart.h"
#include "crc.h"

struct heart_times h_times;

extern u16 heart_beat[57];


void heart_run(void)
{

		if(h_times.second==60)  //秒
	{	
	    h_times.second=0;
		h_times.minute++;
	    heart_beat[7]=h_times.minute;
		heart_beat[28]=CRC16(heart_beat,58);
		HAL_UART_Transmit(&UART1_Handler,(uint8_t*)heart_beat,58,1000);
	}
	
	if(h_times.minute==60)  //分
	{
		h_times.minute=0;
		h_times.hour++;
	    heart_beat[6]=h_times.hour;
	}
	
	if(h_times.hour==24)  //时
	{
		h_times.hour=0;
		h_times.day++;
	    heart_beat[5]=h_times.day;
	}
	
	if(h_times.month%2)
	{
	   if(h_times.day==32)  //日
	   {
		   h_times.day=1;
		   h_times.month++;
	       heart_beat[4]=h_times.month;
	   }
    }
	
	if(!(h_times.month%2))
	{
	  if(h_times.month==2)
	  {
	     if(h_times.day==28) 
		 {	 
		    h_times.day=1;
			h_times.month++;
		    heart_beat[4]=h_times.month;
		 }
	  }		  
	  else
	  {
	     if(h_times.day==30) h_times.day=1;h_times.month++;
		 heart_beat[4]=h_times.month;
	  }
	}
	
	if(h_times.month==13)  //月
	{
		h_times.month=1;
		h_times.year++;
	    heart_beat[3]=h_times.year;
	}
	
//	HAL_UART_Transmit_DMA(&UART1_Handler,SendBuff,SEND_BUF_SIZE);//开启DMA传输

//		    while(1)
//		    {
//                if(__HAL_DMA_GET_FLAG(&UART1TxDMA_Handler,DMA_FLAG_TCIF3_7))//等待DMA2_Steam7传输完成
//                {
//                    __HAL_DMA_CLEAR_FLAG(&UART1TxDMA_Handler,DMA_FLAG_TCIF3_7);//清除DMA2_Steam7传输完成标志
//                    HAL_UART_DMAStop(&UART1_Handler);      //传输完成以后关闭串口DMA
//					break; 
//                }

//		    }	
	
}


