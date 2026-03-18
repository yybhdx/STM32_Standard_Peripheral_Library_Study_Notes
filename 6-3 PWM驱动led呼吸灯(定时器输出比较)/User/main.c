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
	
	while (1)
	{
		
	}
}
