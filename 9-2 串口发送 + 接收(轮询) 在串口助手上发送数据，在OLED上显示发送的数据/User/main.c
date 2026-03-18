#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "Serial.h"

uint8_t KeyNum;		//定义用于接收按键键码的变量


// 你配置的数据位长度是8位​ (USART_WordLength_8b)
// USART硬件会自动剥离起始位和停止位，只将数据位存入数据寄存器。
// USART_ReceiveData函数在8位模式下，返回值的高8位为0，低8位是有效数据，用uint8_t接收是安全且高效的
uint8_t RxData; // 存储串口接收到的数据

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIOB_InitStructure;
	GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOB_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIOB_InitStructure);		
	// PB6默认低电平 PB7默认高电平给OLDE供电
	GPIO_ResetBits(GPIOB, GPIO_Pin_6);

	GPIO_SetBits(GPIOB, GPIO_Pin_7);;
//	char arr[] = {'B'};
//	char arr1[] = "Hello World!"; // 自动添加\0
	
	// 定义一个数组
	uint8_t Array[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
//	// LCD模块初始化
	OLED_Init(); // 初始化OLED显示屏
	
	Serial_Init(); //初始化USART1
	
	while (1)
	{
		// 查询的流程是，在主函数里不断判断RXNE标志位
		// 如果RXNE标志位置1(RDR寄存器(接收数据寄存器)是“满”的，里面有新到的、CPU还未读取的数据。)就说明接收到数据了
		// 在调用USART_ReceiveData()读取DR寄存器就行了
		
		// 读取 USART1 外设的RXNE标志位的状态（是否被置位）
		if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET) // USART_FLAG_RXNE == 1说明RDR寄存器(接收数据寄存器)是“满”的，里面有新到的、CPU还未读取的数据
		{
			
			// 从串口1的接收数据寄存器（RDR/DR）中读取接收到的一帧数据
			// 读完DR可以自动清除RXNE标志位 所以就不需要我们主动清除标志位了
			RxData = USART_ReceiveData(USART1);
			
			/**
			* @brief  OLED显示数字（十六进制，正数）
			* @param  Line 起始行位置，范围：1~4
			* @param  Column 起始列位置，范围：1~16
			* @param  Number 要显示的数字，范围：0~0xFFFFFFFF
			* @param  Length 要显示数字的长度，范围：1~8
			* @retval 无
			*/
			OLED_ShowHexNum(1, 1, RxData, 2);
		}
	}
}
