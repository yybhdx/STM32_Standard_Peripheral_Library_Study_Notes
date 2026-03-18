#include "Buzz.h"                // Device header

void buzz_init(void)
{
	// 开启GPIOB 对应的APB2总线的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// 初始化PB12
	GPIO_InitTypeDef GPIO_InitSturct;
	GPIO_InitSturct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitSturct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitSturct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitSturct);
	
}


void buzz_on(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_12); // 把PB12引脚输出高电平（逻辑 1）
}

void buzz_off(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12); // 把PB12输出低电平（逻辑 0）
}

