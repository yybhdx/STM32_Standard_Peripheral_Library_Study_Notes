#include "stm32f10x.h"                  // Device header

uint16_t MyDMA_Size;
// DMA初始化
// AddrA:寄存器的起始地址
// AddrB:存储器的起始地址
// Size:DMA_BufferSized参数，表示传输几次
void MyDMA_Init(uint32_t AddrA,uint32_t AddrB, uint32_t Size)
{
	MyDMA_Size = Size;
	
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
//	DMA_Cmd(DMA1_Channel1, ENABLE);

	// 注释掉DMA_Cmd(DMA1_Channel1, ENABLE); 并关闭DMA,不让DMA初始化之后，就立刻进行转运，而是等调用Transfer函数后，再进行转运
	// 调用一次MyDMA_Transfer就转运一次
	
	DMA_Cmd(DMA1_Channel1, DISABLE);
	
}

// 再初始化之后再写一个函数
// DMA传输函数(调用一次这个函数，就再次启动一次DMA转运)
void MyDMA_Transfer(void)
{
	// 在里面我们需要重新给传输计数器赋值
	// 传输计数器赋值，必须要先给DMA失能，然后才可以给传输计数器赋值
	// DISABLE（禁用）DMA1通道1
	DMA_Cmd(DMA1_Channel1, DISABLE);
	
	// 给传输计数器赋值
	// 设置指定DMA通道本次传输需要搬运的数据单元数量
	DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size);
	
	// 使能DMA
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	// 等待转运完成
	// 查询DMA传输过程中的各种状态标志
	// 查询DMA1控制器通道1的传输完成标志
	// DMA1_FLAG_TC1：表示 DMA1 的通道1的传输完成（Transfer Complete）标志
//	返回值：FlagStatus枚举类型，返回标志位的状态。
//	SET：表示指定的标志位被置位，对应事件已发生。
//	RESET：表示指定的标志位被清除，对应事件尚未发生
	// 如果没有完成 就一直循环等待
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	
	// 当某个事件（如传输完成）发生后，其对应的标志位会被硬件自动置位 (SET)。
	// 但不会自动清除。在处理完该事件后，必须使用 DMA_ClearFlag函数手动清除标志位，否则会无法准确判断下一次事件的发生。
	
	// 清除STM32微控制器中DMA（直接存储器访问）控制器的特定状态标志位
	// 清除DMA1 的通道1的传输完成（Transfer Complete）标志位
	DMA_ClearFlag(DMA1_FLAG_TC1);
	
}

