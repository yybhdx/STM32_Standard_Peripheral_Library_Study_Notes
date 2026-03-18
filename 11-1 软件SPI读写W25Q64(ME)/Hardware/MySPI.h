#ifndef __MY_SPI_H
#define __MY_SPI_H

/*SPI初始化*/
void MySPI_Init(void);

/*起始信号*/
void MYSPI_Start(void);

/*结束信号*/
void MYSPI_Stop(void);

/*交换一个字节(也叫读写一个字节)*/
uint8_t MySPI_SwapByte(uint8_t ByteSend);
#endif
