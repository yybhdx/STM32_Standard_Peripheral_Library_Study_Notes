/**
  ******************************************************************************
  * @文件    main.c
  * @作者    Jiangxie Technology
  * @版本    V1.0
  * @日期    2026-07-07
  * @摘要    PWM驱动直流电机 - 主程序
  *          通过按键控制电机的速度和方向，OLED实时显示当前速度值。
  *          速度范围：-100 ~ +100，正值正转，负值反转。
  ******************************************************************************
  */

#include "stm32f10x.h"                  // 器件头文件
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Key.h"

uint8_t KeyNum;     /* 存储按键键码值 */
int8_t Speed;       /* 电机速度，范围：-100 ~ +100 */

/**
  * @摘要    主函数
  * @参数    无
  * @返回值  无
  * @说明    程序入口点。初始化外设后，在主循环中检测按键，
  *          每按一次速度+20，超过100后跳转到-100实现反转。
  */
int main(void)
{
	/* 开启GPIOB的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/* GPIO初始化：PB6和PB7用于OLED模块供电控制 */
	GPIO_InitTypeDef GPIOB_InitStructure;
	GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;           /* 推挽输出模式 */
	GPIOB_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;    /* 选择PB6和PB7引脚 */
	GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;          /* 输出速度50MHz */
	GPIO_Init(GPIOB, &GPIOB_InitStructure);

	/* 设置OLED供电引脚电平：PB6低电平(GND)，PB7高电平(VCC) */
	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	GPIO_SetBits(GPIOB, GPIO_Pin_7);

	/* 模块初始化 */
	OLED_Init();        /* OLED显示屏初始化 */
	Key_Init();         /* 按键初始化 */
	Motor_Init();       /* 电机驱动初始化（内部会初始化PWM和方向控制引脚） */

	/* 在OLED第1行第1列显示"Speed"标签 */
	OLED_ShowString(1, 1, "Speed");

	while (1)
	{
		/* 读取按键状态 */
		KeyNum = Key_GetNum();

		/* 如果按键1（PB1）被按下 */
		if (KeyNum == 1)
		{
			/* 速度递增20 */
			Speed += 20;

			/* 当速度超过100时，切换为反转，速度跳转到-100 */
			if (Speed > 100)
			{
				Speed = -100;
			}
		}

		/* 设置电机速度（正值正转，负值反转） */
		Motor_SetSpeed(Speed);

		/* 在OLED第1行第7列显示带符号的速度值，显示宽度为3位 */
		OLED_ShowSignedNum(1, 7, Speed, 3);
	}
}
