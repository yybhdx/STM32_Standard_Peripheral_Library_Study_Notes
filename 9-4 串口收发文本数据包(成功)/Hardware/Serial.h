#ifndef __Serial_H
#define __Serial_H

#include <stdio.h>


extern char Serial_RxPacket[4]; // 存储接收的载荷数据，包头包尾不存在

extern uint8_t Serial_RxFlag; //接收数据包的标志位，如果接收到一个数据包，就置RxFlag

void Serial_Init(void);

void Serial_SendByte(uint8_t Byte);

void Serial_SendArray(uint8_t *Array, uint16_t Length);

void Serial_SendString(char *string);

void Serial_SendNumber(uint32_t Number, uint8_t Length);

void Serial_Printf(char *format, ...);



#endif
