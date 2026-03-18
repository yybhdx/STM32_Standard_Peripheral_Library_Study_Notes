#include "stm32f10x.h"                  // Device header

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
	
	// 配置ADCCLK
	// RCC_ADCCLKConfig():配置ADC模块的时钟源（ADCCLK）的分频系数
	// RCC_PCLK2_Div6:设置ADCCLK = 72MHz / 6 = 12MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	// 2.配置GPIO:把需要用的GPIO配置成模拟输入的模式
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN; // 在AIN模式下，GPIO口是无效的，断开GPIO，防止你GPIO的输入输出对我模拟电压造成干扰，所以AIN模式就是ADC的专属模式
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	// 3.选择规则组的输入通道
	// ADC_RegularChannelConfig()：配置STM32微控制器中ADC（模数转换器）规则通道组的通道、转换顺序和采样时间
	// 在规则组菜单的第一个位置(对应参数1)，写入通道0这个通道(对应参数ADC_Channel_0)
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	
	// 4.配置ADC转换器
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent; // ADC工作模式为:ADC_Mode_Independent  独立模式
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right; // 数据对齐方式选择:ADC_DataAlign_Right:右对齐
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 外部触发转换选择ADC_ExternalTrigConv_None,软件触发(然后通过调用 ADC_SoftwareStartConvCmd(ADCx, ENABLE)来启动转换)
	ADC_InitStruct.ADC_NbrOfChannel = 1; // 通道数目选择1 :指定规则通道转换序列中的总通道数为1(此参数需要与调用 ADC_RegularChannelConfig函数配置的通道序列长度一致)
	ADC_InitStruct.ADC_ScanConvMode = DISABLE; // 关闭扫描模式
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE; // 要使用ADC的ADC_ContinuousConvMode（连续转换模式），那么这个参数就要改成ENABLE
	
	// ADC_Init ():初始化模数转换器（ADC）
	// 指定要初始化ADC1
	// 指向 ADC_InitTypeDef结构体的指针	
	ADC_Init(ADC1, &ADC_InitStruct);

	// 5.使能ADC
	ADC_Cmd(ADC1, ENABLE);
	
	// 6.ADC校准
	// 复位校准ADC
	ADC_ResetCalibration(ADC1);
	
	// 返回复位校准寄存器的当前状态
	// 如果没校准完成，就在这个while空循环里一直等待
	// 如果ADC_GetResetCalibrationStatus(ADC1)为set(1)，说明复位校准还在进行中,while循环死循环
	// 如果ADC_GetResetCalibrationStatus(ADC1)为0，表示复位校准过程已完成 while循环会自动跳出来
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);
	
	// 启动指定ADC（模数转换器）的校准过程
	// 启动指定ADC1（模数转换器）的校准过程
	ADC_StartCalibration(ADC1);
	
	// 查询指定ADC（模数转换器）的校准状态
	// SET(1)：表示校准过程正在进行中,while循环死循环
	// RESET(0)：表示校准过程已完成,while循环会自动跳出来
	while(ADC_GetCalibrationStatus(ADC1) == SET);
	
	// 连续转换模式仅需要在最开始触发一次就行了
	// 所以我们将软件触发转换的函数挪到初始化的最后
	// 在初始化完成之后，触发一次就行了
	// 这时，内部的ADC就会一次接着一次，连续不断地，对我们指定的通道进行转换，转换结果存放在数据寄存器里面
	// 此时数据寄存器会不断地刷新转换结果
	// 所以在下面的AD_GetValue函数中就不需要判断标志位了
	// 同样也不需要再次调用ADC_SoftwareStartConvCmd来触发单次转换
	// 直接return数据寄存器的值就行了
	// 这样程序就是单通道，连续转换非扫描的模式
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

// 启动转换，获取结果
// 执行流程:首先，软件触发转换，然后等待转换完成，也就是等待EOC标志位置1，最后读取ADC数据寄存器就完事了
uint16_t AD_GetValue(void)
{
//	// 通过软件触发的方式启动或停止指定ADC
//	// 通过软件触发的方式启动ADC1
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

//	// 因为转换还需要一些时间，所以我们还需要再等待一会
//	// 具体要等待多长时间呢??
//	/* 我们刚才配置的时候指定了ADC_SampleTime_55Cycles5，这个通道的采样周期为55.5，转换周期是固定的12.5，
//	加在一起，就是68个周期,前面我们配置的ADCCLK是72MHZ的6分频，是12MHZ
//	12MHZ进行68个周期，转换才能完成
//	最终的时间可以算一下，就是1/12M，再*68，结果大概是5.6us*/
//	// 所以这个while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);while循环大概会等待5.6us
//	
//	// 检查模数转换器 (ADC) 特定事件
//	// 指定要查询的ADC外设实例为ADC1
//	// 指定要检查的状态标志为ADC_FLAG_EOC:规则通道转换结束标志。这是最常用的标志，用于判断一次ADC转换是否完成。
//	// 返回值为 FlagStatus类型，只能是 SET(1)（标志位置位，事件已发生）或 RESET(0)（标志位复位，事件未发生）
//	// 当EOC标志位==RESET时，转换未完成，while条件为真，执行空循环
//	// 转换完成后,EOC由硬件自动置1，那while循环就自动跳出了
//	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	
	// 等待完成之后，我们就可以取结果了
	// 从指定的ADC外设的数据寄存器（DR）中读取最近一次规则通道转换的结果值
	// 它会返回最近一次ADC规则通道转换的结果，所以我们直接return过去
	return ADC_GetConversionValue(ADC1);
	
//	/* 因为ADC_GetConversionValue(ADC1);函数会自动读取DR寄存器，
//	读取DR寄存器之后会自动清除EOC标志位
//	所以这之后我们就不需要再手动清除标志位了
//	这样启动，等待，读取的过程就写好了*/
//	// 它和PPT的转换模式的图的流程是对应的，调用一次这个函数，就相当于执行了一次这个流程
	
}
