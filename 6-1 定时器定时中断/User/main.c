#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "Timer.h"

int num = 0;

int main(void)
{
	// OLED供电
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
	
	// 初始化定时器
	Timer_Init();
	
	
	// LCD模块初始化
	OLED_Init(); // 初始化OLED显示屏
	
	// 在第一行 第一列显示NUM的变化情况
	OLED_ShowString(1, 1, "num:"); // 在指定位置显示字符串

	// 在第二行 第一列显示计数器的变化情况(我们设的计数最大值为9999 到10000时置0,num++)
	OLED_ShowString(2, 1, "Count:"); // 在指定位置显示字符串	
	while (1)
	{
		OLED_ShowNum(1, 5, num, 3);
		OLED_ShowNum(2, 5, TIM_GetCounter(TIM2), 5); // 实时显示定时器计数器 CNT 寄存器当前值
	}
}
