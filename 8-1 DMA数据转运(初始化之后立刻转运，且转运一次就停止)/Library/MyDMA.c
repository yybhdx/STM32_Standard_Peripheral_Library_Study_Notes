#include "stm32f10x.h"                  // Device header

// DMA初始化
// AddrA:寄存器的起始地址
// AddrB:存储器的起始地址
// Size:DMA_BufferSized参数，表示传输几次
void MyDMA_Init(uint32_t AddrA,uint32_t AddrB, uint32_t Size)
{
	// 第一步，RCC开启DMA1的时钟
	// DMA是在AHB总线上
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	// 第二步，初始化DMA
	DMA_InitTypeDef DMA_InitStruct;
	// 对于SRAM的数组，它的地址是编译器分配的，并不是固定的，所以我们一般不会写绝对地址，而是通过数组名来获取地址
	// 所以我们就把这个地址提取成这个DMA初始化函数的参数，这样初始化的时候，你想转运哪个数组，就把哪个数组的地址传进来就行了
	DMA_InitStruct.DMA_PeripheralBaseAddr = AddrA; // 外设数据寄存器的基地址为AddrA
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设数据宽度为:DMA_PeripheralDataSize_Byte(以字节的方式传输)	
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable; // 地址是否自增:DMA_PeripheralInc_Enable(地址递增)
	
	// 存储器站点的基地址，我们也把它提取成参数
	DMA_InitStruct.DMA_MemoryBaseAddr = AddrB; // 外设数据寄存器的基地址为AddrB
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;// 存储器数据宽度为:DMA_MemoryDataSize_Byte(8位)
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable; // 存储器地址是否递增:地址递增
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC; // 数据传输方向:外设到存储器
	DMA_InitStruct.DMA_BufferSize = Size; // 待传输次数：Size(传输Size次)
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal; // DMA传输模式:正常模式
	DMA_InitStruct.DMA_M2M = DMA_M2M_Enable; // DMA是否应用于存储器到存储器模式:使能存储器到存储器模式(使用软件触发)
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium; // 通道优先级：中等优先级
	
	// 指定要初始化DMA1通道1
	// 将结构体DMA_InitStruct里面的参数配置到DMA1通道1的位置里面去
	// 这里因为是存储器到存储器之间的转运 用的是软件触发，所以通道可以任意选择
	DMA_Init(DMA1_Channel1, &DMA_InitStruct);
	
	//DMA转运有三个条件，缺一不可
	//1.传输计数器>0 
	//2.触发源有触发信号
	//3.DMA使能
	// 根据上述代码，如果传一个大于0的Size的话，第一个条件满足
	// 触发源为软件触发，所以一直都有触发信号，第二个条件满足
	// 最后一个条件，DMA还没有使能，第三个条件不满足，所以到目前未知，DMA还不会工作
	// 如果你想在初始化之后就立刻工作的话
	// 可以在最后加上DMA_Cmd()
	// 参数第一个DMA1_Channel1,第二个ENABLE
	// 使能条件之后，三个条件满足，DMA就会进行数据转运了，转运一次，传输计数器自减一次
	// 当传输计数器减到0之后，转运完成，同时第一个条件就不满足了，转运停止，这样就完成了一次数组之间的数据转运
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	
}


