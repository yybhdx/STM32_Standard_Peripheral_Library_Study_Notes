#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	
	/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
	OLED_ShowString(1, 1, "SYSCLK:");
	

	// OLED显示主频
	/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
	OLED_ShowNum(1, 8, SystemCoreClock, 8); // 72000000 // 目前显示的主频为72M 修改主频为36M时，OLED显示36M
	
	
	while (1)
	{
		// 在主循环中加入Delay表示当前代码的运行速度，闪烁周期为1S，
		// 当主频修改为36M时，闪烁周期变慢了，目前闪烁周期是2S，说明主频确实降低了一半
		
		// 显示Running
		OLED_ShowString(2, 1, "Running");
		
		Delay_ms(500);
		// 擦除Running
		OLED_ShowString(2, 1, "       ");
		
		Delay_ms(500);
	}
}
