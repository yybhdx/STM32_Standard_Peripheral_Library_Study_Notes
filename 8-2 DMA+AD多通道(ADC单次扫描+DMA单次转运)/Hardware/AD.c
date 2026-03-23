#include "stm32f10x.h"                  // Device header

/*
如果给AD_Value[4]加上Const修饰，那就成为了外设到Flash的转运了
但是这样是不可取的，因为Flahs只能读不能写
*/
uint16_t AD_Value[4];

// ADC初始化
// 配置ADC为单次转换，非扫描的模式
// 调用这个函数之后，ADC就处于准备就绪的状态了
void AD_Init(void)
{
	// 1. 开启RCC的时钟(包括ADC和GPIO)
	// 开启ADC的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	// 开启GPIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 开启DMA的时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	// 配置ADCCLK
	// RCC_ADCCLKConfig():配置ADC模块的时钟源（ADCCLK）的分频系数
	// RCC_PCLK2_Div6:设置ADCCLK = 72MHz / 6 = 12MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	// 2.配置GPIO:把需要用的GPIO配置成模拟输入的模式
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN; // 在AIN模式下，GPIO口是无效的，断开GPIO，防止你GPIO的输入输出对我模拟电压造成干扰，所以AIN模式就是ADC的专属模式
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	// 配置STM32微控制器中ADC（模数转换器）规则通道组的通道、转换顺序和采样时间
	//	它的作用就是这里，给序列的每个位置填写指定的通道
	//	就是填写点菜菜单的过程
	//	第一个参数是ADCX
	//	第二个ADC Channel，就是你想指定的通道
	//	第三个Rank，就是序列几的位置
	//	然后第四个SampleTime，就是指定通道的采样时间

	// ADC1的通道0放在序列1，采样时间为ADC_SampleTime_55Cycles5（55.5个ADC时钟周期）
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	
	// ADC1的通道1放在序列2，采样时间为ADC_SampleTime_55Cycles5（55.5个ADC时钟周期）
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	
	// ADC1的通道2放在序列3，采样时间为ADC_SampleTime_55Cycles5（55.5个ADC时钟周期）
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
	
	// ADC1的通道3放在序列4，采样时间为ADC_SampleTime_55Cycles5（55.5个ADC时钟周期）
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
	
	// 4.配置ADC转换器
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent; // 工作模式：ADC_Mode_Independent（独立模式）
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right; // 数据对齐方式：o	ADC_DataAlign_Right：数据右对齐
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 外部触发转换：ADC_ExternalTrigConv_None，软件触发（无硬件触发源）
	ADC_InitStruct.ADC_NbrOfChannel = 4; // 通道数目选择4，告诉厨师，我点的是四个菜，你看前4个位置就可以了
	ADC_InitStruct.ADC_ScanConvMode = ENABLE; // 扫描模式:设置为 ENABLE：用于多通道转换。ADC会按照规则序列寄存器（ADC_SQRx）中设定的顺序，自动扫描转换多个通道
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE; // 要使用ADC的ADC_ContinuousConvMode（连续转换模式），那么这个参数就要改成ENABLE // 设置为 DISABLE：每次转换都需要由软件或外部触发来启动，适用于单次采样
	ADC_Init(ADC1, &ADC_InitStruct);
	
	/*配置ADC的DMA*/
	// 这里DMA你可以想象成服务员,ADC这个厨师把菜做好了，DMA这个服务员要尽快把菜端出来，防止被覆盖
	
	// 初始化DMA
	DMA_InitTypeDef DMA_InitStruct;
	
	// (uint32_t)&ADC1->DR得到的结果，其实就是刚才写的，4001244C
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; // 外设数据寄存器的基地址，这是端菜的源头，初始把菜做好，就放在ADC_DR寄存器里，所以端菜的源头地址，就填ADC_DR的地址:0X4001244C
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 外设数据宽度为:我们想要DR寄存器低16位的数据，所以数据宽度就是HalfWorld,以半字，16位来转运
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // 外设地址是否递增:DMA_PeripheralInc_Disable(地址固定，始终转运同一个位置的数据，因为ADC转运的数据始终存储在ADC1->DR寄存器中，所以地址不能变化)
	
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)AD_Value; // 外设数据寄存器的基地址:也就是端菜的目的地，我们想要把数据存在SRAM数组里，所以我们先在前面定义一个数组AD_Value[4],然后将这个数组的地址传进去
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;// 存储器数据宽度为:DMA_MemoryDataSize_HalfWord：16位
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable; // 存储器地址是否递增:地址递增,每转运一次地址挪一个坑。到这里，DMA的源端和目的地的参数就配置完成了
	 
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC; // 数据传输方向:外设到存储器
	DMA_InitStruct.DMA_BufferSize = 4; // 待传输次数：4(传输4次)，因为上面我们设置了4个ADC通道，所以传输4次
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal; // DMA传输模式:正常模式
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable; // DMA是否应用于存储器到存储器模式:DMA_M2M_Disable：用于外设和存储器之间的传输，需要外设的DMA请求来触发，(即使用硬件触发)。我们这里使用硬件触发，触发源为ADC1，厨师每个菜做好了，叫我一下，我再去端菜，这样才是合适的时机
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium; // 通道优先级：中等优先级
	
	// 指定要初始化DMA1通道1
	// 将结构体DMA_InitStruct里面的参数配置到DMA1通道1的位置里面去
	// 这里通道就不能任意选择了，因为是硬件触发，DMA1的请求影响图里面，ADC1的硬件触发是只接在了DMA1的通道1上,所以这里通道必须要使用DMA1的通道1，其他的通道都不行
	DMA_Init(DMA1_Channel1, &DMA_InitStruct);
	
	//DMA转运有三个条件，缺一不可
	//1.传输计数器>0 
	//2.触发源有触发信号
	//3.DMA使能
	// 根据上述代码，如果传一个大于0的Size的话，第一个条件满足
	// 触发源为硬件触发
	// 最后一个条件，DMA还没有使能，第三个条件不满足，所以到目前未知，DMA还不会工作
	// 如果你想在初始化之后就立刻工作的话
	// 可以在最后加上DMA_Cmd()
	// 参数第一个DMA1_Channel1,第二个ENABLE
	// 使能条件之后，三个条件满足，DMA就会进行数据转运了，转运一次，传输计数器自减一次
	// 当传输计数器减到0之后，转运完成，同时第一个条件就不满足了，转运停止，这样就完成了一次数组之间的数据转运
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	// 由于现在使用的是硬件触发，第二个条件:触发源有触发信号不满足，因为这里是硬件触发，ADC还没启动，不会有触发信号，所以这里DMA使能后不会立刻工作
	
	// 最后在ADC使能之前，还要一个事前要做，就是开启ADC到DMA的输出
	// 启用或禁用指定ADC模块的DMA传输功能
	// 启用ADC1模块的DMA传输功能
	ADC_DMACmd(ADC1,ENABLE);

	// 5.使能ADC
	ADC_Cmd(ADC1, ENABLE);
	
	// 6.ADC校准
	// 复位校准ADC
	ADC_ResetCalibration(ADC1);
	
	// 返回复位校准寄存器的当前状态
	// 如果没校准完成，就在这个while空循环里一直等待
	// 如果ADC_GetResetCalibrationStatus(ADC1)为set(1)，说明复位校准还在进行中,while循环死循环
	// 如果ADC_GetResetCalibrationStatus(ADC1)为reset(0)，表示复位校准过程已完成 while循环会自动跳出来
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);
	
	// 启动指定ADC（模数转换器）的校准过程
	// 启动指定ADC1（模数转换器）的校准过程
	ADC_StartCalibration(ADC1);
	
	// 查询指定ADC（模数转换器）的校准状态
	// SET(1)：表示校准过程正在进行中,while循环死循环
	// RESET(0)：表示校准过程已完成,while循环会自动跳出来
	while(ADC_GetCalibrationStatus(ADC1) == SET);
	
}

// 执行流程:首先，软件触发ADC转换，然后等待转换完成，也就是等待EOC标志位置1，最后读取ADC数据寄存器就完事了
// ADC开始转换，连续扫描4个通道，DMA也同步进行转运，ADC转换结果，依次放在这上面的AD_Value数组里
void AD_GetValue(void)
{
	
	// 因为DMA也是正常的单次模式，所以在触发ADC之前，需要再重新写入一下传输计数器
	
	// 在里面我们需要重新给传输计数器赋值
	// 传输计数器赋值，必须要先给DMA失能，然后才可以给传输计数器赋值
	// DISABLE（禁用）DMA1通道1
	DMA_Cmd(DMA1_Channel1, DISABLE);
	
	// 给传输计数器赋值
	// 设置指定DMA通道本次传输需要搬运的数据单元数量
	DMA_SetCurrDataCounter(DMA1_Channel1, 4);
	
	// 使能DMA
	// 启用DMA1的通道1
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	// 因为现在ADC还是单次模式，所以还需要软件触发一下ADC开始
	// 通过软件触发的方式启动或停止指定ADC
	// 通过软件触发的方式启动ADC1
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
	//等待ADC转换和DMA转运完成，这里因为转运总是在转换之后，所以我们需要写等待DMA完成的代码，等待ADC转换完成的代码就不需要了
	// 等待转运完成
	// 查询DMA传输过程中的各种状态标志
	// 查询DMA1控制器通道1的传输完成标志
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	
	// 当某个事件（如传输完成）发生后，其对应的标志位会被硬件自动置位 (SET)。
	// 但不会自动清除。在处理完该事件后，必须使用 DMA_ClearFlag函数手动清除标志位，否则会无法准确判断下一次事件的发生。
	
	// 清除STM32微控制器中DMA（直接存储器访问）控制器的特定状态标志位
	// 清除DMA1 的通道1的传输完成（Transfer Complete）标志位
	DMA_ClearFlag(DMA1_FLAG_TC1);
	

}
