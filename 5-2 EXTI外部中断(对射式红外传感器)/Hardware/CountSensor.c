#include "stm32f10x.h"                  // Device header

// 外部中断的信号从GPIO到AFIO，再到EXTI，再到NVIC 最终通向CPU
void CountSensor_Init()
{
	// 开启RCC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// 开启GPIOB的时钟
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  // 开启AFIO的时钟
	
	// 定义结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 浮空输入 I/O口模式要选择浮空输入/上拉输入/下拉输入 // 视频中选择上拉输入
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); // 初始化GPIOB外设
	
	// 把 GPIOB 引脚PB14 连接到对应的 EXTI14 外部中断线
	// 执行完这个函数后 AFIO的第14个数据选择器就拨好了 
	// 其中输入端被拨到了GPIOB外设上，对应PB14号引脚,输出端固定连接的是EXTI的第14个中断线路
	// 这样PB14号引脚的电平信号就可以顺利通过AFIO，进入到后级EXTI电路了
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14); // 设置 AFIO_EXTICR[3] 里面的寄存器，把 EXTI14 的输入信号映射到 PB14。
	
	// 配置EXTI
	// 将EXTI的第14个线路配置为中断模式 下降沿触发
	// 这样PB14的电平信号就能够通过EXTI通向下一级NVIC了
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14; // 因为是14引脚 所以选择的EXTI线为EXTI_Line14
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; // 使能该 EXTI 线
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // 配置工作模式位中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 配置触发方式为下降沿触发(从高电平跳变到低电平的瞬间触发中断):移开红外传感器挡光片时触发
	
	// 初始化EXTI
	EXTI_Init(&EXTI_InitStructure); 
	
	// 配置NVIC 
	// 配置抢占优先级与子优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 先占优先级2位(可取 0~3),从优先级 2 位(可取 0~3)

	// 初始化NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; // 中断通道号:STM32的EXTI10 - EXTI15 都合并到了这个通道里面
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级设置为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // 响应优先级(子优先级)设置为1
	
	NVIC_Init(&NVIC_InitStructure);
	
	
}
uint16_t count = 0; // 统计计数的次数
// 固件库中EXT15 - 10 的中断函数
void EXTI15_10_IRQHandler()
{
	// 进入中断后 在判断中断标志位前+10us延时 防止跳动
	Delay_ms(50);
	
	// 判断中断标志位是否是我想要的中断源
	if(EXTI_GetITStatus(EXTI_Line14) == SET) //判断EXTI14的中断标志位是不是为1
	{
		// 当Delay200ms后 才不会出现每遮挡拿开一次count+2的现象
		Delay_ms(200);
		count++;
		Delay_ms(200);
		
		// 中断结束后清除中断标志位 因为若中断标志位为1 那么程序就会跳转到中断函数
		// 若不清除中断标志位，那它就会一直申请中断 这样程序就会不断响应中断，执行中断函数，那程序就卡死在中断函数里面了
		// 所以每次中断程序结束后，都应该清除一下中断标志位
		EXTI_ClearITPendingBit(EXTI_Line14); // 因为我们是依靠EXTI_Line14触发中断，所以清除EXTI_Line14线的中断挂起标志位
		
	}
	
}
