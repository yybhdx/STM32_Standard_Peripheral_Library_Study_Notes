#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyDMA.h"

// 定义DMA转运的源端数组
uint8_t DataA[] = {0X01, 0X02, 0X03, 0X04}; // 源端数组

// 定义DMA转运的目的数组
uint8_t DataB[] = {0}; // 目的数组

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	
	// 初始化DMA，让DMA把这里DataA的数据，转运到DataB里面去
	// AddrA:寄存器的起始地址 这里选择DataA的首地址
	// AddrB:存储器的起始地址 这里选择DataB的首地址
	// Size:DMA_BufferSized参数，表示传输几次，这里有4个数据，所以传输4次
	MyDMA_Init((uint32_t)DataA, (uint32_t)DataB, 4);
	
	// 显示字符
	OLED_ShowString(1, 1, "DataA");
	OLED_ShowString(3, 1, "DataB");
	
	// 显示地址
	OLED_ShowHexNum(1, 8, (uint32_t)DataA, 8); // 2000 0002(现在可能是模块里的那个Size变量被分配到了0的位置，所以这个地址就往后偏移了2个字节)
	OLED_ShowHexNum(3, 8, (uint32_t)DataB, 8); // 2000 0006
	
	// 显示转换之前 DataA的数据
	OLED_ShowNum(2, 1, DataA[0], 2); // 01
	OLED_ShowNum(2, 4, DataA[1], 2); // 02
	OLED_ShowNum(2, 7, DataA[2], 2); // 03
	OLED_ShowNum(2, 10, DataA[3], 2); // 04
	
	// 显示转换之前 DataB的数据
	OLED_ShowNum(4, 1, DataB[0], 2); // 00
	OLED_ShowNum(4, 4, DataB[1], 2); // 00
	OLED_ShowNum(4, 7, DataB[2], 2); // 00
	OLED_ShowNum(4, 10, DataB[3], 2); // 00
	
	
	while (1)
	{
		// 变化一下源端A的数据，给它们自增1
		DataA[0]++;
		DataA[1]++;
		DataA[2]++;
		DataA[3]++;
		
		// 显示转换之前 DataA的数据	
		OLED_ShowNum(2, 1, DataA[0], 2); 
		OLED_ShowNum(2, 4, DataA[1], 2); 
		OLED_ShowNum(2, 7, DataA[2], 2); 
		OLED_ShowNum(2, 10, DataA[3], 2); 
		
		// 显示转换之前 DataB的数据
		OLED_ShowNum(4, 1, DataB[0], 2); 
		OLED_ShowNum(4, 4, DataB[1], 2); 
		OLED_ShowNum(4, 7, DataB[2], 2); 
		OLED_ShowNum(4, 10, DataB[3], 2);
			
		Delay_ms(1000);
		
		// 开始转运
		// 由于DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size);中的MyDMA_Size是全局变量，且MyDMA_Size = size,size = 4,所以每次转运四次 
		MyDMA_Transfer();
		
		// 显示转换之后 DataA的数据	
		OLED_ShowNum(2, 1, DataA[0], 2); 
		OLED_ShowNum(2, 4, DataA[1], 2); 
		OLED_ShowNum(2, 7, DataA[2], 2); 
		OLED_ShowNum(2, 10, DataA[3], 2); 
		
		// 显示转换之后 DataB的数据
		OLED_ShowNum(4, 1, DataB[0], 2); 
		OLED_ShowNum(4, 4, DataB[1], 2); 
		OLED_ShowNum(4, 7, DataB[2], 2); 
		OLED_ShowNum(4, 10, DataB[3], 2); 
		
		Delay_ms(1000);		
	}
}