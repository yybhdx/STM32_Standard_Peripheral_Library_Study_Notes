#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "CountSensor.h"

extern uint16_t count;

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
	
	
	// 初始化外部中断
	CountSensor_Init();
	
	// 初始化OLED
	OLED_Init();
	
	// 第1行 第1列 显示字符串Count
	OLED_ShowString(1, 1, "Count");
	while (1)
	{
		
		// 第1行 第7列 显示数字count 长度为5
		OLED_ShowNum(1, 7, count, 5);
		
	}
}
