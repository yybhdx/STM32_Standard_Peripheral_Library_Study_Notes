#include "stm32f10x.h"                  // Device header
#include <stdio.h> // 添加C语言头文件库
#include <stdarg.h> // 添加C语言头文件库

// 定义两个缓存区的数组用于收发数据包

// 发送缓存区删掉
//uint8_t Serial_TxPacket[4]; // 存储发送的载荷数据，包头包尾不存在

// 接收的数据类型定义为char 用于接收字符,数量给100，防止溢出，这要求单条指令最长不能超过100个字符
char Serial_RxPacket[100]; // 存储接收的载荷数据，包头包尾不存在

uint8_t Serial_RxFlag; //接收数据包的标志位，如果接收到一个数据包，就置RxFlag

// 串口初始化函数
void Serial_Init(void)
{
	// 1.开启时钟，把需要用到的USART和GPIO的时钟打开
	
	// 开启APB2上的USART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	// 开启APB2上的GPIOA(对应USART1的TX:PA9 RX:PA10)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 2.GPIO初始化，把TX配置成复用输出,由于我们的程序只需要发送,所以不配置RX引脚
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//将PA9引脚初始化为复用推挽输出
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//将PA10引脚初始化为浮空输入/上拉输入
	

	// 3.配置USART，直接使用一个结构体，就可以把里面所有的参数都配置好了
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600; // 波特率配置成9600
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 硬件流控（RTS/CTS）：无硬件流控
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx ; // 模式为:同时使能接收和发送 (全双工，最常用) 
	USART_InitStruct.USART_Parity = USART_Parity_No; // 无奇偶校验 (最常用)
	USART_InitStruct.USART_StopBits = USART_StopBits_1; // 1位停止位 (最常用)
	USART_InitStruct.USART_WordLength = USART_WordLength_8b; // 8位数据 (最常用)
	USART_Init(USART1, &USART_InitStruct);
	
	// 使能串口1
	USART_Cmd(USART1, ENABLE);
	
	// 4.若只需要发送的功能，就直接开启USART就行了
	// 如果你需要接收的功能，可能还需要配置中断，那就再开启USART之前，再加上ITConfig和NVIC的代码就行了
	// 串口接收有中断和查询两种方法
	// 如果使用查询，那么初始化就结束了。
	// 如果使用中断，那还需要在这里开启中断，配置NVIC
	
	
	// 配置USART中断源(单独地使能或失能某个特定的USART中断)
	// 配置USART1的接收数据寄存器非空中断 触发条件:接收数据寄存器（RDR）收到新数据时(即RXNE == 1)。最常用。
	// RXNE为1时会产生中断(手册上就是这样写的)
	 USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	 
	 // 配置NVIC
	 // 设置优先级分组方案
	 // 设置优先级分组方案为抢占优先级位数2,子优先级位数2
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 
	 // 初始化NVIC
	 // 初始化NVIC的USART1通道
	 NVIC_InitTypeDef NVIC_InitStruct;
	 NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;// 中断通道号为USART1
	 NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; // 使能中断(ENABLE就是1)
	 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级为1
	 NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1; // 响应优先级为1
	 NVIC_Init(&NVIC_InitStruct);
	// 5.初始化完成后 若需要发送数据，调用一个发送函数就行了
	// 如果要接收数据，就调用接收的函数
	// 如果要获取发送和接收的状态，就调用获取标志位的函数
	
}

// 串口发送函数
void Serial_SendByte(uint8_t Byte)
{
	// 把一帧数据写入指定串口的发送数据寄存器（TDR/DR），让串口开始发送
	// USARTx:要使用的串口外设 Data你要发送的数据，类型是 uint16_t
	// 为什么用 16 位？因为串口支持 8 位数据帧 和 9 位数据帧 两种情况
	// 这个函数运行后，就将Byte数据写入数据寄存器(TDR)了 
	USART_SendData(USART1, Byte);
	
	// 等待标志位
	// 读取 USART 外设某个标志位的状态（是否被置位）
	// 读取 USART1 外设的USART_FLAG_TXE（发送数据寄存器空）的状态（是否被置位）
	// 只要USART1的USART_FLAG_TXE(TXE位)是0(发送数据寄存器（TDR/DR）不为空，说明数据还没到发送移位寄存器了) 就死循环 
	// 直到TXE位置1(发送数据寄存器（TDR/DR）为空，说明数据已经转到发送移位寄存器了)为止
	// 然后等TDR的数据转移到发送移位寄存器时，数据就可以通过 TX 引脚按 波特率 一位一位发出了
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

// 串口发送数组函数
// uint8_t *Array 指向待发送数组的首地址
// uint16_t Length 发送的数组内的字符数
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	
	// 遍历数组
	for(i=0;i<Length;i++)
	{
		// 依次发送数组的每一项
		Serial_SendByte(Array[i]);
	}
	
//	// 发送回车符和换行符
//	Serial_SendByte('\r');  // 发送回车符（单引号表示字符）;
//	Serial_SendByte('\n');  // 发送换行符（单引号表示字符）
	
}

// 串口发送字符串函数
void Serial_SendString(char *string)
{
	uint8_t i;
	for(i=0;string[i] != '\0';i++)
	{
		// 依次发送字符串的每一项
		Serial_SendByte(string[i]);		
		
	}
	// 发送回车符和换行符
	Serial_SendByte('\r');  // 发送回车符（单引号表示字符）
	Serial_SendByte('\n');  // 发送换行符（单引号表示字符
}

// 返回一个X的Y次方的函数
uint32_t Serial_Pow(uint32_t X,uint32_t Y)
{
	uint32_t Result = 1;
	while(Y--)
	{
		Result *=  X;
	}
	return Result;
}
// 串口发送数字函数
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	// 将Number的个位 十位 百位等以10进制拆分开
	// 然后转换成字符数字对应的数据，依次发送出去
	for(i=0;i<Length;i++)
	{
		// 取出Number的每一位 
		// 若Length为2 
		// 则第一次是 Number / 10 % 10
		// 第二次是Number % 10
		// 因为最终要以字符的形式显示，所以还需要加一个偏移
		// 字符'0' 对应的ASCll为0X30
		Serial_SendByte(Number / Serial_Pow(10,Length - i - 1) % 10 + '0');
	}
	// 发送回车符和换行符
	Serial_SendByte('\r');  // 发送回车符（单引号表示字符）
	Serial_SendByte('\n');  // 发送换行符（单引号表示字符
}

// 重写串口重定向函数
// int fputc(int ch, FILE *f)是在标准C库头文件 stdio.h中定义的fputc函数的原型
int fputc(int ch, FILE *f)
{
	// 把fputc重定向到串口
	Serial_SendByte(ch);
	
	return ch;
}

// 封装Sprintf为可变参数格式
// char *format用来接收格式化字符串 
// ...用来接收可变参数列表
void Serial_Printf(char *format, ...)
{
	// 定义输出的字符串
	char String[100];
	
	// 定义一个参数列表变量
	va_list arg;
	
	// 从format位置开始接收参数表，放在arg里面
	va_start(arg, format);
	
	// 格式化字符串进 String
	// sprintf打印位置是string 格式化字符串是format, 参数表是arg
	// sprintf要改成vsprintf 因为sprintf只能接收直接写的参数，对于这种封装格式，要用vsprintf
	vsprintf(String, format, arg);
	
	// 释放参数表
	va_end(arg);
	
	// 把String发送出去
	Serial_SendString(String);
}

//// 为发送的数组上的四个数据，添加包头包尾
//void Serial_SendPacket(void)
//{
//	// 1.发送包头
//	Serial_SendByte(0XFF);
//	
//	// 2.串口发送数组
//	// 串口发送数组函数
//	// uint8_t *Array 指向待发送数组的首地址
//	// uint16_t Length 发送的数组内的字符数
//	Serial_SendArray(Serial_TxPacket, 4);
//	
//	// 3.发送包尾
//	Serial_SendByte(0XFE);
//	
//}

//// 返回自己设置的标志位函数
//uint8_t Serial_GetRxFlag(void)
//{
//	if(Serial_RxFlag == 1)
//	{
//		Serial_RxFlag = 0;
//		return 1;
//	}
//	
//	return 0;
//}


// 串口接收中断函数
// 进入中断服务函数的原因?：USART1的某个中断事件发生了，但不确定是哪一个。
// 为了确定具体是哪一个中断事件?（如接收完成、发送完成、空闲、错误等）触发了本次中断，从而执行正确的处理逻辑。
void USART1_IRQHandler(void)
{
	// 定义一个静态变量RxState当作状态变量S
	// 这个静态变量类似于全局变量，函数进入只会初始化一次0，在函数推出之后，数据仍然有效
	// 与全局变量不同的是:静态变量只能在本函数中使用
	static uint8_t RxState = 0;
	
	// 定义一个静态变量pRxPacket指示接收到哪个了，最开始默认为0
	static uint8_t pRxPacket = 0;
	
	// 判断是否是USART1的RXNE中断
	// 读取 USART1 外设的RXNE标志位的状态（是否被置位）
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
	{
		// 在接收中断中使用状态机来执行接收逻辑
		// 接收数据包，然后把载荷数据存在RxPacket数组里
		
		// 根据PPT的状态转移图定义一个标志当前状态的变量S
		
		// 根据PPT 三个状态S分别为0、1、2
		// 根据RxState的不同，我们进入不同的处理程序
		
		//获取一下RxData
		uint8_t RxData = USART_ReceiveData(USART1);
		
		// 进入等待包头的程序
		if(RxState == 0)
		{
			// 对于状态0，我们的操作是
			// 如果RxData == '@'，我们就进行状态转移
			// if条件满足，说明收到包头 如果数据==包头，并且Serial_RxFlag == 0,这样才执行接收，
			// 否则就是你发送的太快了，我还没处理完呢，就跳过这个数据包。
			// 这样写数据和读数据就是严格分开的，不会同时进行，就可以避免数据包错位的现象了。
			// 不过这样的话，你发送数据包的频率就不能太快了。否则会丢弃部分包。
			// 或者你还可以再定义一个指令缓冲区，把接受好的字符串放在这个指令缓存区进行排队，这样处理起来会更加有条理。有关这个数据包的收发，其实还是有非常多的细节问题需要考虑的。
			// 实际应用的话，还是得多想想
			if(RxData == '@' && Serial_RxFlag == 0) 
			{
				// 那就可以转移状态RxState = 1
				RxState = 1;
				
				// 状态0转移到状态1时提前清0pRxPacket
				pRxPacket = 0;
			}
			// 如果RxData != '@'，我们就不转移状态，维持状态0
			
		}
		// 进入接收数据的程序(要用else if,如果用if，那么可能在状态转移的时候会出问题，比如在状态0，你想转移到状态1，就置RxState=1,就会造成下面状态1的条件就立马满足了。这样会出现了两个if都成立的情况。)
		// 使用else if 保证每次进来之后，只能选择执行其中一个状态的代码。
		// 或者用switch case语句，也可以保证只有一个条件满足
		// 因为载荷字符数量不确定，所以每次接受之前，我们必须先判断是不是包尾
		else if(RxState == 1) 
		{
			// 判断接收的字符是不是包尾字符
			if(RxData == '\r')
			{
				// 如果是，就跳转到状态2
				RxState = 2;
			}
			else //如果不是则接收数据
			{
				// 进入接收数据的状态
				// 这里依次接收4个数据，存在RxPacket数组里面，所以还需要一个变量pRxPacket来记一下接收几个了
				// 将RxData存在接收数组Serial_RxPacket里面
				Serial_RxPacket[pRxPacket] = RxData;
				
				// 数组下标++，移动到下一个位置
				// 这样就是，每进一次接收状态，数据就转存一次缓存数组，同时存的位置++
				pRxPacket++;				
				
			}		
			// 下面不要了		
			
//			// 当四个载荷数据已经收完时
//			// 就可以转移到下一个状态了
//			if(pRxPacket >= 4)
//			{
//				RxState = 2;
//				
//				// 清0pRxPacket，为下一次接收做准备
//				pRxPacket = 0;
//			}
		}
		else if (RxState == 2) // 进入等待包尾的程序(要用else if,如果用if，那么可能在状态转移的时候会出问题)
		{
			// 判断RxData是不是=='\n',等待第二个包尾
			if(RxData == '\n')
			{
				// 如果是包尾，就将RxState置0 回到最初的状态。同时，代表着数据包接收到了,可以置一个接收标志位Serial_RxFlag
				RxState = 0;
			
				// 同时接收到之后，我们再给我这个字符数组的最后，加一个字符串结束标志位\0，方便我们后续对字符串进行处理
				Serial_RxPacket[pRxPacket] = '\0';
				
				Serial_RxFlag = 1;
				
			}
			
			// 如果不等于FE的话，就是还没收到包尾，我们同样不做处理，仍然在这个状态，等待包尾。到这时，接收数据包的状态机程序就完成了
		}
		// 读取了DR可以自动清除 没读取DR需要手动清除
		// 这里我们手动清除一下
		// 清除USART1的USART_IT_RXNE: 接收数据寄存器非空中断
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

