#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyRTC.h"

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	
	/*实时时钟RTC初始化*/
	MyRTC_Init();

	while (1)
	{
		// OLED显示实时时钟的值
		// RTC_GetCounter():读取RTC（实时时钟）中32位计数器的当前值
		/**
		* @brief  OLED显示数字（十进制，正数）
		* @param  Line 起始行位置，范围：1~4
		* @param  Column 起始列位置，范围：1~16
		* @param  Number 要显示的数字，范围：0~4294967295
		* @param  Length 要显示数字的长度，范围：1~10
		* @retval 无
		*/
		OLED_ShowNum(1, 1, RTC_GetCounter(), 10);
		
	}
}
