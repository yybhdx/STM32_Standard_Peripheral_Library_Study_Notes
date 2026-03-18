#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyFLASH.h"
#include "Key.h"
#include "Store.h"

uint8_t KeyNum;

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	
	// 按键初始化
	Key_Init();
	
//	// 都从0X0800 0000开始读
//	// 读取字
//	OLED_ShowHexNum(1, 1, MyFLASH_ReadWord(0X08000000), 8); // 2000 0660
//	
//	// 读取半字
//	OLED_ShowHexNum(2, 1, MyFLASH_ReadHalfWord(0X08000000), 4); // 0660
//	
//	// 读取字节
//	OLED_ShowHexNum(3, 1, MyFLASH_ReadByte(0X08000000), 2); // 60
//	
//	// 都从0X0800 0010开始读
//	// 读取字
//	OLED_ShowHexNum(1, 1, MyFLASH_ReadWord(0X08000010), 8); // 0800 0351
//	
//	// 读取半字
//	OLED_ShowHexNum(2, 1, MyFLASH_ReadHalfWord(0X08000010), 4); // 0351
//	
//	// 读取字节
//	OLED_ShowHexNum(3, 1, MyFLASH_ReadByte(0X08000010), 2); // 51
//	
//	// 写入之前执行擦除
//	// 先调用页擦除
//	// FLASH_ErasePage()擦除STM32内部FLASH的指定页（扇区）
//	// 页地址为最后一页，0800FC00
//	MyFLASH_ErasePage(0X0800FC00);
//	
//	// 现在就可以写入了
//	// 在最后一页的起始地址FC00下，随便写一个32位数据，比如0X12345678
//	// FLASH_ProgramWord()向STM32的内部FLASH指定地址写入一个32位字（Word）的数据
//	MyFLASH_ProgramWord(0X0800FC00, 0X12345678);
////	MyFLASH_ProgramWord(0X0800FC20, 0X12345678);
//	
//	// 在0X0800FC10的位置写入半字(16位数据)，0XABCD
//	// FLASH_ProgramHalfWord()向STM32的内部FLASH指定地址写入一个16位的半字数据
//	MyFLASH_ProgramHalfWord(0X0800FC10, 0XABCD);
////	MyFLASH_ProgramHalfWord(0X0800FC30, 0XABCD);

// 第一次使用的时候，初始化闪存，然后把闪存备份的数据，加载回SRAM数组，实现SRAM数组的掉电不丢失
	Store_Init();
	
	OLED_ShowString(1, 1, "Flag:");
	
	OLED_ShowString(1, 1, "Data:");
	while (1)
	{
		// 获取键码值，并存入KeyNum中
		KeyNum = Key_GetNum();
		
		// Key1(PB1)按下，将整个闪存都擦除
		if(KeyNum == 1)
		{
//			// 擦除STM32内部FLASH主存储器的所有内容
//			MyFLASH_EraseAllPages();
//			Store_Data[1] = 0X1234;
//			
//			Store_Data[2] = 0XABCD;
			Store_Data[1]++;
			Store_Data[2] += 2;
			Store_Data[3] += 3;
			Store_Data[4] += 4;
			
			// 把SRAM数组备份到闪存
			Store_Save();
		}
		
		// Key2(PB11)按下，进行页擦除
		if(KeyNum == 2)
		{
//			// 擦除STM32内部FLASH的指定页（扇区）
//			// 起始地址为0X08000000，即把闪存的第一页，前1K，1024个字节擦掉
//			MyFLASH_ErasePage(0X08000000);
			
//			// 起始地址为0X08000400，即把闪存的第二页
//			MyFLASH_ErasePage(0X08000400);
//			
//			// 起始地址为0X08000800，即把闪存的第三页
//			MyFLASH_ErasePage(0X08000800);
			
			Store_Clear();
		
		}
		
		OLED_ShowHexNum(1, 6, Store_Data[0], 4);
		OLED_ShowHexNum(3, 1, Store_Data[1], 4);
		OLED_ShowHexNum(3, 6, Store_Data[2], 4);
		OLED_ShowHexNum(4, 1, Store_Data[3], 4);
		OLED_ShowHexNum(4, 6, Store_Data[4], 4);
		
	}
}
