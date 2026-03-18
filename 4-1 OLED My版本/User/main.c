#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"

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
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
	char arr[] = {'B'};
	char arr1[] = "Hello World!"; // 自动添加\0
//	// LCD模块初始化
	OLED_Init(); // 初始化OLED显示屏
	
	// 在第一行 第一列显示字符'A'
	OLED_ShowChar(1, 1, 'A'); // 在指定位置显示单个字符
	
	// 在第一行 第二列显示字符'B'
	OLED_ShowChar(1, 2, arr[0]); 
	
	// 在第一行 第三列显示字符串HELLOWORD
	OLED_ShowString(1, 3, arr1); // 在指定位置显示字符串
	
	// 在第二行 第一列显示数字12345 长度为5
	OLED_ShowNum(2, 1, 12345, 5); // 显示十进制正整数
	
	// 在第二行 第七列显示数字12345 长度为5(前面会带+号)
	OLED_ShowSignedNum(2, 7, 12345, 5); // 显示带符号的十进制数
	
	// 在第三行 第一列显示数字-66 长度为2(前面会带-号)
	OLED_ShowSignedNum(3, 1, -66, 2); // 显示带符号的十进制数
	
	// 在第四行 第一列显示十六进制数字0XAA55 长度为4(前面会带-号)
	OLED_ShowHexNum(4, 1, 0XAA55, 4);   // 显示十六进制数
	
	// 在第四行 第七列显示二进制数字0XFF(二进制用十六进制代替) 长度为8 若为9 则前面会补0
	OLED_ShowBinNum(4, 7, 0XFF, 8); // 显示二进制数

	
	while (1)
	{

	}
}
