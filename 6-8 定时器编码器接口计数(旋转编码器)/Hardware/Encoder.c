#include "stm32f10x.h"                  // Device header

/*旋转编码器初始化*/
//第一步:RCC开启时钟，开启GPIO和定时器的时钟
//第二步:配置GPIO，这里需要把PA6和PA7配置成输入模式
//第三步:配置时基单元，这里预分频器我们一般选择不分频，自动重装，一般给最大65535，只需要个CNT执行计数就行了。
//第四步:配置输入捕获单元，不过这里的输入捕获单元只有滤波器和极性这两个参数有用。后面的参数没有用到，与编码器无关。
//第五步:配置编码器接口模式，这个直接调用一个库函数就可以了。
//第六步:最后，调用TIM_Cmd，启动定时器，就完事了。
//电路初始化完成之后，CNT就会随着编码器旋转而自增自减。
//如果想要测量编码器的位置，那直接读出CNT的值就行了。
//如果想测量编码器的速度和方向，那就需要每隔一段固定的闸门时间，取出一次CNT，然后再把CNT清零，这样就是测频法测量速度了。

void Encoder_Init(void)
{
	//第一步:RCC开启时钟，开启GPIO和定时器的时钟
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//开启TIM3的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//开启GPIOA的时钟 // 对应TIM3_CH1(定时器3通道一的引脚) PA6
	
	//第二步:配置GPIO，这里需要把PA6和PA7配置成输入模式
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // GPIO模式配置成上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	 // PA6 PA7
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);							
																		
	//第三步:配置时基单元，这里预分频器我们一般选择不分频，自动重装，一般给最大65535，只需要个CNT执行计数就行了。
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;					//计数周期，即ARR的值 // 这里的65536 - 1是满量程计数
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;				//预分频器，即PSC的值 1 - 1 = 0，预分频给0，就是不分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);             //将结构体变量交给TIM_TimeBaseInit，配置TIM3的时基单元

	//第四步:配置输入捕获单元，不过这里的输入捕获单元只有滤波器和极性这两个参数有用。后面的参数没有用到，与编码器无关。
	/*配置定时器的输入捕获功能*/
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
  //初始化输入捕获结构体 TIM_ICInitTypeDef的各个成员为默认值
	//因为输入捕获单元只有滤波器和极性这两个参数有用 所以要配置默认值
	TIM_ICStructInit(&TIM_ICInitStruct);
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1; // 因为我们计划配置的是TIM3_CH1(PA6)，所以选择TIM_Channel_1
	TIM_ICInitStruct.TIM_ICFilter = 0XF; // 配置数字滤波器的参数 0x00（不滤波）到 0x0F（最大滤波）
	
	// 这里的上升沿并不代表上升沿有效，因为编码器接口始终都是上升沿，下降沿都有效的。
	// 这里的上升沿参数代表的是高低电平极性不反转(也就是定时器编码器接口工作模式是否反相)
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising; // 设置捕获信号的边沿极性 // TIM_ICPolarity_Rising为上升沿触发
//	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 设置输入捕获预分频器 // TIM_ICPSC_DIV1:（每个事件都捕获）
//	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; // 设置输入捕获通道的信号映射关系。 // 直接映射，如TI1FP1映射到IC1）
	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2; // 因为我们计划配置的是TIM3_CH2(PA7)，所以选择TIM_Channel_2
	TIM_ICInitStruct.TIM_ICFilter = 0XF; // 配置数字滤波器的参数 0x00（不滤波）到 0x0F（最大滤波）
	
	// 这里的上升沿并不代表上升沿有效，因为编码器接口始终都是上升沿，下降沿都有效的。
	// 这里的上升沿参数代表的是高低电平极性不反转(也就是定时器编码器接口工作模式是否反相)
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising; // 设置捕获信号的边沿极性 // TIM_ICPolarity_Rising为上升沿触发
//	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 设置输入捕获预分频器 // TIM_ICPSC_DIV1:（每个事件都捕获）
//	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; // 设置输入捕获通道的信号映射关系。 // 直接映射，如TI1FP1映射到IC1）
	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	
  // 第五步:配置编码器接口模式，这个直接调用一个库函数就可以了
	// 配置定时器用于读取正交编码器
	// 配置完这个函数之后 可以将上述的TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising; // 设置捕获信号的边沿极性 // TIM_ICPolarity_Rising为上升沿触发 这段代码删除
	// 删除TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising之后只剩滤波器代码(TIM_ICInitStruct.TIM_ICFilter = 0XF)有用了。
	TIM_EncoderInterfaceConfig(TIM3,
																TIM_EncoderMode_TI12, // 决定了编码器信号的检测模式 // 为在TI1和TI2边沿都计数
                                TIM_ICPolarity_Rising, // 设置通道1（A相）的输入极性 // 为TIM_ICPolarity_Rising：不反相（上升沿有效）。这是最常见的设置，表示信号不反相。
                                TIM_ICPolarity_Rising);// 设置通道2（B相）的输入极性 // 为TIM_ICPolarity_Rising：不反相（上升沿有效）。这是最常见的设置，表示信号不反相。
															
  //第六步:最后，调用TIM_Cmd，启动定时器，就完事了。
	TIM_Cmd(TIM3, ENABLE);
}

// 返回CNT的值
int16_t Encoder_Get(void)
{
	
	return TIM_GetCounter(TIM3);
}
