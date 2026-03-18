#include "stm32f10x.h"                  // Device header

extern int num; // 声明num变量

//输入时钟??：72 MHz

//??预分频后??：72 MHz / 720 = 100 kHz

//??计数周期??：10000 × 10 μs = 100 ms

//??结论??：定时器每隔 ??100毫秒?? 触发一次更新中断（或事件）。
void Timer_Init(void)
{
	// 第一步:开启时钟
	// TIM2挂载在APB1外设总线上,开启TIM2的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 第二步:选择时基单元的时钟为内部时钟
	// 选择TIM2为内部时钟，若不调用此函数，TIM默认也为内部时钟
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频为1分频(不进行分频,频率不变)
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; // 计数模式为向上计数
	TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1; // 自动重装载值（ARR)为10000 - 1(实际为10)
	TIM_TimeBaseInitStruct.TIM_Prescaler = 720-1; // 预分频器为720-1(实际为720)
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0; //重复计数器为0(高级定时器用，普通写0)
	
	// 第三步:配置时基单元
	// 配置定时器2的时基单元
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	// 第四步:使能中断类型
	// 使能TIM2的更新中断
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	// 第五步 NVIC
	
	// NVIC设置优先级分组
	// 设置优先级分组方案2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	// 配置NVIC中单条中断优先级和使能状态
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn; // 中断通道号为TIM2_IRQn
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; // 使能中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2; // 抢占优先级为2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1; // 响应优先级为1
	NVIC_Init(&NVIC_InitStruct);
	
	// 最后一步:启动定时器
	// 使能TIM2定时器
	TIM_Cmd(TIM2, ENABLE);
}

// 读取定时器2计数器 CNT 寄存器当前值
uint16_t Timer_GetCounter(void)
{
	return TIM_GetCounter(TIM2);
}

// 定时器2的中断函数
void TIM2_IRQHandler(void)
{
	// 获取中断标志位
	// 检查定时器2的更新事件标志位是否被置位(因为上面TIM_ITConfig()使能定时器2的中断方式为 TIM_IT_Update —— 更新中断)
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		num++;
		// 清除标志位
		// 清除TIM2的更新中断标志位
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
	
}
