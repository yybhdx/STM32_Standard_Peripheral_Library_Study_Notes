#ifndef __W25Q64_H
#define __W25Q64_H

/*W25Q64初始化*/
void W25Q64_Init(void);

/*读取设备ID*/
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID);

/*页编程函数(写入数据)*/
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count);

/*扇区擦除*/
void W25Q64_SectorErase(uint32_t Address);

/*读取数据*/
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count);
#endif
