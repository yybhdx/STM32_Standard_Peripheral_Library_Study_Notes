/**
  ******************************************************************************
  * @文件    PWM.c
  * @作者    Jiangxie Technology
  * @版本    V1.0
  * @日期    2026-07-07
  * @摘要    PWM输出驱动模块
  *          使用TIM2的输出比较通道3（OC3），对应引脚PA2，产生PWM信号。
  *          PWM频率 = 72MHz / (PSC+1) / (ARR+1) = 72MHz / 720 / 100 = 1kHz
  *          占空比通过修改CCR3的值来调节，范围：0~100，对应0%~100%。
  *
  * @原理    本模块使用的是TIM定时器的【输出比较（Output Compare）】功能。
  *          具体来说，使用的是输出比较的PWM模式1（PWM Mode 1）：
  *          - 当计数器值 CNT < CCR 时，输出有效电平（高电平）
  *          - 当计数器值 CNT >= CCR 时，输出无效电平（低电平）
  *          通过改变CCR的值，即可改变PWM的占空比。
  ******************************************************************************
  */

#include "stm32f10x.h"                  // 器件头文件

/**
  * @摘要    PWM初始化函数
  * @参数    无
  * @返回值  无
  * @说明    配置TIM2通道3输出PWM信号，初始化步骤如下：
  *          第1步：开启TIM2和GPIOA的RCC时钟
  *          第2步：选择内部时钟源，配置时基单元（PSC、ARR、计数模式）
  *          第3步：配置输出比较单元（PWM模式、极性、CCR初始值）
  *          第4步：配置GPIO引脚PA2为复用推挽输出
  *          第5步：使能TIM2定时器，开始输出PWM波形
  */
void PWM_Init(void)
{
	/*======================== 第1步：开启RCC时钟 ========================*/

	/* TIM2挂载在APB1总线上，开启TIM2的时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* GPIOA挂载在APB2总线上，开启GPIOA的时钟（PA2用于PWM输出） */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/*======================== 引脚重映射（已注释） ========================*/
	/* 如果需要将TIM2的PWM输出重映射到其他引脚，取消以下注释：
	 * 1. 开启AFIO时钟（重映射功能必须先开启AFIO时钟）
	 * 2. 配置TIM2的部分重映射方案（具体映射关系参考参考手册）
	 * 3. 如果重映射到JTAG引脚，还需要禁用JTAG功能
	 */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	/*======================== 第2步：配置时基单元 ========================*/

	/* 选择TIM2的时钟源为内部时钟（72MHz）
	 * 注意：不调用此函数时，TIM默认也使用内部时钟 */
	TIM_InternalClockConfig(TIM2);

	/* 配置时基单元参数 */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;         /* 时钟分频因子：不分频（用于输入滤波器，不影响时基） */
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;     /* 计数模式：向上计数 */
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;                     /* 自动重装载值ARR = 99，计数范围0~99，共100个计数 */
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;                  /* 预分频值PSC = 719，将72MHz分频为100kHz */
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                /* 重复计数器：0（高级定时器专用，此处不使用） */
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

	/*======================== 第3步：配置输出比较单元 ========================*/

	/* 配置TIM2的输出比较通道3（OC3） */
	// TIM_OC3Init初始化定时器输出比较通道3(对应的是通道几不是定时器几)
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);                             /* 先给结构体赋默认值，避免未初始化成员导致异常 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                  /* 输出比较模式：PWM模式1（CNT<CCR时输出高电平） */
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;           /* 输出极性：高电平有效（不取反） */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;       /* 输出使能：开启OC3输出 */
	TIM_OCInitStructure.TIM_Pulse = 0;                                  /* CCR初始值 = 0，初始占空比为0% */
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);                           /* 将配置写入TIM2的输出比较通道3 */

	/*======================== 第4步：配置GPIO引脚 ========================*/

	/* 将PA2配置为复用推挽输出（AF_PP）
	 * TIM2_CH3的默认引脚就是PA2，受TIM2外设控制，因此必须配置为复用模式 */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                     /* 复用推挽输出模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                           /* 选择PA2引脚 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                   /* 输出速度50MHz */
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*======================== 第5步：使能定时器 ========================*/

	/* 启动TIM2，定时器开始计数，PWM信号开始输出 */
	TIM_Cmd(TIM2, ENABLE);
}

/**
  * @摘要    设置PWM占空比（通过修改CCR3的值）
  * @参数    Compare: 要写入CCR3的值，范围：0~100
  * @返回值  无
  * @说明    占空比 = CCR / (ARR + 1) = Compare / 100
  *          例如：Compare=50 → 占空比50%，Compare=100 → 占空比100%
  */
void PWM_SetCompare3(uint16_t Compare)
{
	// TIM_SetCompare3():设置定时器通道 3 的捕获/比较寄存器 CCR3 的值(运行时可以用来更改占空比),数字代表通道号
	TIM_SetCompare3(TIM2, Compare);     /* 设置TIM2通道3的CCR值 */
}
