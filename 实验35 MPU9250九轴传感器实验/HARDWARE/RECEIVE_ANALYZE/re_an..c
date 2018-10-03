#include "re_an.h"
#include "usart.h"
#include "sys.h"
#include "data.h"
#include "crc.h"

extern struct _data_package data_package;
u16 crc_check[57];

void re_an(void)
{
    u8 len,i;
	u8 data_len;  //length of data
	
	
	if(USART_RX_STA&0x8000)
		{		
	        len=USART_RX_STA&0x3fff;//get lenth 
		    
			if(len==58)
			{
				if((USART_RX_BUF[0]==0x03)&&
				   (USART_RX_BUF[1]==0x9c)&&
				   (USART_RX_BUF[2]==0xa0)&&
				   (USART_RX_BUF[3]==0x10)
				  )
				{
					for(i=0;i<58;i++) crc_check[i]=USART_RX_BUF[i];
					crc_check[0]=CRC16(crc_check,58);
					
//					if((USART_RX_BUF[56]==crc_check[0])&&
//					   (USART_RX_BUF[57]==crc_check[1])
//					  )
//					{
					for(i=55;i>5;i--)   //in cace there are some datas are zero
					{
					   if(USART_RX_BUF[i]!=0) 
					   {
						   goto a;
						   
					   }
					}
					
					a:data_len=i-5;
//					printf("%d\r\n",data_len);
//					printf("1111111111111111111");
					
					data_package.commond=0xa010;
					data_package.len=data_len;
//				    }
				}
		
				if((USART_RX_BUF[0]==0x03)&&
				   (USART_RX_BUF[1]==0x9c)&&
				   (USART_RX_BUF[2]==0x10)&&
				   (USART_RX_BUF[3]==0x10)		
				  )
				{
					for(i=55;i>5;i--)   //in cace there are some datas are zero
					{
						if(USART_RX_BUF[i]!=0) 
						  {
							  goto b;
							   
						  }
				    }
					
					b:data_len=i-5;
					
					data_package.commond=0x1010;
					data_package.len=data_len;
				}
				
				if((USART_RX_BUF[0]==0x03)&&
				   (USART_RX_BUF[1]==0x9c)&&
				   (USART_RX_BUF[2]==0x20)&&
				   (USART_RX_BUF[3]==0x10)		
				  )
				{
					for(i=55;i>5;i--)   //in cace there are some datas are zero
					{
					   if(USART_RX_BUF[i]!=0) 
					   {
						   goto c;
						   
					   }
					}
					
					c:data_len=i-5;
					
					data_package.commond=0x2010;
					data_package.len=data_len;
				}
				
				if((USART_RX_BUF[0]==0x03)&&
				   (USART_RX_BUF[1]==0x9c)&&
				   (USART_RX_BUF[2]==0x20)&&
				   (USART_RX_BUF[3]==0x20)	
				  )
				{
					for(i=55;i>5;i--)   //in cace there are some datas are zero
					{
					   if(USART_RX_BUF[i]!=0) 
					   {
						   goto d;
						   
					   }
					}
					
					d:data_len=i-5;
					
					data_package.commond=0x2020;
					data_package.len=data_len;
				}
				

				USART_RX_STA=0;
			
			}
			
		}
	

}


