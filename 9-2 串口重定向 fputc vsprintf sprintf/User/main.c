#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "Serial.h"

uint8_t KeyNum;		//定义用于接收按键键码的变量


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
	
	// 发送0X41 
	// 运行之后 TX引脚就会产生一个0X41对应的波形 这个波形可以发送给其他支持串口的模块
	// 也可以通过USB转串口模块发送到电脑端
//	Serial_SendByte(0X41); 
	
	// 与上面的显示一致
	// 0X41的的字符型就是'A'
//	Serial_SendByte('A'); 
	
	// 发送数组
	// Array:要传输的数组的首地址
	// 8:要传输的数组的总大小
//	Serial_SendArray(Array, 8);
	
	// 发送字符串
//	Serial_SendString("Hello World!");
	
	// 发送数字
	// 12345要发送的数字
	// 数字的长度
//	Serial_SendNumber(12345, 5);ADC1

//	printf("num = %d\r\n", 666);
	
//	// 定义一个字符串
//	char String[100];
//	// 把格式化字符输出到一个字符串里
//	sprintf(String, "num = %d\r\n", 666);
//	
//	// 把字符串String通过串口发送出去
//	Serial_SendString(String);
	
	// 串口助手中的文本编码改UTF-8就能发送中文 
	// 我们keli的编码格式还是GB2312 不需要改
	Serial_Printf("击飞大魔王\r\n");
	while (1)
	{
		
	}
}
