#include "stm32f10x.h"                  // Device header

// PWM初始化函数
//第一步:开启RCC时钟，把我们要用的TIM外设和GPIO外设的时钟打开
//第二步:配置时基单元，包括前面的时钟源选择和时基单元都配置好
//第三步:配置输出比较单元，里面包括CCR的值，输出比较模式，极性选择，输出使能
//第四步:配置GPIO,把PWM对应的GPIO口，初始化为复用推挽输出的配置(可以参考引脚定义表)
//第五步:运行控制，启动计数器，这样就能输出PWM了

// 初始化定时器 生成PWM频率为1000 PWM占空比为50% PWM分辨率为1%的PWM信号
// 依据PWM参数计算的三个公式(三个未知数 预分频系数PSC 重装载值ARR 脉冲值CRR) 又因为时钟频率为72MHZ
// 可以得出三个未知数对应的值为  PSC:720 ARR:100  CRR:50
void pwm_init(void)
{
	// 1.开启RCC时钟
	// TIM2挂载在APB1外设总线上，开启TIM2的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// LED对应的PAO的GPIOA挂载在APB2外设总线哈桑，开启GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 2.配置时基单元
	
	// 配置定时器的时钟源为内部时钟
	TIM_InternalClockConfig(TIM2);
	
	// 配置定时器时基单元
	// 定时器频率为 10000HZ(时钟频率) / 100(自动冲装载值) = 1000HZ
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频为1分频(不进行分频,频率不变)
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; // 计数模式为向上计数
	TIM_TimeBaseInitStruct.TIM_Period = 100 - 1; // 自动重装载值（ARR)为100 - 1(实际为100)
	TIM_TimeBaseInitStruct.TIM_Prescaler = 720-1; // 预分频器(PSC)为720-1(实际为720) 72MHZ / 720=100000HZ
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter= 0; // 重复计数器为0(只有 高级定时器（TIM1/TIM8） 才支持，普通写0) 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	// 3.配置输出比较单元
	// 由于使用的是PA0口所以选择第一个输出比较通道
	// 因为TIM2_CH1的映射引脚之一就是PA0。这意味着，如果您想使用PA0引脚来输出PWM，就必须将它配置为TIM2的通道1功能（AFIO配置），然后使用TIM2的通道1来生成信号
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	// 给结构体赋默认值
	// 若不给它的成员赋初始值，它成员的值就是不确定的，这可能会导致一些问题
	// 比如将高级定时器TIM1当作通用定时器输出PWM时 由于下面代码中只配置了通用定时器所需的值
	// 高级定时器中的相应寄存器没有赋值，就会导致高级定时器输出PWM时出现奇怪的问题
	TIM_OCStructInit(&TIM_OCInitStruct);

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; // 输出模式为PWM模式1
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; // 输出极性为高极性(有效电平为高电平)
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; // 输出使能为:使能输出（信号驱动引脚）
	TIM_OCInitStruct.TIM_Pulse = 50; // 脉冲值(CCR)为50
	TIM_OC1Init(TIM2, &TIM_OCInitStruct);
	
	// 4.配置GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; // 根据外设的GPIO配置  输出比较通道配置为推挽复用输出功能
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// 5:启动定时器
	// 使能TIM2定时器
	TIM_Cmd(TIM2, ENABLE);
}

