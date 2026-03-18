#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void key_init(void)
{
	// 开启APB2总线上的GPIOB外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
}

// 按键检测
int Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
	{
		
		Delay_ms(50);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0); // PB1按键抬起后退出while循环
		Delay_ms(20);
		KeyNum = 1; // PB1按键按下抬起后返回1
	}
	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
	{
		
		Delay_ms(50);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0); // PB11按键抬起后退出while循环
		Delay_ms(20);
		KeyNum = 2; // PB11按键按下抬起后返回2
	}
	
	
	return KeyNum;
}


