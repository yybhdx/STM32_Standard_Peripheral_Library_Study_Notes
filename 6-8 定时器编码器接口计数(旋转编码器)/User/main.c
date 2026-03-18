//编码器计次
//目的：测量总位移/累计脉冲数
//CNT值不清零，一直累加
//结果：显示从开始到现在的总脉冲数，所以不像CNT测速那样需要清零。

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "Encoder.h"

uint16_t Num;			//定义在定时器中断里自增的变量

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
//	Timer_Init();		//定时中断初始化
	
/*旋转编码器初始化*/
	Encoder_Init();
	
	/*显示静态字符串*/
	OLED_ShowString(1, 1, "CNT:");			//1行1列显示字符串Num:
	
	while (1)
	{
		OLED_ShowSignedNum(1, 5, Encoder_Get(), 5);	// 旋转编码器左转 CNT-- 右转 CNT++
	}
}

/*现在模式是右转CNT++  左转 CNT--
如果我们想改变 那么如何变??*/
// 1.硬件方面:将旋转编码器的A B两根线换一下
// 2.软件方面:修改TIM_EncoderInterfaceConfig函数的极性，把任意一个极性反转一下(这里我们将第三个参数的Rising改为Falling)，方向就会反过来
// 如果两个极性都反转 ，那么极性还是表示不变
//	TIM_EncoderInterfaceConfig(TIM3,
//																TIM_EncoderMode_TI12, // 决定了编码器信号的检测模式 // 为在TI1和TI2边沿都计数
//                                TIM_ICPolarity_Falling, // 设置通道1（A相）的输入极性 // 为TIM_ICPolarity_Rising：不反相（上升沿有效）。这是最常见的设置，表示信号不反相。
//                                TIM_ICPolarity_Rising);// 设置通道2（B相）的输入极性 // 为TIM_ICPolarity_Rising：不反相（上升沿有效）。这是最常见的设置，表示信号不反相。
