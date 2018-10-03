#ifndef _DATA_H
#define _DATA_H
#include "sys.h"

struct _data_package
{
   u16 head;
   u16 commond;
   u16 len;
   u16 data[49];
   u16 crc;
   


};

u16 data_write();



#endif

