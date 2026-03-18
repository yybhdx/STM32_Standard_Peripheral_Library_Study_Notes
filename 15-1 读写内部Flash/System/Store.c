#include "stm32f10x.h"                  // Device header
#include "MyFLASH.h"

#define STORE_START_ADDRESS 0X0800FC00
#define STORE_COUNT 				512
uint16_t Store_Data[STORE_COUNT];


void Store_Init(void)
{
	// 取第一个半字，如果第一个半字不是A5A5就说明是第一次使用。这个A5A5是自己随便规定的一个标志位
	if(MyFLASH_ReadHalfWord(STORE_START_ADDRESS) != 0XA5A5)
	{
		// 1.擦除最后一页
		MyFLASH_ErasePage(STORE_START_ADDRESS);
		
		// 2.在起始地址0X0800FC00，也就是第一个半字的位置，写入规定的标志位0XA5A5
		MyFLASH_ProgramHalfWord(STORE_START_ADDRESS, 0xA5A5);
		
		// 3.把剩余的存储空间，全都置为默认值0
		for(uint16_t i=1; i < STORE_COUNT; i++)
		{
			// 向STM32的内部FLASH指定地址写入一个16位的半字数据
			// 从这个地址0X0800FC00开始，然后每个循环，加上i因为一个半字占用两个地址，所以i要乘2，这就是后续每个半字的起始地址，写入的半字我们给0X0000
			MyFLASH_ProgramHalfWord(STORE_START_ADDRESS + i*2,0x0000);
		}

	}
	
	for(uint16_t i=0; i < STORE_COUNT; i++)
	{
		// 读取半字，到Store_Data数组里面
		Store_Data[i] = MyFLASH_ReadHalfWord(STORE_START_ADDRESS + i * 2);
	}
}

void Store_Save(void)
{
	MyFLASH_ErasePage(STORE_START_ADDRESS);
	
	for(uint16_t i=0; i < STORE_COUNT; i++)
	{
		// 写入一个半字
		MyFLASH_ProgramHalfWord(STORE_START_ADDRESS + i * 2, Store_Data[i]);
	}
}

void Store_Clear(void)
{
	for(uint16_t i=1; i < STORE_COUNT; i++)
	{
		Store_Data[i] = 0X0000;
	}
	Store_Save();
}
