/**
  ******************************************************************************
  * @文件    LED.c
  * @作者    Jiangxie Technology
  * @版本    V1.0
  * @日期    2026-07-07
  * @摘要    LED驱动模块
  *          控制PA1和PA2引脚上的两个LED灯的开启、关闭和状态翻转。
  *          LED采用低电平点亮的接法（引脚输出低电平时LED亮）。
  ******************************************************************************
  */

#include "stm32f10x.h"                  // 器件头文件

/**
  * @摘要    LED初始化
  * @参数    无
  * @返回值  无
  * @说明    将PA1和PA2配置为推挽输出模式，初始输出高电平（LED熄灭）。
  */
void LED_Init(void)
{
	/* 开启GPIOA的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* 配置PA1和PA2为推挽输出模式 */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            /* 推挽输出模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;     /* 选择PA1和PA2引脚 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           /* 输出速度50MHz */
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 设置PA1和PA2初始电平为高电平（LED默认熄灭） */
	GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);
}

/**
  * @摘要    LED1开启（点亮）
  * @参数    无
  * @返回值  无
  * @说明    将PA1引脚置为低电平，LED1点亮
  */
void LED1_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);      /* PA1输出低电平，LED1点亮 */
}

/**
  * @摘要    LED1关闭（熄灭）
  * @参数    无
  * @返回值  无
  * @说明    将PA1引脚置为高电平，LED1熄灭
  */
void LED1_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_1);        /* PA1输出高电平，LED1熄灭 */
}

/**
  * @摘要    LED1状态翻转
  * @参数    无
  * @返回值  无
  * @说明    读取PA1当前输出电平，取反后输出，实现LED1状态翻转
  */
void LED1_Turn(void)
{
	/* 读取PA1输出寄存器的当前状态 */
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0)     /* 当前输出低电平（LED亮） */
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_1);                    /* 翻转为高电平（LED灭） */
	}
	else                                                     /* 当前输出高电平（LED灭） */
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);                  /* 翻转为低电平（LED亮） */
	}
}

/**
  * @摘要    LED2开启（点亮）
  * @参数    无
  * @返回值  无
  * @说明    将PA2引脚置为低电平，LED2点亮
  */
void LED2_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);      /* PA2输出低电平，LED2点亮 */
}

/**
  * @摘要    LED2关闭（熄灭）
  * @参数    无
  * @返回值  无
  * @说明    将PA2引脚置为高电平，LED2熄灭
  */
void LED2_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_2);        /* PA2输出高电平，LED2熄灭 */
}

/**
  * @摘要    LED2状态翻转
  * @参数    无
  * @返回值  无
  * @说明    读取PA2当前输出电平，取反后输出，实现LED2状态翻转
  */
void LED2_Turn(void)
{
	/* 读取PA2输出寄存器的当前状态 */
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2) == 0)     /* 当前输出低电平（LED亮） */
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_2);                    /* 翻转为高电平（LED灭） */
	}
	else                                                     /* 当前输出高电平（LED灭） */
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);                  /* 翻转为低电平（LED亮） */
	}
}
