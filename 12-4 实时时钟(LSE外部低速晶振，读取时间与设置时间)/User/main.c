#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyRTC.h"


int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	
//	/*实时时钟RTC初始化*/
	MyRTC_Init();
	
	/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
	// 显示静态字符串
	OLED_ShowString(1, 1, "Date:XXXX-XX-XX");
	OLED_ShowString(2, 1, "Time:XX:XX:XX");
	OLED_ShowString(3, 1, "Cnt:");
	
	// 显示余数寄存器
	OLED_ShowString(4, 1, "DIV :");
	
	while (1)
	{
		// 读取时间
		MyRTC_ReadTime();
		
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
		
		// 年
		OLED_ShowNum(1, 6, MyRTC_Time[0], 4);
		
		// 月
		OLED_ShowNum(1, 11, MyRTC_Time[1], 2);
		
		// 日
		OLED_ShowNum(1, 14, MyRTC_Time[2], 2);
		
		// 小时
		OLED_ShowNum(2, 6, MyRTC_Time[3], 2);
		
		// 分钟
		OLED_ShowNum(2, 9, MyRTC_Time[4], 2);
		
		// 秒
		OLED_ShowNum(2, 12, MyRTC_Time[5], 2);
		
		//CNT
		OLED_ShowNum(3, 6, RTC_GetCounter(), 10);
		
		// RTC_GetDivider()读取RTC预分频余数寄存器（RTC_DIV）的值
		OLED_ShowNum(4, 6, RTC_GetDivider(), 10);
		// DIV变换范围为32767-0
		OLED_ShowNum(4, 6, RTC_GetDivider(), 10);
		
//		// 对DIV进行线性变换，将32767-0变为0-999
//		// DIV变换范围为0-999
//		OLED_ShowNum(4, 6, (32767 - RTC_GetDivider()) / 32767.0 * 999, 10);
		
	}
}
