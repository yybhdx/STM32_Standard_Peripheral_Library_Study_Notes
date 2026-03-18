#include "stm32f10x.h"    // Device header

int16_t Encoder_Count;

void Encoder_Init(void)
{
	// Encoder接了PB0 PB1
	
	// 开启GPIOB的时钟
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// 开启AFIO的时钟
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	// 初始化GPIOB
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// 通过使用AFIO寄存器来选择GPIO管脚作外部中断线
	// 执行完这个函数后AFIO的第0个数据选择器就拨好了
	// 其中输入端被拨到了GPIOB外设上,对应PB0号引脚
	// 输出端固定连接的是EXTI的第14个中断线路这样PB14号引脚的电平信号就可以顺利通过AFIO,进入到后级EXT1电路了
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	
	
	// 初始化EXTI
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1; // 指令中断线为EXTI_Line0 和 EXTI_Line1
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // 设置 EXTI 线路为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // 设置触发方式为上升沿和下降沿
	EXTI_Init(&EXTI_InitStructure);
	
	// 配置抢占优先级与子优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 抢占优先级2位，子优先级2位
	
	// 初始化NVIC
	
	// 配置EXTI0通道
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // 子优先级为1
	NVIC_Init(&NVIC_InitStructure);
	
	// 配置EXTI1通道
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; // 子优先级为2
	NVIC_Init(&NVIC_InitStructure); 
	
//下面的NVIC_Init(&NVIC_InitStructure);不会将上面的NVIC_Init(&NVIC_InitStructure);覆盖吗
//答案是：不会覆盖前一次的配置，因为 NVIC 初始化函数是 针对某个具体中断通道（IRQChannel） 生效的。
	
}

// 返回Encoder_Count的变化值函数
int16_t Encoder_Count_Get(void)
{
	int16_t temp;
	temp = Encoder_Count;
	Encoder_Count = 0;
	return temp;
}


// EXTI0的中断函数
void EXTI0_IRQHandler()
{
	// 查询EXTI0的中断标志位
	// 函数返回SET —— 表示该中断线的中断挂起标志(Pending bit)被置位（说明有中断事件发生）。
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		// 读取PB0的输入电平是不是0
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
		{
			// 如果是 那就是反转 那次数就是--
			Encoder_Count--;
		}
		
		// 清除EXTI0的中断挂起标志位
		EXTI_ClearITPendingBit(EXTI_Line0);		
	}
	
}

// EXTI1的中断函数
void EXTI1_IRQHandler(void)
{
	// 查询EXTI1的中断标志位
	// 函数返回SET —— 表示该中断线的中断挂起标志(Pending bit)被置位（说明有中断事件发生）。
	if(EXTI_GetITStatus(EXTI_Line1) == SET)
	{
		// 读取PB1的输入电平是不是0
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
		{
			// 如果是 那就是正转 那次数就是++
			Encoder_Count++;
		}
		// 清除EXTI1的中断挂起标志位
		EXTI_ClearITPendingBit(EXTI_Line1);		
	}
	
}
