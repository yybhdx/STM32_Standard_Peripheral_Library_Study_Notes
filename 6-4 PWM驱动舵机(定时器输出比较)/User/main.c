#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "PWM.h"

uint8_t KeyNum;		//定义用于接收按键键码的变量


int main(void)
{
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
	
  // LCD模块初始化
	OLED_Init(); // 初始化OLED显示屏
	
	// PWM初始化
	pwm_init();
	
	// 设置定时器通道 2 的捕获/比较寄存器 CCR2 的值为500(对应0.5ms -90°转动)
	pwm_Tim_SetCompare2(500);
//	
//	// 设置定时器通道 2 的捕获/比较寄存器 CCR2 的值为1000(对应1.0ms -45°转动)
//	pwm_Tim_SetCompare2(1000);
//	
//	// 设置定时器通道 2 的捕获/比较寄存器 CCR2 的值为1500(对应1.5ms -0°转动)
//	pwm_Tim_SetCompare2(1500);
//	
	// 设置定时器通道 2 的捕获/比较寄存器 CCR2 的值为2000(对应2.0ms 45°转动)
//	pwm_Tim_SetCompare2(2000);
	
//	// 设置定时器通道 2 的捕获/比较寄存器 CCR2 的值为2500(对应2.5ms 90°转动)
//	pwm_Tim_SetCompare2(2500);
	
	while (1)
	{
		
	}
	
//根据您提供的代码和图片中的舵机PWM信号要求，代码对定时器TIM2的配置确实满足了舵机的控制需求。以下是具体分析：

//1. ​​周期匹配（20ms）​​
//​​时钟源​​：STM32内部时钟频率为72MHz。

//​​预分频器（PSC）​​：设置为 72-1（实际分频系数为72），将时钟频率降低为：

//72MHz / 72 = 1MHz(每个计数周期=1μs)
	
//​​自动重装载值（ARR）​​：设置为 20000-1（实际值为20000），定时器计数周期为：

//20000×1μs=20ms
//✅ 完全符合舵机要求的​​20ms信号周期​​。

//2. ​​高电平宽度范围（0.5ms ~ 2.5ms）​​
//高电平宽度0.5ms~2.5ms​​：高电平宽度由捕获/比较寄存器（CCR）控制。在PWM模式1下，当计数器值小于CCR时输出高电平。因此：

//设置 CCR = 500时，高电平宽度 = 500×1μs=0.5ms

//设置 CCR = 2500时，高电平宽度 = 2500×1μs=2.5ms

//这正好覆盖了舵机要求的范围。

//✅ 完全覆盖舵机要求的​​0.5ms~2.5ms高电平范围​​。

//3. ​​输出配置​​
//​​输出模式​​：TIM_OCMode_PWM1（高电平有效，计数小于CCR时输出高电平）。

//​​极性​​：TIM_OCPolarity_High（高电平为有效电平）。

//​​GPIO配置​​：PA1引脚设置为复用推挽输出（GPIO_Mode_AF_PP），正确映射到TIM2通道2（TIM2_CH2）。
}
