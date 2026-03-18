#include "stm32f10x.h"                  // Device header
#include "math.h"

int main()
{
	// 使用寄存器的方式打开GPIOC的时钟 
	RCC->APB2ENR = 0X00000010;

	// 配置PC13口的模式
	GPIOC->CRH = 0X00300000;
	
	// ODR前16位始终为0000
	// 点亮PC13对应的LED  对应位置零(低电平点亮)
	GPIOC->ODR = 0X00000000;
	
	// 熄灭PC13对应的LED  对应位置1(高电平熄灭)
	GPIOC->ODR = 0X00002000;
	while(1)
	{
		
	}
	
}

