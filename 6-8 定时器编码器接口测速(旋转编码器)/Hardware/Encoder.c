#include "stm32f10x.h"                  // Device header

/*旋转编码器初始化*/
//第一步:开启RCC时钟，包括GPIO和定时器的时钟
//第二步:配置GPIO，需要将PA6和PA7配置成输入模式
//第三步:配置时基单元，预分频器一般选择不分频，自动重装值一般给65535，只需要让CNT执行计数即可。
//第四步:配置输入捕获单元，输入捕获单元只用到了滤波器和极性选择。其他参数用不到，无关紧要。
//第五步:配置编码器接口模式，直接调用一个库函数就可以了。
//第六步:最后，调用TIM_Cmd启动定时器即可。
//初始化之后CNT就会随编码器旋转自增自减
//如果需要测位置，直接读取CNT的值即可。
//测速度和方向就需要每隔一段固定的闸门时间，取出一次CNT，然后再把CNT清零，这就是测频法测速度了。

void Encoder_Init(void)
{
	//第一步:开启RCC时钟，包括GPIO和定时器的时钟
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//开启TIM3的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//开启GPIOA的时钟 // 对应TIM3_CH1(定时器3通道一) PA6
	
	//第二步:配置GPIO，需要将PA6和PA7配置成输入模式
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // GPIO模式配置为上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	 // PA6 PA7
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						
																
	//第三步:配置时基单元，预分频器一般选择不分频，自动重装值一般给65535，只需要让CNT执行计数即可。
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;					//计数周期，即ARR的值 // 给65536 - 1最大量程即可
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;				//预分频器，即PSC的值 1 - 1 = 0，预分频为0就是不分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);             //将结构体变量交给TIM_TimeBaseInit，配置TIM3的时基单元

	//第四步:配置输入捕获单元，输入捕获单元只用到了滤波器和极性选择。其他参数用不到，无关紧要。
	/*配置定时器输入捕获*/
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
  //初始化输入捕获结构体 TIM_ICInitTypeDef 的各成员为默认值
	//因为输入捕获单元只用到了滤波器和极性，其他需要用默认值
	TIM_ICStructInit(&TIM_ICInitStruct);
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1; // 因为这里计划用到TIM3_CH1(PA6)，所以选择TIM_Channel_1
	TIM_ICInitStruct.TIM_ICFilter = 0XF; // 滤波器参数 0x00不滤波 0x0F最大滤波
	
	// 这里极性选择为上升沿，实际上在编码器接口模式下，上升沿和下降沿都是有效的。
	// 这里极性代表的是高低电平是否反转(也就是定时器编码器接口工作模式下的是否反相)
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising; // 设置输入信号的边沿极性 // TIM_ICPolarity_Rising为不反相
//	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 输入捕获预分频器 // TIM_ICPSC_DIV1:捕获每个事件
//	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; // 输入捕获通道信号映射关系 // 直接映射，即TI1FP1映射到IC1上
	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2; // 因为这里计划用到TIM3_CH2(PA7)，所以选择TIM_Channel_2
	TIM_ICInitStruct.TIM_ICFilter = 0XF; // 滤波器参数 0x00不滤波 0x0F最大滤波
	
	// 这里极性选择为上升沿，实际上在编码器接口模式下，上升沿和下降沿都是有效的。
	// 这里极性代表的是高低电平是否反转(也就是定时器编码器接口工作模式下的是否反相)
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising; // 设置输入信号的边沿极性 // TIM_ICPolarity_Rising为不反相
//	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 输入捕获预分频器 // TIM_ICPSC_DIV1:捕获每个事件
//	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; // 输入捕获通道信号映射关系 // 直接映射，即TI1FP1映射到IC1上
	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	
  // 第五步:配置编码器接口模式，直接调用一个库函数就可以了
	// 配置定时器编码器读取模式
	// 配置之后，以上TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising; // 设置输入信号的边沿极性 // TIM_ICPolarity_Rising为不反相 的配置会被覆盖
	// 删掉TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising之后只剩下了滤波器参数(TIM_ICInitStruct.TIM_ICFilter = 0XF)而已。
	TIM_EncoderInterfaceConfig(TIM3,
															TIM_EncoderMode_TI12, // 决定了编码器信号的检测模式 // 为在TI1和TI2边沿都计数
                                TIM_ICPolarity_Rising, // 设置通道1（A相）的输入极性 // 为TIM_ICPolarity_Rising：不反相（上升沿有效），最常见的设置，表示信号不反相。
                                TIM_ICPolarity_Rising);// 设置通道2（B相）的输入极性 // 为TIM_ICPolarity_Rising：不反相（上升沿有效），最常见的设置，表示信号不反相。
														
  //第六步:最后，调用TIM_Cmd启动定时器即可。
	TIM_Cmd(TIM3, ENABLE);
}

//// 读取CNT的值
//int16_t Encoder_Get(void)
//{
//	
//	return TIM_GetCounter(TIM3);
//}

// 测速度怎么办??
// 就是在固定的闸门时间内，取出一次CNT，然后把CNT清零
// 修改Encoder_Get，需要在读取CNT后清零(接下来就这样写)
int16_t Encoder_Get(void)
{
	// 需要一个中间变量暂存一下时的值
	int16_t Temp = 0;
	
	// 读取CNT(获取一次CNT的值)
	Temp = TIM_GetCounter(TIM3);
	
	// 清零CNT
	// 直接设置定时器 CNT 寄存器的值
	TIM_SetCounter(TIM3, 0);
	
	return Temp;
}
