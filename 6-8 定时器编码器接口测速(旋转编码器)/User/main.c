//代码实现编码器测速的方式是频率测量法，即测量单位时间内脉冲的个数。
//如何获取速度??
//每秒钟的CNT值就是速度值，所以需要每读取一次CNT值就清零一次


#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "Encoder.h"

//uint16_t Num;			//定义在定时器中断里自增的变量
int16_t Speed;     // 速度 // 若变量类型不是有符号型 则不会显示负数

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	Timer_Init();		//定时中断初始化
	
/*旋转编码器初始化*/
	Encoder_Init();
	
	/*显示静态字符串*/
	OLED_ShowString(1, 1, "Speed:");			//1行1列显示字符串Num:
	
	while (1)
	{
		// CNT的值就代表了速度 单位是脉冲个数/s
		
		// 主循环中每隔一段时间获取一次CNT
		OLED_ShowSignedNum(1, 7, Speed, 5);	// 旋转编码器左转 CNT-- 右转 CNT++
		
//		// 闸门时间为1S(因为我们是手动转，所以闸门时间就给1S 如果是点击飞速转，闸门就可以给短点，这样还可以提高速度刷新的频率，而且防止计数器溢出)
//		Delay_ms(1000);
	}
}

// 由于在while循环中使用Delay_ms会严重阻塞程序运行(程序小时还正常，但是多的时候会很麻烦)
/**
  * 函    数：TIM2中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  */
//​触发中断的是TIM2，而不是TIM3。TIM2就像一个秒表，每隔1秒“滴答”一次，这个“滴答”信号就是更新中断。
//在这个中断服务程序（TIM2_IRQHandler）里，您执行了Speed = Encoder_Get();。这行代码的本质是去读取TIM3这个编码器定时器的当前计数值。
//读取完毕后，您清零了TIM2的中断标志，等待下一次1秒的到来。而TIM3的CNT值会一直被清零吗？​这取决于您的Encoder_Get()函数是如何实现的。
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)		//判断是否是TIM2的更新事件触发的中断
	{
		
		Speed = Encoder_Get();												// 每隔一秒读取一下速度，存在Speed变量里面
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);			//清除TIM2更新事件的中断标志位
															//中断标志位必须清除
															//否则中断将连续不断地触发，导致主程序卡死
	}
}


/*现在模式是右转CNT++  左转 CNT--
如果我们想改变 那么如何变??*/
// 1.硬件方面:将旋转编码器的A B两根线换一下
// 2.软件方面:修改TIM_EncoderInterfaceConfig函数的极性，把任意一个极性反转一下(这里我们将第三个参数的Rising改为Falling)，方向就会反过来
// 如果两个极性都反转 ，那么极性还是表示不变
//	TIM_EncoderInterfaceConfig(TIM3,
//																TIM_EncoderMode_TI12, // 决定了编码器信号的检测模式 // 为在TI1和TI2边沿都计数
//                                TIM_ICPolarity_Falling, // 设置通道1（A相）的输入极性 // 为TIM_ICPolarity_Falling：反相（下降沿有效）。如果编码器输出的信号相位反了，可以通过设置此参数来纠正方向判断。
//                                TIM_ICPolarity_Rising);// 设置通道2（B相）的输入极性 // 为TIM_ICPolarity_Rising：不反相（上升沿有效）。这是最常见的设置，表示信号不反相。
