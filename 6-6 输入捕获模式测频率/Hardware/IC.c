#include "stm32f10x.h"                  // Device header

//第一步:开启RCC时钟，把GPIO和TIM的时钟打开
//第二步:GPIO初始化，把GPIO配置成输入模式(一般选择上拉输入或浮空输入)
//第三步:配置时基单元，让CNT计数器在内部时钟的驱动下自增运行
//第四步:配置输入捕获单元，包括滤波器，极性，直连通道还是交叉通道，分频器这些参数，用一个结构体就可以统一进行配置了。
//第五步:选择从模式的触发源，触发源选择为TI1FP1，这里调用一个库函数，给一个参数就行了
//第六步:选择触发之后执行的操作，执行Reset操作，这里也是调用一个库函数就行了。
//第七步:当这些电路都配置好后，调用TIM_Cmd函数，开始启动定时器。

/*输入捕获初始化*/
void IC_Init(void)
{
	//第一步:开启RCC时钟，把GPIO和TIM的时钟打开
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//开启TIM3的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//开启GPIOA的时钟 // 对应TIM3_CH1(定时器3通道一的引脚) PA6
	
	//第二步:GPIO初始化，把GPIO配置成输入模式(一般选择上拉输入或浮空输入)
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // GPIO模式配置成上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	 // PA6
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);							
																		
	
	//第三步:配置时基单元，让CNT计数器在内部时钟的驱动下自增运行
	/*配置时钟源*/
	TIM_InternalClockConfig(TIM3);		//配置定时器3的时钟源为内部时钟（CK_INT），若不调用此函数，TIM默认也为内部时钟
	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;					//计数周期，即ARR的值 // 这里的65536 - 1是满量程计数
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;				//预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);             //将结构体变量交给TIM_TimeBaseInit，配置TIM3的时基单元

	//第四步:配置输入捕获单元，包括滤波器，极性，直连通道还是交叉通道，分频器这些参数，用一个结构体就可以统一进行配置了。
	/*配置定时器的输入捕获功能*/
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1; // 因为我们计划配置的是TIM3_CH1，所以选择TIM_Channel_1
	TIM_ICInitStruct.TIM_ICFilter = 0X01; // 配置数字滤波器的参数 0x00（不滤波）到 0x0F（最大滤波）
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising; // 设置捕获信号的边沿极性 // TIM_ICPolarity_Rising为上升沿触发
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 设置输入捕获预分频器 // TIM_ICPSC_DIV1:（每个事件都捕获）
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; // 设置输入捕获通道的信号映射关系。 // 直接映射，如TI1FP1映射到IC1）
	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	
	
	//第五步:选择从模式的触发源，触发源选择为TI1FP1，这里调用一个库函数，给一个参数就行了
	//目标定时器为TIM3
	//触发源选择为TIM_TS_TI1FP1:通道1的滤波后输入信号（TI1FP1），可配置为特定边沿触发
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
	
	//第六步:选择触发之后执行的操作，执行Reset操作，这里也是调用一个库函数就行了。
	//目标定时器为TIM3
	//TIM_SlaveMode_Reset:触发信号上升沿复位计数器CNT并产生更新事件
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
	
	//第七步:当这些电路都配置好后，调用TIM_Cmd函数，开始启动定时器。
	//使能定时器3
	/*当我们启动定时器之后，CNT就会在内部时钟的驱动下不断自增
	即使没有信号过来，它也会不断自增，不过这也没有关系，因为有信号来的时候 它就会在从陌生的作用下自动清零，并不会影响测量*/
	TIM_Cmd(TIM3, ENABLE);
	
}

/*返回最新一个周期的频率值*/
/*当我们查看频率时，可以通过读取CCR进行计算*/
uint32_t IC_GetFreq(void)
{
	// TIM_GetCapture1():读取STM32定时器通道1的捕获/比较寄存器（CCR1）的值
	return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}
