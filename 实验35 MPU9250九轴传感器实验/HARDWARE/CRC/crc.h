#ifndef _CRC_H
#define _CRC_H
#include "sys.h"

u16 CRC16(u16 *pchMsg, u16 wDataLen);
unsigned short CRC16_CCITT(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_CCITT_FALSE(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_XMODEM(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_X25(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_IBM(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_MAXIM(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_USB(unsigned char *puchMsg, unsigned int usDataLen);


#endif


