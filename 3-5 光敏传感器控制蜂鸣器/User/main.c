#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define Buzzer_OFF GPIO_SetBits(GPIOB, GPIO_Pin_12)

#define Buzzer_ON GPIO_ResetBits(GPIOB, GPIO_Pin_12)
void buzzer_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

}
// 蜂鸣器
void buzzer_turn(void)
{
	if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12) == 0)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
	}else{
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	}
}


void lightSensor_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // 模式为上拉输入
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}


// 返回PB13输入引脚输入状态
int lightSensor_Get(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
}



uint8_t KeyNum;
int main(void)
{
	buzzer_init();
	lightSensor_init();
	
	Buzzer_OFF;
	while(1)
	{
		// 光线暗时蜂鸣器响
		if(lightSensor_Get() == 1)
		{
			Buzzer_ON;
		}
		// 光线亮时蜂鸣器灭
		else
		{
			Buzzer_OFF;
		}
		
	}
	
}