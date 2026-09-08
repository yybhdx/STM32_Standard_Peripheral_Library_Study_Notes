/**
  ******************************************************************************
  * @文件    Key.c
  * @作者    Jiangxie Technology
  * @版本    V1.0
  * @日期    2026-07-07
  * @摘要    按键驱动模块
  *          支持两个独立按键（PB1和PB11），采用阻塞式消抖检测。
  *          按键按下返回对应键码值，未按下返回0。
  ******************************************************************************
  */

#include "stm32f10x.h"                  // 器件头文件
#include "Delay.h"

/**
  * @摘要    按键初始化
  * @参数    无
  * @返回值  无
  * @说明    将PB1和PB11配置为上拉输入模式。
  *          上拉输入：引脚默认为高电平，按键按下时拉低为低电平。
  */
void Key_Init(void)
{
	/* 开启GPIOB的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/* 配置PB1和PB11为上拉输入模式 */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;               /* 上拉输入模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;    /* 选择PB1和PB11引脚 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           /* 输入模式下此参数无实际影响 */
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @摘要    获取按键键码值
  * @参数    无
  * @返回值  按下的按键键码值：
  *          1 - 按键1（PB1）被按下
  *          2 - 按键2（PB11）被按下
  *          0 - 没有按键被按下
  * @说明    此函数为阻塞式操作：当按键按住不放时，函数会一直等待，
  *          直到按键松手后才返回键码值。包含20ms的消抖延时处理。
  */
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;     /* 默认键码值为0，表示无按键按下 */

	/* 检测按键1（PB1）是否按下：读取PB1输入电平，低电平表示按下 */
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
	{
		Delay_ms(20);                                               /* 按下消抖：延时20ms */
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);     /* 等待按键松手 */
		Delay_ms(20);                                               /* 松手消抖：延时20ms */
		KeyNum = 1;                                                 /* 确认按键1被按下，键码置1 */
	}

	/* 检测按键2（PB11）是否按下：读取PB11输入电平，低电平表示按下 */
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
	{
		Delay_ms(20);                                               /* 按下消抖：延时20ms */
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0);    /* 等待按键松手 */
		Delay_ms(20);                                               /* 松手消抖：延时20ms */
		KeyNum = 2;                                                 /* 确认按键2被按下，键码置2 */
	}

	return KeyNum;          /* 返回键码值，若无按键按下则返回默认值0 */
}
