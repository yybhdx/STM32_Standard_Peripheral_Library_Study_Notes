#ifndef __STORE_H
#define __STORE_H


extern uint16_t Store_Data[512];
// 数组数量可以不写
//extern uint16_t Store_Data[];
void Store_Init(void);
void Store_Save(void);
void Store_Clear(void);

#endif
