#include "stm32f10x.h"                  // Device header

// 传入的参数为32位，因为只有32位的变量才足够存得下这么大的门牌号

// 读取全字
uint32_t MyFLASH_ReadWord(uint32_t Address)
{
	// 返回指定地址的内容
	return *((__IO uint32_t *)(Address));
}

// 读取半字
uint16_t MyFLASH_ReadHalfWord(uint32_t Address)
{
	// 返回指定地址的内容
	return *((__IO uint16_t *)(Address));
}

// 读取字节
uint8_t MyFLASH_ReadByte(uint32_t Address)
{
	// 返回指定地址的内容
	return *((__IO uint8_t *)(Address));
}

// 擦除STM32内部FLASH主存储器的所有内容
void MyFLASH_EraseAllPages(void)
{
	// 解锁
	FLASH_Unlock();
	
	// FLASH_EraseAllPages()擦除STM32内部FLASH主存储器的所有内容
	FLASH_EraseAllPages();
	
	// 加锁
	// FLASH_Lock()启用写保护
	FLASH_Lock();

}

// 擦除STM32内部FLASH的指定页（扇区）
void MyFLASH_ErasePage(uint32_t PageAddress)
{
	// 解锁
	FLASH_Unlock();
	
	// FLASH_ErasePage()擦除STM32内部FLASH的指定页（扇区）
	FLASH_ErasePage(PageAddress);
	
	// 加锁
	// FLASH_Lock()启用写保护
	FLASH_Lock();
}

// 写入一个字
void MyFLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
	// 解锁
	FLASH_Unlock();

	// FLASH_ProgramWord()向STM32的内部FLASH指定地址写入一个32位字（Word）的数据
	FLASH_ProgramWord(Address, Data);
	
	// 加锁
	// FLASH_Lock()启用写保护
	FLASH_Lock();

}

// 写入一个半字
void MyFLASH_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
	// 解锁
	FLASH_Unlock();	
	
	// FLASH_ProgramHalfWord()向STM32的内部FLASH指定地址写入一个16位的半字数据
	FLASH_ProgramHalfWord(Address, Data);
	
	// 加锁
	// FLASH_Lock()启用写保护
	FLASH_Lock();
}

