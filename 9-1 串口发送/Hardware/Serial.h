#ifndef __Serial_H
#define __Serial_H
void Serial_Init(void);

void Serial_SendByte(uint8_t Byte);

void Serial_SendArray(uint8_t *Array, uint16_t Length);

void Serial_SendString(char *string);

void Serial_SendNumber(uint32_t Number, uint8_t Length);

#endif
