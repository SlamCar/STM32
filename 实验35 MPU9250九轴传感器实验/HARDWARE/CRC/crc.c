#include "crc.h"





//unsigned short CRC16_CCITT(unsigned char *puchMsg, unsigned int usDataLen)
//{
//  unsigned short wCRCin = 0x0000;
//  unsigned short wCPoly = 0x1021;
//  unsigned char wChar = 0;
//  
//  while (usDataLen--) 	
//  {
//        wChar = *(puchMsg++);
//        InvertUint8(&wChar,&wChar);
//        wCRCin ^= (wChar << 8);
//        for(int i = 0;i < 8;i++)
//        {
//          if(wCRCin & 0x8000)
//            wCRCin = (wCRCin << 1) ^ wCPoly;
//          else
//            wCRCin = wCRCin << 1;
//        }
//  }
//  InvertUint16(&wCRCin,&wCRCin);
//  return (wCRCin) ;
//}




//unsigned short CRC16_CCITT_FALSE(unsigned char *puchMsg, unsigned int usDataLen)
//{
//	u8 i;
//  unsigned short wCRCin = 0xFFFF;
//  unsigned short wCPoly = 0x1021;
//  unsigned char wChar = 0;
//  
//  while (usDataLen--) 	
//  {
//        wChar = *(puchMsg++);
//        wCRCin ^= (wChar << 8);
//        for(i=0;i<8;i++)
//        {
//          if(wCRCin & 0x8000)
//            wCRCin = (wCRCin << 1) ^ wCPoly;
//          else
//            wCRCin = wCRCin << 1;
//        }
//  }
//  return (wCRCin) ;
//}




//unsigned short CRC16_XMODEM(unsigned char *puchMsg, unsigned int usDataLen)
//{
//	u8 i;
//  unsigned short wCRCin = 0x0000;
//  unsigned short wCPoly = 0x1021;
//  unsigned char wChar = 0;
//  
//  while (usDataLen--) 	
//  {
//        wChar = *(puchMsg++);
//        wCRCin ^= (wChar << 8);
//        for(i=0;i<8;i++)
//        {
//          if(wCRCin & 0x8000)
//            wCRCin = (wCRCin << 1) ^ wCPoly;
//          else
//            wCRCin = wCRCin << 1;
//        }
//  }
//  return (wCRCin) ;
//}
// 



//unsigned short CRC16_X25(unsigned char *puchMsg, unsigned int usDataLen)
//{
//  unsigned short wCRCin = 0xFFFF;
//  unsigned short wCPoly = 0x1021;
//  unsigned char wChar = 0;
//  
//  while (usDataLen--) 	
//  {
//        wChar = *(puchMsg++);
//        InvertUint8(&wChar,&wChar);
//        wCRCin ^= (wChar << 8);
//        for(int i = 0;i < 8;i++)
//        {
//          if(wCRCin & 0x8000)
//            wCRCin = (wCRCin << 1) ^ wCPoly;
//          else
//            wCRCin = wCRCin << 1;
//        }
//  }
//  InvertUint16(&wCRCin,&wCRCin);
//  return (wCRCin^0xFFFF) ;
//}
 

//unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen)
//{
//  unsigned short wCRCin = 0xFFFF;
//  unsigned short wCPoly = 0x8005;
//  unsigned char wChar = 0;
//  
//  while (usDataLen--) 	
//  {
//        wChar = *(puchMsg++);
//        InvertUint8(&wChar,&wChar);
//        wCRCin ^= (wChar << 8);
//        for(int i = 0;i < 8;i++)
//        {
//          if(wCRCin & 0x8000)
//            wCRCin = (wCRCin << 1) ^ wCPoly;
//          else
//            wCRCin = wCRCin << 1;
//        }
//  }
//  InvertUint16(&wCRCin,&wCRCin);
//  return (wCRCin) ;
//}



//unsigned short CRC16_IBM(unsigned char *puchMsg, unsigned int usDataLen)
//{
//  unsigned short wCRCin = 0x0000;
//  unsigned short wCPoly = 0x8005;
//  unsigned char wChar = 0;
//  
//  while (usDataLen--) 	
//  {
//        wChar = *(puchMsg++);
//        InvertUint8(&wChar,&wChar);
//        wCRCin ^= (wChar << 8);
//        for(int i = 0;i < 8;i++)
//        {
//          if(wCRCin & 0x8000)
//            wCRCin = (wCRCin << 1) ^ wCPoly;
//          else
//            wCRCin = wCRCin << 1;
//        }
//  }
//  InvertUint16(&wCRCin,&wCRCin);
//  return (wCRCin) ;
//}



//unsigned short CRC16_MAXIM(unsigned char *puchMsg, unsigned int usDataLen)
//{
//  unsigned short wCRCin = 0x0000;
//  unsigned short wCPoly = 0x8005;
//  unsigned char wChar = 0;
//  
//  while (usDataLen--) 	
//  {
//        wChar = *(puchMsg++);
//        InvertUint8(&wChar,&wChar);
//        wCRCin ^= (wChar << 8);
//        for(int i = 0;i < 8;i++)
//        {
//          if(wCRCin & 0x8000)
//            wCRCin = (wCRCin << 1) ^ wCPoly;
//          else
//            wCRCin = wCRCin << 1;
//        }
//  }
//  InvertUint16(&wCRCin,&wCRCin);
//  return (wCRCin^0xFFFF) ;
//}


//unsigned short CRC16_USB(unsigned char *puchMsg, unsigned int usDataLen)
//{
//  unsigned short wCRCin = 0xFFFF;
//  unsigned short wCPoly = 0x8005;
//  unsigned char wChar = 0;
//  
//  while (usDataLen--) 	
//  {
//        wChar = *(puchMsg++);
//        InvertUint8(&wChar,&wChar);
//        wCRCin ^= (wChar << 8);
//        for(int i = 0;i < 8;i++)
//        {
//          if(wCRCin & 0x8000)
//            wCRCin = (wCRCin << 1) ^ wCPoly;
//          else
//            wCRCin = wCRCin << 1;
//        }
//  }
//  InvertUint16(&wCRCin,&wCRCin);
//  return (wCRCin^0xFFFF) ;
//}


static const int wCRCTalbeAbs[] =
{
    0x0000,0xCC01,0xD801,0x1400,0xF001,0x3C00,0x2800,0xE401,0xA001,0x6C00,0x7800,0xB401,0x5000,0x9C01,0x8801,0x4400
};
 
u16 CRC16(u16 *pchMsg, u16 wDataLen)
{
int wCRC = 0xFFFF;
int i;
unsigned char chChar;
for (i=0;i<wDataLen;i++)
{
	chChar = *pchMsg++;
	wCRC = wCRCTalbeAbs[(chChar ^ wCRC) & 15] ^ (wCRC >> 4);
	wCRC = wCRCTalbeAbs[((chChar >> 4) ^ wCRC) & 15] ^ (wCRC >> 4);
}
    return wCRC;
}
 



