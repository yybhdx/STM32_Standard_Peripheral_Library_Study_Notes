#include "stm32f10x.h"                  // Device header

//代码中之所以将TIM3的触发源配置为其自身通道1的输入信号（TI1FP1），
//正是因为在硬件上，TIM2的通道1输出引脚（PA0）通过一根物理导线连接到了TIM3的通道1输入引脚（PA6）。

//TIM3：输入捕获频率计 (输入)

//功能：测量外部输入信号的频率。

//具体配置与作用：

//在 IC.c的 IC_Init()函数中，TIM3被初始化为输入捕获模式，并工作在从模式-复位下。

//其通道1（CH1）连接到GPIO引脚 PA6，用于捕获外部信号的上升沿。

//在 main.c中，程序不断调用 IC_GetFreq()函数。该函数读取TIM3捕获到的数据，计算出PA6引脚上信号的频率，并显示在OLED上。

//简单来说：TIM3在这个系统里就像一个频率计，负责测量输入信号的频率。


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
	/*配置成两个通道同时捕获同一个引脚的模式
	怎么配置呢?
	那就是将通道初始化的部分复制一份*/
	/*通道1是直连输入，上升沿触发*/
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1; // 因为我们计划配置的是TIM3_CH1，所以选择TIM_Channel_1
	TIM_ICInitStruct.TIM_ICFilter = 0XF; // 配置数字滤波器的参数 0x00（不滤波）到 0x0F（最大滤波）
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising; // 设置捕获信号的边沿极性 // TIM_ICPolarity_Rising为上升沿触发
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 设置输入捕获预分频器 // TIM_ICPSC_DIV1:（每个事件都捕获）
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; // 设置输入捕获通道的信号映射关系。 // 直接映射，如TI1FP1映射到IC1）
	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	
	/*通道1改成通道2*/
	/*直连输入改成交叉输入*/
//	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2; // 因为我们计划配置的是TIM3_CH1，所以选择TIM_Channel_1
//	TIM_ICInitStruct.TIM_ICFilter = 0XF; // 配置数字滤波器的参数 0x00（不滤波）到 0x0F（最大滤波）
//	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling; // 设置捕获信号的边沿极性 // TIM_ICPolarity_Falling为下降沿触发
//	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 设置输入捕获预分频器 // TIM_ICPSC_DIV1:（每个事件都捕获）
//	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_IndirectTI; // 设置输入捕获通道的信号映射关系。 // 交叉映射，如TI2FP1映射到IC1）
//	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	
	/*与上述注释掉的代码作用相同*/
	/*TIM_PWMIConfig配置定时器进入PWM输入模式（PWMI模式）*/
	/*使用这个函数，只需要传入一个通道的参数就行了
	在函数里，会自动把剩下的通道初始化成相反的配置
	比如我上面传入通道1 直连 上升沿
	那函数里面就会顺带配置通道2 交叉 下降沿*/
	/*如果我传入通道2 直连 上升沿
	函数就会顺带配置通道1 交叉 下降沿*/
	TIM_PWMIConfig(TIM3, &TIM_ICInitStruct);
	
	
	//第五步:选择从模式的触发源，触发源选择为TI1FP1，这里调用一个库函数，给一个参数就行了
	//目标定时器为TIM3
	//触发源选择为TIM_TS_TI1FP1:通道1的滤波后输入信号（TI1FP1），可配置为特定边沿触发
//	代码中参数 TI1FP1的“TI1”部分，指的是输入捕获定时器（TIM3）自身的通道1，而不是信号源定时器（TIM2）的通道1。
//	根据您的代码，信号的流向是：

//	TIM2(发生器) 的 CH1 → 从引脚 PA0输出PWM信号。

//	通过导线，信号物理连接到了 PA6引脚。

//	PA6? 引脚，恰好是 TIM3(捕获器) 的 CH1输入引脚。

//	所以，从物理连接上看，TIM2_CH1的输出，连接到了 TIM3_CH1的输入。
//		TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
	
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

/*获取占空比*/
uint32_t IC_GetDuty(void)
{
	/*高电平的计数值存在CCR2里
	整个周期的计数值存在CCR1里*/
	/*我们用CCR2 / CCR1 就能得到占空比了*/
	// 不*100的话 返回的值为0-1 *100 返回的值为0-100 对应百分比
	// 经过实测 CCR总会少一个数，所以我们给它各加一个1，补回来，看着舒服些
	return ((TIM_GetCapture2(TIM3) + 1) * 100) / (TIM_GetCapture1(TIM3) + 1);
	
	
}

/*目前我们所测的标准频率为1MHZ
计数器最大只能计到65535
所以所测量的最低频率是1M/65535 这个值大概是15hz
如果信号频率再低(所需的计数器就需要>65535) 计数器就要溢出了，所以最低频率就是15hz左右
那如果想要再降低一些最低频率的限制，我们可以把预分频加大点
这样标准频率就更低，所支持测量的最低频率也就更低，这是测量频率的下限*/

/*测量频率的上限，就是支持的最大频率
这个最大的频率并没有一个明显的界限
因为随着待测频率的增大，误差也会逐渐增大
如果非要找个频率上限那应该就是标准频率1MHz
超过1Mhz，信号频率比标准频率还高，那肯定测不了了
但是这个1MHz的上限没有意义，因为信号频率接近1Mhz时，误差已经非常大了
所以最大频率要看你对误差的要求*/