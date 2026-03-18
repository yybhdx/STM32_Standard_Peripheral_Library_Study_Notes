#ifndef __MYFLASH_H
#define __MYFLASH_H

// 读取全字
uint32_t MyFLASH_ReadWord(uint32_t Address);

// 读取半字
uint16_t MyFLASH_ReadHalfWord(uint32_t Address);

// 读取字节
uint8_t MyFLASH_ReadByte(uint32_t Address);


void MyFLASH_EraseAllPages(void);

void MyFLASH_ErasePage(uint32_t PageAddress);

// 写入一个字
void MyFLASH_ProgramWord(uint32_t Address, uint32_t Data);

// 写入一个半字
void MyFLASH_ProgramHalfWord(uint32_t Address, uint16_t Data);

#endif
