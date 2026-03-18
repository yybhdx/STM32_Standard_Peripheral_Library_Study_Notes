#include "stm32f10x.h"                  // Device header
#include <stdio.h> // 添加C语言头文件库
#include <stdarg.h> // 添加C语言头文件库


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

	// 3.配置USART，直接使用一个结构体，就可以把里面所有的参数都配置好了
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600; // 波特率配置成9600
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 硬件流控（RTS/CTS）：无硬件流控
	USART_InitStruct.USART_Mode = USART_Mode_Tx ; // 使能发送 (Transmitter)
	USART_InitStruct.USART_Parity = USART_Parity_No; // 无奇偶校验 (最常用)
	USART_InitStruct.USART_StopBits = USART_StopBits_1; // 1位停止位 (最常用)
	USART_InitStruct.USART_WordLength = USART_WordLength_8b; // 8位数据 (最常用)
	USART_Init(USART1, &USART_InitStruct);
	
	// 使能串口1
	USART_Cmd(USART1, ENABLE);
	// 4.若只需要发送的功能，就直接开启USART就行了
	// 如果你需要接收的功能，可能还需要配置中断，那就再开启USART之前，再加上ITConfig和NVIC的代码就行了
	
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
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	
	// 遍历数组
	for(i=0;i<Length;i++)
	{
		// 依次发送数组的每一项
		Serial_SendByte(Array[i]);
	}
	
	// 发送回车符和换行符
	Serial_SendByte('\r');  // 发送回车符（单引号表示字符）;
	Serial_SendByte('\n');  // 发送换行符（单引号表示字符）
	
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

