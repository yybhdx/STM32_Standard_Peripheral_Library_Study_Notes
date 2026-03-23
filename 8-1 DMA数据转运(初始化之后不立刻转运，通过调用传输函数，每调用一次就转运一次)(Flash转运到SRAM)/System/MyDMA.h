#ifndef __MYDMA_H
#define __MYDMA_H

// DMA初始化
void MyDMA_Init(uint32_t AddrA,uint32_t AddrB, uint32_t Size);

// DMA传输函数(调用一次这个函数，就再次启动一次DMA转运)
void MyDMA_Transfer(void);

#endif
