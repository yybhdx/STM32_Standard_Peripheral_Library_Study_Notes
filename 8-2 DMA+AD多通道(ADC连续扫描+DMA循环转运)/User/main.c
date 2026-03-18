#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
/*如何实现多通道?
我们可以使用上面的这个单词转换，非扫描的模式，来实现多通道
只需要在每次触发转换之前，手动更改一下列表第一个位置的通道就行了
比如第一次转换，先写入通道0，之后触发、等待、读值
第二次转换，再把通道0改成通道1，之后触发，等待，读值
第三次转换，再先改成通道2，等等等等
这样，在转换前，先指定一下通道，再启动转换
就可以轻松实现多通道转换的功能了*/


int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	
	AD_Init(); // ADC初始化
	
	OLED_ShowString(1, 1, "AD0:");
	
	OLED_ShowString(2, 1, "AD1:");
	
	OLED_ShowString(3, 1, "AD2:");
	
	OLED_ShowString(4, 1, "AD3:");
	
	
	while (1)
	{
		// 主循环啥都不干 直接读取AD_Value数组，就能得到结果
		
		OLED_ShowNum(1, 5, AD_Value[0], 4); // 显示滑动变阻器的数据
		
		OLED_ShowNum(2, 5, AD_Value[1], 4); // 显示光敏电阻的数据
		
		OLED_ShowNum(3, 5, AD_Value[2], 4); // 显示热敏电阻的数据
		
		OLED_ShowNum(4, 5, AD_Value[3], 4); // 显示对射式红外该传感器的数据
		
		Delay_ms(100);
	}
}
