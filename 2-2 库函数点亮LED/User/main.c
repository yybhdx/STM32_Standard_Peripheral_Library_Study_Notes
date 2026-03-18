#include "stm32f10x.h"                  // Device header
#include "math.h"

int main()
{
	// // 使用寄存器的方式打开GPIOC的时钟 
	// RCC->APB2ENR = 0X00000010;

	// // 配置PC13口的模式
	// GPIOC->CRH = 0X00300000;
	
	// // ODR前16位始终为0000
	// // 点亮PC13对应的LED  对应位置零(低电平点亮)
	// GPIOC->ODR = 0X00000000;
	
	// // 熄灭PC13对应的LED  对应位置1(高电平熄灭)
	// GPIOC->ODR = 0X00002000;

	// 开启GPIOC的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	// 配置端口模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); // 初始化GPIO
	
	// GPIOC 13输出高电平(熄灭)
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	
	// GPIOC 13输出低电平(点亮)
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	while(1)
	{
		
	}
	
}

