#include "data.h"
#include "sys.h"
#include "crc.h"

struct _data_package data_package;
extern u16 feed_back[57];

u8 i;
u8 flag;

u16 data_write()
{
	
	if(data_package.commond==0xa010)
	{
		feed_back[0]=data_package.head;
		feed_back[1]=data_package.commond;
		feed_back[2]=data_package.len;
		
		
		for(i=0;i<25;i++)
		{
			feed_back[i+3]=data_package.data[i];
		}
		feed_back[28]=CRC16(feed_back,58);

		return 1;
	}
	
	if(data_package.commond==0x1010)
	{
	  feed_back[0]=data_package.head;
		feed_back[1]=data_package.commond;
		feed_back[2]=data_package.len;
		
		
		for(i=0;i<25;i++)
		{
			feed_back[i+3]=data_package.data[i];
		}
		feed_back[28]=CRC16(feed_back,58);
		return 1;
	}
	
	if(data_package.commond==0xa010)
	{
	  feed_back[0]=data_package.head;
		feed_back[1]=data_package.commond;
		feed_back[2]=data_package.len;
		
		
		for(i=0;i<25;i++)
		{
			feed_back[i+3]=data_package.data[i];
		}
		feed_back[28]=CRC16(feed_back,58);
		return 1;
	}
	
	if(data_package.commond==0xa010)
	{
	  feed_back[0]=data_package.head;
		feed_back[1]=data_package.commond;
		feed_back[2]=data_package.len;
		
		
		for(i=0;i<25;i++)
		{
			feed_back[i+3]=data_package.data[i];
		}
		feed_back[28]=CRC16(feed_back,58);
		return 1;
	}
	
	return 0;

}

