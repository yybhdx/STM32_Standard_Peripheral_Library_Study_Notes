#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "Serial.h"

uint8_t KeyNum;		//定义用于接收按键键码的变量

extern uint8_t Serial_RxData; // 存储串口接收中断的数据
extern uint8_t Serial_RxFlag; // 标志位

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
//	uint8_t Array[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
//	// LCD模块初始化
	OLED_Init(); // 初始化OLED显示屏
	
	Serial_Init(); //初始化USART1
	
	
	OLED_ShowString(1,1, "RxData:");
	
	while (1)
	{

		// 判断自己设置的标志位是否为 若为1则表示发生了中断
		if(Serial_GetRxFlag() == 1) 
		{
			// 返回接收到的数据函数给RxData
			RxData = Serial_GetRxData();
			
			// 数据回传
			// 将接收到的数据回传到电脑
			Serial_SendByte(RxData);
			
			OLED_ShowHexNum(1, 8, RxData, 2);
		}
	}
}
