#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

// 定义DMA转运的源端数组
uint8_t DataA[] = {0X01, 0X02, 0X03, 0X04}; // 源端数组

// 定义DMA转运的目的数组
uint8_t DataB[] = {0}; // 目的数组

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	
	// 显示转换之前 DataA的数据
	OLED_ShowNum(1, 1, DataA[0], 2); // 01
	OLED_ShowNum(1, 4, DataA[1], 2); // 02
	OLED_ShowNum(1, 7, DataA[2], 2); // 03
	OLED_ShowNum(1, 10, DataA[3], 2); // 04
	
	// 显示转换之前 DataB的数据
	OLED_ShowNum(2, 1, DataB[0], 2); // 00
	OLED_ShowNum(2, 4, DataB[1], 2); // 00
	OLED_ShowNum(2, 7, DataB[2], 2); // 00
	OLED_ShowNum(2, 10, DataB[3], 2); // 00
	
	// 初始化DMA，让DMA把这里DataA的数据，转运到DataB里面去
	// AddrA:寄存器的起始地址 这里选择DataA的首地址
	// AddrB:存储器的起始地址 这里选择DataB的首地址
	// Size:DMA_BufferSized参数，表示传输几次，这里有4个数据，所以传输4次
	// 当调用完这个函数之后,DMA就会立刻工作，把DataA数组的数据，转运到DataB里面去
	// 并且转运一次之后，DMA就停止了，如果DataA的数据又变化了，我们想再转运一次，那该怎么办呢
	// 这时我们就要给传输计数器重新赋值了
	MyDMA_Init((uint32_t)DataA, (uint32_t)DataB, 4);
	
	// 显示转换之后 DataA的数据
	OLED_ShowNum(3, 1, DataA[0], 2); // 01
	OLED_ShowNum(3, 4, DataA[1], 2); // 02
	OLED_ShowNum(3, 7, DataA[2], 2); // 03
	OLED_ShowNum(3, 10, DataA[3], 2); // 04
	
	// 显示转换之后 DataB的数据
	OLED_ShowNum(4, 1, DataB[0], 2); // 01
	OLED_ShowNum(4, 4, DataB[1], 2); // 02
	OLED_ShowNum(4, 7, DataB[2], 2); // 03
	OLED_ShowNum(4, 10, DataB[3], 2); // 04
	
	while (1)
	{
		
	}
}