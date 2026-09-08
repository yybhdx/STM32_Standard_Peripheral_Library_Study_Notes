/**
  ******************************************************************************
  * @文件    motor.c
  * @作者    Jiangxie Technology
  * @版本    V1.0
  * @日期    2026-07-07
  * @摘要    直流电机驱动模块
  *          通过H桥驱动芯片控制电机的正反转和速度。
  *          方向控制：PA4和PA5两个GPIO引脚控制H桥的输入端（IN1、IN2），
  *                    两个引脚电平相反时电机转动，相同时电机停止。
  *          速度控制：通过PWM信号调节电机的等效供电电压，从而控制转速。
  *
  * @引脚    PA2 - PWM信号输出（TIM2_CH3，连接到H桥的使能端EN）
  *          PA4 - 电机方向控制IN1
  *          PA5 - 电机方向控制IN2
  ******************************************************************************
  */

#include "stm32f10x.h"                  // 器件头文件
#include "PWM.h"

/**
  * @摘要    电机驱动初始化
  * @参数    无
  * @返回值  无
  * @说明    初始化PWM输出和电机方向控制的GPIO引脚（PA4、PA5）。
  *          PA4和PA5配置为推挽输出模式，用于控制H桥的两个方向输入端。
  */
void Motor_Init(void)
{
	/* 初始化PWM输出（TIM2通道3，PA2引脚） */
	PWM_Init();

	/* 开启GPIOA的时钟（PA4、PA5用于电机方向控制） */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* 配置PA4和PA5为推挽输出模式 */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            /* 推挽输出模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;     /* 选择PA4和PA5引脚 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           /* 输出速度50MHz */
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  * @摘要    设置电机运行速度和方向
  * @参数    speed: 电机速度值，范围：-100 ~ +100
  *                 正值 → 电机正转，绝对值为占空比
  *                 负值 → 电机反转，绝对值为占空比
  *                 0    → 电机停止
  * @返回值  无
  * @说明    方向控制原理（H桥驱动）：
  *          - 正转：PA4=高电平，PA5=低电平 → 电流从IN1流向IN2
  *          - 反转：PA4=低电平，PA5=高电平 → 电流从IN2流向IN1
  *          速度通过PWM占空比控制，占空比 = |speed| / 100
  */
void Motor_SetSpeed(int8_t speed)
{
	if (speed >= 0)
	{
		/* ---- 正转模式 ---- */

		/* 设置方向：PA4高电平，PA5低电平 */
		GPIO_SetBits(GPIOA, GPIO_Pin_4);        /* IN1 = 高电平 */
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);      /* IN2 = 低电平 */

		/* 设置PWM占空比，speed值直接作为CCR值（0~100） */
		PWM_SetCompare3(speed);
	}
	else
	{
		/* ---- 反转模式 ---- */

		/* 设置方向：PA4低电平，PA5高电平（与正转相反） */
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);      /* IN1 = 低电平 */
		GPIO_SetBits(GPIOA, GPIO_Pin_5);        /* IN2 = 高电平 */

		/* 设置PWM占空比，speed为负数，取反后作为CCR值 */
		PWM_SetCompare3(-speed);
	}
}
