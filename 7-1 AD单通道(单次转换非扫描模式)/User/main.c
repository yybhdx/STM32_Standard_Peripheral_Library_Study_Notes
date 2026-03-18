#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

uint16_t ADValue; // 用于存储ADC的转换结果值

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	
	AD_Init(); // ADC初始化
	
	OLED_ShowString(1, 1, "ADValue:");
	
	while (1)
	{
		// 获取ADC的转换结果值
		ADValue = AD_GetValue();
		
		// 在OLED的1行九列显示ADValue的值
		OLED_ShowNum(1, 9, ADValue, 4); 
		// 显示的ADValue值的末尾会有些抖动，这是正常的波动
		/* 如果你想对这个值进行判断，再执行一些操作
		比如光线的AD值小于某一阈值，就开灯，大于某一阈值，就关灯
		那可能会存在这样的情况:比如光线逐渐变暗，AD值逐渐变小
		但是由于波动，AD值会在判断阈值负极来回跳变
		这会导致输出产生抖动，来回开灯关灯开灯关灯*/
		/*那如何避免这种情况??
		这个可以使用迟滞比较的方法来完成
		设置两个阈值，低于下阈值时，开灯
		高于上阈值时才关灯，这就可以避免输出抖动的问题了
		这个跟GPIO那一节讲的施密特触发器是一个原理*/
		/*另外，如果你觉得数据跳变太厉害
		还可以采用滤波的方法，让AD值平滑一些
		比如均值滤波，就是读10个或20个值，取平均值，作为滤波的AD值
		或者还可以裁剪分辨率，把数据的尾数去掉
		这样也可以减少数据波动*/
	}
}
