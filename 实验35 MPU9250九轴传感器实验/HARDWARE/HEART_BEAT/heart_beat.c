#include "heart_beat.h"
#include "sys.h"
#include "includes.h"
#include "usart.h"
#include "crc.h"

struct heart_times h_times;

extern u16 heart_beat[57];


void heart_run(void)
{

		if(h_times.second==60)  //��
	{	
	    h_times.second=0;
		h_times.minute++;
	    heart_beat[7]=h_times.minute;
		heart_beat[28]=CRC16(heart_beat,58);
		HAL_UART_Transmit(&UART1_Handler,(uint8_t*)heart_beat,58,1000);
	}
	
	if(h_times.minute==60)  //��
	{
		h_times.minute=0;
		h_times.hour++;
	    heart_beat[6]=h_times.hour;
	}
	
	if(h_times.hour==24)  //ʱ
	{
		h_times.hour=0;
		h_times.day++;
	    heart_beat[5]=h_times.day;
	}
	
	if(h_times.month%2)
	{
	   if(h_times.day==32)  //��
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
	
	if(h_times.month==13)  //��
	{
		h_times.month=1;
		h_times.year++;
	    heart_beat[3]=h_times.year;
	}
	
//	HAL_UART_Transmit_DMA(&UART1_Handler,SendBuff,SEND_BUF_SIZE);//����DMA����

//		    while(1)
//		    {
//                if(__HAL_DMA_GET_FLAG(&UART1TxDMA_Handler,DMA_FLAG_TCIF3_7))//�ȴ�DMA2_Steam7�������
//                {
//                    __HAL_DMA_CLEAR_FLAG(&UART1TxDMA_Handler,DMA_FLAG_TCIF3_7);//���DMA2_Steam7������ɱ�־
//                    HAL_UART_DMAStop(&UART1_Handler);      //��������Ժ�رմ���DMA
//					break; 
//                }

//		    }	
	
}


