#include "stm32f10x.h" // Device header
#include "PWM.h"

void Motor_Init(void)
{
	/*初始化PWM*/
	PWM_Init();

	/*因为在这个电机的模块里面还多了一些东西
	就是电机方向控制的那两个脚
	所以在这里我们还需要额外初始化方向控制的两个脚*/

	// 开启GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	// 初始化GPIOA_PIN4 GPIOA_PIN5
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*设置电机运行速度*/
/*负数表示反转，所以参数我们给一个带符号的变量*/
void Motor_SetSpeed(int8_t speed)
{
	/*用if来分别处理正传与反转*/
	/*正传*/
	if (speed >= 0)
	{
		/*设置方向控制角一个高电平 一个低电平
		设置哪个不重要，只要极性不一样就行*/

		/*设置GPIOA_PIN4为高电平*/
		GPIO_SetBits(GPIOA, GPIO_Pin_4);

		/*设置GPIOA_PIN5为低电平*/
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);

		/*设置定时器通道 3的CCR值*/
		PWM_SetCompare3(speed);
	}
	/*反转*/
	else
	{
		/*设置方向控制角一个高电平 一个低电平
		与上述的正传反过来*/

		/*设置GPIOA_PIN4为低电平*/
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);

		/*设置GPIOA_PIN5为高电平*/
		GPIO_SetBits(GPIOA, GPIO_Pin_5);

		/*设置定时器通道 3的CCR值
		因为此时speed为负数，而CCR值必须是正数，所以这里Speed要加负号*/
		PWM_SetCompare3(-speed);		
	}
}
